#pragma once

#include <Windows.h>
#include <string>
#include <list>

namespace FileSystem
{
  //!@brief Version
  //!  �s�񪩥���
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
    //  ���o���w�ɮת�������T
    //!@param dllname �ؼ��ɮ�
    //!@param ver �s�񪩥���T
    //!@return
    //!  FUNCTION_NOERROR
    //!  FUNCTION_ERROR
    DWORD GetDllVersion(const std::wstring &dllname, Version & ver);
    //!@brief GetModifiedTime
    //!  ���o���w�ɮת��̫�ק�ɶ�
    //!@param file ���w���ɦW
    //!@return
    //!  FUNCTION_ERROR
    //!  ���o���ɶ�
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
    //!  ���o�ڪ���󪺹�ڸ��|
    //!@return �ڪ���󪺹�ڸ��|
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