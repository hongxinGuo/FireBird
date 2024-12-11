// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H_INCLUDE_
#define PCH_H_INCLUDE_

#include"stdafx.h"
using namespace std;

//import std;
#include "framework.h"

#include"gtest/gtest.h"
#include"gmock/gmock.h"

#include"nlohmann/json.hpp"

//#include"concurrentqueue/concurrentqueue.h"
//using namespace moodycamel;

#undef max // 包含concurrencpp.h之前，需要注销max的定义
#include"concurrencpp/concurrencpp.h"
using namespace concurrencpp;

#endif
