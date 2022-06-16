#ifndef _REGISTRY_H
#define _REGISTRY_H
#include <string>
#include <Windows.h>

namespace Registry
{
  //!@brief getValueWString
  //!  ���o�n���ɫ��wroot�U��key�Ȫ�value
  //!@param root A handle to an open registry key
  //!@param key The name of the registry subkey to be opened.
  //!@param value The name of the registry value.
  //!@return ���o���r��
  std::wstring GetValueWString( HKEY root , std::wstring key, std::wstring value );

  inline DWORD SetValueDWORD( HKEY root , const std::wstring &key,  const std::wstring &valueName, DWORD value )
  {
    HKEY hKey;
    DWORD dwErr = ERROR_SUCCESS;

    if (RegOpenKeyEx(root, key.c_str() , 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
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
};

#endif