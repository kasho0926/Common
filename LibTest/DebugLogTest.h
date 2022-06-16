#pragma once

#pragma once
#include "Commlib\debug_log.h"
#include "test_macros.h"
#include <Windows.h>

FUNC_TEST( LogMessageW  )
{
  std::wstring wstr = L"wstr";
  //DebugLog::logMessageW(((std::wstring)L"FUNCTION: %s: " +L"this is str:%s").c_str(), __FUNCTIONW__, wstr.c_str());
  LOGWT( L"this is a str: %s", wstr.c_str() ); 
}

FUNC_TEST( getValueWString  )
{
  // L"Software\\Tencent\\WeChat\\FileSavePath"
  std::wstring wstr = Registry::getValueWString( HKEY_CURRENT_USER, L"Software\\Tencent\\WeChat", L"FileSavePath" );
  LOGWF_TRACE( L"%s", wstr.c_str() );
}


FUNC_TEST( GetMyDocument  )
{
  //components.xml
  std::wcout << FileUtil::GetMyDocument() << std::endl;
}