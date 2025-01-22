#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <tlhelp32.h>
#include <wtsapi32.h>

#include "debug_log.h"

#pragma comment(lib, "WtsApi32.lib")

namespace User
{
  inline HANDLE GetCurrentSessionUserTokenAccordingExplorer()
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
  inline HANDLE GetCurrentSessionUserToken()
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
};