#pragma once

#include <exception>
#include <iostream>
#include <stdio.h>

#define MYTEST( name ) TEST_##name
#define FUNC_TEST( name ) void name##Test()
#define RUN(x, name ) if( x|| TEST_ALL) name##Test();


#define ZIP_TEST 1

const bool TEST_ALL = 0 ;
const bool TEST_printPESectionInfo = 0 ;
const bool TEST_getPESectionInfo = 0 ;
const bool TEST_byteSearch = 0 ;
const bool TEST_DW2LE = 0 ;
const bool TEST_LogMessageW = 0 ;
const bool TEST_getValueWString = 0 ;
const bool TEST_enumDirectory = 0 ;
const bool TEST_getWriteTime = 1 ;
const bool TEST_getMyDoc = 1 ;