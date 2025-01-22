#include "debug_log_extension.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

void DebugLog::ReadExtraSettings(const std::wstring & conf)
{
  g_dummy = GetPrivateProfileInt(L"DEBUG", L"log_size", 5, conf.c_str());
}