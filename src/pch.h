// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H_INCLUDE_
#define PCH_H_INCLUDE_

#include"stdafx.h"
using namespace std;
#include"gtest/gtest.h"
#include"gmock/gmock.h"
using namespace testing;

// fmt库默认使用unicode字符,本系统默认使用多字节字符。关闭fmt默认项
#define FMT_UNICODE 0

#include"simdjson.h"
using namespace simdjson;

#include"globedef.h"

#endif
