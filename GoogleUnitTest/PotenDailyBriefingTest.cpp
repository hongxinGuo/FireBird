#include "pch.h"

#include"PotenDailyBriefing.h"

namespace StockAnalysisTest {
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
}