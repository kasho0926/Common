#include "registry.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace Registry
{
  std::wstring GetValueWString( HKEY root , const std::wstring &key, const std::wstring &value )
  {
    HKEY hKey;

    if (RegOpenKeyEx(root, key.c_str() , 0, KEY_READ, &hKey) != ERROR_SUCCESS)
      return L"";

    DWORD cbData = MAX_PATH;
    BYTE PerfData[MAX_PATH];

    DWORD dwRet = RegQueryValueEx(hKey,
      value.c_str(),
      NULL,
      NULL,
      PerfData,
      &cbData);

    if (dwRet == ERROR_SUCCESS)
    {
      std::wstring wstr = (WCHAR*)PerfData;
      RegCloseKey(hKey);
      return wstr;
    }

    RegCloseKey(hKey);
    return L"";
  }
}