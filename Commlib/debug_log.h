#pragma once

#include <string>
#include <Windows.h>

//#define LOG( ... ) do { JLog( __VA_ARGS__ ); } while(0)  // Ansi/Unicode �ҥi
#define LOG(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_INFO) DebugLog::LogA( "[%s]" format , __FUNCTION__, __VA_ARGS__); } while(0)  // Ansi/Unicode �ҥi
#define LOGAF(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_INFO) DebugLog::LogA( "[%s]" format , __FUNCTION__, __VA_ARGS__); } while(0)
#define LOGAF_TRACE(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_TRACE) DebugLog::LogA("[TRACE][%s]" format , __FUNCTION__, __VA_ARGS__); } while(0)
#define LOGAF_DEBUG(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_DEBUG) DebugLog::LogA("[DEBUG][%s]" format , __FUNCTION__, __VA_ARGS__); } while(0)
#define LOGAF_INFO(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_INFO) DebugLog::LogA( "[%s]" format , __FUNCTION__, __VA_ARGS__); } while(0)
#define LOGAF_WARN(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_WARN) DebugLog::LogA("[WARN][%s]" format , __FUNCTION__, __VA_ARGS__); } while(0)
#define LOGAF_ERROR(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_ERROR) DebugLog::LogA("[ERROR][%s]" format , __FUNCTION__, __VA_ARGS__); } while(0)
#define LOGAF_FATAL(format,...) do {if(DebugLog::g_debugLevel >= DEBUG_FATAL) DebugLog::LogA("[FATAL][%s]" format , __FUNCTION__, __VA_ARGS__); } while(0)

#define LOGWF_TRACE __noop
#define LOGWF_DEBUG  __noop
#define LOGWF_INFO  __noop
#define LOGWF_WARN  __noop
#define LOGWF_ERROR  __noop
#define LOGWF_FATAL  __noop

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
  // LogDir ���|
  extern WCHAR g_logDir[MAX_PATH];
  extern WCHAR g_newestPath[MAX_PATH];
  extern WCHAR g_archivePath[MAX_PATH];
  extern WCHAR g_confPath[MAX_PATH];
  extern WCHAR g_debugConfPath[MAX_PATH];
  // �ӼҲժ��N�� Key
  extern WCHAR g_key[128];
  extern DebugLevel g_debugLevel;
  extern UINT g_logSize;
  // ��l�ơA���� g_LogDir �M�g�� Thread
  void Init(const WCHAR * key);
  void Stop();

  void LogA(const char * format, ...);
  // �N Message �g�J�ɮ�
  void WriteToFile(const char * pMessage);
  // �ˬd�}��
  bool CheckLogConfExist();
  bool CheckDebugConfExist();
  DebugLevel ReadConfig(WCHAR * fileName);
  // ���� g_logDir
  DWORD GetLogPath();
};