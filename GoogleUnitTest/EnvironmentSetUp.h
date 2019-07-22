#pragma once

#include"stdafx.h"
#include"pch.h"

using namespace testing;

namespace StockAnalysisTest {

  class TestEnvironment : public::testing::Environment {  // 全局初始化，由main()函数调用。
  public:
    virtual ~TestEnvironment() {
    }

    virtual void SetUp(void) {
      int a = 0;
      a = a + 1;

    }

    virtual void TearDown(void) {

    }

  };
}