#include "stdafx.h"
#include "Argument.h"
#include <iostream>
#include "macros.h"
#include "debug_log.h"

using namespace std;

IArgument::IArgument()
{
} // IArgument()

IArgument::IArgument(int argc, _TCHAR * argv[])
{
  
  SetArg(argc, argv);
} // IArgument()

IArgument::~IArgument()
{
  m_arg.erase(m_arg.begin(), m_arg.end());
} // ~IArgument()

int IArgument::ParseArg()
{
  return 0;
}

int IArgument::SetArg(int argc, _TCHAR * argv[])
{
  LOGWF_TRACE( L" ARGC: %d", argc ); 
  wstring wstr;
  /// push all arg into vector
  for (int i = 0; i < argc; i++)
  {
    wstr = argv[i];
    m_arg.push_back(wstr);
  }
  return 0;
} // getFilename()