#ifndef _INI_HANDLE_H
#define _INI_HANDLE_H
#include <string>

/*!@class IniUtil ini_handle.h
* @brief Ū��ini�ɤ����ܼ�
* Example:
*   IniUtil ih;
*   ih.setIniFile( L"clinet.ini" );
*   ih.getIniString( L"ServerInfo", L"xfort_system_id" );
*/
class IniUtil
{
public:
  //!@brief getIniString
  //!  ��oini�ɤ��S�wsection���A�S�wkey���r��A�̤j���׬�MAX_PATH
  //!@param section
  //!  �ؼЪ�section
  //!@param key
  //!  �ؼЪ�key
  //!@return �bsection�����wkey����
  std::wstring GetIniString(const std::wstring &fullpath,const std::wstring &section, const std::wstring &key);
};

#endif