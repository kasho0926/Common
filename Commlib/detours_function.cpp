#include "stdafx.h"
#include "detours_function.h"
#include "Detour3.0\include\detours.h"
#include "string_utils.hpp"
#include "structured_exception_handle.hpp"
#include <sstream>

#ifdef _M_X64

#pragma comment( lib, "..\\..\\Detour3.0\\lib.X64\\detours.lib" )
#else
#pragma comment( lib, "..\\..\\Detour3.0\\lib.X86\\detours.lib" )
#endif

int detours::detachFunction(PVOID *ppTrueFunc, PVOID pMyFunc)
{
  LOGWF_TRACE( L"DetachDetourFuncion!!!" );
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  DetourDetach(ppTrueFunc, pMyFunc);
  DetourTransactionCommit();
  return 0;
}

int detours::detourOneFunction(PVOID *ppTrueFunc, PVOID pMyFunc)
{
  LOGWF_TRACE( L"DetourFuncion!!!" );
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());

  if( detourFunction( ppTrueFunc, pMyFunc) == 1 )
  {
    return 1;
  }
  DetourTransactionCommit();

  return 0;
}

int detours::detourFunction(PVOID *ppTrueFunc, PVOID pMyFunc)
{
  LOGWF_TRACE( L"DetourTarget: %p , MyFunction: %p ", ppTrueFunc, pMyFunc );

  __try
  {
    if (DetourAttach(ppTrueFunc, pMyFunc) == NO_ERROR)
    {
      LOGWF_TRACE(L"DetourAttach Succeed!!");
      return 0;
    }
    else
    {
      LOGWF_ERROR(L"DetourAttach Failed!!");
      return 1;
    }
  }
  SEHEXCEPT;
}
int detours::detourFunction(std::wstring trueFuncSymbol, std::wstring dllname, PVOID pMyFunc, PVOID& oldFunc)
{
  std::string cFuncSymbol = StringUtils::Convert::Utf16WideCharToAnsiMultiByte(trueFuncSymbol.c_str());
  std::string cDllname = StringUtils::Convert::Utf16WideCharToAnsiMultiByte(dllname.c_str());
  oldFunc = DetourFindFunction(cDllname.c_str(), cFuncSymbol.c_str()); // 用Dll名稱與SYMBOL尋找目標函式位址
  LOGWF_TRACE( L"Old function address: %p", oldFunc);
  if (oldFunc == NULL)
  {
    LOGWF_ERROR( L"DetourFindFunction Error!!!" );
    return 1;
  }

  return detourFunction(&(PVOID&)oldFunc, pMyFunc);
}

int detours::detourFunction(DWORD trueFuncOffset, std::wstring dllname, PVOID pMyFunc, PVOID& oldFunc)
{
  HMODULE hDll = GetModuleHandle(dllname.c_str()); // 取得目標函式所在的dll HMODULE
  if (hDll == NULL)
  {
    LOGWF_ERROR(L"GetModuleHandle Failed!!");
    return 1;
  }

  oldFunc = (PVOID)((DWORD)hDll + (DWORD)trueFuncOffset); // 計算函式位址=dll起始位址+offset

  LOGWF_TRACE(L"hDll: %p, trueFuncOffset: %p, OldFunc: %p");

  return detourFunction(&(PVOID&)oldFunc, pMyFunc);
}

int detours::detourFunction(DWORD trueFuncOffset, PVOID pMyFunc, PVOID& oldFunc)
{
  return detourFunction(&(PVOID&)oldFunc, pMyFunc);
}