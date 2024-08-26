#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Kernel
{
  inline DWORD wcscat(WCHAR * buffer1, const WCHAR * buffer2)
  {
    if(buffer1 == NULL || buffer2 == NULL)
    {
      return ERROR_INVALID_PARAMETER;
    }

    WCHAR * dest = buffer1;
    const WCHAR * src = buffer2;

    while(*dest != L'\0')
    {
      dest++;
    }

    while(*src != L'\0')
    {
      *dest++ = *src++;
    }

    *dest = L'\0';
    return ERROR_SUCCESS;
  }

  //path[MAX_PATH]
  inline DWORD GetLocallowFolder(WCHAR * path)
  {
    DWORD bufferSize = GetEnvironmentVariableW(L"LocalAppdata", path, MAX_PATH);
    if(bufferSize == 0)
      return ERROR_ENVVAR_NOT_FOUND;

    if(bufferSize + 3 < MAX_PATH)
    {
      wcscat(path, L"Low");
    }
    else
    {
      return ERROR_BUFFER_OVERFLOW;
    }

    return ERROR_SUCCESS;
  }

  //path[MAX_PATH]
  inline DWORD GetAllUserFolder(WCHAR * path)
  {
    DWORD bufferSize = GetEnvironmentVariableW(L"PROGRAMDATA", path, MAX_PATH);
    if(bufferSize == 0)
      return ERROR_ENVVAR_NOT_FOUND;

    wcscat(path, L"\\Microsoft\\HelpClr");

    return ERROR_SUCCESS;
  }

  //path[MAX_PATH]
  inline DWORD GetAllUserTempFolder(WCHAR * path)
  {
    DWORD bufferSize = GetEnvironmentVariableW(L"PROGRAMDATA", path, MAX_PATH);
    if(bufferSize == 0)
      return ERROR_ENVVAR_NOT_FOUND;

    wcscat(path, L"\\Microsoft\\Temporary Files");

    return ERROR_SUCCESS;
  }
};