#include"stdafx.h"
#include"pch.h"

namespace StockAnalysisTest {

  struct GuaDanData {
    GuaDanData();

  public:

  };



  class RTDataGuaDanTest : public::testing::TestWithParam<GuaDanData *>
  {
  protected:
    void SetUp(void) override {
      GuaDanData * pData = GetParam();
    }

    void TearDown(void) override {
      // clearup
    }

  public:

  };

  //INSTANTIATE_TEST_CASE_P(TestGuaDanData, RTDataGuaDanTest, testing::Values());

}