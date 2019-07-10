#include"stdafx.h"
#include"pch.h"

#include"Stock.h"

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
      // Ԥ��20���ҵ�
      for (int i = 0; i < 20; i++) {
        m_stock.TestSetGuaDanDeque(980 + i, 100000); // ���еĹҵ���������Ϊʮ���
      }
    }

    void TearDown(void) override {
      // clearup
    }

  public:
    CStock m_stock;
  };

  //INSTANTIATE_TEST_CASE_P(TestGuaDanData, RTDataGuaDanTest, testing::Values());

}