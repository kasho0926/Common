#pragma once

#include <string>
#include <exception>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include "filesystem_util.h"
#include "debug_log.h"

namespace Windows
{
  typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

  BOOL IsWow64(HANDLE hProcess)
  {
    BOOL bIsWow64 = FALSE;

    LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
      GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

    if(NULL != fnIsWow64Process)
    {
      if(!fnIsWow64Process(hProcess, &bIsWow64))
      {
        DWORD err = GetLastError();
        LOGAF_ERROR("Call IsWow64Process fail, err=%d", err);
      }
    }

    LOGWF_TRACE(L"bIsWow64: %d", bIsWow64);
    return bIsWow64;
  }

  bool Is64bitSystem()
  {
    SYSTEM_INFO si;
    GetNativeSystemInfo(&si);
    if(si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
      si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
      return true;
    else
      return false;
  }

  BOOL RemoteInject(HANDLE hProcess, const WCHAR* dllPath)
  {
    BOOL ret = FALSE;
    int bytesToAlloc = (1 + lstrlenW(dllPath)) * sizeof(WCHAR);

    // Returns a pointer to the LoadLibrary address. This will be the same on the remote process as in our current process.
    LPVOID loadLibraryAddress = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
    if(loadLibraryAddress == NULL)
    {
      LOGWF_ERROR(L"GetProcAddress unsuccessful: %p", GetLastError());
      return ret;
    }

    LPVOID dllPathAddressInRemoteMemory = VirtualAllocEx(hProcess, NULL, bytesToAlloc, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if(dllPathAddressInRemoteMemory == NULL)
    {
      LOGWF_ERROR(L"VirtualAllocEx unsuccessful: %p", GetLastError());
      return ret;
    }

    // Write DLL's path name to remote process
    BOOL succeededWriting = WriteProcessMemory(hProcess, dllPathAddressInRemoteMemory, dllPath, bytesToAlloc, NULL);
    if(!succeededWriting)
    {
      LOGWF_ERROR(L"WriteProcessMemory unsuccessful: %p", GetLastError());
    }
    else
    {
      HANDLE remoteThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)loadLibraryAddress, dllPathAddressInRemoteMemory, NULL, NULL);
      if(remoteThread == NULL)
      {
        LOGWF_ERROR(L"CreateRemoteThread unsuccessful: %p", GetLastError());
      }
      else
      {
        WaitForSingleObject(remoteThread, 1000); // 成功呼叫過 LoadLibrary 就算 OK
        CloseHandle(remoteThread);
        ret = TRUE;
      }
    }

    VirtualFreeEx(hProcess, dllPathAddressInRemoteMemory, 0, MEM_RELEASE | MEM_DECOMMIT);
    return ret;
  }
}

class Injector
{
public:

  typedef enum _ImageType {
    IMAGE_16BIT = 0,
    IMAGE_32BIT,
    IMAGE_64BIT,
    IMAGE_UNKNOWN
  } ImageType;

  void Init(HMODULE module)
  {
    m_is64OS = Windows::Is64bitSystem();
    m_RunWDPath = FileSystem::Path::GetModulePath(module);

#ifdef _M_X64
    m_is64Process = true;
#else
    m_is64Process = false;
#endif
  }

  ImageType GetBit(HANDLE hProcess)
  {
    ImageType type = IMAGE_UNKNOWN;
    if(m_is64OS && !Windows::IsWow64(hProcess))
    {
      type = IMAGE_64BIT;
    }
    else
    {
      type = IMAGE_32BIT;
    }

    return type;
  }

  ImageType CheckPidBit(DWORD dwPid)
  {
    ImageType type = IMAGE_UNKNOWN;

    DWORD dwErr = 0;
    HANDLE hProcess = NULL;

    // 1. 根據 dwPid 取得 Remote process
    if(dwPid == 0)
    {
      LOG("CheckPidBit: dwPid can't be zero");
      throw std::exception("CheckPidBit: dwPid can't be zero");
    }

    hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPid);
    if(!hProcess)
    {
      LOG("CheckPidBit: Open process NG, dwErr = %u", GetLastError());
      throw std::exception("CheckPidBit: Open process NG");
    }

    type = GetBit(hProcess);

    if(hProcess)
      CloseHandle(hProcess);

    return type;
  }

  std::wstring CreateCmdline(const std::wstring &dll, const DWORD& pid)
  {
    ImageType type = CheckPidBit(pid);

    if(!FileSystem::File::Exist(dll))
    {
      LOGWF_WARN(L"InjectDllIntoPid: Dll = (%s) not exist", dll.c_str());
      throw std::exception("dll not exist!");
    }

    if(m_RunWDPath.empty())
    {
      throw std::exception("m_RunWDPath empty!");
    }

    std::wstring wstrRun;

    if(type == IMAGE_64BIT)
    {
      wstrRun = FileSystem::Path::Combine(m_RunWDPath, L"RunWD64.exe");
    }
    else
    {
      wstrRun = FileSystem::Path::Combine(m_RunWDPath, L"RunWD32.exe");
    }

    std::wstring wstrCmd;
    wstrCmd.append(L"\"").append(wstrRun).append(L"\" ");
    wstrCmd.append(L"\"").append(dll).append(L"\" ");
    wstrCmd.append(L"PID:").append(std::to_wstring(pid));

    return wstrCmd;
  }

  void InvokeRemoteInject(const WCHAR * wszCmdRaw)
  {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    LOGWF_INFO(L"InvokeRemoteInject: Cmd = (%s)", wszCmdRaw);

    memset((void*)&si, 0, sizeof(STARTUPINFO));
    memset((void*)&pi, 0, sizeof(PROCESS_INFORMATION));
    si.cb = sizeof(STARTUPINFO);

    int len = wcslen(wszCmdRaw);
    WCHAR * wszCmd = new WCHAR[len + 1];
    if(!wszCmd)
    {
      LOGWF_INFO(L"<< InvokeRemoteInject.Alloc wszCmd NG, size = %u", len + 1);
      return;
    }

    // Launch & Suspend
    wcscpy_s(wszCmd, len + 1, wszCmdRaw);

    // 先檢查條件過,才 Create & Suspend
    //!! wszCmdLine is in-out, can't be const string
    if(!CreateProcess(NULL,   // No module name (use command line).
      (LPWSTR)wszCmd, // Command line.
      NULL,             // Process handle not inheritable.
      NULL,             // Thread handle not inheritable.
      FALSE,            // Set handle inheritance to FALSE.
      0,
      NULL,             // Use parent's environment block.
      NULL,             // Use parent's starting directory.
      &si,              // Pointer to STARTUPINFO structure.
      &pi)             // Pointer to PROCESS_INFORMATION structure.
      )
    {
      LOGWF_INFO(L"InvokeRemoteInject.CreateProcess NG. dwErr = %u", GetLastError());
    }
    else
    {
      WaitForSingleObject(pi.hProcess, 1000);
      // Close process and thread handles.
      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
    }

    if(wszCmd)
      delete[] wszCmd;

    return;
  }

  void InjectDLLRunWD(const std::wstring &dll, const DWORD& pid)
  {
    try
    {
      InvokeRemoteInject(CreateCmdline(dll, pid).c_str());
    }
    catch(std::exception &e)
    {
      LOGAF_ERROR("Exception: %s!", e.what());
    }
  }

  //TODO: 64bit Injector直接注入32行程
  bool InjectDLLDirect(const std::wstring &dll, DWORD pid)
  {
    bool ret = false;

    if(pid == 0)
    {
      LOG("Pid can't be zero");
      return ret;
    }

    HANDLE process = NULL;
    process = OpenProcess(PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if(!process)
    {
      LOG("Open process NG, dwErr = %u", GetLastError());
      return ret;
    }

    ImageType type = GetBit(process);

    LOGAF_TRACE("Self64: %d, target64: %d", m_is64Process, (type == IMAGE_64BIT));

    if((m_is64Process && type == IMAGE_64BIT) || (!m_is64Process && type == IMAGE_32BIT))
    {
      if(Windows::RemoteInject(process, dll.c_str()))
      {
        ret = true;
      }
      else
      {
        LOGAF_ERROR("Direct Inject fail!");
      }
    }

    if(process)
      CloseHandle(process);

    return ret;
  }

  bool InjectDLL(const std::wstring &dll, const DWORD& pid)
  {
    if(InjectDLLDirect(dll, pid))
    {
    }
    else
    {
      InjectDLLRunWD(dll, pid);
    }
  }

private:
  bool m_is64OS;
  bool m_is64Process;
  std::wstring m_RunWDPath;
};