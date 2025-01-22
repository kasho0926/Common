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
#include <vector>
#include <deque>
#include <exception>
#include <chrono>
#include "test_macros.h"
#include "ZipTest.hpp"
#include "Commlib/filesystem_util.h"
#include "Commlib/Injector.hpp"
#include "Commlib/string_utils.hpp"
#include "Commlib/ExeWrapper.hpp"
#include "Commlib/debug_log.h"

#pragma comment(lib,"..\\Release\\Commlib.lib")

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

  DebugLog::Init(L"XFNetHook");

  LOGW(L"test中文測試");
  AUTOLOGW;


  system("Pause");
  return 0;


  setlocale(LC_CTYPE, "");
  clock_t start;
  clock_t end;
  clock_t duration;

  ExeWrapper::Instance().Init(LR"(F:\Git\Client64\client64\XRegexTag\Tika4Fort\bin\Release\Tika4Fort.exe)");

  start = clock();

  std::wstring input = LR"(F:\WorkSpace\Benchmark\simple.txt)";
  std::wstring output = LR"(F:\WorkSpace\Benchmark\output.txt)";
  ExeWrapper::Instance().Start(input, output, 3000000);

  end = clock();
  duration = end - start;
  cout << "Time elapsed: " << duration << " ticks. \n" << endl;
  cout << "Time elapsed: " << duration / CLOCKS_PER_SEC << " sec. \n" << endl;

  system("Pause");
  return 0;
}