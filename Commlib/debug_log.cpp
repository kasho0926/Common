#include "debug_log.h"

#include <iostream>
#include <Shlwapi.h>

#include "AutoLock.hpp"
#include "filesystem_util.h"
#include "string_utils.hpp"

#ifdef DEBUG_EXTENTION
#include "debug_log_extension.h"
#endif

#pragma comment (lib, "Shlwapi.lib")

using Sync::AutoCriticalSection;
using Sync::AutoLock;
using namespace FileSystem;

const int MSG_SZ = 1024;

DebugLevel DebugLevelFilter(WCHAR * debugLevel)
{
  if(wcscmp(debugLevel, L"OFF") == 0)
  {
    return DEBUG_OFF;
  }
  else if(wcscmp(debugLevel, L"FATAL") == 0)
  {
    return DEBUG_FATAL;
  }
  else if(wcscmp(debugLevel, L"ERROR") == 0)
  {
    return DEBUG_ERROR;
  }
  else if(wcscmp(debugLevel, L"WARN") == 0)
  {
    return DEBUG_WARN;
  }
  else if(wcscmp(debugLevel, L"INFO") == 0)
  {
    return DEBUG_INFO;
  }
  else if(wcscmp(debugLevel, L"DEBUG") == 0)
  {
    return DEBUG_DEBUG;
  }
  else if(wcscmp(debugLevel, L"TRACE") == 0)
  {
    return DEBUG_TRACE;
  }
  else
  {
    return DEBUG_INFO;
  }
}

namespace DebugLog
{
  DWORD g_log = 0;
  DWORD g_debug = 0;
  DebugLevel g_debugLevel = DEBUG_OFF;
  WCHAR g_logDir[MAX_PATH] = { 0 };
  WCHAR g_newestPath[MAX_PATH] = { 0 };
  WCHAR g_archivePath[MAX_PATH] = { 0 };
  WCHAR g_confPath[MAX_PATH] = { 0 };
  WCHAR g_debugConfPath[MAX_PATH] = { 0 };
  WCHAR g_key[128] = { 0 };
  UINT g_logSize = 5;
  AutoCriticalSection JLogCriticalSection;

  void Init(const WCHAR * key)
  {
    wcsncpy(g_key, key, 128);

    if(GetLogPath()) return;
    bool EnableLog = CheckLogConfExist();
    bool EnableDebug = CheckDebugConfExist();
    if(g_log == 0 && EnableLog)
    {
      g_log = 1;
    }
    else if(g_log == 1 && !EnableLog)
    {
      Stop();
    }

    if(g_debug == 0 && EnableDebug)
    {
      g_debug = 1;
    }
    else if(g_debug == 1 && !EnableDebug)
    {
      g_debug = 0;
    }
  }

  DebugLevel ReadConfig(WCHAR * fileName)
  {
    WCHAR buffer[MAX_PATH] = L"";
    GetPrivateProfileStringW(L"DEBUG", L"debug_level", L"INFO", buffer, MAX_PATH, fileName); /// 讀取ini檔中的字串
    //pf_log=1
    return DebugLevelFilter(buffer);
  }

  void Stop()
  {
    g_log = 0;
  }

  void WriteToFile(const BYTE * pMessage, size_t size)
  {
    HANDLE hFile;

    // Open file
    hFile = CreateFileW(
      g_newestPath,				// file to create
      GENERIC_WRITE,			// open for writing
      0,						// do not share
      NULL,					// default security
      OPEN_ALWAYS,			// overwrite existing
      FILE_ATTRIBUTE_NORMAL,	// normal file
      NULL);					// no attr. template

    if(hFile == INVALID_HANDLE_VALUE)
      return;

    try
    {
      // Check size of file (no more than 2MB)
      if(GetFileSize(hFile, NULL) > 1024 * 1024 * g_logSize)
      {
        // Close file
        CloseHandle(hFile);

        // Move file

        if(PathFileExists(g_archivePath))
          DeleteFile(g_archivePath);
        MoveFile(g_newestPath, g_archivePath);

        // Reopen file
        hFile = CreateFileW(
          g_newestPath,
          GENERIC_WRITE,
          0,
          NULL,
          OPEN_ALWAYS,
          FILE_ATTRIBUTE_NORMAL,
          NULL);
      }

      if(hFile == INVALID_HANDLE_VALUE)
        return;

      if(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_END))
      {
        CloseHandle(hFile);
        return;
      }
      size_t MessageLength = size;
      DWORD BytesWritten;
      WriteFile(hFile, pMessage, MessageLength, &BytesWritten, NULL);
    }
    catch(...)
    {
    }

    CloseHandle(hFile);
  }

  void WriteToFile(const char * pMessage)
  {
    size_t MessageLength = strnlen(pMessage, MSG_SZ);	// 確定字串最長只有 MSG_SZ
    WriteToFile((BYTE*)pMessage, MessageLength);
  }

  void WriteToFile(const WCHAR * pMessage)
  {
    size_t MessageLength = wcsnlen(pMessage, MSG_SZ) * 2;	// 確定字串最長只有 MSG_SZ
    WriteToFile((BYTE*)pMessage, MessageLength);
  }

  void LogA(const char * format, ...)
  {
    if(g_log == 0)
    {
      return;
    }

    AutoLock Lock(JLogCriticalSection);

    va_list valist;
    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();

    SYSTEMTIME SystemTime;
    GetLocalTime(&SystemTime);

    std::string message;
    message.resize(MSG_SZ, '\0');

    // Generate log message and push to queue
    int n = _snprintf_s(&message[0], MSG_SZ, MSG_SZ - 3, "%hu/%02hu/%02hu %02hu:%02hu:%02hu.%03hu [Pid=%5u][Tid=%5u]",
      SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds, pid, tid);
    if(n > 0)
    {
      va_start(valist, format);
      _vsnprintf_s(&message[0] + n, MSG_SZ - n, MSG_SZ - n - 3, format, valist);
      va_end(valist);

      if(g_debug)
      {
        OutputDebugStringA(message.c_str());
      }

      strcat_s(&message[0], MSG_SZ, "\r\n");
      WriteToFile(message.c_str());
    }
  }

  void LogW(const WCHAR * format, ...)
  {
    if(g_log == 0)
    {
      return;
    }

    AutoLock Lock(JLogCriticalSection);

    va_list valist;
    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();

    SYSTEMTIME SystemTime;
    GetLocalTime(&SystemTime);

    std::wstring message;
    message.resize(MSG_SZ, L'\0');

    // Generate log message and push to queue
    int n = _snwprintf_s(&message[0], MSG_SZ, MSG_SZ - 3, L"%hu/%02hu/%02hu %02hu:%02hu:%02hu.%03hu [Pid=%5u][Tid=%5u]",
      SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds, pid, tid);
    if(n > 0)
    {
      va_start(valist, format);
      _vsnwprintf_s(&message[0] + n, MSG_SZ - n, MSG_SZ - n - 3, format, valist);
      va_end(valist);

      if(g_debug)
      {
        OutputDebugStringW(message.c_str());
      }

      wcscat_s(&message[0], MSG_SZ, L"\r\n");
      WriteToFile(StringUtils::Convert::Utf16WideCharToUtf8MultiByte(message.c_str()).c_str());
    }
  }

  bool CheckLogConfExist()
  {
    if(PathFileExists(g_confPath) == TRUE)
    {
      g_logSize = GetPrivateProfileInt(L"DEBUG", L"log_size", 5, g_confPath);
      g_debugLevel = ReadConfig(g_confPath);

#ifdef DEBUG_EXTENTION
      ReadExtraSettings(g_confPath);
#endif
      return true;
    }

    return false;
  }

  bool CheckDebugConfExist()
  {
    if(PathFileExists(g_debugConfPath) == TRUE)
    {
      return true;
    }

    return false;
  }

  DWORD GetLogPath()
  {
    if(g_logDir[0] != 0)
    {
      return 0;
    }

    std::wstring path = Path::GetAppHistoryPath();
    if(path.size() == 0)
      return -1;

    wcscpy_s(g_logDir, MAX_PATH, path.c_str());
    wcscpy_s(g_newestPath, MAX_PATH, Path::Combine(path, std::wstring(g_key) + L".newest.log").c_str());
    wcscpy_s(g_archivePath, MAX_PATH, Path::Combine(path, std::wstring(g_key) + L".archive.log").c_str());
    wcscpy_s(g_confPath, MAX_PATH, Path::Combine(path, std::wstring(g_key) + L".conf").c_str());
    wcscpy_s(g_debugConfPath, MAX_PATH, Path::Combine(path, std::wstring(g_key) + L".debug.conf").c_str());

    return 0;
  }
}