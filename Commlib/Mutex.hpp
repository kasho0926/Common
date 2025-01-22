#pragma once
#include <string>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

class WinMutex
{
public:
  WinMutex(const std::wstring &mutexName)
  {
    std::wstring wstrMutex = L"Global\\";
    wstrMutex += mutexName;
    m_mutex = ::CreateMutex(NULL, FALSE, wstrMutex.c_str());

    if(m_mutex == NULL)
    {
      printf("CreateMutex error: %d\n", GetLastError());
      return;
    }
  }

  HRESULT Lock()
  {
    HRESULT hr = E_FAIL;
    DWORD dRet = WaitForSingleObject(m_mutex, INFINITE);
    switch(dRet)
    {
    case WAIT_TIMEOUT:  // Time out
      break;
    case WAIT_OBJECT_0:  // Process over
      hr = S_OK;
      break;
    case WAIT_OBJECT_0 + 1: // Don't Know
      break;
    }
    return hr;
  }

  void Unlock()
  {
    ::ReleaseMutex(m_mutex);
  }

private:
  HANDLE m_mutex;
};