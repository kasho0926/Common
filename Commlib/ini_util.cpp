#include "stdafx.h"
#include "ini_util.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <fstream>
#include <iostream>
#include "macros.h"
#include "debug_log.h"

std::wstring IniUtil::GetIniString(const std::wstring &fullpath, const std::wstring &section, const std::wstring &key)
{
  std::wstring wstr;

  WCHAR buffer[MAX_PATH] = L"";

  if(GetPrivateProfileStringW(section.c_str(), key.c_str(), L"", buffer, MAX_PATH, fullpath.c_str()))
  {
    wstr = buffer;
  }
  else
  {
    LOGAF_ERROR("ErrorCode: %p", GetLastError());
  }

  if(wstr.empty()) /// 檢查key存不存在
  {
    LOGWF_WARN(L"In section: %s,  key: %s not found!!!", section.c_str(), key.c_str());
  }

  return wstr;
}