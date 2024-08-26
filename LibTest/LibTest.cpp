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
#include "test_macros.h"
#include "ZipTest.hpp"
#include "Commlib/filesystem_util.h"
#include "Commlib/Injector.hpp"
#include "Commlib/string_utils.hpp"

#pragma comment(lib,"..\\Release\\Commlib.lib")

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
  setlocale(LC_CTYPE, "");
  //RUN(ZIP_TEST, FZip);
  string str = FileSystem::File::ReadAll(L"encode.txt");
  
  //wstring wstr = StringUtils::Convert::AnsiMultiByteToUtf16WideChar(str.c_str());
  //wprintf(L"%s\r\n", wstr.c_str());



  //wofstream ofs(L"output.txt", ios::binary | ios::out);

  //char bom[] = { '\xFF', '\xFE' };
  //ofs.write((WCHAR*)bom, sizeof(bom));
  //ofs.write(wstr.c_str(), wstr.length( );


  str = StringUtils::Convert::MultiByteToUtf8MultiByte(str.c_str(), 20932);
  cout << str << endl;

  ofstream ofs(L"output.txt", ios::binary | ios::out);
  ofs.write(str.c_str(), str.length());

  system("Pause");
  return 0;
}