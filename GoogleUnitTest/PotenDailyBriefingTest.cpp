#include "pch.h"

#include"PotenDailyBriefing.h"

namespace StockAnalysisTest {
  TEST(CPotenDailyBriefingTest, TestReset) {
    CPotenDailyBriefing id;
    EXPECT_FALSE(id.IsTodayUpdated());
    id.SetTodayUpdated(true);
    id.Reset();
    EXPECT_FALSE(id.IsTodayUpdated());
  }

  TEST(CPotenDailyBriefingTest, TestIsTodayDataUpdated) {
    CPotenDailyBriefing id;
    EXPECT_FALSE(id.IsTodayUpdated());
    id.SetTodayUpdated(true);
    EXPECT_TRUE(id.IsTodayUpdated());
    id.SetTodayUpdated(false);
    EXPECT_FALSE(id.IsTodayUpdated());
  }

  TEST(CPotenDailyBriefingTest, TestGetCurrentDataTime) {
    CPotenDailyBriefing id;
    EXPECT_EQ(id.GetCurrentDataTime(), 0);
    id.SetNewestDataTime(20190101);
    EXPECT_EQ(id.GetCurrentDataTime(), 20190101);
  }

  TEST(CPotenDailyBriefingTest, TestGetDay) {
    CPotenDailyBriefing id;
    EXPECT_EQ(id.GetDay(), 0);
    id.SetDay(20190101);
    EXPECT_EQ(id.GetDay(), 20190101);
  }

  TEST(CPotenDailyBriefingTest, TestGetSet) {
    CPotenDailyBriefing id;
    id.m_dTD3C = 1;
    EXPECT_DOUBLE_EQ(id.GetTD3C(), 1);
  }

  TEST(CPotenDailyBriefingTest, TestConvertStringToTime) {
    CString str = _T("04/11/2018");
    CPotenDailyBriefing id;
    EXPECT_EQ(id.ConvertStringToTime(str), 20180411);
  }

  TEST(CPotenDailyBriefingTest, TestGetOneValue) {
    CPotenDailyBriefing id;
    EXPECT_DOUBLE_EQ(id.GetOneValue(_T("7,600")), 7600);
    EXPECT_DOUBLE_EQ(id.GetOneValue(_T("(7,600)")), -7600);
    EXPECT_DOUBLE_EQ(id.GetOneValue(_T("(1,100)")), -1100);
    EXPECT_DOUBLE_EQ(id.GetOneValue(_T("(7,600")), -7600);
  }

  TEST(CPotenDailyBriefingTest, TestLoadData) {
    CPotenDailyBriefing id, id2;
    CSetPotenDailyBriefing setPoten;

    setPoten.m_strFilter = _T("[Day] = 20300101");
    setPoten.Open();
    setPoten.m_pDatabase->BeginTrans();
    while (!setPoten.IsEOF()) {
      setPoten.Delete();
      setPoten.MoveNext();
    }
    setPoten.m_pDatabase->CommitTrans();
    setPoten.Close();

    id.m_lDay = 20300101;
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

    setPoten.m_strFilter = _T("[Day] = 20300101");
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
}