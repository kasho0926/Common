#pragma once
#ifndef _ASSEMBLY_H
#define _ASSEMBLY_H

#include "macros.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <DbgHelp.h>
#include "debug_log.h"
#include "structured_exception_handle.hpp"

#pragma comment( lib, "dbghelp.lib" )

#ifdef _WIN64
typedef DWORD64                  ADDR;
#else
typedef DWORD                  ADDR;
#endif

//!@class Assembly assembly.h
namespace Assembly
{
  //!@brief printPESectionInfo
  //!  印出PE模組各section的資訊(section名稱、位置、大小)
  //!@param hModule PE模組之HANDLE
  void PrintPESectionInfo(HANDLE hModule);
  //!@brief getPESectionInfo
  //!  得到PE模組中指定section名稱的位址與大小
  //!@param hModule PE模組之HANDLE
  //!@param sectionName section名稱
  //!@param begin 放找到的section的起始位址
  //!@param size 放找到的section的大小
  //!@return
  //!  FUNCTION_NOERROR
  //!  FUNCTION_ERROR
  int GetPESectionInfo(HANDLE hModule, const char * sectionName, ADDR &begin, DWORD &size);
  //!@brief byteSearch
  //!  尋找目標範圍中有沒有指定的連續byte序列
  //!@param bytes 要尋找的連續byte序列
  //!@param bytesSize 連續byte序列的大小
  //!@param begin 查找範圍的起點
  //!@param size 查找範圍的大小
  //!@return
  //!  如果function成功，回傳找到的位址
  //!  否則回傳FUNCTION_ERROR
  ADDR ByteSearch(const char * bytes, const size_t bytesSize, const ADDR &begin, const DWORD &size);
  //!@brief getCallAddr
  //!  將組語中，相對CALL的offset換算成實際的位址
  //!@param currAddr CALL指令的位址
  //!@param offset CALL指令的offset
  //!@return
  //!  CALL指令的目標絕對位址
  ADDR GetCallAddress(const ADDR & currAddr, const DWORD & offset);
  //!@brief DW2LE
  //!  將DWORD轉成用Little endian表示的BYTE陣列
  //!@param data 要轉換的DWORD
  //!@param result 指向轉換後的BYTE陣列
  //!@return
  //!  FUNCTION_NOERROR
  //!  FUNCTION_ERROR
  inline int ADDR2LE(const ADDR &data, BYTE *result)
  {
    __try
    {
      for(int i = 0; i < sizeof(data); i++)
      {
        result[i] = (BYTE)((data >> i * 8) & 0xFF);
      }
    }
    SEHEXCEPT;

    return FUNCTION_NOERROR;
  }

  inline DWORD DWReverse(const DWORD &data)
  {
    DWORD res;
    BYTE * result = (BYTE*)&res;
    __try
    {
      result[3] = (BYTE)data;
      result[2] = (BYTE)(((DWORD)data >> 8) & 0xFF);
      result[1] = (BYTE)(((DWORD)data >> 16) & 0xFF);
      result[0] = (BYTE)(((DWORD)data >> 24) & 0xFF);
    }
    SEHEXCEPT;

    return res;
  }

  inline DWORD LE2DW(BYTE *b)
  {
    return (DWORD)((b[0]) | (b[1] << 8) | (b[2] << 16) | (b[3] << 24));
  }
};

#endif
