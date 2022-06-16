#pragma once
#ifndef _CSV_PARSER_H
#define _CSV_PARSER_H
#include <iostream>
#include <string>
#include <vector>
#include "macros.h"
#include "string_handle.hpp"
#include "debug_log.h"
//TODO: 把delimiter抽取出來, Vector改為stringlist

//!@class CSVParser csv_parser.hpp
//!@brief 把csv格式的字串分隔放入vector中
//!Example:
//! std::wstring result = CSVParser::csvParser( str );
class CSVParser
{
public:
  //!@brief csvParser
  //!  將csv格式的字串依照逗號分隔拆分放入vector中，有用'\"'包裹住的item則會將'\"'去掉，//!"\"\"" 也會轉成"\""
  //!@param str
  //!  csv格式的字串
  //!@return 存放拆開來的字串的vector
  static std::vector<std::wstring> csvParserTab(std::wstring str)
  {
    return StringHandle::split(str, L"\t", StringHandle::CONTAIN_EMPTY);
  }

  static std::vector<std::wstring> csvParser(std::wstring str)
  {
    std::vector<std::wstring> vec; ///存放拆開來的字串
    size_t end = 0; ///子字串的結尾
    bool breakout = false; ///到結尾
    std::wstring tmp = str; /// 還沒拆分的csv字串部份
    size_t length = tmp.size();
    while (length > 0)
    {
      if (tmp[0] == L'"') /// 是被'\""包裹的字串
      {
        for (int i = 1; i < length; i++)
        {
          if (tmp[i] == L'"')
          {
            if (i + 1 == length) /// 整個字串結尾
            {
              end = i + 1;
              breakout = true;
              break;
            }
            else if (tmp[i + 1] == L'"') /// "\"\""
            {
              i++;
            }
            else if (tmp[i + 1] == L',') /// item結尾
            {
              end = i + 1;
              break;
            }
            else /// 例外
            {
              LOGWF_ERROR(L"CSV Systax ERROR!!!");
            } // else
          } // if (tmp[i] == L'"')
        } // for (int i = 1; i < tmp.size(); i++)
      } // if (tmp.substr(0, 1) == L"\"")
      else
      {
        end = tmp.find_first_of(L","); /// 找第一個item結尾
        if (end == std::wstring::npos) /// 找不到','
        {
          end = length; /// item為整個tmp字串
          breakout = true;
        }
      }

      std::wstring res = tmp.substr(0, (end)); /// 去除','
      StringHandle::strReplace(res, L"\"\"", L"\""); /// 將 "" -> "
      vec.push_back(res); /// 將拆分出來的字串放入vec中
      if (breakout) /// 到結尾跳出
        break;
      tmp = tmp.substr(end + 1, length - (end + 1)); /// 更新tmp字串
      length = tmp.size();
    }

    if (!breakout) /// 到結尾跳出
      vec.push_back(L"");

    return vec;
  }

  static std::vector<std::wstring> csvParserRec(std::wstring str)
  {
    if (str == L"")
    {
      return std::vector<std::wstring>({ L"" });
    }
    std::vector<std::wstring> vec; ///存放拆開來的字串
    size_t end = 0; ///子字串的結尾
    bool breakout = false; ///到結尾
    std::wstring tmp = str; /// 還沒拆分的csv字串部份

    if (tmp.substr(0, 1) == L"\"") /// 是被'\""包裹的字串
    {
      for (int i = 1; i < tmp.size(); i++)
      {
        if (tmp[i] == L'"')
        {
          if (i + 1 < tmp.size() && tmp[i + 1] == L'"') /// "\"\""
          {
            i++;
          }
          else if (i + 1 < tmp.size() && tmp[i + 1] == L',') /// item結尾
          {
            end = i + 1;
            break;
          }
          else if (i + 1 == tmp.size()) /// 整個字串結尾
          {
            end = i + 1;
            breakout = true;
            break;
          }
          else /// 例外
          {
            LOGWF_ERROR(L"CSV Systax ERROR!!!");
          } // else
        } // if (tmp[i] == L'"')
      } // for (int i = 1; i < tmp.size(); i++)
    } // if (tmp.substr(0, 1) == L"\"")
    else
    {
      end = tmp.find_first_of(L","); /// 找第一個item結尾
      if (end == std::wstring::npos) /// 找不到','
      {
        end = tmp.size(); /// item為整個tmp字串
        breakout = true;
      }
    }

    std::wstring res = tmp.substr(0, (end)); /// 去除','
    StringHandle::strReplace(res, L"\"\"", L"\""); /// 將 "" -> "

    vec.push_back(res); /// 將拆分出來的字串放入vec中

    std::vector<std::wstring> tmpVec = csvParserRec(tmp.substr(end + 1, tmp.size() - (end + 1)));
    vec.insert(vec.end(), tmpVec.begin(), tmpVec.end());
    return vec;
  }
private:
};

#endif