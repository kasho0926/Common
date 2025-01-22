#pragma once
#include <string>
#include <queue>
#include <fstream>
#include <sstream>
#include <filesystem>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include "debug_log.h"
#include "JSON/json.h"
#include "string_utils.hpp"
#include "FZipDllWrapper.hpp"

#define _LOGW
#define LOG_ERROR

#define MAX_LPM_SIZE ((unsigned __int64) 4*1024*1024*1024-256)

using Fzip = FZipWrapper;
namespace fs = std::filesystem;

class LPMPacker
{
public:

#pragma pack(push, 1)  // structure pack

  typedef struct _LPM_HEADER
  {
    BYTE btUidA[8];
    DWORD dwDataSize; // Variable
    BYTE btTag[16];
    DWORD dwLogType;
    DWORD dwLogN;  // Variable
    //// Data .....
  } LPM_HEADER;

#pragma pack(pop)  // structure pack

  void SetTempFolder(const std::wstring & path)
  {
    this->TempFolder = path;
  }

  void SetLPMFolder(const std::wstring & path)
  {
    this->LPMFolder = path;
  }

  void SetClientID(UINT64 clientID)
  {
    ClientID = clientID;
  }

  void SetLogType(DWORD logType)
  {
    LogType = logType;
  }

  void SetPrefix(const std::wstring &prefix)
  {
    this->Prefix = prefix;
  }

  std::wstring GetBackupName(const std::string & json)
  {
    std::wstring name = L"";

    Json::Value root;
    Json::Reader reader(Json::Features::all());
    if(!reader.parse(json, root))
    {
      return name;
    }

    if(root.isMember("backupName") && root["backupName"].isString())
    {
      name = StringUtils::Convert::AnsiMultiByteToUtf16WideChar(root["backupName"].asCString());
      _LOGW(L"backupName: %s", name.c_str());
    }

    return name;
  }

  LPM_HEADER CreateHeader()
  {
    LPM_HEADER header;
    memset(&header, 0, sizeof(header));
    memcpy(header.btTag, "LPM", sizeof("LPM") - 1);
    memcpy(header.btUidA, &this->ClientID, sizeof(UINT64));
    header.dwLogType = this->LogType;
    header.dwDataSize = 0;
    header.dwLogN = 0;

    return header;
  }

  std::wstring CreateLPMName()
  {
    static DWORD index = 0;

    WCHAR wszBuf[MAX_PATH] = { 0 };
    SYSTEMTIME sysT;
    GetLocalTime(&sysT);
    _snwprintf_s(wszBuf, MAX_PATH, MAX_PATH, L"%s%s_%d%02d%02d_%02d%02d%02d_%d_%d.LPM",
      this->TempFolder.c_str(), this->Prefix.c_str(), sysT.wYear, sysT.wMonth, sysT.wDay, sysT.wHour, sysT.wMinute, sysT.wSecond, GetCurrentProcessId(), index);

    index = (index + 1) % 8192;

    _LOGW(L"LPMTempName: %s", wszBuf);

    return std::wstring(wszBuf);
  }

  bool MoveFolder(const std::wstring & name)
  {
    std::wstring filename = this->LPMFolder + fs::path(name).filename().wstring();

    if(!MoveFileEx(name.c_str(), filename.c_str(),
      MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH))
    { // ­«½Æ«h Fail
      DWORD dwErr = GetLastError();
      bool bnOK = DeleteFile(name.c_str());
      _LOGW(L"MoveFolder: Move fail, dwErr=%d, delete temp, bnOK=%d, from (%s) to (%s)",
        dwErr, bnOK, name.c_str(), filename.c_str());
      return false;
    }

    _LOGW(L"MoveFolder: Move OK, from (%s) to (%s)", name.c_str(), filename.c_str());

    return true;
  }

  void PackBackup(std::ofstream &os, const std::wstring &file, size_t & size)
  {
    if(file.empty())
    {
      return;
    }

    try
    {
      std::wstring backup = file;
      Fzip::Instance().Encrypt(file, file + L".zip");
      backup = file + L".zip";

      UINT64 backupLen = fs::file_size(backup);
      if(backupLen + size < MAX_LPM_SIZE)
      {
        std::ifstream is;
        is.open(backup, std::ifstream::in | std::ifstream::binary);
        if(is.good())
        {
          os.write((char*)&backupLen, 8);
          os << is.rdbuf();
          size += (size_t)backupLen + 8;

          is.close();
        }
        else
        {
          LOG_ERROR("PackBackup::Open backup Fail");
        }
      }
      else
      {
        LOG_ERROR("PackBackup::backup Too Large");
      }

      _wremove(file.c_str());
      _wremove(backup.c_str());
    }
    catch(std::exception &e)
    {
      LOG_ERROR("PackBackup::Exception: %s", e.what());
    }
    catch(...)
    {
      LOG_ERROR("PackBackup::Exception!");
    }
  }

  bool Pack(const std::wstring &file)
  {
    bool ret = false;
    size_t size = sizeof(LPM_HEADER);
    std::wstring backup;
    std::wstring name = CreateLPMName();

    try
    {
      std::ofstream os;
      os.open(name, std::ios::out | std::ios::binary);
      if(!os.good())
      {
        LOG_ERROR("Pack::Open TempLPM Fail");
        return ret;
      }

      std::ifstream is(file, std::ifstream::in | std::ifstream::binary);
      if(!is.good())
      {
        LOG_ERROR("Pack::Open SML Fail");
        return ret;
      }

      std::stringstream ss;
      ss << is.rdbuf();

      LPM_HEADER header = CreateHeader();
      os.write((char*)&header, sizeof(header));

      int jsonLen = (int)fs::file_size(file);
      os.write((char*)&jsonLen, 4);
      os << ss.str();
      size += jsonLen + 4;

      is.close();
      is.clear();
      _wremove(file.c_str());

      backup = GetBackupName(ss.str());
      PackBackup(os, backup, size);

      os.seekp(0, std::ios::beg);
      header.dwDataSize = size;
      os.write((char*)&header, sizeof(header));

      os.close();
      ret = MoveFolder(name);
    }
    catch(std::exception &e)
    {
      LOG_ERROR("Pack::Exception: %s", e.what());
    }
    catch(...)
    {
      LOG_ERROR("Pack::Exception!");
    }

    if(!backup.empty())
    {
      _wremove(backup.c_str());
    }
    _wremove(file.c_str());
    _wremove(name.c_str());

    return ret;
  }

  bool PackAll(std::queue<std::wstring> &files)
  {
    bool ret = false;

    while(!files.empty())
    {
      std::wstring file = files.front();
      files.pop();

      ret |= Pack(file);
    }

    return ret;
  }

private:
  std::wstring TempFolder;
  std::wstring LPMFolder;
  UINT64 ClientID;
  DWORD LogType;
  std::wstring Prefix;
};