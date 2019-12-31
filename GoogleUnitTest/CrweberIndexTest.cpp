#include "pch.h"

#include"Market.h"
#include"crweberIndex.h"

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
    id.m_dVLCC_USGSPORE = 22.0;
    id.m_dSUEZMAX_CBSUSG = 23.0;
    id.m_dTC1 = 24.0;
    id.m_dTC2 = 25.0;
    id.m_dTC3 = 26.0;
    id.m_dTC4 = 27.0;
    id.m_dTC5 = 28.0;
    id.m_dTC14 = 29.0;

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

    setCrweberIndex.Open();
    id2.LoadData(setCrweberIndex);
    setCrweberIndex.Close();
    EXPECT_EQ(id.m_lDay, id2.m_lDay);
    EXPECT_DOUBLE_EQ(id.m_dTD1, id2.m_dTD1);

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

    gl_CrweberIndex = id;
    gl_CrweberIndexLast = id;
    EXPECT_FALSE(id.IsDataChanged());
    gl_CrweberIndex.m_dTD1 = id.m_dTD1 + 1;
    EXPECT_TRUE(id.IsDataChanged());
    gl_CrweberIndex.m_dTD1 = id.m_dTD1;
    EXPECT_FALSE(id.IsDataChanged());
    gl_CrweberIndex.m_dTD2 = id.m_dTD2 + 1;
    EXPECT_TRUE(id.IsDataChanged());
    gl_CrweberIndex.m_dTD2 = id.m_dTD2;
    EXPECT_FALSE(id.IsDataChanged());
    gl_CrweberIndex.m_dTD3C = id.m_dTD3C + 1;
    EXPECT_TRUE(id.IsDataChanged());
    gl_CrweberIndex.m_dTD3C = id.m_dTD3C;
    EXPECT_FALSE(id.IsDataChanged());
    gl_CrweberIndex.m_dTD5 = id.m_dTD5 + 1;
    EXPECT_TRUE(id.IsDataChanged());
  }

  TEST(CrweberIndexTest, TestGetMonth) {
    CCrweberIndex Index;
    EXPECT_EQ(Index.GetMonth(_T("January")), 1);
    EXPECT_EQ(Index.GetMonth(_T("Febrary")), 2);
    EXPECT_EQ(Index.GetMonth(_T("March")), 3);
    EXPECT_EQ(Index.GetMonth(_T("April")), 4);
    EXPECT_EQ(Index.GetMonth(_T("May")), 5);
    EXPECT_EQ(Index.GetMonth(_T("June")), 6);
    EXPECT_EQ(Index.GetMonth(_T("July")), 7);
    EXPECT_EQ(Index.GetMonth(_T("August")), 8);
    EXPECT_EQ(Index.GetMonth(_T("September")), 9);
    EXPECT_EQ(Index.GetMonth(_T("October")), 10);
    EXPECT_EQ(Index.GetMonth(_T("November")), 11);
    EXPECT_EQ(Index.GetMonth(_T("December")), 12);
    EXPECT_EQ(Index.GetMonth(_T("DEcmber")), gl_systemTime.GetMonth());
    EXPECT_EQ(Index.GetMonth(_T("Junuary")), gl_systemTime.GetMonth());
  }
}