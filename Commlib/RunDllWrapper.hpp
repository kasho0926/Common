#pragma once
#include "DllWrapper.hpp"
#include "RunDef.hpp"

// Dll Export ���q�Ψ禡�����Ы��A
typedef DWORD(__stdcall * PF_Run) (
  DWORD	dwCmdID,	// �R�O���N�X, �ѹ�@ Dll ���W��ѩw�q
  BYTE *	pParamIn,	// ���V�s���ڶǤJ���ѼƭȪ��O���餧����
  DWORD	dwSizeIn,	// pParamIn �ҫ����O���餧�j�p
  BYTE *	pParamOut,	// ���V�s��N�n�ǥX���ѼƭȪ��O���餧����
  DWORD *	pdwSizeOut	// ���V DWORD �ܼƪ�����, ��J�ɫ��w pParamOut �ҫ����O����j�p, �I�s���J��ڪ��ǥX������ pParamOut ���j�p
  );

class RunDllWrapper : public DllWrapper
{
public:
  void Init(const std::wstring & path) override
  {
    pRun = NULL;
    DllWrapper::Init(path);

    if(hDll)
    {
      pRun = (PF_Run)GetProcAddress(hDll, "Run");
      if(!pRun)
      {
        LOGAF_ERROR("GetProcAddress Fail!");
      }
    }
  }

  DWORD DoRun(DWORD	dwCmdID,
    BYTE *	pParamIn,
    DWORD	dwSizeIn,
    BYTE *	pParamOut,
    DWORD *	pdwSizeOut)
  {
    if(pRun)
    {
      return pRun(dwCmdID, pParamIn, dwSizeIn, pParamOut, pdwSizeOut);
    }

    return (DWORD)RRET_EXCEPTION;
  }

protected:
  PF_Run pRun;
};