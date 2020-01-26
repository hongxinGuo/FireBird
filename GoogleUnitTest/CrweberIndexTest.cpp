#include "pch.h"

#include"ChinaMarket.h"
#include"crweberIndex.h"

using namespace std;
#include<string>

namespace StockAnalysisTest {
  TEST(CrweberTest, TestInitialize) {
    CCrweberIndex Index;
    EXPECT_FALSE(Index.IsTodayUpdated());
    Index.SetNewestDataTime(20191101);
    EXPECT_EQ(Index.GetCurrentDataTime(), 20191101);
    EXPECT_DOUBLE_EQ(Index.GetTD1(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD2(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD3C(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD5(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD6(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD7(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD8(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD9(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD12(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD15(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD19(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD20(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD21(), 0);
    EXPECT_DOUBLE_EQ(Index.GetVLCC_USGSPORE(), 0);
    EXPECT_DOUBLE_EQ(Index.GetSUEZMAX_CBSUSG(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTC1(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTC2(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTC3(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTC4(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTC5(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTC14(), 0);
    EXPECT_DOUBLE_EQ(Index.GetCPP_USGCBS(), 0);
    EXPECT_DOUBLE_EQ(Index.GetVLCC_1YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetSUEZMAX_1YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetAFRAMAX_1YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetMR_1YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetHANDY_1YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetVLCC_3YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetSUEZMAX_3YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetAFRAMAX_3YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetMR_3YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetHANDY_3YEAR(), 0);
  }

  TEST(CrwberTest, TestLoadData) {
    CSetCrweberIndex setCrweberIndex;
    CCrweberIndex id, id2;
    id.m_lDay = 20000101;
    id.m_dTD1 = 1.0;
    id.m_dTD2 = 2.0;
    id.m_dTD3C = 3.0;
    id.m_dTD5 = 4.0;
    id.m_dTD6 = 5.0;
    id.m_dTD7 = 6.0;
    id.m_dTD8 = 7.0;
    id.m_dTD9 = 8.0;
    id.m_dTD12 = 9.0;
    id.m_dTD15 = 10.0;
    id.m_dTD19 = 11.0;
    id.m_dTD20 = 12.0;
    id.m_dTD21 = 13.0;
    id.m_dVLCC_USGSPORE = 14.0;
    id.m_dSUEZMAX_CBSUSG = 15.0;
    id.m_dTC1 = 16.0;
    id.m_dTC2 = 17.0;
    id.m_dTC3 = 18.0;
    id.m_dTC4 = 19.0;
    id.m_dTC5 = 20.0;
    id.m_dTC14 = 21.0;
    id.m_dCPP_USGCBS = 22;
    id.m_dVLCC_TC_1YEAR = 23;
    id.m_dVLCC_TC_3YEAR = 24;
    id.m_dSUEZMAX_TC_1YEAR = 25;
    id.m_dSUEZMAX_TC_3YEAR = 26;
    id.m_dAFRAMAX_TC_1YEAR = 27;
    id.m_dAFRAMAX_TC_3YEAR = 28;
    id.m_dPANAMAX_TC_1YEAR = 29;
    id.m_dPANAMAX_TC_3YEAR = 30;
    id.m_dMR_TC_1YEAR = 31;
    id.m_dMR_TC_3YEAR = 32;
    id.m_dHANDY_TC_1YEAR = 33;
    id.m_dHANDY_TC_3YEAR = 34;

    setCrweberIndex.Open();
    setCrweberIndex.m_pDatabase->BeginTrans();
    while (!setCrweberIndex.IsEOF()) {
      setCrweberIndex.Delete();
      setCrweberIndex.MoveNext();
    }
    setCrweberIndex.m_pDatabase->CommitTrans();
    setCrweberIndex.Close();
    setCrweberIndex.Open();
    setCrweberIndex.m_pDatabase->BeginTrans();
    setCrweberIndex.AddNew();
    id.SaveData(setCrweberIndex);
    setCrweberIndex.Update();
    setCrweberIndex.m_pDatabase->CommitTrans();
    setCrweberIndex.Close();

    setCrweberIndex.m_strFilter = _T("[Day] = 20000101");
    setCrweberIndex.Open();
    id2.LoadData(setCrweberIndex);
    setCrweberIndex.Close();
    EXPECT_EQ(id.m_lDay, id2.m_lDay);
    EXPECT_DOUBLE_EQ(id.m_dTD1, id2.m_dTD1);
    EXPECT_DOUBLE_EQ(id.m_dTD2, id2.m_dTD2);
    EXPECT_DOUBLE_EQ(id.m_dTD3C, id2.m_dTD3C);
    EXPECT_DOUBLE_EQ(id.m_dTD5, id2.m_dTD5);
    EXPECT_DOUBLE_EQ(id.m_dTD6, id2.m_dTD6);
    EXPECT_DOUBLE_EQ(id.m_dTD7, id2.m_dTD7);
    EXPECT_DOUBLE_EQ(id.m_dTD8, id2.m_dTD8);
    EXPECT_DOUBLE_EQ(id.m_dTD9, id2.m_dTD9);
    EXPECT_DOUBLE_EQ(id.m_dTD12, id2.m_dTD12);
    EXPECT_DOUBLE_EQ(id.m_dTD15, id2.m_dTD15);
    EXPECT_DOUBLE_EQ(id.m_dTD19, id2.m_dTD19);
    EXPECT_DOUBLE_EQ(id.m_dTD20, id2.m_dTD20);
    EXPECT_DOUBLE_EQ(id.m_dTD21, id2.m_dTD21);
    EXPECT_DOUBLE_EQ(id.m_dVLCC_USGSPORE, id2.m_dVLCC_USGSPORE);
    EXPECT_DOUBLE_EQ(id.m_dSUEZMAX_CBSUSG, id2.m_dSUEZMAX_CBSUSG);

    EXPECT_DOUBLE_EQ(id.m_dTC1, id2.m_dTC1);
    EXPECT_DOUBLE_EQ(id.m_dTC2, id2.m_dTC2);
    EXPECT_DOUBLE_EQ(id.m_dTC3, id2.m_dTC3);
    EXPECT_DOUBLE_EQ(id.m_dTC4, id2.m_dTC4);
    EXPECT_DOUBLE_EQ(id.m_dTC5, id2.m_dTC5);
    EXPECT_DOUBLE_EQ(id.m_dTC14, id2.m_dTC14);
    EXPECT_DOUBLE_EQ(id.m_dCPP_USGCBS, id2.m_dCPP_USGCBS);

    EXPECT_DOUBLE_EQ(id.m_dVLCC_TC_1YEAR, id2.m_dVLCC_TC_1YEAR);
    EXPECT_DOUBLE_EQ(id.m_dVLCC_TC_3YEAR, id2.m_dVLCC_TC_3YEAR);
    EXPECT_DOUBLE_EQ(id.m_dSUEZMAX_TC_1YEAR, id2.m_dSUEZMAX_TC_1YEAR);
    EXPECT_DOUBLE_EQ(id.m_dSUEZMAX_TC_3YEAR, id2.m_dSUEZMAX_TC_3YEAR);
    EXPECT_DOUBLE_EQ(id.m_dAFRAMAX_TC_1YEAR, id2.m_dAFRAMAX_TC_1YEAR);
    EXPECT_DOUBLE_EQ(id.m_dAFRAMAX_TC_3YEAR, id2.m_dAFRAMAX_TC_3YEAR);
    EXPECT_DOUBLE_EQ(id.m_dPANAMAX_TC_1YEAR, id2.m_dPANAMAX_TC_1YEAR);
    EXPECT_DOUBLE_EQ(id.m_dPANAMAX_TC_3YEAR, id2.m_dPANAMAX_TC_3YEAR);
    EXPECT_DOUBLE_EQ(id.m_dMR_TC_1YEAR, id2.m_dMR_TC_1YEAR);
    EXPECT_DOUBLE_EQ(id.m_dMR_TC_3YEAR, id2.m_dMR_TC_3YEAR);
    EXPECT_DOUBLE_EQ(id.m_dHANDY_TC_1YEAR, id2.m_dHANDY_TC_1YEAR);
    EXPECT_DOUBLE_EQ(id.m_dHANDY_TC_3YEAR, id2.m_dHANDY_TC_3YEAR);

    id.m_dTD1 = 30.0;
    setCrweberIndex.Open();
    setCrweberIndex.m_pDatabase->BeginTrans();
    id.AppendData(setCrweberIndex);
    setCrweberIndex.m_pDatabase->CommitTrans();
    setCrweberIndex.Close();

    setCrweberIndex.Open();
    setCrweberIndex.MoveNext();
    id2.LoadData(setCrweberIndex);
    setCrweberIndex.Close();
    EXPECT_DOUBLE_EQ(id2.m_dTD1, 30.0);
  }

  TEST(CrweberIndexTest, TestIsDataChanged) {
    CCrweberIndex id, id2;
    id.m_lDay = 20000101;
    id.m_dTD1 = 1.0;
    id.m_dTD2 = 2.0;
    id.m_dTD3C = 3.0;
    id.m_dTD5 = 4.0;
    id.m_dTD6 = 5.0;
    id.m_dTD7 = 6.0;
    id.m_dTD8 = 7.0;
    id.m_dTD9 = 8.0;
    id.m_dTD12 = 9.0;
    id.m_dTD15 = 10.0;
    id.m_dTD19 = 11.0;
    id.m_dTD20 = 12.0;
    id.m_dTD21 = 13.0;
    id.m_dVLCC_USGSPORE = 22.0;
    id.m_dSUEZMAX_CBSUSG = 23.0;
    id.m_dTC1 = 24.0;
    id.m_dTC2 = 25.0;
    id.m_dTC3 = 26.0;
    id.m_dTC4 = 27.0;
    id.m_dTC5 = 28.0;
    id.m_dTC14 = 29.0;

    CCrweberIndex CrweberIndex = id;
    CCrweberIndex CrweberIndexLast = id;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD1 = id.m_dTD1 + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD1 = id.m_dTD1;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD2 = id.m_dTD2 + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD2 = id.m_dTD2;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD3C = id.m_dTD3C + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD3C = id.m_dTD3C;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD5 = id.m_dTD5 + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD5 = id.m_dTD5;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD6 = id.m_dTD6 + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD6 = id.m_dTD6;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD7 = id.m_dTD7 + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD7 = id.m_dTD7;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD8 = id.m_dTD8 + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD8 = id.m_dTD8;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD9 = id.m_dTD9 + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD9 = id.m_dTD9;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD12 = id.m_dTD12 + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD12 = id.m_dTD12;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD15 = id.m_dTD15 + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD15 = id.m_dTD15;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD19 = id.m_dTD19 + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD19 = id.m_dTD19;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD20 = id.m_dTD20 + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTD20 = id.m_dTD20;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dVLCC_USGSPORE = id.m_dVLCC_USGSPORE + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dVLCC_USGSPORE = id.m_dVLCC_USGSPORE;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTC1 = id.m_dTC1 + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTC1 = id.m_dTC1;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTC2 = id.m_dTC2 + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTC2 = id.m_dTC2;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTC3 = id.m_dTC3 + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTC3 = id.m_dTC3;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTC4 = id.m_dTC4 + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTC4 = id.m_dTC4;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTC5 = id.m_dTC5 + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTC5 = id.m_dTC5;
    EXPECT_FALSE(id.IsDataChanged(CrweberIndex));
    CrweberIndex.m_dTC14 = id.m_dTC14 + 1;
    EXPECT_TRUE(id.IsDataChanged(CrweberIndex));
  }

  TEST(CrweberIndexTest, TestGetMonthOfYear) {
    CCrweberIndex Index;
    EXPECT_EQ(Index.GetMonthOfTheYear(_T("January")), 1);
    EXPECT_EQ(Index.GetMonthOfTheYear(_T("Febrary")), 2);
    EXPECT_EQ(Index.GetMonthOfTheYear(_T("March")), 3);
    EXPECT_EQ(Index.GetMonthOfTheYear(_T("April")), 4);
    EXPECT_EQ(Index.GetMonthOfTheYear(_T("May")), 5);
    EXPECT_EQ(Index.GetMonthOfTheYear(_T("June")), 6);
    EXPECT_EQ(Index.GetMonthOfTheYear(_T("July")), 7);
    EXPECT_EQ(Index.GetMonthOfTheYear(_T("August")), 8);
    EXPECT_EQ(Index.GetMonthOfTheYear(_T("September")), 9);
    EXPECT_EQ(Index.GetMonthOfTheYear(_T("October")), 10);
    EXPECT_EQ(Index.GetMonthOfTheYear(_T("November")), 11);
    EXPECT_EQ(Index.GetMonthOfTheYear(_T("December")), 12);
    EXPECT_EQ(Index.GetMonthOfTheYear(_T("DEcmber")), gl_CrweberIndexMarket.GetMonthOfYear());
    EXPECT_EQ(Index.GetMonthOfTheYear(_T("Junuary")), gl_CrweberIndexMarket.GetMonthOfYear());
  }

  struct CrweberIndexTime {
    CrweberIndexTime(long lId, CString strTime, long lTime) {
      m_lId = lId;
      m_strTime = strTime;
      m_lTime = lTime;
    }

  public:
    long m_lId;
    CString m_strTime;
    long m_lTime;
  };

  CrweberIndexTime CrweberIndexTimeData1(1, _T(" January 25 2019 "), 20190125);
  CrweberIndexTime CrweberIndexTimeData2(2, _T(" Febrary 2 2019 "), 20190202);
  CrweberIndexTime CrweberIndexTimeData3(3, _T(" March 5 2019 "), 20190305);
  CrweberIndexTime CrweberIndexTimeData4(4, _T(" April 25 2019 "), 20190425);
  CrweberIndexTime CrweberIndexTimeData5(5, _T(" May 25 2019 "), 20190525);
  CrweberIndexTime CrweberIndexTimeData6(6, _T(" June 25 2019 "), 20190625);
  CrweberIndexTime CrweberIndexTimeData7(7, _T(" July 25 2019 "), 20190725);
  CrweberIndexTime CrweberIndexTimeData8(8, _T(" August 25 2019 "), 20190825);
  CrweberIndexTime CrweberIndexTimeData9(9, _T(" September 25 2019 "), 20190925);
  CrweberIndexTime CrweberIndexTimeData10(10, _T(" October 7 2019 "), 20191007);
  CrweberIndexTime CrweberIndexTimeData11(11, _T(" November 5 2019 "), 20191105);
  CrweberIndexTime CrweberIndexTimeData12(12, _T(" December 2 2019 "), 20191202);
  CrweberIndexTime CrweberIndexTimeData13(13, _T(" decmber 1 2019 "), 20190101);
  CrweberIndexTime CrweberIndexTimeData14(14, _T(" January, 25 2019 "), 20190125);
  CrweberIndexTime CrweberIndexTimeData15(15, _T(" January, 25 2019 "), 20190125);

  class CrweberIndexTimeTest : public testing::TestWithParam<CrweberIndexTime*> {
  protected:
    static void SetUpTestSuite() {
    }
    static void TearDownTestSuite() {
    }
    virtual void SetUp(void) override {
      ASSERT(!gl_fNormalMode);
      CrweberIndexTime* pCrweberIndexTime = GetParam();
      m_lId = pCrweberIndexTime->m_lId;
      m_strTime = pCrweberIndexTime->m_strTime;
      m_lTime = pCrweberIndexTime->m_lTime;
    }
    virtual void TearDown(void) override {
      // clearup
    }
  public:
    long m_lId;
    CString m_strTime;
    long m_lTime;
  };

  INSTANTIATE_TEST_CASE_P(TestCrweberIndexTime, CrweberIndexTimeTest,
                          testing::Values(&CrweberIndexTimeData1
                                          , &CrweberIndexTimeData2, &CrweberIndexTimeData3, &CrweberIndexTimeData4, &CrweberIndexTimeData5
                                          , &CrweberIndexTimeData6, &CrweberIndexTimeData7, &CrweberIndexTimeData8, &CrweberIndexTimeData9
                                          , &CrweberIndexTimeData10, &CrweberIndexTimeData11, &CrweberIndexTimeData12, &CrweberIndexTimeData13
                                          , &CrweberIndexTimeData14, &CrweberIndexTimeData15));

  TEST_P(CrweberIndexTimeTest, TestCrweberIndexTime) {
    CCrweberIndex Index;
    long lMonth = m_lTime / 100 - (m_lTime / 10000) * 100;
    long lTime = Index.ConvertStringToTime(m_strTime);

    switch (m_lId) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    EXPECT_EQ(lTime, m_lTime);
    break;
    default:
    EXPECT_EQ(lTime, m_lTime - lMonth * 100 + gl_CrweberIndexMarket.GetMonthOfYear() * 100);
    }
  }
}