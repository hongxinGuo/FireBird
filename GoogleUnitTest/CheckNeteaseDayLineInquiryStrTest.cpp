#include"pch.h"

#include"globedef.h"

namespace StockAnalysisTest {
  struct CheckNeteaseDayLineInquiryStrData {
    CheckNeteaseDayLineInquiryStrData(int count, CString Data) {
      m_iCount = count;
      m_strData = Data;
    }
  public:
    int m_iCount;
    CString m_strData;
  };

  CheckNeteaseDayLineInquiryStrData Data1(1, _T("0600000"));
  CheckNeteaseDayLineInquiryStrData Data2(2, _T("1000001"));
  // 无效上海股票代码
  CheckNeteaseDayLineInquiryStrData Data3(3, _T("0700000"));
  CheckNeteaseDayLineInquiryStrData Data4(4, _T("040000"));
  CheckNeteaseDayLineInquiryStrData Data5(5, _T("0400000"));
  // 无效深圳股票代码
  CheckNeteaseDayLineInquiryStrData Data6(6, _T("1400000"));
  CheckNeteaseDayLineInquiryStrData Data7(7, _T("140000"));
  CheckNeteaseDayLineInquiryStrData Data8(8, _T("1400000"));

  class CheckNeteaseDayLineInquiryStrTest : public::testing::TestWithParam<CheckNeteaseDayLineInquiryStrData*> {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      CheckNeteaseDayLineInquiryStrData* pData = GetParam();
      m_iCount = pData->m_iCount;
      m_strCode = pData->m_strData;
    }

    virtual void TearDown(void) override {
      // clearup
    }

  public:
    int m_iCount;
    CString m_strCode;
  };

  INSTANTIATE_TEST_SUITE_P(TestCheckNeteaseDayLineInquiryData, CheckNeteaseDayLineInquiryStrTest,
                           testing::Values(&Data1, &Data2, &Data3, &Data4, &Data5, &Data6, &Data7, &Data8
                                           ));

  TEST_P(CheckNeteaseDayLineInquiryStrTest, TestCheck) {
    bool fSucceed = gl_pChinaStockMarket->CheckValidOfNeteaseDayLineInquiringStr(m_strCode);
    switch (m_iCount) {
    case 1:
    EXPECT_TRUE(fSucceed);
    break;
    case 2:
    EXPECT_TRUE(fSucceed);
    break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    EXPECT_FALSE(fSucceed);
    break;
    default:
    break;
    }
  }
}