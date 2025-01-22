#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <string>

class WriteFileStream
{
public:
  WriteFileStream(void);
  ~WriteFileStream(void);
  BOOL OpenFile(WCHAR * filePath);
  BOOL WriteWString(WCHAR * value);
  BOOL WriteInt(const int &value);
  BOOL WriteDouble(const double &value);
  BOOL WriteSystemTime(const SYSTEMTIME &value);
  BOOL WriteCmd(const int &type, const SYSTEMTIME &time, WCHAR * value);
  BOOL SetEOF();
private:
  HANDLE m_file;
};


struct CommandLog
{
  int type;
  double time;
  std::wstring log;
};

class ReadFileStream
{
public:
  ReadFileStream(void);
  ~ReadFileStream(void);
  BOOL OpenFile(WCHAR * filePath);
  std::wstring ReadWString();
  int ReadInt();
  double ReadDouble();
  SYSTEMTIME ReadSystemTime();
  CommandLog ReadCmd();
private:
  HANDLE m_file;
};
