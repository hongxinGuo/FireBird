#pragma once

#include"stdafx.h"
#include"pch.h"

namespace StockAnalysisTest {

  class TestEnvironment : public::testing::Environment {
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