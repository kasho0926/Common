#include "stdafx.h"
#include "assembly.h"
#include "macros.h"
#include "debug_log.h"
#include "string_utils.hpp"
#include <algorithm>

void Assembly::PrintPESectionInfo(HANDLE hModule)
{
  // get the location of the module's IMAGE_NT_HEADERS structure
  IMAGE_NT_HEADERS *pNtHdr = ImageNtHeader(hModule);

  // section table immediately follows the IMAGE_NT_HEADERS
  IMAGE_SECTION_HEADER *pSectionHdr = (IMAGE_SECTION_HEADER *)(pNtHdr + 1);

  const char* imageBase = (const char*)hModule;
  char scnName[sizeof(pSectionHdr->Name) + 1];
  scnName[sizeof(scnName) - 1] = '\0'; // enforce nul-termination for scn names that are the whole length of pSectionHdr->Name[]

  for (int scn = 0; scn < pNtHdr->FileHeader.NumberOfSections; ++scn)
  {
    // Note: pSectionHdr->Name[] is 8 bytes long. If the scn name is 8 bytes long, ->Name[] will
    // not be nul-terminated. For this reason, copy it to a local buffer that's nul-terminated
    // to be sure we only print the real scn name, and no extra garbage beyond it.
    strncpy_s(scnName,_countof(scnName), (const char*)pSectionHdr->Name, sizeof(pSectionHdr->Name));
    LOGAF_TRACE("  Section %3d: %p...%p %-10s (%u bytes)\n",
      scn,
      imageBase + pSectionHdr->VirtualAddress,
      imageBase + pSectionHdr->VirtualAddress + pSectionHdr->Misc.VirtualSize - 1,
      scnName,
      pSectionHdr->Misc.VirtualSize);

    ++pSectionHdr;
  }
}

int Assembly::GetPESectionInfo(HANDLE hModule, const char * sectionName, DWORD &begin, DWORD &size)
{
  // get the location of the module's IMAGE_NT_HEADERS structure
  IMAGE_NT_HEADERS *pNtHdr = ImageNtHeader(hModule);
  if( pNtHdr == NULL )
  {
    LOGWF_ERROR(L"ImageNtHeader failed!");
    return 1;
  }

  // section table immediately follows the IMAGE_NT_HEADERS
  IMAGE_SECTION_HEADER *pSectionHdr = (IMAGE_SECTION_HEADER *)(pNtHdr + 1);
  const char* imageBase = (const char*)hModule;
  char scnName[sizeof(pSectionHdr->Name) + 1];
  scnName[sizeof(scnName) - 1] = '\0'; // enforce nul-termination for scn names that are the whole length of pSectionHdr->Name[]

  for (int scn = 0; scn < pNtHdr->FileHeader.NumberOfSections; ++scn)
  {
    // Note: pSectionHdr->Name[] is 8 bytes long. If the scn name is 8 bytes long, ->Name[] will
    // not be nul-terminated. For this reason, copy it to a local buffer that's nul-terminated
    // to be sure we only print the real scn name, and no extra garbage beyond it.
    if( strncpy_s(scnName,_countof(scnName), (const char*)pSectionHdr->Name, sizeof(pSectionHdr->Name)) != 0 )
    {
      LOGWF_ERROR( L"strncpy_s failed!" );
      return 1;
    }
    LOGAF_TRACE("Section Name: %s",scnName );
    if( strcmp( sectionName, scnName ) == 0 )
    {
      begin = (DWORD)imageBase + pSectionHdr->VirtualAddress;
      size =  pSectionHdr->Misc.VirtualSize;
      LOGAF_TRACE("Section Name: %s, begin: %p, size: %d",scnName, begin , size );
      return 0;
    }
    ++pSectionHdr;
  }
  return 1;
}

DWORD Assembly::ByteSearch(const char * str, const size_t strSize, const DWORD &begin , const DWORD &size)
{
  const char* memory =(char*)(begin); //指向目標範圍
  DWORD dwAddr =0; //目標位址
  __try
  {
    dwAddr = (DWORD)std::search(memory, memory+size, str, str + strSize);
  }
  SEHEXCEPT; //處理結構化例外

  if ( dwAddr == ((DWORD)memory+size) )
  {
    LOGWF_TRACE(L"ByteSearch failed!!");
    return 0;
  }
  else
  {
    LOGWF_TRACE(L"ByteSearch succeed!!:%p", dwAddr);
    return dwAddr;
  }
}

DWORD Assembly::GetCallAddress(const DWORD & currAddr , const DWORD & offset)
{
  LOGWF_TRACE(L"currAddr:%p, offset:%p", currAddr, offset);
  return (currAddr + 5 + offset); // EIP+OFFSET+5
}

int Assembly::DW2LE(const DWORD &data, BYTE *result)
{
  __try
  {
    result[0] = (BYTE)data;
    result[1] = (BYTE)(((DWORD)data >> 8) & 0xFF);
    result[2] = (BYTE)(((DWORD)data >> 16) & 0xFF);
    result[3] = (BYTE)(((DWORD)data >> 24) & 0xFF);
  }
  SEHEXCEPT;

  return 0;
}

//沒用
DWORD Assembly::DWReverse( const DWORD &data )
{
  DWORD res ;
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

//沒用
DWORD Assembly::LE2DW(BYTE *b)
{
  return (DWORD)((b[0]) | (b[1] << 8) | (b[2] << 16) | (b[3] << 24));
}