#include "pch.h"

#include"globedef.h"

#include"PotenDailyBriefing.h"

namespace StockAnalysisTest {
  class CPotenDailyBriefingTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
    }

    virtual void TearDown(void) override {
      // clearup
    }
  };

  TEST_F(CPotenDailyBriefingTest, TestReset) {
    CPotenDailyBriefing id;
    EXPECT_FALSE(id.IsTodayUpdated());
    id.SetTodayUpdated(true);
    id.Reset();
    EXPECT_FALSE(id.IsTodayUpdated());
  }

  TEST_F(CPotenDailyBriefingTest, TestIsTodayDataUpdated) {
    CPotenDailyBriefing id;
    EXPECT_FALSE(id.IsTodayUpdated());
    id.SetTodayUpdated(true);
    EXPECT_TRUE(id.IsTodayUpdated());
    id.SetTodayUpdated(false);
    EXPECT_FALSE(id.IsTodayUpdated());
  }

  TEST_F(CPotenDailyBriefingTest, TestGetCurrentDataTime) {
    CPotenDailyBriefing id;
    EXPECT_EQ(id.GetCurrentDataTime(), 0);
    id.SetNewestDataTime(20190101);
    EXPECT_EQ(id.GetCurrentDataTime(), 20190101);
  }

  TEST_F(CPotenDailyBriefingTest, TestGetDay) {
    CPotenDailyBriefing id;
    EXPECT_EQ(id.GetDay(), 0);
    id.SetDay(20190101);
    EXPECT_EQ(id.GetDay(), 20190101);
  }

  TEST_F(CPotenDailyBriefingTest, TestGetSet) {
    CPotenDailyBriefing id;
    id.m_dTD3C = 1;
    EXPECT_DOUBLE_EQ(id.GetTD3C(), 1);
  }

  TEST_F(CPotenDailyBriefingTest, TestConvertStringToTime) {
    CString str = _T("04/11/2018");
    CPotenDailyBriefing id;
    EXPECT_EQ(id.ConvertStringToTime(str), 20180411);
  }

  TEST_F(CPotenDailyBriefingTest, TestGetOneValue) {
    CPotenDailyBriefing id;
    EXPECT_DOUBLE_EQ(id.GetOneValue(_T("7,600")), 7600);
    EXPECT_DOUBLE_EQ(id.GetOneValue(_T("(7,600)")), -7600);
    EXPECT_DOUBLE_EQ(id.GetOneValue(_T("(1,100)")), -1100);
    EXPECT_DOUBLE_EQ(id.GetOneValue(_T("(7,600")), -7600);
  }

  TEST_F(CPotenDailyBriefingTest, TestLoadData) {
    CPotenDailyBriefing id, id2;
    CSetPotenDailyBriefing setPoten;
    long lDay = gl_pPotenDailyBriefingMarket->GetDay();
    char buffer[30];
    sprintf_s(buffer, "%08d", lDay);
    CString strDay = buffer;

    setPoten.m_strFilter = _T("[Day] = ") + strDay;
    setPoten.Open();
    setPoten.m_pDatabase->BeginTrans();
    while (!setPoten.IsEOF()) {
      setPoten.Delete();
      setPoten.MoveNext();
    }
    setPoten.m_pDatabase->CommitTrans();
    setPoten.Close();

    id.m_lDay = lDay;
    id.m_dTD3C = 1;
    id.m_dTD9 = 2;
    id.m_dTD20 = 3;
    id.m_dTD21 = 4;
    id.m_dTC1 = 5;
    id.m_dTC2 = 6;
    id.m_dTC5 = 7;
    id.m_dTC14 = 8;
    id.m_dVLCC_TC_1YEAR = 9;
    id.m_dSUEZMAX_TC_1YEAR = 10;
    id.m_dAFRAMAX_TC_1YEAR = 11;
    id.m_dLR2_TC_1YEAR = 12;
    id.m_dLR1_TC_1YEAR = 13;
    id.m_dMR_TC_1YEAR = 14;
    id.m_dHANDYMAX_TC_1YEAR = 15;

    id.m_dVLCC_NewBuild = 16;
    id.m_dSUEZMAX_NewBuild = 17;
    id.m_dAFRAMAX_NewBuild = 18;
    id.m_dLR2_NewBuild = 19;
    id.m_dLR1_NewBuild = 20;
    id.m_dMR_NewBuild = 21;
    id.m_dHANDYMAX_NewBuild = 22;

    id.m_dVLCC_5YearOld = 23;
    id.m_dSUEZMAX_5YearOld = 24;
    id.m_dAFRAMAX_5YearOld = 25;
    id.m_dLR2_5YearOld = 26;
    id.m_dLR1_5YearOld = 27;
    id.m_dMR_5YearOld = 28;
    id.m_dHANDYMAX_5YearOld = 29;

    id.m_dHandyUSFlag_USG_USAC = 30;

    id.m_dLNG_160M3WestWeekly = 31;
    id.m_dLNG_160M3WestMonthly = 32;
    id.m_dLNG_138M3WestWeekly = 33;
    id.m_dLNG_138M3WestMonthly = 34;
    id.m_dLNG_160M3EastWeekly = 35;
    id.m_dLNG_160M3EastMonthly = 36;
    id.m_dLNG_138M3EastWeekly = 37;
    id.m_dLNG_138M3EastMonthly = 38;

    id.m_dLPG_VLGC44LastSpotRate = 39;
    id.m_dLPG_VLGC44SpotTCERate = 40;

    setPoten.Open();
    setPoten.m_pDatabase->BeginTrans();
    id.AppendData(setPoten);
    setPoten.m_pDatabase->CommitTrans();
    setPoten.Close();

    setPoten.m_strFilter = _T("[Day] = ") + strDay;
    setPoten.Open();
    EXPECT_DOUBLE_EQ(atof(setPoten.m_VLCC_TD3C), id.m_dTD3C);
    EXPECT_DOUBLE_EQ(atof(setPoten.m_SUEZMAX_TD20), id.m_dTD20);
    EXPECT_DOUBLE_EQ(id.m_dLNG_160M3WestWeekly, atof(setPoten.m_LNG_160M3WestWeekly));
    //EXPECT_DOUBLE_EQ(id, id2);

    id2.LoadData(setPoten);
    EXPECT_DOUBLE_EQ(id.m_dTD3C, id2.m_dTD3C);
    EXPECT_DOUBLE_EQ(id.m_dTD20, id2.m_dTD20);
    EXPECT_DOUBLE_EQ(id.m_dTD9, id2.m_dTD9);
    EXPECT_DOUBLE_EQ(id.m_dTD21, id2.m_dTD21);

    EXPECT_DOUBLE_EQ(id.m_dTC1, id2.m_dTC1);
    EXPECT_DOUBLE_EQ(id.m_dTC2, id2.m_dTC2);
    EXPECT_DOUBLE_EQ(id.m_dTC5, id2.m_dTC5);
    EXPECT_DOUBLE_EQ(id.m_dTC14, id2.m_dTC14);

    EXPECT_DOUBLE_EQ(id.m_dVLCC_TC_1YEAR, id2.m_dVLCC_TC_1YEAR);
    EXPECT_DOUBLE_EQ(id.m_dVLCC_NewBuild, id2.m_dVLCC_NewBuild);
    EXPECT_DOUBLE_EQ(id.m_dVLCC_5YearOld, id2.m_dVLCC_5YearOld);
    EXPECT_DOUBLE_EQ(id.m_dSUEZMAX_TC_1YEAR, id2.m_dSUEZMAX_TC_1YEAR);
    EXPECT_DOUBLE_EQ(id.m_dSUEZMAX_NewBuild, id2.m_dSUEZMAX_NewBuild);
    EXPECT_DOUBLE_EQ(id.m_dSUEZMAX_5YearOld, id2.m_dSUEZMAX_5YearOld);
    EXPECT_DOUBLE_EQ(id.m_dAFRAMAX_TC_1YEAR, id2.m_dAFRAMAX_TC_1YEAR);
    EXPECT_DOUBLE_EQ(id.m_dAFRAMAX_NewBuild, id2.m_dAFRAMAX_NewBuild);
    EXPECT_DOUBLE_EQ(id.m_dAFRAMAX_5YearOld, id2.m_dAFRAMAX_5YearOld);
    EXPECT_DOUBLE_EQ(id.m_dLR2_TC_1YEAR, id2.m_dLR2_TC_1YEAR);
    EXPECT_DOUBLE_EQ(id.m_dLR2_NewBuild, id2.m_dLR2_NewBuild);
    EXPECT_DOUBLE_EQ(id.m_dLR2_5YearOld, id2.m_dLR2_5YearOld);
    EXPECT_DOUBLE_EQ(id.m_dLR1_TC_1YEAR, id2.m_dLR1_TC_1YEAR);
    EXPECT_DOUBLE_EQ(id.m_dLR1_NewBuild, id2.m_dLR1_NewBuild);
    EXPECT_DOUBLE_EQ(id.m_dLR1_5YearOld, id2.m_dLR1_5YearOld);
    EXPECT_DOUBLE_EQ(id.m_dMR_TC_1YEAR, id2.m_dMR_TC_1YEAR);
    EXPECT_DOUBLE_EQ(id.m_dMR_NewBuild, id2.m_dMR_NewBuild);
    EXPECT_DOUBLE_EQ(id.m_dMR_5YearOld, id2.m_dMR_5YearOld);
    EXPECT_DOUBLE_EQ(id.m_dHANDYMAX_TC_1YEAR, id2.m_dHANDYMAX_TC_1YEAR);
    EXPECT_DOUBLE_EQ(id.m_dHANDYMAX_NewBuild, id2.m_dHANDYMAX_NewBuild);
    EXPECT_DOUBLE_EQ(id.m_dHANDYMAX_5YearOld, id2.m_dHANDYMAX_5YearOld);
    EXPECT_DOUBLE_EQ(id.m_dHandyUSFlag_USG_USAC, id2.m_dHandyUSFlag_USG_USAC);

    EXPECT_DOUBLE_EQ(id.m_dLNG_138M3EastWeekly, id2.m_dLNG_138M3EastWeekly);
    EXPECT_DOUBLE_EQ(id.m_dLNG_138M3EastMonthly, id2.m_dLNG_138M3EastMonthly);
    EXPECT_DOUBLE_EQ(id.m_dLNG_138M3WestWeekly, id2.m_dLNG_138M3WestWeekly);
    EXPECT_DOUBLE_EQ(id.m_dLNG_138M3WestMonthly, id2.m_dLNG_138M3WestMonthly);
    EXPECT_DOUBLE_EQ(id.m_dLNG_160M3EastWeekly, id2.m_dLNG_160M3EastWeekly);
    EXPECT_DOUBLE_EQ(id.m_dLNG_160M3EastMonthly, id2.m_dLNG_160M3EastMonthly);
    EXPECT_DOUBLE_EQ(id.m_dLNG_160M3WestWeekly, id2.m_dLNG_160M3WestWeekly);
    EXPECT_DOUBLE_EQ(id.m_dLNG_160M3WestMonthly, id2.m_dLNG_160M3WestMonthly);

    EXPECT_DOUBLE_EQ(id.m_dLPG_VLGC44LastSpotRate, id2.m_dLPG_VLGC44LastSpotRate);
    EXPECT_DOUBLE_EQ(id.m_dLPG_VLGC44SpotTCERate, id2.m_dLPG_VLGC44SpotTCERate);

    setPoten.Close();
  }

  struct PotenNextString {
    PotenNextString(long lIndex, CString str, CString strProcessed) {
      m_id = lIndex;
      m_strInput = str;
      m_strProcessed = strProcessed;
    }
  public:
    long m_id;
    CString m_strInput;
    CString m_strProcessed;
  };

  PotenNextString PotenNextStringData1(1, _T("<abcde>abcde<"), _T("abcde"));
  PotenNextString PotenNextStringData2(2, _T("<abcde>\nabcde<"), _T("abcde"));
  PotenNextString PotenNextStringData3(3, _T("<abcde>\rabcde<"), _T("abcde"));
  PotenNextString PotenNextStringData4(4, _T("<abcde> abcde<"), _T("abcde"));
  PotenNextString PotenNextStringData5(5, _T("<abcde>\nabcde"), _T("abcde"));
  PotenNextString PotenNextStringData6(6, _T("<abcde>\rabcde"), _T("abcde"));
  PotenNextString PotenNextStringData7(7, _T("<abcde> abcde"), _T("abcde"));
  PotenNextString PotenNextStringData8(8, _T("<abcde>\rab,cde"), _T("abcde"));
  PotenNextString PotenNextStringData9(9, _T("<abcde>\nabc,de<"), _T("abcde"));
  PotenNextString PotenNextStringData10(10, _T("<abcde>"), _T(""));

  class PotenNextStringTest : public testing::TestWithParam<PotenNextString*> {
  protected:
    virtual void SetUp(void) override {
      ASSERT(!gl_fNormalMode);
      PotenNextString* NextString = GetParam();
      m_lId = NextString->m_id;
      m_strInput = NextString->m_strInput;
      m_strProcessed = NextString->m_strProcessed;
      m_WebDataPtr = make_shared<CWebData>();
      m_WebDataPtr->SetBufferLength(m_strInput.GetLength());
      m_WebDataPtr->m_pDataBuffer = new char[m_strInput.GetLength() + 1];
      strcpy_s(m_WebDataPtr->m_pDataBuffer, m_strInput.GetLength() + 1, m_strInput.GetBuffer());
      m_WebDataPtr->m_pDataBuffer[m_strInput.GetLength()] = 0x000;
      m_WebDataPtr->ResetCurrentPos();
    }
    virtual void TearDown(void) override {
      // clearup
    }
  public:
    long m_lId;
    CString m_strInput;
    CString m_strProcessed;
    CWebDataPtr m_WebDataPtr;
  };

  INSTANTIATE_TEST_SUITE_P(TestPotenNextString, PotenNextStringTest,
                           testing::Values(&PotenNextStringData1, &PotenNextStringData2, &PotenNextStringData3
                                           , &PotenNextStringData4, &PotenNextStringData5, &PotenNextStringData6
                                           , &PotenNextStringData7, &PotenNextStringData8, &PotenNextStringData9
                                           , &PotenNextStringData10
                                           ));

  TEST_P(PotenNextStringTest, TestPotenNextString) {
    CPotenDailyBriefing Index;

    CString strInput = Index.GetNextString(m_WebDataPtr);
    EXPECT_STREQ(strInput, m_strProcessed);
  }

  struct PotenSkipOverString {
    PotenSkipOverString(long lIndex, CString str, CString strTest, bool fFound) {
      m_id = lIndex;
      m_strInput = str;
      m_strTest = strTest;
      m_fFound = fFound;
    }
  public:
    long m_id;
    CString m_strInput;
    CString m_strTest;
    bool m_fFound;
  };

  PotenSkipOverString PotenSkipOverStringData1(1, _T("<abcde>abcde<ab>VLCC"), _T("abcde"), true);
  PotenSkipOverString PotenSkipOverStringData2(2, _T("<abcde>\nabcde<abc>abc"), _T("abcde"), true);
  PotenSkipOverString PotenSkipOverStringData3(3, _T("<abcde>\rabcde<>SUEZ"), _T("abce"), false);
  PotenSkipOverString PotenSkipOverStringData4(4, _T("<abcde> abcd<"), _T("abcde"), false);
  PotenSkipOverString PotenSkipOverStringData5(5, _T("<abcde>\nabcde"), _T("abcde"), true);
  PotenSkipOverString PotenSkipOverStringData6(6, _T("<abcde>\rabcde"), _T("abcde"), true);
  /*   PotenSkipOverString PotenSkipOverStringData7(7, _T("<abcde> abcde"), _T("abcde"));
  PotenSkipOverString PotenSkipOverStringData8(8, _T("<abcde>\rab,cde"), _T("abcde"));
  PotenSkipOverString PotenSkipOverStringData9(9, _T("<abcde>\nabc,de<"), _T("abcde"));
  PotenSkipOverString PotenSkipOverStringData10(10, _T("<abcde>"), _T(""));
  */
  class PotenSkipOverStringTest : public testing::TestWithParam<PotenSkipOverString*> {
  protected:
    virtual void SetUp(void) override {
      ASSERT(!gl_fNormalMode);
      PotenSkipOverString* SkipOverString = GetParam();
      m_lId = SkipOverString->m_id;
      m_strInput = SkipOverString->m_strInput;
      m_strTest = SkipOverString->m_strTest;
      m_fFound = SkipOverString->m_fFound;
      m_WebDataPtr = make_shared<CWebData>();
      m_WebDataPtr->SetBufferLength(m_strInput.GetLength());
      m_WebDataPtr->m_pDataBuffer = new char[m_strInput.GetLength() + 1];
      strcpy_s(m_WebDataPtr->m_pDataBuffer, m_strInput.GetLength() + 1, m_strInput.GetBuffer());
      m_WebDataPtr->m_pDataBuffer[m_strInput.GetLength()] = 0x000;
      m_WebDataPtr->ResetCurrentPos();
    }
    virtual void TearDown(void) override {
      // clearup
    }
  public:
    long m_lId;
    CString m_strInput;
    CString m_strTest;
    bool m_fFound;
    CWebDataPtr m_WebDataPtr;
  };

  INSTANTIATE_TEST_SUITE_P(TestPotenSkipOverString, PotenSkipOverStringTest,
                           testing::Values(&PotenSkipOverStringData1, &PotenSkipOverStringData2, &PotenSkipOverStringData3
                                           , &PotenSkipOverStringData4, &PotenSkipOverStringData5, &PotenSkipOverStringData6
                                           //    , &PotenSkipOverStringData7, &PotenSkipOverStringData8, &PotenSkipOverStringData9
                                            //   , &PotenSkipOverStringData10
                                           ));

  TEST_P(PotenSkipOverStringTest, TestPotenSkipOverString) {
    CPotenDailyBriefing Index;

    bool fFound = Index.SkipOverStrings(m_WebDataPtr, m_strTest);
    EXPECT_TRUE(fFound == m_fFound);
  }
}