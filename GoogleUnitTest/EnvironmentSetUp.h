#pragma once

#include"stdafx.h"
#include"pch.h"

using namespace testing;

namespace StockAnalysisTest {

  class TestEnvironment : public::testing::Environment {  // ȫ�ֳ�ʼ������main()�������á�
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