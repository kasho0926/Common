#ifndef _STRUCTURED_EXCEPTION_HANDLE_H
#define _STRUCTURED_EXCEPTION_HANDLE_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include "debug_log.h"

#define SEHEXCEPT __except( filter( GetExceptionCode() ) )\
    {\
      LOGWF_FATAL(L"Exception: %s!!!", SEDescription(GetExceptionCode()) );\
      return 1;\
    }

#define SEHEXCEPTVOID __except( filter( GetExceptionCode() ) )\
    {\
      LOGWF_FATAL(L"Exception: %s!!!", SEDescription(GetExceptionCode()) );\
    }

inline const wchar_t* SEDescription( const DWORD& code )
{
  switch( code ) {
  case EXCEPTION_ACCESS_VIOLATION:         return L"EXCEPTION_ACCESS_VIOLATION"         ;
  case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return L"EXCEPTION_ARRAY_BOUNDS_EXCEEDED"    ;
  case EXCEPTION_BREAKPOINT:               return L"EXCEPTION_BREAKPOINT"               ;
  case EXCEPTION_DATATYPE_MISALIGNMENT:    return L"EXCEPTION_DATATYPE_MISALIGNMENT"    ;
  case EXCEPTION_FLT_DENORMAL_OPERAND:     return L"EXCEPTION_FLT_DENORMAL_OPERAND"     ;
  case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return L"EXCEPTION_FLT_DIVIDE_BY_ZERO"       ;
  case EXCEPTION_FLT_INEXACT_RESULT:       return L"EXCEPTION_FLT_INEXACT_RESULT"       ;
  case EXCEPTION_FLT_INVALID_OPERATION:    return L"EXCEPTION_FLT_INVALID_OPERATION"    ;
  case EXCEPTION_FLT_OVERFLOW:             return L"EXCEPTION_FLT_OVERFLOW"             ;
  case EXCEPTION_FLT_STACK_CHECK:          return L"EXCEPTION_FLT_STACK_CHECK"          ;
  case EXCEPTION_FLT_UNDERFLOW:            return L"EXCEPTION_FLT_UNDERFLOW"            ;
  case EXCEPTION_ILLEGAL_INSTRUCTION:      return L"EXCEPTION_ILLEGAL_INSTRUCTION"      ;
  case EXCEPTION_IN_PAGE_ERROR:            return L"EXCEPTION_IN_PAGE_ERROR"            ;
  case EXCEPTION_INT_DIVIDE_BY_ZERO:       return L"EXCEPTION_INT_DIVIDE_BY_ZERO"       ;
  case EXCEPTION_INT_OVERFLOW:             return L"EXCEPTION_INT_OVERFLOW"             ;
  case EXCEPTION_INVALID_DISPOSITION:      return L"EXCEPTION_INVALID_DISPOSITION"      ;
  case EXCEPTION_NONCONTINUABLE_EXCEPTION: return L"EXCEPTION_NONCONTINUABLE_EXCEPTION" ;
  case EXCEPTION_PRIV_INSTRUCTION:         return L"EXCEPTION_PRIV_INSTRUCTION"         ;
  case EXCEPTION_SINGLE_STEP:              return L"EXCEPTION_SINGLE_STEP"              ;
  case EXCEPTION_STACK_OVERFLOW:           return L"EXCEPTION_STACK_OVERFLOW"           ;
  default: 
    return L"UNKNOWN EXCEPTION" ;
  }
}

inline const DWORD filter( const DWORD& code )
{
  switch( code ) {
  case EXCEPTION_ACCESS_VIOLATION:         return 1; 
  case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return 1;
  case EXCEPTION_BREAKPOINT:               return 1;
  case EXCEPTION_DATATYPE_MISALIGNMENT:    return 1;
  case EXCEPTION_FLT_DENORMAL_OPERAND:     return 1;
  case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return 1;
  case EXCEPTION_FLT_INEXACT_RESULT:       return 1;
  case EXCEPTION_FLT_INVALID_OPERATION:    return 1;
  case EXCEPTION_FLT_OVERFLOW:             return 1;
  case EXCEPTION_FLT_STACK_CHECK:          return 1;
  case EXCEPTION_FLT_UNDERFLOW:            return 1;
  case EXCEPTION_ILLEGAL_INSTRUCTION:      return 1;
  case EXCEPTION_IN_PAGE_ERROR:            return 1;
  case EXCEPTION_INT_DIVIDE_BY_ZERO:       return 1;
  case EXCEPTION_INT_OVERFLOW:             return 1;
  case EXCEPTION_INVALID_DISPOSITION:      return 1;
  case EXCEPTION_NONCONTINUABLE_EXCEPTION: return 1;
  case EXCEPTION_PRIV_INSTRUCTION:         return 1;
  case EXCEPTION_SINGLE_STEP:              return 1;
  case EXCEPTION_STACK_OVERFLOW:           return 1;
  default: return 0 ;
  }
}



#endif