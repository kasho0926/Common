#include "MyFileStream.h"
#include "debug_log.h"

double SystemToDelphiTime(const SYSTEMTIME &sysTime)
{
  __int64 FileTime;
  double delphiTime;
  SystemTimeToFileTime(&sysTime, (FILETIME*)&FileTime);
  delphiTime = (double(FileTime - 94353120000000000)) / 864000000000.0;

  return delphiTime;
}

WriteFileStream::WriteFileStream(void)
{
  m_file = NULL;
}

WriteFileStream::~WriteFileStream(void)
{
  if(m_file != NULL)
  {
    CloseHandle(m_file);
  }
}

BOOL WriteFileStream::OpenFile(WCHAR * filePath)
{
  m_file = CreateFile(filePath,                // name of the write
    GENERIC_WRITE,          // open for writing
    0,                      // do not share
    NULL,                   // default security
    CREATE_ALWAYS,             // create new file only
    FILE_ATTRIBUTE_NORMAL,  // normal file
    NULL);                  // no attr. template

  if(m_file == INVALID_HANDLE_VALUE)
  {
    LOGWF_ERROR(L"CreateFile Fail: %p", GetLastError());
  }

  return (m_file != INVALID_HANDLE_VALUE);
}

BOOL WriteFileStream::WriteWString(WCHAR * value)
{
  BOOL ret = 0;
  DWORD dwBytesWritten = 0;
  int size = sizeof(WCHAR) * (wcslen(value) + 1);
  ret = WriteInt(size);

  if(ret != 0)
  {
    ret = WriteFile(
      m_file,           // open file handle
      value,      // start of data to write
      size,  // number of bytes to write
      &dwBytesWritten, // number of bytes that were written
      NULL);

    if(!ret)
    {
      LOGWF_ERROR(L"WriteFile Fail: %p", GetLastError());
    }
  }
  return ret;
}

BOOL WriteFileStream::WriteInt(const int &value)
{
  BOOL ret = 0;
  DWORD dwBytesWritten = 0;

  ret = WriteFile(
    m_file,           // open file handle
    &value,      // start of data to write
    sizeof(int),  // number of bytes to write
    &dwBytesWritten, // number of bytes that were written
    NULL);

  if(!ret)
  {
    LOGWF_ERROR(L"WriteFile Fail: %p", GetLastError());
  }

  return ret;
}

BOOL WriteFileStream::WriteSystemTime(const SYSTEMTIME &value)
{
  BOOL ret = 0;
  DWORD dwBytesWritten = 0;
  ret = WriteFile(
    m_file,           // open file handle
    &value,      // start of data to write
    sizeof(SYSTEMTIME),  // number of bytes to write
    &dwBytesWritten, // number of bytes that were written
    NULL);

  if(!ret)
  {
    LOGWF_ERROR(L"WriteFile Fail: %p", GetLastError());
  }

  return ret;
}

BOOL WriteFileStream::WriteDouble(const double &value)
{
  BOOL ret = 0;
  DWORD dwBytesWritten = 0;
  ret = WriteFile(
    m_file,           // open file handle
    &value,      // start of data to write
    sizeof(double),  // number of bytes to write
    &dwBytesWritten, // number of bytes that were written
    NULL);

  if(!ret)
  {
    LOGWF_ERROR(L"WriteFile Fail: %p", GetLastError());
  }

  return ret;
}

BOOL WriteFileStream::WriteCmd(const int &type, const SYSTEMTIME &time, WCHAR * value)
{
  BOOL ret = 0;
  ret = WriteInt(type);
  if(ret == 0)
  {
    return ret;
  }

  ret = WriteDouble(SystemToDelphiTime(time));
  if(ret == 0)
  {
    return ret;
  }

  ret = WriteWString(value);
  return ret;
}

BOOL WriteFileStream::SetEOF()
{
  return SetEndOfFile(m_file);
}

ReadFileStream::ReadFileStream(void)
{
  m_file = NULL;
}

ReadFileStream::~ReadFileStream(void)
{
  if(m_file != NULL)
  {
    CloseHandle(m_file);
  }
}

BOOL ReadFileStream::OpenFile(WCHAR * filePath)
{
  m_file = CreateFile(filePath,               // file to open
    GENERIC_READ,          // open for reading
    FILE_SHARE_READ,       // share for reading
    NULL,                  // default security
    OPEN_EXISTING,         // existing file only
    FILE_ATTRIBUTE_NORMAL , // normal file
    NULL);                 // no attr. template

  if(m_file == INVALID_HANDLE_VALUE)
  {
    LOGWF_ERROR(L"CreateFile Fail: %p", GetLastError());
  }
  return (m_file != INVALID_HANDLE_VALUE);
}

std::wstring ReadFileStream::ReadWString()
{
  std::wstring wstr = L"";

  int length = ReadInt()/sizeof(WCHAR);
  WCHAR * value = new WCHAR[length];
  DWORD dwRead;

  if(ReadFile(m_file, value, length * sizeof(WCHAR), &dwRead, NULL))
  {
    wstr = value;
    LOGWF_TRACE(L"%s", wstr.c_str());
  }
  else
  {
    LOGWF_ERROR(L"ReadFile Fail: %p", GetLastError());
  }

  delete []value;
  return wstr;
}
int ReadFileStream::ReadInt()
{
  int value = 0;
  DWORD dwRead = 0;
  if(!ReadFile(m_file, &value, sizeof(int), &dwRead, NULL))
  {
    LOGWF_ERROR(L"ReadFile Fail: %p", GetLastError());
  }
  return value;
}
double ReadFileStream::ReadDouble()
{
  double value;
  DWORD dwRead;
  if(!ReadFile(m_file, &value, sizeof(double), &dwRead, NULL))
  {
    LOGWF_ERROR(L"ReadFile Fail: %p", GetLastError());
  }
  return value;
}
SYSTEMTIME ReadFileStream::ReadSystemTime()
{
  SYSTEMTIME value;
  DWORD dwRead;
  if(!ReadFile(m_file, &value, sizeof(SYSTEMTIME), &dwRead, NULL))
  {
    LOGWF_ERROR(L"ReadFile Fail: %p", GetLastError());
  }
  return value;
}
CommandLog ReadFileStream::ReadCmd()
{
  CommandLog cmd;
  cmd.type = ReadInt();
  cmd.time = ReadDouble();
  cmd.log = ReadWString();
  return cmd;
}