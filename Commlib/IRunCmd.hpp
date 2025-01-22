#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

class IRunCmd
{
public:
  DWORD DoRun();

protected:
};