#pragma once
#include <sstream>
#include <exception>
#include <fstream>
#include <string>

#include "RunDllWrapper.hpp"

class FZipWrapper :public RunDllWrapper
{
public:
  enum RunCmdCode
  {
    FZIP_INIT = 0x10,
    FZIP_FREE = 0x11,
    FZIP_ENCRYPT_FILE = 0x05010002,
    FZIP_GET_HASH = 0x05010003
  };

  static FZipWrapper& Instance() {
    static FZipWrapper instance;
    return instance;
  }

  FZipWrapper(FZipWrapper const&) = delete;
  void operator=(FZipWrapper const&) = delete;

  void Init()
  {
    RunDllWrapper::Init(L"FZIP.dll");
  }

  void Encrypt(const std::wstring & src, const std::wstring & dst)
  {
    AUTOLOG;
    DWORD code = RRET_OK;

    std::wstringstream wss;
    wss.write(src.c_str(), src.length());
    wss.write(L"\0", 1);
    wss.write(dst.c_str(), dst.length());
    wss.write(L"\0\0", 2);

    std::wstring input = wss.str();
    LOGAF("Input: %S\r\n", input.c_str());

    if((code = DoRun(FZIP_ENCRYPT_FILE, (BYTE*)input.c_str(), input.length() * 2, NULL, NULL)) != RRET_OK)
    {
      DeleteFileW(dst.c_str());
      throw std::exception((std::string("Encrypt Fail: ") + std::to_string(code)).c_str());
    }

    return;
  }

  std::string GetHash(const std::wstring & src)
  {
    AUTOLOG;

    std::string ret(1024, '\0');
    DWORD size = ret.length();

    DWORD code = RRET_OK;

    std::wstring filename = src;
    filename.append(L"\0");

    LOGAF_DEBUG("Input: %S", filename.c_str());

    if((code = DoRun(FZIP_GET_HASH, (BYTE*)filename.c_str(), filename.length() * 2, (BYTE*)&ret[0], &size)) != RRET_OK)
    {
      throw std::exception((std::string("GetHash Fail: ") + std::to_string(code)).c_str());
    }
    ret.resize(size);

    if(ret.back() == '\0')
    {
      ret.pop_back();
    }

    LOGAF_DEBUG("size: %d", ret.size());

    return ret;
  }

protected:
private:
  FZipWrapper()
  {
  }

  ~FZipWrapper()
  {
    DllWrapper::Free();
  }
};