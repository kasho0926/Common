#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

enum RunRet
{
  RRET_OK = 0,
  RRET_CMD_NO_SUPPORT = 1,
  RRET_INVALID_PARAM = 2,
  RRET_EXCEPTION = 0xffffffff
};