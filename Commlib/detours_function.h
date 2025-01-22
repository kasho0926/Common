#ifndef _DETOURS_FUNCTION_H
#define _DETOURS_FUNCTION_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <string>

#define DETOUR_FUNCTION(func) if(real_##func != NULL){LOGAF_INFO("Hooking " #func);detours::detourOneFunction(&(PVOID&)real_##func, my_##func);} else{LOGAF_ERROR("%s null", #func);}
namespace detours
{
  //!@brief detourOneFunction
  //!  detour一個函式，含完整的transition流程
  //!@param ppTrueFunc target function
  //!@param pMyFunc my function
  //!@return 
  //!  FUNCTION_NOERROR
  //!  FUNCTION_ERROR
  int detourOneFunction(PVOID *ppTrueFunc, PVOID pMyFunc);
  //!@brief detourOneFunction
  //!  detour一個函式，不含transtition
  //!@param ppTrueFunc target function
  //!@param pMyFunc my function
  //!@return 
  //!  FUNCTION_NOERROR
  //!  FUNCTION_ERROR
  int detourFunction(PVOID *ppTrueFunc, PVOID pMyFunc);
  //!@brief detourOneFunction
  //!  detour一個函式
  //!@param trueFuncSymbol target function symbol
  //!@param dllname dll name
  //!@param ppTrueFunc target function
  //!@param pMyFunc my function
  //!@return 
  //!  FUNCTION_NOERROR
  //!  FUNCTION_ERROR
  int detourFunction(std::wstring trueFuncSymbol , std::wstring dllname, PVOID pMyFunc, PVOID& oldFunc);
  //!@brief detourOneFunction
  //!  detour一個函式
  //!@param trueFuncOffset target function offset
  //!@param dllname dll name
  //!@param ppTrueFunc target function
  //!@param pMyFunc my function
  //!@return 
  //!  FUNCTION_NOERROR
  //!  FUNCTION_ERROR
  int detourFunction(DWORD trueFuncOffset, std::wstring dllname, PVOID pMyFunc, PVOID& oldFunc);
  //!@brief detourOneFunction
  //!  detour一個函式
  //!@param trueFuncAddress target function address
  //!@param ppTrueFunc target function
  //!@param pMyFunc my function
  //!@return 
  //!  FUNCTION_NOERROR
  //!  FUNCTION_ERROR
  int detourFunction(DWORD trueFuncAddress, PVOID pMyFunc, PVOID& oldFunc);
  //detach 沒用
  int detachFunction(PVOID *ppTrueFunc, PVOID pMyFunc);
}

#endif