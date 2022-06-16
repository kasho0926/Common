#ifndef _INI_HANDLE_H
#define _INI_HANDLE_H
#include <string>

/*!@class IniUtil ini_handle.h
* @brief 讀取ini檔中的變數
* Example:
*   IniUtil ih;
*   ih.setIniFile( L"clinet.ini" );
*   ih.getIniString( L"ServerInfo", L"xfort_system_id" );
*/
class IniUtil
{
public:
  //!@brief getIniString
  //!  獲得ini檔內特定section中，特定key的字串，最大長度為MAX_PATH
  //!@param section
  //!  目標的section
  //!@param key
  //!  目標的key
  //!@return 在section中指定key的值
  std::wstring GetIniString(const std::wstring &fullpath,const std::wstring &section, const std::wstring &key);
};

#endif