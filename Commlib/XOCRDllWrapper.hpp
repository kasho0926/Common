#pragma once
#include <sstream>
#include <exception>
#include <fstream>
#include <string>

#include "JSON/json.h"
#include "RunDllWrapper.hpp"

class XOCRWrapper :public RunDllWrapper
{
public:
  enum RunCmdCode
  {
    XOCR_INIT = 0x10,
    XOCR_FREE = 0x11,
    XOCR_FILE = 0x400
  };

  static XOCRWrapper& Instance() {
    static XOCRWrapper instance;
    return instance;
  }

  XOCRWrapper(XOCRWrapper const&) = delete;
  void operator=(XOCRWrapper const&) = delete;

  void Init()
  {
    RunDllWrapper::Init(L"XOCRWrap.dll");
  }

  std::string ExtractText(const std::string &path)
  {
    AUTOLOG;
    std::string result;
    DWORD code = RRET_OK;

    Json::Value root;
    root["items"].append(path);

    Json::FastWriter writer;
    std::string json = writer.write(root) + "\0";

    LOGAF("Input: %S\r\n", json.c_str());

    std::string ret(1024 * 1024, '\0');
    DWORD size = ret.length();

    if((code = DoRun(XOCR_FILE, (BYTE*)json.c_str(), json.length(), (BYTE*)&ret[0], &size)) != RRET_OK)
    {
      throw std::exception((std::string("ExtractText Fail: ") + std::to_string(code)).c_str());
    }
    else
    {
      Json::Reader reader;
      if(!reader.parse(ret, root))
      {
        throw std::exception((std::string("Json Fail: ") + std::to_string(code)).c_str());
      }

      for(auto & item : root["items"])
        for(auto & line : item["lines"])
        {
          result += line.asString();
        }
    }

    return result;
  }

protected:
private:
  XOCRWrapper()
  {
  }

  ~XOCRWrapper()
  {
    DllWrapper::Free();
  }
};