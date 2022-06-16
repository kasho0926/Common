#pragma once
#ifndef _ASSEMBLY_H
#define _ASSEMBLY_H

#include "macros.h"
#include <Windows.h>
#include <DbgHelp.h>
#include "debug_log.h"
#include "structured_exception_handle.hpp"

#pragma comment( lib, "dbghelp.lib" )


namespace Assembly
{
  //!@brief PrintPESectionInfo
  //!  印出PE模組各section的資訊(section名稱、位置、大小)
  //!@param hModule PE模組之HANDLE
  void PrintPESectionInfo(HANDLE hModule);
  //!@brief GetPESectionInfo
  //!  得到PE模組中指定section名稱的位址與大小
  //!@param hModule PE模組之HANDLE
  //!@param sectionName section名稱
  //!@param begin 放找到的section的起始位址
  //!@param size 放找到的section的大小
  //!@return
  //!  FUNCTION_NOERROR
  //!  FUNCTION_ERROR
  int GetPESectionInfo(HANDLE hModule, const char * sectionName, DWORD &begin, DWORD &size);
  //!@brief ByteSearch
  //!  尋找目標範圍中有沒有指定的連續byte序列
  //!@param bytes 要尋找的連續byte序列
  //!@param bytesSize 連續byte序列的大小
  //!@param begin 查找範圍的起點
  //!@param size 查找範圍的大小
  //!@return
  //!  如果function成功，回傳找到的位址
  //!  否則回傳FUNCTION_ERROR
  DWORD ByteSearch(const char * bytes, const size_t bytesSize, const DWORD &begin , const DWORD &size);
  //!@brief GetCallAddress
  //!  將組語中，相對CALL的offset換算成實際的位址
  //!@param currAddr CALL指令的位址
  //!@param offset CALL指令的offset
  //!@return
  //!  CALL指令的目標絕對位址
  DWORD GetCallAddress(const DWORD & currAddr , const DWORD & offset);
  //!@brief DW2LE
  //!  將DWORD轉成用Little endian表示的BYTE陣列
  //!@param data 要轉換的DWORD
  //!@param result 指向轉換後的BYTE陣列
  //!@return
  //!  FUNCTION_NOERROR
  //!  FUNCTION_ERROR
  int DW2LE(const DWORD &data, BYTE *result);

  //沒用
  DWORD DWReverse( const DWORD &data );

  //沒用
  DWORD LE2DW(BYTE *b);
} //Assembly

#endif
