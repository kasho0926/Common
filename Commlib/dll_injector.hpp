#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#include "debug_log.h"
namespace DllInjector
{
  int remoteInject( DWORD dwPID, LPCTSTR szDllPath )
  {
    HANDLE hProcess = NULL;
    HANDLE hThread = NULL;
    HMODULE hMod = NULL;
    LPVOID pRemoteBuf = NULL;
    DWORD dwBufSize = (DWORD)(_tcslen(szDllPath) + 1) * sizeof(TCHAR);
    LPTHREAD_START_ROUTINE pThreadProc;
    if( !(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)) )  // 取得目標程序的HANDLE
    {
      LOGWF_ERROR( L"OpenProcess failed!!!: %d, Error code: %d", dwPID, GetLastError() );
      return 1;
    }
    pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)szDllPath, dwBufSize, NULL);
    hMod = GetModuleHandle(L"kernel32.dll");
    pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryW");
    hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf,0, NULL); //在目標程序建一個thread來load目標dll
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    CloseHandle(hProcess);
    return 0;
  }
};