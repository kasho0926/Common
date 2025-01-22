#pragma once

#include <string>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

//#define LOG( ... ) do { JLog( __VA_ARGS__ ); } while(0)  // Ansi/Unicode 皆可

#ifdef STDOUT
#define LOG(format,...) do {printf( "[%s]" format "\r\n", __FUNCTION__, __VA_ARGS__); } while(0)  // Ansi/Unicode 皆可
#define LOGAF(format,...) do {printf( "[%s]" format "\r\n", __FUNCTION__, __VA_ARGS__); } while(0)
#define LOGAF_TRACE(format,...)  do {printf( "[%s]" format "\r\n", __FUNCTION__, __VA_ARGS__) ;} while(0)
#define LOGAF_DEBUG(format,...)  do {printf( "[%s]" format "\r\n", __FUNCTION__, __VA_ARGS__) ;} while(0)
#define LOGAF_INFO(format,...) do {printf( "[%s]" format "\r\n", __FUNCTION__, __VA_ARGS__); } while(0)
#define LOGAF_WARN(format,...) do {printf( "[%s]" format "\r\n", __FUNCTION__, __VA_ARGS__); } while(0)
#define LOGAF_ERROR(format,...) do {printf( "[%s]" format "\r\n", __FUNCTION__, __VA_ARGS__); } while(0)
#define LOGAF_FATAL(format,...) do {printf( "[%s]" format "\r\n", __FUNCTION__, __VA_ARGS__); } while(0)
#endif

#ifdef NOOP
#define LOGWF_TRACE __noop
#define LOGWF_DEBUG  __noop
#define LOGWF_INFO  __noop
#define LOGWF_WARN  __noop
#define LOGWF_ERROR  __noop
#define LOGWF_FATAL  __noop
#define AUTOLOG  __noop
#else

#define LOG(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_INFO) DebugLog::LogA( "[%s]" format , __FUNCTION__, __VA_ARGS__); } while(0)  // Ansi/Unicode 皆可
#define LOGA(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_INFO) DebugLog::LogA( "[%s]" format , __FUNCTION__, __VA_ARGS__); } while(0)  // Ansi/Unicode 皆可
#define LOGAF(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_INFO) DebugLog::LogA( "[%s]" format , __FUNCTION__, __VA_ARGS__); } while(0)
#define LOGAF_TRACE(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_TRACE) DebugLog::LogA("[TRACE][%s]" format , __FUNCTION__, __VA_ARGS__); } while(0)
#define LOGAF_DEBUG(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_DEBUG) DebugLog::LogA("[DEBUG][%s]" format , __FUNCTION__, __VA_ARGS__); } while(0)
#define LOGAF_INFO(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_INFO) DebugLog::LogA( "[%s]" format , __FUNCTION__, __VA_ARGS__); } while(0)
#define LOGAF_WARN(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_WARN) DebugLog::LogA("[WARN][%s]" format , __FUNCTION__, __VA_ARGS__); } while(0)
#define LOGAF_ERROR(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_ERROR) DebugLog::LogA("[ERROR][%s]" format , __FUNCTION__, __VA_ARGS__); } while(0)
#define LOGAF_FATAL(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_FATAL) DebugLog::LogA("[FATAL][%s]" format , __FUNCTION__, __VA_ARGS__); } while(0)

#define LOGW(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_INFO) DebugLog::LogW( L"[%s]" format , __FUNCTIONW__, __VA_ARGS__); } while(0)  // Ansi/Unicode 皆可
#define LOGWF(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_INFO) DebugLog::LogW( L"[%s]" format , __FUNCTIONW__, __VA_ARGS__); } while(0)
#define LOGWF_TRACE(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_TRACE) DebugLog::LogW(L"[TRACE][%s]" format , __FUNCTIONW__, __VA_ARGS__); } while(0)
#define LOGWF_DEBUG(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_DEBUG) DebugLog::LogW(L"[DEBUG][%s]" format , __FUNCTIONW__, __VA_ARGS__); } while(0)
#define LOGWF_INFO(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_INFO) DebugLog::LogW( L"[%s]" format , __FUNCTIONW__, __VA_ARGS__); } while(0)
#define LOGWF_WARN(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_WARN) DebugLog::LogW(L"[WARN][%s]" format , __FUNCTIONW__, __VA_ARGS__); } while(0)
#define LOGWF_ERROR(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_ERROR) DebugLog::LogW(L"[ERROR][%s]" format , __FUNCTIONW__, __VA_ARGS__); } while(0)
#define LOGWF_FATAL(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_FATAL) DebugLog::LogW(L"[FATAL][%s]" format , __FUNCTIONW__, __VA_ARGS__); } while(0)
#endif

#define AUTOLOG DebugLog::AutoLogA autoLog(__FUNCTION__)
#define AUTOLOGA DebugLog::AutoLogA autoLog(__FUNCTION__)
#define AUTOLOGW DebugLog::AutoLogW autoLog(__FUNCTIONW__)

enum DebugLevel
{
  DEBUG_OFF = 0,
  DEBUG_FATAL = 100,
  DEBUG_ERROR = 200,
  DEBUG_WARN = 300,
  DEBUG_INFO = 400,
  DEBUG_DEBUG = 500,
  DEBUG_TRACE = 600
};

namespace DebugLog
{
  // Write to file
  extern DWORD g_log;
  // Write to DbgView
  extern DWORD g_debug;
  // LogDir 路徑
  extern WCHAR g_logDir[MAX_PATH];
  extern WCHAR g_newestPath[MAX_PATH];
  extern WCHAR g_archivePath[MAX_PATH];
  extern WCHAR g_confPath[MAX_PATH];
  extern WCHAR g_debugConfPath[MAX_PATH];
  // 該模組的代表 Key
  extern WCHAR g_key[128];
  extern DebugLevel g_debugLevel;
  extern UINT g_logSize;
  // 初始化，產生 g_LogDir 和寫檔 Thread
  void Init(const WCHAR * key);
  void Stop();

  void LogA(const char * format, ...);
  void LogW(const WCHAR * format, ...);
  // 將 Message 寫入檔案
  void WriteToFile(const BYTE * pMessage, size_t size);
  void WriteToFile(const char * pMessage);
  void WriteToFile(const WCHAR * pMessage);
  // 檢查開關
  bool CheckLogConfExist();
  bool CheckDebugConfExist();
  DebugLevel ReadConfig(WCHAR * fileName);
  // 產生 g_logDir
  DWORD GetLogPath();

  class AutoLogA
  {
  public:
    AutoLogA(char * functionName) :m_functionName(functionName)
    {
      if(g_debugLevel >= DEBUG_DEBUG)
        LOGA("[DEBUG][%s][START]\r\n", m_functionName.c_str());
    }
    ~AutoLogA()
    {
      if(g_debugLevel >= DEBUG_DEBUG)
        LOGA("[DEBUG][%s][END]\r\n", m_functionName.c_str());
    }
  private:
    std::string m_functionName;
  };

  class AutoLogW
  {
  public:
    AutoLogW(WCHAR * functionName) :m_functionName(functionName)
    {
      if(g_debugLevel >= DEBUG_DEBUG)
        LOGW(L"[DEBUG][%s][START]\r\n", m_functionName.c_str());
    }
    ~AutoLogW()
    {
      if(g_debugLevel >= DEBUG_DEBUG)
        LOGW(L"[DEBUG][%s][END]\r\n", m_functionName.c_str());
    }
  private:
    std::wstring m_functionName;
  };
};