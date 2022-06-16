#include "filesystem_util.h"

#include <Windows.h>
#include <Shlobj.h>
#include <Shlwapi.h>
#include <tlhelp32.h>
#include <wtsapi32.h>

#include "macros.h"
#include "debug_log.h"

#pragma comment(lib, "WtsApi32.lib")
#pragma comment(lib, "version.lib")

using std::wstring;

namespace FileSystem
{
  HANDLE GetCurrentSessionUserTokenAccordingExplorer()
  {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hProcessSnap == INVALID_HANDLE_VALUE)
    {
      LOGAF_ERROR("CreateToolhelp32Snapshot fail");
      return NULL;
    }

    DWORD dwSid = (DWORD)-1;
    if(!ProcessIdToSessionId(GetCurrentProcessId(), &dwSid))
      return NULL;

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if(!Process32First(hProcessSnap, &pe32))
    {
      LOGAF_ERROR("Process32First fail");
      CloseHandle(hProcessSnap);    // Must clean up the
      return NULL;       //   snapshot object!
    }

    // Now walk the snapshot of processes, and
    // display information about each process in turn
    DWORD dwPid = (DWORD)-1;
    do
    {
      if(_wcsicmp(pe32.szExeFile, L"explorer.exe") == 0)
      {
        DWORD dwSidTmp = (DWORD)-1;
        dwPid = pe32.th32ProcessID;
        ProcessIdToSessionId(dwPid, &dwSidTmp);
        if(dwSidTmp == dwSid)
          break;
      }
      dwPid = (DWORD)-1; // reset
    } while(Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);

    HANDLE hProcess = NULL;
    if(dwPid != (DWORD)-1)
    {
      hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
    }
    if(hProcess == NULL)
    {
      LOGAF_ERROR("Can't find the hProcess!");
      return NULL;
    }

    HANDLE hToken = NULL;
    BOOL bnOK = OpenProcessToken(hProcess, TOKEN_READ | TOKEN_DUPLICATE, &hToken);
    LOGAF("GetCurrentSessionUserTokenAccordingExplorer: Get the hToken=0x%p, bnOK=%d, dwSid=%d", hToken, bnOK, dwSid);
    return hToken;
  }

  // return <> NULL: OK
  // return NULL: Fail
  HANDLE GetCurrentSessionUserToken()
  {
    // 不需要依靠 explorer 的方法
    DWORD dwSid = 0;
    HANDLE hToken = NULL;
    ProcessIdToSessionId(GetCurrentProcessId(), &dwSid);

    if(!WTSQueryUserToken(dwSid, &hToken))
    {
      DWORD dwErr = GetLastError();
      LOGAF_ERROR("GetCurrentSessionUserToken: NG, dwSid = %u, dwErr = %d ( 0x%08X ), use old method", dwSid, dwErr, dwErr);

      int iTry = 0;
      while((hToken = GetCurrentSessionUserTokenAccordingExplorer()) == NULL && iTry < 5)
      {
        Sleep(1000);
        iTry++;
        LOGAF_ERROR("GetCurrentSessionUserTokenAccordingExplorer: Can't get hToken of the user in current session, iTry=%d", iTry);
      }
    }

    return hToken; // 注意! Caller 必須自行 CloseHandle( hToken ) !!
  }

  DWORD File::GetDllVersion(const std::wstring & dllname, Version & ver)
  {
    DWORD dwLen = 0; //放GetFileVersionInfoSize
    DWORD dwHandle = 0; // lpdwHandle
    wchar_t wszName[MAX_PATH]; // wide string
    wchar_t * lpData;
    VS_FIXEDFILEINFO *pFileInfo;
    UINT BufLen;
    HMODULE hMod = GetModuleHandle(dllname.c_str()); // 取得檔案HANDLE
    if(hMod == NULL)
    {
      LOGAF_ERROR("Can't not find module: %S", (TCHAR*)dllname.c_str());
      return FUNCTION_ERROR;
    }
    GetModuleFileName(hMod, wszName, MAX_PATH); // 取得檔案名稱
    dwLen = GetFileVersionInfoSize(wszName, &dwHandle); // 取得版本資訊的大小
    if(!dwLen)
      return FUNCTION_ERROR;
    lpData = new wchar_t[dwLen];
    GetFileVersionInfo(wszName, dwHandle, dwLen, lpData); //取得版本資訊
    if(VerQueryValue(lpData, L"\\", (LPVOID *)&pFileInfo, (PUINT)&BufLen))  //解析版本資訊
    {
      ver.MajorVersion = HIWORD(pFileInfo->dwFileVersionMS);
      ver.MinorVersion = LOWORD(pFileInfo->dwFileVersionMS);
      ver.BuildNumber = HIWORD(pFileInfo->dwFileVersionLS);
      ver.RevisionNumber = LOWORD(pFileInfo->dwFileVersionLS);
      delete[] lpData;
      return FUNCTION_NOERROR;
    }

    delete[] lpData;
    return FUNCTION_ERROR;
  }

  ULONGLONG File::GetModifiedTime(const std::wstring &file)
  {
    FILETIME writeTime;
    ULARGE_INTEGER ull;
    HANDLE hFile = CreateFile(file.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
      OPEN_EXISTING, 0, NULL);

    if(hFile == INVALID_HANDLE_VALUE)
    {
      LOGAF_WARN("Createfile Error!!!: %S, Error code: %d", file.c_str(), GetLastError());
      return FUNCTION_ERROR;
    }

    if(GetFileTime(hFile, NULL, NULL, &writeTime) == 0)
    {
      LOGAF_WARN("GetFileTime Error!!!: %S, Error code: %d", file.c_str(), GetLastError());
      return FUNCTION_ERROR;
    }

    ull.LowPart = writeTime.dwLowDateTime;
    ull.HighPart = writeTime.dwHighDateTime;

    CloseHandle(hFile);
    return ull.QuadPart;
  }

  std::wstring Path::GetMyDocument()
  {
    wchar_t * path;

    if(SHGetKnownFolderPath(FOLDERID_Documents, NULL, NULL, &path) == S_OK)
    {
      std::wstring ret = path;
      CoTaskMemFree(path);
      return ret;
    }
    else
      return L"";
  }

  bool File::Exist(const std::wstring &path)
  {
    bool isExist = PathFileExistsW(path.c_str());
    LOGAF_TRACE("isExist: %d", isExist);
    return isExist;
  }

  std::wstring Path::GetWindowsEnvironmentVariable(const std::wstring &key)
  {
    DWORD bufferSize = 65535; //Limit according to http://msdn.microsoft.com/en-us/library/ms683188.aspx
    std::wstring buff;
    buff.resize(bufferSize);
    bufferSize = GetEnvironmentVariableW(key.c_str(), &buff[0], bufferSize);
    if(!bufferSize)
    {
      buff = L"";
      std::string message = "GetEnvironmentVariableW Fail: " + std::to_string(GetLastError());
      LOGAF_ERROR("%s", message.c_str());
      throw std::exception(message.c_str());
    }
    buff.resize(bufferSize);

    LOGAF_TRACE("buff: %S", buff.c_str());
    return buff;
  }

  std::wstring Path::GetCurrentDir()
  {
    WCHAR buffer[MAX_PATH];
    if(GetCurrentDirectoryW(MAX_PATH, buffer) == 0) /// 獲取當前資料夾路徑
    {
      std::string message = "GetCurrentDirectory Fail: " + std::to_string(GetLastError());
      LOGAF_ERROR("%s", message.c_str());
      throw std::exception(message.c_str());
    }
    return buffer;
  }

  DWORD Directory::Delete(const wstring &path)
  {
    WIN32_FIND_DATA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD error = 0;

    wstring file = path + L"\\*";

    LOGAF("FindFile: %S", file.c_str());
    hFind = FindFirstFile(file.c_str(), &ffd);
    if(hFind == INVALID_HANDLE_VALUE)
    {
      error = GetLastError();
      LOGAF_ERROR("FindFirstFile FAIL! %p", error);
      return error;
    }

    do
    {
      // "." OR ".."
      if(wcscmp(ffd.cFileName, L".") == 0 || wcscmp(ffd.cFileName, L"..") == 0)
      {
        LOGAF_TRACE("Skip . or ..");
      }
      else if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
        wstring && sub = path + L"\\" + ffd.cFileName;
        Delete(sub);
        RemoveDirectoryW(sub.c_str());
      }
      else
      {
        wstring && fullpath = path + L"\\" + ffd.cFileName;
        if(!DeleteFile(fullpath.c_str()))
        {
          LOGAF_ERROR("DeleteFile %S Fail! %p", fullpath.c_str(), GetLastError());
        }
      }
    } while(FindNextFile(hFind, &ffd) != 0);

    error = GetLastError();
    if(error != ERROR_NO_MORE_FILES)
    {
      LOGAF_ERROR("FindNextFile FAIL! %p", error);
    }
    else
    {
      error = 0;
    }

    FindClose(hFind);

    RemoveDirectoryW(path.c_str());
    return error;
  }

  bool Directory::SafeCreate(const std::wstring & Directory)
  {
    BOOL bResult = CreateDirectory(Directory.c_str(), NULL);
    if(bResult != 0)
    {
      // Succeeds of creating directory
      return true;
    }
    else if(GetLastError() == ERROR_ALREADY_EXISTS)
    {
      return true;
    }
    if(GetLastError() == ERROR_PATH_NOT_FOUND)
    {
      if(SafeCreate(GetParent(Directory)))
      {
        return SafeCreate(Directory);
      }
    }
    return false;
  }

  std::wstring Directory::GetParent(const std::wstring & Directory)
  {
    wstring FatherDirectory = wstring(Directory);
    wchar_t cLast = FatherDirectory.back();
    if(cLast == '\\')
    {
      FatherDirectory.pop_back();
    }
    wstring::size_type nPosition = FatherDirectory.find_last_of('\\');
    return FatherDirectory.substr(0, nPosition);
  }

  BOOL Directory::Exist(const std::wstring & Directory)
  {
    DWORD dwAttrib = GetFileAttributes(Directory.c_str());

    return (dwAttrib != INVALID_FILE_ATTRIBUTES);
  }

  std::list<std::string> File::ListAll(const std::string &path, bool recursive)
  {
    std::list<std::string> vec;

    WIN32_FIND_DATAA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD error = 0;

    std::string file = path + "\\*";

    LOGAF("FindFile: %s", file.c_str());
    hFind = FindFirstFileA(file.c_str(), &ffd);
    if(hFind == INVALID_HANDLE_VALUE)
    {
      error = GetLastError();
      LOGAF_ERROR("FindFirstFile FAIL! %p", error);
      return vec;
    }

    do
    {
      // "." OR ".."
      if(recursive && ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
        if(strcmp(ffd.cFileName, ".") == 0 || strcmp(ffd.cFileName, "..") == 0)
        {
          LOGAF_DEBUG("Skip FILE_ATTRIBUTE_DIRECTORY");
        }
        else
        {
          vec.splice(vec.end(), ListAll(path + "\\" + ffd.cFileName, true));
        }
      }
      else
      {
        vec.emplace_back(path + "\\" + ffd.cFileName);
      }
    } while(FindNextFileA(hFind, &ffd) != 0);

    error = GetLastError();
    if(error != ERROR_NO_MORE_FILES)
    {
      LOGAF_ERROR("FindNextFile FAIL! %p", error);
    }
    else
    {
      error = 0;
    }

    FindClose(hFind);
    return vec;
  }

  std::wstring Path::Combine(const std::wstring &path1, const std::wstring &path2)
  {
    std::wstring path;

    if(path1.length() == 0 || path2.length() == 0)
    {
      return path;
    }

    if(path1.back() == '\\')
    {
      path += path1;
    }
    else
    {
      path += path1.substr(0, path.length() - 1);
    }

    if(path2.front() == '\\')
    {
      path += path2;
    }
    else
    {
      path += '\\';
      path += path2;
    }

    return path;
  }

  std::wstring Path::GetMailLogStrPath(const std::wstring &filename)
  {
    return Combine(Combine(GetWindowsEnvironmentVariable(L"PROGRAMDATA"), L"\\Microsoft\\Dbgctr\\"), filename);
  }

  std::wstring Path::GetClientIniPath()
  {
    std::wstring path = Path::GetAllUsersDir();
    if(path.empty())
    {
      return std::wstring();
    }

    path = Combine(path, L"Microsoft\\Installer\\client.ini");

    return path;
  }

  wstring Path::GetAllUsersDir()
  {
    WCHAR buffer[MAX_PATH] = { 0 };

    if(S_OK != SHGetFolderPathW(NULL, CSIDL_COMMON_APPDATA | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, buffer))
    { // fail
      return wstring();
    }

    return wstring(buffer);
  }

  wstring Path::GetRedirectAllUsersDir()
  {
    wstring iniPath = GetClientIniPath();
    if(iniPath.empty())
    {
      return Path::GetAllUsersDir();
    }

    wstring path;
    WCHAR buffer[MAX_PATH] = { 0 };
    if(GetPrivateProfileIntW(L"Settings", L"Redirect_Enabled", 0, iniPath.c_str()))
    {
      if(GetPrivateProfileStringW(L"Settings", L"Redirect_Path", L"", buffer, MAX_PATH, iniPath.c_str()))
      {
        path = Combine(buffer, L"All Users");
      }
    }

    if(path.empty())
    {
      path = GetAllUsersDir();
    }

    return path;
  }

  wstring Path::GetLocalAppDataDir()
  {
    HANDLE token = GetCurrentSessionUserToken();

    WCHAR buffer[MAX_PATH] = { 0 };
    if(S_OK != SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, token, SHGFP_TYPE_CURRENT, buffer))
    { // fail
      return wstring();
    }

    return wstring(buffer);
  }

  wstring Path::GetRedirectLocalAppDataDir()
  {
    wstring localPath = GetLocalAppDataDir();

    if(localPath.empty())
    {
      return wstring();
    }

    wstring iniPath = Combine(localPath, L"Microsoft\\DbgCtr\\UserCfg.ini");
    WCHAR buffer[MAX_PATH] = { 0 };

    if(GetPrivateProfileStringW(L"Settings", L"CurrentUserPath", L"", buffer, MAX_PATH, iniPath.c_str()))
    {
      localPath = buffer;
    }

    return localPath;
  }

  std::wstring Path::GetLocalAppDataLowDir()
  {
    wstring path;

    WCHAR* pathBuffer;
    if(FAILED(SHGetKnownFolderPath(FOLDERID_LocalAppDataLow, KF_FLAG_CREATE, 0, &pathBuffer)))
    {
      return wstring();
    }

    path = pathBuffer;
    CoTaskMemFree(pathBuffer);

    return path;
  }

  wstring Path::GetDbgCtrPath()
  {
    wstring path = GetAllUsersDir();
    if(path.empty())
    {
      return wstring();
    }

    path = Combine(path, L"Microsoft\\DbgCtr");
    return path;
  }

  wstring Path::GetHelpClrPath()
  {
    wstring path = GetRedirectAllUsersDir();
    if(path.empty())
    {
      return wstring();
    }

    path = Combine(path, L"Microsoft\\HelpClr");
    return path;
  }

  wstring Path::GetAppHistoryPath()
  {
    wstring path = GetRedirectAllUsersDir();
    if(path.empty())
    {
      return wstring();
    }

    path = Combine(path, L"Microsoft\\App_History");

    return path;
  }

  wstring Path::GetTemporaryFilesPath()
  {
    wstring path = GetRedirectAllUsersDir();
    if(path.empty())
    {
      return wstring();
    }

    path = Combine(path, L"Microsoft\\Temporary Files");

    return path;
  }
}