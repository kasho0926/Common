#pragma once
#include "DllWrapper.hpp"
#include "RunDef.hpp"

// Dll Export 的通用函式之指標型態
typedef DWORD(__stdcall * PF_Run) (
  DWORD	dwCmdID,	// 命令的代碼, 由實作 Dll 的規格書定義
  BYTE *	pParamIn,	// 指向存放實際傳入的參數值的記憶體之指標
  DWORD	dwSizeIn,	// pParamIn 所指的記憶體之大小
  BYTE *	pParamOut,	// 指向存放將要傳出的參數值的記憶體之指標
  DWORD *	pdwSizeOut	// 指向 DWORD 變數的指標, 輸入時指定 pParamOut 所指的記憶體大小, 呼叫後填入實際的傳出的有效 pParamOut 的大小
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