#ifndef _STANDARD_EXCEPTION_HANDLE_H
#define _STANDARD_EXCEPTION_HANDLE_H

#define EXCEPTION_OUTOFRANGE 
#define EXCEPTION_ALL catch (std::exception& e)\
  {\
    LOGAF_FATAL( "Exception caught: %s", e.what() );\
  }

#endif