#pragma once
#include <string>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include "debug_log.h"

class DllWrapper
{
public:
  virtual void Init(const std::wstring & path)
  {
    hDll = LoadLibraryW(path.c_str());
    if(!hDll)
    {
      LOGAF_ERROR("LoadLibraryW %S Fail!: %d", path.c_str(), GetLastError());
    }
  }

  void Free()
  {
    if(hDll)
    {
      FreeLibrary(hDll);
      hDll = NULL;
    }
  }

protected:
  HMODULE hDll;
};