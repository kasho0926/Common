#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <Windows.h>

#include "debug_log.h"

//!@class StringUtils string_utils.hpp
//!@brief 對字串處理
namespace StringUtils
{
  template <class TString>
  inline bool BeginsWith(const TString &src, const TString &pattern)
  {
    return src.compare(0, pattern.size(), pattern) == 0;
  }

  template <class TString>
  inline bool EndsWith(const TString &src, const TString &pattern)
  {
    if(pattern.size() > src.size())
      return false;

    size_t pos = src.size() - pattern.size();
    return src.compare(pos, pattern.size(), pattern) == 0;
  }

  template <class TString>
  inline std::vector<TString> string_split(const TString &str, const TString &delimiter)
  {
    std::vector<TString> output;

    size_t start = 0;
    size_t end = str.find_first_of(delimiter);

    while(end != TString::npos)
    {
      output.emplace_back(str.substr(start, end - start));

      start = end + 1;
      end = str.find_first_of(delimiter, start);
    }

    if(start < str.length())
    {
      output.emplace_back(str.substr(start));
    }

    return output;
  }

  inline void ReplaceString(std::wstring & strBig, const std::wstring & strSrc, const std::wstring &strDst)
  {
    std::wstring::size_type pos = 0; /// 子字串的位置
    std::wstring::size_type srcLen = strSrc.size(); /// 被取代的子字串長度
    std::wstring::size_type dstLen = strDst.size(); /// 目的子字串長度
    while((pos = strBig.find(strSrc, pos)) != std::wstring::npos) ///找子字串位置
    {
      strBig.replace(pos, srcLen, strDst); /// 取代
      pos = pos + dstLen; /// 從取代後的位置開始繼續找
    } // while()
  } // ReplaceString()

  namespace Convert
  {
    inline void ToLower(std::string & str)
    {
      std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    }

    inline void ToUpper(std::string & str)
    {
      std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    }

    inline void ToLower(std::wstring & str)
    {
      std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    }

    inline void ToUpper(std::wstring & str)
    {
      std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    }

    inline std::string Utf16WideCharToMultiByte(const WCHAR * pSource, UINT charset)
    {
      std::string result;
      try
      {
        //Get size without terminating null character
        size_t nSize = WideCharToMultiByte(charset, 0, pSource, -1, NULL, 0, NULL, false);

        if(nSize > 0)
        {
          nSize--;

          if(nSize > result.max_size())
          {
            nSize = result.max_size();
            LOGAF_WARN("string too long! truncated!");
          }

          result.resize(nSize);
          WideCharToMultiByte(charset, 0, pSource, wcslen(pSource), &result[0], nSize, NULL, false);
        }
      }
      catch(std::exception &e)
      {
        LOGAF_ERROR("Exception: %s", e.what());
      }
      catch(...)
      {
        LOGAF_ERROR("Caught an exception in catch(...).");
      }

      return result;
    }

    inline std::wstring MultiByteToUtf16WideChar(const CHAR * pSource, UINT charset)
    {
      std::wstring result;
      try
      {
        size_t nSize = MultiByteToWideChar(charset, 0, pSource, -1, NULL, 0);

        if(nSize > 0)
        {
          nSize--;

          if(nSize > result.max_size())
          {
            nSize = result.max_size();
            LOGAF_WARN("string too long! truncated!");
          }

          result.resize(nSize);
          MultiByteToWideChar(charset, 0, pSource, strlen(pSource), &result[0], nSize);
        }
      }
      catch(std::exception &e)
      {
        LOGAF_ERROR("Exception: %s", e.what());
      }
      catch(...)
      {
        LOGAF_ERROR("Caught an exception in catch(...).");
      }

      return result;
    }

    inline std::string Utf16WideCharToUtf8MultiByte(const WCHAR * pSource)
    {
      return Utf16WideCharToMultiByte(pSource, CP_UTF8);
    }

    inline std::string Utf16WideCharToAnsiMultiByte(const WCHAR * pSource)
    {
      return Utf16WideCharToMultiByte(pSource, CP_ACP);
    }

    inline std::wstring AnsiMultiByteToUtf16WideChar(const CHAR * pSource)
    {
      return MultiByteToUtf16WideChar(pSource, CP_ACP);
    }

    inline std::wstring Utf8MultiByteToUtf16WideChar(const CHAR * pSource)
    {
      return MultiByteToUtf16WideChar(pSource, CP_UTF8);
    }

    inline std::string AnsiMultiByteToUtf8MultiByte(const CHAR * pSource)
    {
      std::wstring Utf16WideChar = AnsiMultiByteToUtf16WideChar(pSource);
      std::string Utf8MultiByte = Utf16WideCharToUtf8MultiByte(Utf16WideChar.c_str());
      return Utf8MultiByte;
    }

    inline std::string MultiByteToUtf8MultiByte(const CHAR * pSource, UINT charset)
    {
      std::wstring Utf16WideChar = MultiByteToUtf16WideChar(pSource, charset);
      std::string Utf8MultiByte = Utf16WideCharToUtf8MultiByte(Utf16WideChar.c_str());

      return Utf8MultiByte;
    }
  }
};