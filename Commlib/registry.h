#ifndef _REGISTRY_H
#define _REGISTRY_H
#include <string>
#include <Windows.h>

namespace Registry
{
  //!@brief getValueWString
  //!  取得登錄檔指定root下的key值的value
  //!@param root A handle to an open registry key
  //!@param key The name of the registry subkey to be opened.
  //!@param value The name of the registry value.
  //!@return 取得的字串
  std::wstring GetValueWString(HKEY root, const std::wstring &key, const std::wstring &value);

  inline DWORD SetValueDWORD(HKEY root, const std::wstring &key, const std::wstring &valueName, DWORD value)
  {
    HKEY hKey;
    DWORD dwErr = ERROR_SUCCESS;

    if(RegOpenKeyEx(root, key.c_str(), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    {
      dwErr = GetLastError();
      return dwErr;
    }

    if(RegSetValueExW(hKey, valueName.c_str(), 0, REG_DWORD, (BYTE*)&value, sizeof(DWORD)) != ERROR_SUCCESS)
    {
      dwErr = GetLastError();
    }

    RegCloseKey(hKey);
    return dwErr;
  }

  inline DWORD GetValueDWORD(HKEY root, const std::wstring &key, const std::wstring &value)
  {
    DWORD ret = -1;

    HKEY hKey;

    if(RegOpenKeyEx(root, key.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
      return ret;

    DWORD dwBufferSize(sizeof(DWORD));

    DWORD dwRet = RegQueryValueEx(hKey,
      value.c_str(),
      NULL,
      NULL,
      (LPBYTE)&ret,
      &dwBufferSize);

    RegCloseKey(hKey);
    return ret;
  }
};

#endif