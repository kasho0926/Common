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

  for(int scn = 0; scn < pNtHdr->FileHeader.NumberOfSections; ++scn)
  {
    // Note: pSectionHdr->Name[] is 8 bytes long. If the scn name is 8 bytes long, ->Name[] will
    // not be nul-terminated. For this reason, copy it to a local buffer that's nul-terminated
    // to be sure we only print the real scn name, and no extra garbage beyond it.
    strncpy_s(scnName, _countof(scnName), (const char*)pSectionHdr->Name, sizeof(pSectionHdr->Name));
    LOGAF_TRACE("  Section %3d: %p...%p %-10s (%u bytes)\n",
      scn,
      imageBase + pSectionHdr->VirtualAddress,
      imageBase + pSectionHdr->VirtualAddress + pSectionHdr->Misc.VirtualSize - 1,
      scnName,
      pSectionHdr->Misc.VirtualSize);

    ++pSectionHdr;
  }
}

int Assembly::GetPESectionInfo(HANDLE hModule, const char * sectionName, ADDR &begin, DWORD &size)
{
  // get the location of the module's IMAGE_NT_HEADERS structure
  IMAGE_NT_HEADERS *pNtHdr = ImageNtHeader(hModule);
  if(pNtHdr == NULL)
  {
    LOGWF_ERROR(L"ImageNtHeader failed!");
    return FUNCTION_ERROR;
  }

  // section table immediately follows the IMAGE_NT_HEADERS
  IMAGE_SECTION_HEADER *pSectionHdr = (IMAGE_SECTION_HEADER *)(pNtHdr + 1);
  const char* imageBase = (const char*)hModule;
  char scnName[sizeof(pSectionHdr->Name) + 1];
  scnName[sizeof(scnName) - 1] = '\0'; // enforce nul-termination for scn names that are the whole length of pSectionHdr->Name[]

  for(int scn = 0; scn < pNtHdr->FileHeader.NumberOfSections; ++scn)
  {
    // Note: pSectionHdr->Name[] is 8 bytes long. If the scn name is 8 bytes long, ->Name[] will
    // not be nul-terminated. For this reason, copy it to a local buffer that's nul-terminated
    // to be sure we only print the real scn name, and no extra garbage beyond it.
    if(strncpy_s(scnName, _countof(scnName), (const char*)pSectionHdr->Name, sizeof(pSectionHdr->Name)) != 0)
    {
      LOGWF_ERROR(L"strncpy_s failed!");
      return FUNCTION_ERROR;
    }
    LOGAF_TRACE("Section Name: %s", scnName);
    if(strcmp(sectionName, scnName) == 0)
    {
      begin = (ADDR)imageBase + pSectionHdr->VirtualAddress;
      size = pSectionHdr->Misc.VirtualSize;
      LOGAF_TRACE("Section Name: %s, begin: %p, size: %d", scnName, begin, size);
      return FUNCTION_NOERROR;
    }
    ++pSectionHdr;
  }
  return FUNCTION_ERROR;
}

ADDR Assembly::ByteSearch(const char * str, const size_t strSize, const ADDR &begin, const DWORD &size)
{
  const char* memory = (char*)(begin); //指向目標範圍
  ADDR dwAddr = 0; //目標位址
  __try
  {
    dwAddr = (ADDR)std::search(memory, memory + size, str, str + strSize);
  }
  SEHEXCEPT; //處理結構化例外

  if(dwAddr == ((ADDR)memory + size))
  {
    LOGWF_TRACE(L"[%s]byteSearch failed!!", __FUNCTIONW__);
    return FUNCTION_ERROR;
  }
  else
  {
    LOGWF_TRACE(L"[%s]byteSearch succeed!!:%p", __FUNCTIONW__, dwAddr);
    return dwAddr;
  }
}

ADDR Assembly::GetCallAddress(const ADDR & currAddr, const DWORD & offset)
{
  ADDR callOffset = offset;

#ifdef _WIN64
  if(offset >= 0xff000000)
  {
    callOffset = 0xffffffff00000000 | offset;
  }
#endif

  LOGWF_TRACE(L"currAddr:%p, offset:%p", currAddr, callOffset);
  return (currAddr + 5 + callOffset); // EIP+OFFSET+5(push instruction size)
}