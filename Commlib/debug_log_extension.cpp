#include "debug_log_extension.h"

#include <Windows.h>

void DebugLog::ReadExtraSettings(const std::wstring & conf)
{
  g_dummy = GetPrivateProfileInt(L"DEBUG", L"log_size", 5, conf.c_str());
}