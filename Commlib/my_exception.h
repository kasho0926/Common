#pragma once
#include <exception>

#define DEFINE_EXCEPTION(ClassName,Message) \
class ClassName : public std::exception \
{\
public:\
  ClassName(char* msg):m_msg(msg){}\
  virtual const char* what() const throw()\
  {\
  return m_msg;\
  }\
private:\
  const char *m_msg;\
};


DEFINE_EXCEPTION(test_error,"test_error");
