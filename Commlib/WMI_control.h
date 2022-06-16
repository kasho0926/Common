#pragma once

#define _WIN32_DCOM
#include <comdef.h>
#include <Wbemidl.h>
#include <atlbase.h>
#include <iostream>

class WMIControl
{
public:
  WMIControl(void);
  ~WMIControl(void);
  int init();
};

