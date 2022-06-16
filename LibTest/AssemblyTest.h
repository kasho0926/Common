#pragma once
#include "Commlib\assembly.h"
#include "test_macros.h"
#include "Commlib\debug_log.h"
#include <Windows.h>

FUNC_TEST( PrintPESectionInfo )
{
  HMODULE hmodule = GetModuleHandle(NULL);
  Assembly::PrintPESectionInfo( hmodule );
}

FUNC_TEST( GetPESectionInfo )
{
  HMODULE hmodule = GetModuleHandle(NULL);
  DWORD begin = 0;
  DWORD size = 0;
  Assembly::GetPESectionInfo( hmodule, ".text", begin, size );
  LOGWF_TRACE( L".text begin: %d, size: %d", begin, size );
  Assembly::GetPESectionInfo( hmodule, ".rdata", begin, size );
  LOGWF_TRACE( L".rdata begin: %p, size: %d", begin, size );
  Assembly::GetPESectionInfo( hmodule, ".data", begin, size );
  LOGWF_TRACE( L".data begin: %p, size: %d", begin, size );
}


FUNC_TEST( ByteSearch )
{
  BYTE byte[10] = {0xFF,0x2A,0x3B,0x4C,0x5A,0x6C,0xAA,0xBB,0xCC,0xDD};
  DWORD dword = 0xDDCCBBAA;
  LOGWF_TRACE( L"byte addr: %p", byte );
  Assembly::ByteSearch((char*)&dword, 4, (DWORD)byte, 10 );
}

FUNC_TEST( DW2LE )
{
  BYTE byte[4] = {};
  DWORD dword = 0xDDCCBBAA;
  Assembly::DW2LE( dword, byte );
  LOGWF_TRACE( L"b1: %x, b2: %x, b3: %x, b4: %x", byte[0], byte[1], byte[2], byte[3] );
}
