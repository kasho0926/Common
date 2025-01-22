#pragma once

#include <string>
#include "debug_log.h"
#include "filesystem_util.h"

enum ExitCode
{
  EC_SUCCESS = 0,
  EC_INPUT_ERROR = 0x1,
  EC_TIMEOUT = 0x2,
  EC_INIT_FAIL = 0x3,
  EC_UNKNOWN_FORMAT = 0x4,
  EC_CREATEPROCESS_FAIL = 0x5,
  EC_ORIGINAL_FILE = 0x13,
  EC_EXCEPTION = 0x14,
  EC_IMAGE_FILE = 0x15
};

class ExeWrapper
{
public:

  static ExeWrapper& Instance()
  {
    static ExeWrapper instance;
    return instance;
  }

  ExeWrapper(ExeWrapper const&) = delete;
  void operator=(ExeWrapper const&) = delete;

  ExeWrapper()
  {
    isInit = false;
  }

  virtual void Init(const std::wstring & path)
  {
    exePath = path;
  }

  void LoadExe()
  {
    if(isInit)
      return;

    if(!FileSystem::File::Exist(exePath))
    {
      LOGAF_ERROR("Exe %S Not Found!", exePath.c_str());
      return;
    }

    isInit = true;
  }

  int Start(const std::wstring &input, const std::wstring &output, DWORD timeout)
  {
    LoadExe();

    if(!isInit)
    {
      return EC_INIT_FAIL;
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    HANDLE h = CreateFile(_T("out.log"),
      FILE_APPEND_DATA,
      FILE_SHARE_WRITE | FILE_SHARE_READ,
      &sa,
      OPEN_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      NULL);

    si.hStdError = h;
    si.hStdOutput = h;


    std::wstring cmdstr = exePath + L" \"" + input + L"\"" + L" \"" + output + L"\"";
    LOGAF_DEBUG("Cmd: %S", cmdstr.c_str());
    wchar_t cmd[MAX_PATH * 2 + 2] = { '\0' };
    wcscpy(cmd, cmdstr.c_str());

    // Start the child process.
    if(!CreateProcessW(NULL,   // No module name (use command line)
      cmd,        // Command line
      NULL,           // Process handle not inheritable
      NULL,           // Thread handle not inheritable
      FALSE,          // Set handle inheritance to FALSE
      CREATE_NEW_CONSOLE,              // No creation flags
      NULL,           // Use parent's environment block
      NULL,           // Use parent's starting directory
      &si,            // Pointer to STARTUPINFO structure
      &pi)           // Pointer to PROCESS_INFORMATION structure
      )
    {
      LOGAF_WARN("CreateProcess Fail!");
      return EC_CREATEPROCESS_FAIL;
    }

    // Wait until child process exits.
    DWORD exitCode = 0;
    if(WaitForSingleObject(pi.hProcess, timeout) == WAIT_TIMEOUT)
    {
      TerminateProcess(pi.hProcess, 0);
      WaitForSingleObject(pi.hProcess, 500);
      exitCode = EC_TIMEOUT;
    }
    else
    {
      bool result = GetExitCodeProcess(pi.hProcess, &exitCode);
    }

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    if(exitCode)
    {
      LOGAF_DEBUG("ExitCode: %d", exitCode);
    }

    return exitCode;
  }

  void Free()
  {
  }

protected:
  std::wstring exePath;
  bool isInit;
};