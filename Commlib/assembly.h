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
  //!  �L�XPE�ҲզUsection����T(section�W�١B��m�B�j�p)
  //!@param hModule PE�Ҳդ�HANDLE
  void PrintPESectionInfo(HANDLE hModule);
  //!@brief getPESectionInfo
  //!  �o��PE�Ҳդ����wsection�W�٪���}�P�j�p
  //!@param hModule PE�Ҳդ�HANDLE
  //!@param sectionName section�W��
  //!@param begin ���쪺section���_�l��}
  //!@param size ���쪺section���j�p
  //!@return
  //!  FUNCTION_NOERROR
  //!  FUNCTION_ERROR
  int GetPESectionInfo(HANDLE hModule, const char * sectionName, ADDR &begin, DWORD &size);
  //!@brief byteSearch
  //!  �M��ؼнd�򤤦��S�����w���s��byte�ǦC
  //!@param bytes �n�M�䪺�s��byte�ǦC
  //!@param bytesSize �s��byte�ǦC���j�p
  //!@param begin �d��d�򪺰_�I
  //!@param size �d��d�򪺤j�p
  //!@return
  //!  �p�Gfunction���\�A�^�ǧ�쪺��}
  //!  �_�h�^��FUNCTION_ERROR
  ADDR ByteSearch(const char * bytes, const size_t bytesSize, const ADDR &begin, const DWORD &size);
  //!@brief getCallAddr
  //!  �N�ջy���A�۹�CALL��offset���⦨��ڪ���}
  //!@param currAddr CALL���O����}
  //!@param offset CALL���O��offset
  //!@return
  //!  CALL���O���ؼе����}
  ADDR GetCallAddress(const ADDR & currAddr, const DWORD & offset);
  //!@brief DW2LE
  //!  �NDWORD�ন��Little endian��ܪ�BYTE�}�C
  //!@param data �n�ഫ��DWORD
  //!@param result ���V�ഫ�᪺BYTE�}�C
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
