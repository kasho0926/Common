#pragma once
#ifndef _CSV_PARSER_H
#define _CSV_PARSER_H
#include <iostream>
#include <string>
#include <vector>
#include "macros.h"
#include "string_handle.hpp"
#include "debug_log.h"
//TODO: ��delimiter����X��, Vector�אּstringlist

//!@class CSVParser csv_parser.hpp
//!@brief ��csv�榡���r����j��Jvector��
//!Example:
//! std::wstring result = CSVParser::csvParser( str );
class CSVParser
{
public:
  //!@brief csvParser
  //!  �Ncsv�榡���r��̷ӳr�����j�����Jvector���A����'\"'�]�q��item�h�|�N'\"'�h���A//!"\"\"" �]�|�ন"\""
  //!@param str
  //!  csv�榡���r��
  //!@return �s���}�Ӫ��r�ꪺvector
  static std::vector<std::wstring> csvParserTab(std::wstring str)
  {
    return StringHandle::split(str, L"\t", StringHandle::CONTAIN_EMPTY);
  }

  static std::vector<std::wstring> csvParser(std::wstring str)
  {
    std::vector<std::wstring> vec; ///�s���}�Ӫ��r��
    size_t end = 0; ///�l�r�ꪺ����
    bool breakout = false; ///�쵲��
    std::wstring tmp = str; /// �٨S�����csv�r�곡��
    size_t length = tmp.size();
    while (length > 0)
    {
      if (tmp[0] == L'"') /// �O�Q'\""�]�q���r��
      {
        for (int i = 1; i < length; i++)
        {
          if (tmp[i] == L'"')
          {
            if (i + 1 == length) /// ��Ӧr�굲��
            {
              end = i + 1;
              breakout = true;
              break;
            }
            else if (tmp[i + 1] == L'"') /// "\"\""
            {
              i++;
            }
            else if (tmp[i + 1] == L',') /// item����
            {
              end = i + 1;
              break;
            }
            else /// �ҥ~
            {
              LOGWF_ERROR(L"CSV Systax ERROR!!!");
            } // else
          } // if (tmp[i] == L'"')
        } // for (int i = 1; i < tmp.size(); i++)
      } // if (tmp.substr(0, 1) == L"\"")
      else
      {
        end = tmp.find_first_of(L","); /// ��Ĥ@��item����
        if (end == std::wstring::npos) /// �䤣��','
        {
          end = length; /// item�����tmp�r��
          breakout = true;
        }
      }

      std::wstring res = tmp.substr(0, (end)); /// �h��','
      StringHandle::strReplace(res, L"\"\"", L"\""); /// �N "" -> "
      vec.push_back(res); /// �N����X�Ӫ��r���Jvec��
      if (breakout) /// �쵲�����X
        break;
      tmp = tmp.substr(end + 1, length - (end + 1)); /// ��stmp�r��
      length = tmp.size();
    }

    if (!breakout) /// �쵲�����X
      vec.push_back(L"");

    return vec;
  }

  static std::vector<std::wstring> csvParserRec(std::wstring str)
  {
    if (str == L"")
    {
      return std::vector<std::wstring>({ L"" });
    }
    std::vector<std::wstring> vec; ///�s���}�Ӫ��r��
    size_t end = 0; ///�l�r�ꪺ����
    bool breakout = false; ///�쵲��
    std::wstring tmp = str; /// �٨S�����csv�r�곡��

    if (tmp.substr(0, 1) == L"\"") /// �O�Q'\""�]�q���r��
    {
      for (int i = 1; i < tmp.size(); i++)
      {
        if (tmp[i] == L'"')
        {
          if (i + 1 < tmp.size() && tmp[i + 1] == L'"') /// "\"\""
          {
            i++;
          }
          else if (i + 1 < tmp.size() && tmp[i + 1] == L',') /// item����
          {
            end = i + 1;
            break;
          }
          else if (i + 1 == tmp.size()) /// ��Ӧr�굲��
          {
            end = i + 1;
            breakout = true;
            break;
          }
          else /// �ҥ~
          {
            LOGWF_ERROR(L"CSV Systax ERROR!!!");
          } // else
        } // if (tmp[i] == L'"')
      } // for (int i = 1; i < tmp.size(); i++)
    } // if (tmp.substr(0, 1) == L"\"")
    else
    {
      end = tmp.find_first_of(L","); /// ��Ĥ@��item����
      if (end == std::wstring::npos) /// �䤣��','
      {
        end = tmp.size(); /// item�����tmp�r��
        breakout = true;
      }
    }

    std::wstring res = tmp.substr(0, (end)); /// �h��','
    StringHandle::strReplace(res, L"\"\"", L"\""); /// �N "" -> "

    vec.push_back(res); /// �N����X�Ӫ��r���Jvec��

    std::vector<std::wstring> tmpVec = csvParserRec(tmp.substr(end + 1, tmp.size() - (end + 1)));
    vec.insert(vec.end(), tmpVec.begin(), tmpVec.end());
    return vec;
  }
private:
};

#endif