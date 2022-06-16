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
  //!  �L�XPE�ҲզUsection����T(section�W�١B��m�B�j�p)
  //!@param hModule PE�Ҳդ�HANDLE
  void PrintPESectionInfo(HANDLE hModule);
  //!@brief GetPESectionInfo
  //!  �o��PE�Ҳդ����wsection�W�٪���}�P�j�p
  //!@param hModule PE�Ҳդ�HANDLE
  //!@param sectionName section�W��
  //!@param begin ���쪺section���_�l��}
  //!@param size ���쪺section���j�p
  //!@return
  //!  FUNCTION_NOERROR
  //!  FUNCTION_ERROR
  int GetPESectionInfo(HANDLE hModule, const char * sectionName, DWORD &begin, DWORD &size);
  //!@brief ByteSearch
  //!  �M��ؼнd�򤤦��S�����w���s��byte�ǦC
  //!@param bytes �n�M�䪺�s��byte�ǦC
  //!@param bytesSize �s��byte�ǦC���j�p
  //!@param begin �d��d�򪺰_�I
  //!@param size �d��d�򪺤j�p
  //!@return
  //!  �p�Gfunction���\�A�^�ǧ�쪺��}
  //!  �_�h�^��FUNCTION_ERROR
  DWORD ByteSearch(const char * bytes, const size_t bytesSize, const DWORD &begin , const DWORD &size);
  //!@brief GetCallAddress
  //!  �N�ջy���A�۹�CALL��offset���⦨��ڪ���}
  //!@param currAddr CALL���O����}
  //!@param offset CALL���O��offset
  //!@return
  //!  CALL���O���ؼе����}
  DWORD GetCallAddress(const DWORD & currAddr , const DWORD & offset);
  //!@brief DW2LE
  //!  �NDWORD�ন��Little endian��ܪ�BYTE�}�C
  //!@param data �n�ഫ��DWORD
  //!@param result ���V�ഫ�᪺BYTE�}�C
  //!@return
  //!  FUNCTION_NOERROR
  //!  FUNCTION_ERROR
  int DW2LE(const DWORD &data, BYTE *result);

  //�S��
  DWORD DWReverse( const DWORD &data );

  //�S��
  DWORD LE2DW(BYTE *b);
} //Assembly

#endif
