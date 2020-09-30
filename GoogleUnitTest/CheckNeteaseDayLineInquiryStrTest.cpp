#include"pch.h"

#include"globedef.h"

namespace StockAnalysisTest {
  struct CheckNeteaseDLInquiryStrData {
    CheckNeteaseDLInquiryStrData(int count, CString Data) {
      m_iCount = count;
      m_strData = Data;
    }
  public:
    int m_iCount;
    CString m_strData;
  };

  CheckNeteaseDLInquiryStrData Data1(1, _T("0600000"));
  CheckNeteaseDLInquiryStrData Data2(2, _T("1000001"));
  // 无效上海股票代码
  CheckNeteaseDLInquiryStrData Data3(3, _T("0700000"));
  CheckNeteaseDLInquiryStrData Data4(4, _T("040000"));
  CheckNeteaseDLInquiryStrData Data5(5, _T("0400000"));
  // 无效深圳股票代码
  CheckNeteaseDLInquiryStrData Data6(6, _T("1400000"));
  CheckNeteaseDLInquiryStrData Data7(7, _T("140000"));
  CheckNeteaseDLInquiryStrData Data8(8, _T("1400000"));

  class CheckNeteaseDLInquiryStrTest : public::testing::TestWithParam<CheckNeteaseDLInquiryStrData*> {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      EXPECT_EQ(gl_pChinaStockMarket->GetDLNeedUpdateNumber(), 12000);
      CheckNeteaseDLInquiryStrData* pData = GetParam();
      m_iCount = pData->m_iCount;
      m_strCode = pData->m_strData;
    }

    virtual void TearDown(void) override {
      // clearup
      EXPECT_EQ(gl_pChinaStockMarket->GetDLNeedUpdateNumber(), 12000);
    }

  public:
    int m_iCount;
    CString m_strCode;
  };

  INSTANTIATE_TEST_SUITE_P(TestCheckNeteaseDLInquiryData, CheckNeteaseDLInquiryStrTest,
                           testing::Values(&Data1, &Data2, &Data3, &Data4, &Data5, &Data6, &Data7, &Data8
                           ));

  TEST_P(CheckNeteaseDLInquiryStrTest, TestCheck) {
    bool fSucceed = gl_pChinaStockMarket->CheckValidOfNeteaseDLInquiringStr(m_strCode);
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