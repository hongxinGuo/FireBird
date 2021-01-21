#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"accessory.h"

namespace StockAnalysisTest {
  class AccessoryTest : public ::testing::Test {
    virtual void SetUp(void) override {
    }
    virtual void TearDown(void) override {
      gl_fNormalMode = false;
      gl_fTestMode = true;
    }
  };

  TEST_F(AccessoryTest, TestGetChinaMarketSchemaAnalysis) {
    EXPECT_FALSE(gl_fNormalMode); // 默认状态下此标识为假。
    EXPECT_STREQ(GetChinaMarketSchemaConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
    EXPECT_TRUE(gl_fTestMode);
    gl_fNormalMode = true;
    EXPECT_STREQ(GetChinaMarketSchemaConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
    EXPECT_FALSE(gl_fTestMode);
    gl_fNormalMode = false; // 运行单元测试时，必须将此标识设置为假，故而在运行完此测试函数后，需要再次将其置为假，否则会出错。
    EXPECT_STREQ(GetChinaMarketSchemaConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
    EXPECT_TRUE(gl_fTestMode);
  }

  TEST_F(AccessoryTest, TestGetAmericaMarketSchemaAnalysis) {
    EXPECT_FALSE(gl_fNormalMode); // 默认状态下此标识为假。
    EXPECT_STREQ(GetAmericaMarketSchemaConnect(), _T("DSN=AmericaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
    EXPECT_TRUE(gl_fTestMode);
    gl_fNormalMode = true;
    EXPECT_STREQ(GetAmericaMarketSchemaConnect(), _T("DSN=AmericaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
    EXPECT_FALSE(gl_fTestMode);
    gl_fNormalMode = false; // 运行单元测试时，必须将此标识设置为假，故而在运行完此测试函数后，需要再次将其置为假，否则会出错。
    EXPECT_STREQ(GetAmericaMarketSchemaConnect(), _T("DSN=AmericaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
    EXPECT_TRUE(gl_fTestMode);
  }

  TEST_F(AccessoryTest, TestFormatToDate) {
    tm tm_, tm_2;
    tm_.tm_year = 2000 - 1900;
    tm_.tm_mon = 0;
    tm_.tm_mday = 5;
    tm_.tm_hour = 0;
    tm_.tm_min = 0;
    tm_.tm_sec = 0;
    tm_2 = tm_;
    long lDate = FormatToDate(&tm_);
    time_t tt = mktime(&tm_2);
    long lDate2 = FormatToDate(tt);
    EXPECT_EQ(lDate, 20000105);
    EXPECT_EQ(lDate2, 20000105);
  }

  TEST_F(AccessoryTest, TestFormatToTime) {
    tm tm_, tm_2;
    tm_.tm_year = 2000 - 1900;
    tm_.tm_mon = 0;
    tm_.tm_mday = 5;
    tm_.tm_hour = 10;
    tm_.tm_min = 20;
    tm_.tm_sec = 30;
    tm_2 = tm_;
    long lTime = FormatToTime(&tm_);
    time_t tt = mktime(&tm_2);
    long lTime2 = FormatToTime(tt);
    EXPECT_EQ(lTime, 102030);
    EXPECT_EQ(lTime2, 102030);
  }

  TEST_F(AccessoryTest, TestFormatToDateTime) {
    tm tm_, tm_2;
    tm_.tm_year = 2000 - 1900;
    tm_.tm_mon = 0;
    tm_.tm_mday = 5;
    tm_.tm_hour = 10;
    tm_.tm_min = 20;
    tm_.tm_sec = 30;
    tm_2 = tm_;
    INT64 lDateTime = FormatToDateTime(&tm_);
    time_t tt = mktime(&tm_2);
    INT64 lDateTime2 = FormatToDateTime(tt);
    EXPECT_EQ(lDateTime, 20000105102030);
    EXPECT_EQ(lDateTime2, 20000105102030);
  }

  TEST_F(AccessoryTest, TestGetNextMonday) {
    EXPECT_EQ(20200727, GetNextMonday(20200720));
    EXPECT_EQ(20200727, GetNextMonday(20200721));
    EXPECT_EQ(20200727, GetNextMonday(20200722));
    EXPECT_EQ(20200727, GetNextMonday(20200723));
    EXPECT_EQ(20200727, GetNextMonday(20200724));
    EXPECT_EQ(20200727, GetNextMonday(20200725));
    EXPECT_EQ(20200727, GetNextMonday(20200726));
    EXPECT_EQ(20200720, GetNextMonday(20200713));
    EXPECT_EQ(20200720, GetNextMonday(20200714));
    EXPECT_EQ(20200720, GetNextMonday(20200715));
    EXPECT_EQ(20200720, GetNextMonday(20200716));
    EXPECT_EQ(20200720, GetNextMonday(20200717));
    EXPECT_EQ(20200720, GetNextMonday(20200718));
    EXPECT_EQ(20200720, GetNextMonday(20200719));
  }

  TEST_F(AccessoryTest, TestGetPrevMonday) {
    EXPECT_EQ(20200720, GetPrevMonday(20200721));
    EXPECT_EQ(20200720, GetPrevMonday(20200722));
    EXPECT_EQ(20200720, GetPrevMonday(20200723));
    EXPECT_EQ(20200720, GetPrevMonday(20200724));
    EXPECT_EQ(20200720, GetPrevMonday(20200725));
    EXPECT_EQ(20200720, GetPrevMonday(20200726));
    EXPECT_EQ(20200720, GetPrevMonday(20200727));
    EXPECT_EQ(20200713, GetPrevMonday(20200714));
    EXPECT_EQ(20200713, GetPrevMonday(20200715));
    EXPECT_EQ(20200713, GetPrevMonday(20200716));
    EXPECT_EQ(20200713, GetPrevMonday(20200717));
    EXPECT_EQ(20200713, GetPrevMonday(20200718));
    EXPECT_EQ(20200713, GetPrevMonday(20200719));
    EXPECT_EQ(20200713, GetPrevMonday(20200720));
  }

  TEST_F(AccessoryTest, TestGetCurrentMonday) {
    EXPECT_EQ(20200720, GetCurrentMonday(20200720)) << _T("20200720为星期一");
    EXPECT_EQ(20200720, GetCurrentMonday(20200721));
    EXPECT_EQ(20200720, GetCurrentMonday(20200722));
    EXPECT_EQ(20200720, GetCurrentMonday(20200723));
    EXPECT_EQ(20200720, GetCurrentMonday(20200724));
    EXPECT_EQ(20200720, GetCurrentMonday(20200725));
    EXPECT_EQ(20200720, GetCurrentMonday(20200726));
    EXPECT_EQ(20200713, GetCurrentMonday(20200713));
    EXPECT_EQ(20200713, GetCurrentMonday(20200714));
    EXPECT_EQ(20200713, GetCurrentMonday(20200715));
    EXPECT_EQ(20200713, GetCurrentMonday(20200716));
    EXPECT_EQ(20200713, GetCurrentMonday(20200717));
    EXPECT_EQ(20200713, GetCurrentMonday(20200718));
    EXPECT_EQ(20200713, GetCurrentMonday(20200719));
  }

  TEST_F(AccessoryTest, TestFormatToTTime) {
    tm tm_;
    tm_.tm_year = 2000 - 1900;
    tm_.tm_mon = 0;
    tm_.tm_mday = 5;
    tm_.tm_hour = 0;
    tm_.tm_min = 0;
    tm_.tm_sec = 0;
    time_t tt = mktime(&tm_);
    long lDate = FormatToDate(tt);
    EXPECT_EQ(lDate, 20000105);
  }

  TEST_F(AccessoryTest, TestEnlarge) {
    vector<double> vData{ 0, 100, 50, 49, 51 };

    ZoomIn(vData, 50, 10);
    EXPECT_EQ(vData.at(0), 0);
    EXPECT_EQ(vData.at(1), 100);
    EXPECT_EQ(vData.at(2), 50);
    EXPECT_EQ(vData.at(3), 40);
    EXPECT_EQ(vData.at(4), 60);
  }
}

namespace StockAnalysisTest {
  struct strConvertBufferToTime {
    strConvertBufferToTime(CString strFormat, CString strBuffer, INT64 iTime) {
      m_strFormat = strFormat;
      m_strBuffer = strBuffer;
      m_Time = iTime;
    }

  public:
    CString m_strFormat;
    CString m_strBuffer;
    INT64 m_Time;
  };

  strConvertBufferToTime Data101(_T("%4d%2d%2d%2d%2d%2d"), _T("20191030102030"), 20191030102030);
  strConvertBufferToTime Data102(_T("%d-%d-%d %d:%d:%d"), _T("2019-10-30 10:12:30"), 20191030101230);
  strConvertBufferToTime Data105(_T("%4d/%2d/%2d %2d:%2d:%2d"), _T("2019/11/05 12:11:15"), 20191105121115);
  strConvertBufferToTime Data103(_T("%4d/%2d/%2d %2d:%2d:%2d"), _T("0"), -1);
  strConvertBufferToTime Data104(_T("%4d/%4d/%4d %2d:%2d:%2d"), _T("1900/01/01"), -1);
  strConvertBufferToTime Data106(_T("%4d/%4d%4d %2d:%2d:%2d"), _T(" 12:12:12"), -1);

  class ConvertBufferToTimeTest : public::testing::TestWithParam<strConvertBufferToTime*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      strConvertBufferToTime* pData = GetParam();
      strBuffer = pData->m_strBuffer;
      strFormat = pData->m_strFormat;
      iTime = pData->m_Time;
    }

    virtual void TearDown(void) override {
      // clearup
    }

  public:
    CString strFormat;
    CString strBuffer;
    INT64 iTime;
  };

  INSTANTIATE_TEST_SUITE_P(TestConvertBufferToTime, ConvertBufferToTimeTest, testing::Values(&Data101, &Data102, &Data103,
                                                                                             &Data104, &Data105, &Data106));

  TEST_P(ConvertBufferToTimeTest, TestConvertBufferToTime) {
    time_t tt = ConvertBufferToTime(strFormat, strBuffer.GetBuffer());
    time_t tt2;
    tm tm_;
    INT64 year = iTime / 10000000000;
    INT64 month = iTime / 100000000 - year * 100;
    INT64 day = iTime / 1000000 - year * 10000 - month * 100;
    INT64 hour = iTime / 10000 - year * 1000000 - month * 10000 - day * 100;
    INT64 minute = iTime / 100 - year * 100000000 - month * 1000000 - day * 10000 - hour * 100;
    INT64 second = iTime - year * 10000000000 - month * 100000000 - day * 1000000 - hour * 10000 - minute * 100;
    tm_.tm_year = year - 1900;
    tm_.tm_mon = month - 1;
    tm_.tm_mday = day;
    tm_.tm_hour = hour;
    tm_.tm_min = minute;
    tm_.tm_sec = second;
    tm_.tm_isdst = 0;
    tt2 = mktime(&tm_);
    if (iTime < 19000101000000) tt2 = iTime;
    EXPECT_EQ(tt, tt2);
  }

  TEST_P(ConvertBufferToTimeTest, TestConvertStringToTime) {
    time_t tt = ConvertStringToTime(strFormat, strBuffer);
    time_t tt2;
    tm tm_;
    INT64 year = iTime / 10000000000;
    INT64 month = iTime / 100000000 - year * 100;
    INT64 day = iTime / 1000000 - year * 10000 - month * 100;
    INT64 hour = iTime / 10000 - year * 1000000 - month * 10000 - day * 100;
    INT64 minute = iTime / 100 - year * 100000000 - month * 1000000 - day * 10000 - hour * 100;
    INT64 second = iTime - year * 10000000000 - month * 100000000 - day * 1000000 - hour * 10000 - minute * 100;
    tm_.tm_year = year - 1900;
    tm_.tm_mon = month - 1;
    tm_.tm_mday = day;
    tm_.tm_hour = hour;
    tm_.tm_min = minute;
    tm_.tm_sec = second;
    tm_.tm_isdst = 0;
    tt2 = mktime(&tm_);
    if (iTime < 19000101000000) tt2 = iTime;
    EXPECT_EQ(tt, tt2);
  }

  struct StrConvertDoubleToString {
    StrConvertDoubleToString(double dValue, CString strValue, long lDividend) {
      m_dValue = dValue;
      m_strValue = strValue;
      m_lDividend = lDividend;
    }

  public:
    double m_dValue;
    CString m_strValue;
    long m_lDividend;
  };

  StrConvertDoubleToString Data0(10.234, _T("10.234"), 1);
  StrConvertDoubleToString Data1(-110.2343, _T("-110.234"), 1);
  StrConvertDoubleToString Data2(1210.2346, _T("1210.235"), 1);
  StrConvertDoubleToString Data3(-10234, _T("-10.234"), 1000);
  StrConvertDoubleToString Data4(110234, _T("110.234"), 1000);
  StrConvertDoubleToString Data5(-12102346, _T("-1210.235"), 10000);

  class ConvertDoubleToStringTest : public::testing::TestWithParam<StrConvertDoubleToString*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      StrConvertDoubleToString* pData = GetParam();
      dValue = pData->m_dValue;
      strValue = pData->m_strValue;
      lDividend = pData->m_lDividend;
    }

    virtual void TearDown(void) override {
      // clearup
    }

  public:
    double dValue;
    CString strValue;
    long lDividend;
  };

  INSTANTIATE_TEST_SUITE_P(TestConvertDoubleToString, ConvertDoubleToStringTest, testing::Values(&Data0, &Data1, &Data2, &Data3, &Data4
                                                                                                 , &Data5));

  TEST_P(ConvertDoubleToStringTest, TestDouble) {
    CString str = ConvertValueToString(dValue, lDividend);
    EXPECT_STREQ(str, strValue);
  }

  struct StrConvertLongToString {
    StrConvertLongToString(long lValue, CString strValue, long lDividend) {
      m_lValue = lValue;
      m_strValue = strValue;
      m_lDividend = lDividend;
    }

  public:
    long m_lValue;
    CString m_strValue;
    long m_lDividend;
  };

  StrConvertLongToString Data10(10234, _T("10234.000"), 1);
  StrConvertLongToString Data11(-11023.43, _T("-11023.000"), 1);
  StrConvertLongToString Data12(12102346, _T("12102346.000"), 1);
  StrConvertLongToString Data13(-10234, _T("-10.234"), 1000);
  StrConvertLongToString Data14(1102344, _T("110.234"), 10000);
  StrConvertLongToString Data15(-12102346, _T("-1210.235"), 10000);
  /*
  StrConvertLongToString Data6
  StrConvertLongToString Data7
  StrConvertLongToString Data8
  StrConvertLongToString Data9
  StrConvertLongToString Data10
  StrConvertLongToString Data11
  StrConvertLongToString Data12
  */

  class ConvertLongToStringTest : public::testing::TestWithParam<StrConvertLongToString*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      StrConvertLongToString* pData = GetParam();
      lValue = pData->m_lValue;
      strValue = pData->m_strValue;
      lDividend = pData->m_lDividend;
    }

    virtual void TearDown(void) override {
      // clearup
    }

  public:
    long lValue;
    CString strValue;
    long lDividend;
  };

  INSTANTIATE_TEST_SUITE_P(TestConvertLongToString, ConvertLongToStringTest, testing::Values(&Data10, &Data11,
                                                                                             &Data12, &Data13, &Data14, &Data15));

  TEST_P(ConvertLongToStringTest, TestLong) {
    CString str = ConvertValueToString(lValue, lDividend);
    EXPECT_STREQ(str, strValue);
  }

  struct StrConvertIntegerToString {
    StrConvertIntegerToString(int iValue, CString strValue, long lDividend) {
      m_iValue = iValue;
      m_strValue = strValue;
      m_lDividend = lDividend;
    }

  public:
    int m_iValue;
    CString m_strValue;
    long m_lDividend;
  };

  StrConvertIntegerToString Data20(10234, _T("10234.000"), 1);
  StrConvertIntegerToString Data21(-11023.43, _T("-11023.000"), 1);
  StrConvertIntegerToString Data22(12102346, _T("12102346.000"), 1);
  StrConvertIntegerToString Data23(-10234, _T("-10.234"), 1000);
  StrConvertIntegerToString Data24(1102344, _T("110.234"), 10000);
  StrConvertIntegerToString Data25(-12102346, _T("-1210.235"), 10000);
  /*
  StrConvertIntegerToString Data26
  StrConvertIntegerToString Data27
  StrConvertIntegerToString Data28
  StrConvertIntegerToString Data29
  StrConvertIntegerToString Data30
  StrConvertIntegerToString Data31
  StrConvertIntegerToString Data32
  */

  class ConvertIntegerToStringTest : public::testing::TestWithParam<StrConvertIntegerToString*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      StrConvertIntegerToString* pData = GetParam();
      iValue = pData->m_iValue;
      strValue = pData->m_strValue;
      lDividend = pData->m_lDividend;
    }

    virtual void TearDown(void) override {
      // clearup
    }

  public:
    int iValue;
    CString strValue;
    long lDividend;
  };

  INSTANTIATE_TEST_SUITE_P(TestConvertIntegerToString, ConvertIntegerToStringTest, testing::Values(&Data20, &Data21,
                                                                                                   &Data22, &Data23, &Data24, &Data25));

  TEST_P(ConvertIntegerToStringTest, TestInteger) {
    CString str = ConvertValueToString(iValue, lDividend);
    EXPECT_STREQ(str, strValue);
  }

  struct StrConvertINT64ToString {
    StrConvertINT64ToString(INT64 iValue, CString strValue, long lDividend) {
      m_iValue = iValue;
      m_strValue = strValue;
      m_lDividend = lDividend;
    }

  public:
    INT64 m_iValue;
    CString m_strValue;
    long m_lDividend;
  };

  StrConvertINT64ToString Data40(10234, _T("10234.000"), 1);
  StrConvertINT64ToString Data41(-11023.43, _T("-11023.000"), 1);
  StrConvertINT64ToString Data42(12102346, _T("12102346.000"), 1);
  StrConvertINT64ToString Data43(-10234, _T("-10.234"), 1000);
  StrConvertINT64ToString Data44(1102344, _T("110.234"), 10000);
  StrConvertINT64ToString Data45(-12102346, _T("-1210.235"), 10000);
  /*
  StrConvertINT64ToString Data46
  StrConvertINT64ToString Data47
  StrConvertINT64ToString Data48
  StrConvertINT64ToString Data49
  StrConvertINT64ToString Data50
  StrConvertINT64ToString Data51
  StrConvertINT64ToString Data52
  */

  class ConvertINT64ToStringTest : public::testing::TestWithParam<StrConvertINT64ToString*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      StrConvertINT64ToString* pData = GetParam();
      iValue = pData->m_iValue;
      strValue = pData->m_strValue;
      lDividend = pData->m_lDividend;
    }

    virtual void TearDown(void) override {
      // clearup
    }

  public:
    INT64 iValue;
    CString strValue;
    long lDividend;
  };

  INSTANTIATE_TEST_SUITE_P(TestConvertINT64ToString, ConvertINT64ToStringTest, testing::Values(&Data40, &Data41,
                                                                                               &Data42, &Data43, &Data44, &Data45));

  TEST_P(ConvertINT64ToStringTest, TestINT64) {
    CString str = ConvertValueToString(iValue, lDividend);
    EXPECT_STREQ(str, strValue);
  }

  class CRSReferenceTest : public ::testing::Test {
    virtual void SetUp(void) override {
    }
    virtual void TearDown(void) override {
      gl_fNormalMode = false;
      gl_fTestMode = true;
    }
  };

  TEST_F(CRSReferenceTest, TestInitialize) {
    CRSReference RSReference;
    EXPECT_FALSE(RSReference.m_fActive);
    for (int i = 0; i < 4; i++) {
      EXPECT_EQ(RSReference.m_lStrongDayLength[i], 0);
      EXPECT_EQ(RSReference.m_lDayLength[i], 0);
      EXPECT_DOUBLE_EQ(RSReference.m_dRSStrong[i], 50.0);
    }
  }
}