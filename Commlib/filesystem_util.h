#pragma once

#include <Windows.h>
#include <string>
#include <list>

namespace FileSystem
{
  //!@brief Version
  //!  存放版本號
  struct Version
  {
    WORD MajorVersion;
    WORD MinorVersion;
    WORD BuildNumber;
    WORD RevisionNumber;
  };

  namespace File
  {
    //!@brief GetDllVersion
    //  取得指定檔案的版本資訊
    //!@param dllname 目標檔案
    //!@param ver 存放版本資訊
    //!@return
    //!  FUNCTION_NOERROR
    //!  FUNCTION_ERROR
    DWORD GetDllVersion(const std::wstring &dllname, Version & ver);
    //!@brief GetModifiedTime
    //!  取得指定檔案的最後修改時間
    //!@param file 指定的檔名
    //!@return
    //!  FUNCTION_ERROR
    //!  取得的時間
    ULONGLONG GetModifiedTime(const std::wstring &file);
    bool Exist(const std::wstring &path);
    std::list<std::string> ListAll(const std::string &path, bool recursive = false);
  };

  namespace Directory
  {
    BOOL Exist(const std::wstring & Directory);
    DWORD Delete(const std::wstring &path);

    // Create directory recursively if not exist
    // Parameters:
    // 	   const wstring & Directory
    // 		   The full path of directory to be created.
    // Return values:
    //	   true if the directory is created successfully or already exists, or false on fail
    //
    bool SafeCreate(const std::wstring & Directory);
    //
    // Get the parent directory path of the input directory
    //
    std::wstring GetParent(const std::wstring & Directory);
  };

  namespace Path
  {
    //!@brief GetMyDocument
    //!  取得我的文件的實際路徑
    //!@return 我的文件的實際路徑
    std::wstring GetMyDocument();
    std::wstring GetWindowsEnvironmentVariable(const std::wstring &key);
    std::wstring GetCurrentDir();

    std::wstring Combine(const std::wstring &path1, const std::wstring &path2);
    std::wstring GetAllUsersDir();
    std::wstring GetLocalAppDataDir();
    std::wstring GetLocalAppDataLowDir();

#define XFORT
#ifdef XFORT
    std::wstring GetMailLogStrPath(const std::wstring &filename);
    std::wstring GetClientIniPath();
    std::wstring GetRedirectAllUsersDir();
    std::wstring GetRedirectLocalAppDataDir();
    std::wstring GetDbgCtrPath();
    std::wstring GetHelpClrPath();
    std::wstring GetAppHistoryPath();
    std::wstring GetTemporaryFilesPath();
#endif
  };

 
}