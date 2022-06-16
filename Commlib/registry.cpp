#include "registry.h"
#include <Windows.h>

namespace Registry
{
  std::wstring GetValueWString( HKEY root , std::wstring key, std::wstring value )
  {
    HKEY hKey;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, key.c_str() , 0, KEY_READ, &hKey) != ERROR_SUCCESS)
      return L"";

    DWORD cbData = MAX_PATH;
    BYTE PerfData[MAX_PATH];

    DWORD dwRet = RegQueryValueEx(hKey,
      TEXT("FileSavePath"),
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