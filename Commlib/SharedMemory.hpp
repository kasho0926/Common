#pragma once

#include <string>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>


class WinSharedMemory
{
public:
  WinSharedMemory()
  {
   
  }

  void Init(const std::wstring &name)
  {
    std::wstring wstrMutex = L"Global\\" + name;
    m_mutex = ::CreateMutex(NULL, FALSE, wstrMutex.c_str());

    if(m_mutex == NULL)
    {
      printf("CreateMutex error: %d\n", GetLastError());
      return;
    }
  }

private:
  HANDLE m_mapFile;
  LPCTSTR m_buffer;
};