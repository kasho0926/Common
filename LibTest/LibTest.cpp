// LibTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <unordered_map>
#include <algorithm>
#include "Commlib\string_utils.hpp"
#include "Commlib\debug_log.h"
#include "Commlib\assembly.h"
#include "Commlib\file_util.h"
#include "Commlib\detours_function.h"
#include <vector>
#include <deque>
#include "AssemblyTest.h"
#include "DebugLogTest.h"
#include "Commlib\WMI_control.h"
#include<exception>
#include "Commlib\my_exception.h"
#include "Commlib\MyFileStream.h"
#include "Commlib\JSON\json.h"
#include "Commlib\registry.h"

#pragma comment(lib,"..\\Release\\Commlib.lib")

using namespace std;

#define TESTALL 0
#define GETDLLVERSION_TEST 0
#define DW2LE_TEST 0
#define LE2DW_TEST 0
#define DWREVERSE_TEST 0
#define GETCALLADDR_TEST 0
#define LOGMESSAGE_TEST 1

inline std::string WStringToString(const std::wstring & wstr)
{
  std::string str;
  size_t size;
  str.resize(wstr.length());
  wcstombs_s(&size, &str[0], str.size() + 1, wstr.c_str(), wstr.size());
  return str;
}

int _tmain(int argc, _TCHAR* argv[])
{
  wstring wstr = L"這是測試";
  string str = StringUtils::Utf16ToMultiByte(wstr);
  printf("%s\r\n", str.c_str());
  for(int i = 0; i < str.length(); ++i)
  {
    printf("%04x ", str[i]);
  }
  cout << endl;

  str =  WStringToString(wstr);

  printf("%s\r\n", str.c_str());
  for(int i = 0; i < str.length(); ++i)
  {
    printf("%04x ", str[i]);
  }

  system("Pause");

  return 0;
  Json::Value root;
  root["Content"] =  Json::Value("20190218_112817_2\r\n20190218_112817_3\r\n");
  Json::FastWriter fast_writer;
  LOGAF_TRACE("fast_writer:%s", fast_writer.write(root["Content"]).c_str());

  getchar();
  //true_fprintf(stdout, "%ws", L"  TCP    10.1.1.73:139          case7-server1:52299    ESTABLISHED\r\n");

  //fprintf(stdout, "%ws", sz);
  //getchar();
  return 0;

  RUN(getValueWString);
  RUN(DW2LE);
  RUN(LogMessageW);

  system("Pause");
  return 0;

  if( TESTALL || DW2LE_TEST )
  {
    DWORD dwHex = 0x6F5A1B44;
    BYTE bByte[4]={0};
    Assembly::DW2LE( dwHex, bByte  );
    cout << std::hex << (DWORD)bByte[0] << (DWORD)bByte[1] << (DWORD)bByte[2] << (DWORD)bByte[3] << endl;
    cout << std::hex << *(DWORD*)bByte;
  }

  if( TESTALL || LE2DW_TEST )
  {
    DWORD dwHex =0;
    BYTE bByte[6]={0x6F,0x5A,0x1B,0x44,0x55,0x66};
    BYTE * bb = (BYTE*) ((DWORD)bByte);
    dwHex = Assembly::LE2DW( bByte+2  );
    //Assembly::LE2DW( bb  );

    cout << std::hex <<dwHex<< endl;
  }

  if( TESTALL || DWREVERSE_TEST )
  {
    DWORD offset =0x6799DEFF;
    cout <<"DWREVERSE: " << endl;
    Assembly::DWReverse( offset ) ;
    cout  <<hex<<Assembly::DWReverse( offset )<< endl;
  }

  if( TESTALL || GETCALLADDR_TEST )
  {
    DWORD dwHex =0x6F551652;

    DWORD offset =0x6799DEFF;

    cout <<"getCallAddrTest: " << endl;
    // 0x6F33AFC0
    cout << hex <<Assembly::GetCallAddress(dwHex,offset)<< endl;
  }

  if( TESTALL || LOGMESSAGE_TEST )
  {
    string str = "mystr";
    wstring wstr = L"mywstr";
    int n = 99;

    //LOGW(L"this is str:%s", wstr.c_str());
    //LOGAF("this is str:%s, int:%d", str.c_str(), n );

    //LOGAF("this is str: %s, int: %d", str.c_str(), n);
    LOGWF(L"this is wstr: %s, int: %d", wstr.c_str(), n);
  }

  system("Pause");

  //DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;
  //STARTUPINFO si;
  //PROCESS_INFORMATION pi;
  //ZeroMemory(&si, sizeof(si));
  //ZeroMemory(&pi, sizeof(pi));
  //si.cb = sizeof(si);

  //WIN32_FIND_DATA FindFileData;
  //HANDLE hFind;
  //hFind = FindFirstFile( L"C:\\Program Files (x86)\\Tencent\\WeChat\\WeChat.exe", &FindFileData );
  //if (hFind == INVALID_HANDLE_VALUE)
  //{
  //myDLog << L"Dll not exist!: "  << WENDLF;
  //return 1;
  //}

  //hFind = FindFirstFile(  L"C:\\Program Files (x86)\\Tencent\\WeChat\\WeChatWin.dll", &FindFileData );
  //if (hFind == INVALID_HANDLE_VALUE)
  //{
  //myDLog << L"Dll not exist!: "  << WENDLF;
  //return 1;
  //}

  //hFind = FindFirstFile(  L"E:\\Project\\DetourTest\\Release\\DetourTestDll.dll", &FindFileData );
  //if (hFind == INVALID_HANDLE_VALUE)
  //{
  //myDLog << L"Dll not exist!: "  << WENDLF;
  //return 1;
  //}

  //myDLog<< L"CreateProcess ! : " << WENDLF;

  //TCHAR szLPCTSTR[MAX_PATH] = L"C:\\Program Files (x86)\\Tencent\\WeChat\\WeChat.exe";
  //
  //if(CreateProcess(NULL, // Use command line
  //  szLPCTSTR, // Exe Name
  //  NULL, // Process handle not inheritable
  //  NULL, // Thread handle not inheritable
  //  TRUE, // Set handle inheritance to FALSE
  //  NULL, // CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED
  //  NULL, // Use parent's environment block
  //  NULL, // Use parent's starting directory
  //  &si, // Pointer to STARTUPINFO structure
  //  &pi // Pointer to PROCESS_INFORMATION structure
  //  ) == 0 )
  //  myDLog<< L"CreateProcess ERROR! : " << GetLastError() << WENDLF;

  //myDLog<< L"WeChatWin.dll : " << WENDLF;
  //FileUtil::remoteInject( pi.dwProcessId, L"C:\\Program Files (x86)\\Tencent\\WeChat\\WeChatWin.dll" );
  //myDLog<< L"DetourTestDll.dll : " << WENDLF;
  ////while( GetModuleHandle( L"WeChatWin.dll" ) != NULL );
  ////FileUtil::remoteInject( pi.dwProcessId, L"E:\\Project\\DetourTest\\Release\\DetourTestDll.dll" );
  //FileUtil::remoteInject( pi.dwProcessId, L"E:\\Project\\WeChatMsg\\Release\\WeChatMsgDll.dll" );

  /*
  DWORD begin = 0;
  DWORD size= 0;
  Assembly::print_PE_section_info(GetModuleHandle(NULL));
  Assembly::GetPESectionInfo(GetModuleHandle(NULL), ".rdata", begin, size);
  cout << Assembly::ByteSearch( "test1test2test3", begin, size);
  */
  /*
  DLog::myDLog << L"ssssss" << WENDL;
  DLog::myDLog.flush();*/

  /* locale::global(locale("")); /// 環境設定locale為global
  vector<wstring> vec = StringHandle::split(L".", L",.",StringHandle::IGNORE_EMPTY);
  wcout << vec.size() << endl;
  for (int i = 0; i < vec.size(); i++)
  {
  wcout << vec[i] << endl;
  }
  */
  //system("Pause");
  return 0;
}