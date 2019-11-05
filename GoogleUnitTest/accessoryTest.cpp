#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"accessory.h"

namespace StockAnalysicTest {
  TEST(TestGetDefaultSchemaConnection, GetDefaultSchemaAnalysisTest) {
    EXPECT_FALSE(gl_fNormalMode); // Ĭ��״̬�´˱�ʶΪ�١�
    EXPECT_STREQ(GetDefaultSchemaConnect(), _T("DSN=mysqlTest;UID=Test;PASSWORD=test;charset=utf8"));
    EXPECT_TRUE(gl_fTestMode);
    gl_fNormalMode = true;
    EXPECT_STREQ(GetDefaultSchemaConnect(), _T("DSN=mysql;UID=guo;PASSWORD=guo1426hx;charset=utf8"));
    EXPECT_FALSE(gl_fTestMode);
    gl_fNormalMode = false; // ���е�Ԫ����ʱ�����뽫�˱�ʶ����Ϊ�٣��ʶ���������˲��Ժ�������Ҫ�ٴν�����Ϊ�٣�����������
    EXPECT_STREQ(GetDefaultSchemaConnect(), _T("DSN=mysqlTest;UID=Test;PASSWORD=test;charset=utf8"));
    EXPECT_TRUE(gl_fTestMode);
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
  /*strConvertBufferToTime Data105(_T("%4d%2d%2d%2d%2d%2d", _T("-11023.000"));
  strConvertBufferToTime Data103(_T("%4d%2d%2d%2d%2d%2d", _T("12102346.000"));
  strConvertBufferToTime Data104(_T("%d-%d-%d %d:%d:%d"), _T("-10.234"), 1000);
  strConvertBufferToTime Data106(_T("%d-%d-%d %d:%d:%d"), _T("-1210.235"));*/

  class ConvertBufferToTimeTest : public::testing::TestWithParam<strConvertBufferToTime*>
  {
  protected:
    void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      strConvertBufferToTime* pData = GetParam();
      strBuffer = pData->m_strBuffer;
      strFormat = pData->m_strFormat;
      iTime = pData->m_Time;
    }

    void TearDown(void) override {
      // clearup
    }

  public:
    CString strFormat;
    CString strBuffer;
    INT64 iTime;
  };

  INSTANTIATE_TEST_CASE_P(TestConvertBufferToTime, ConvertBufferToTimeTest, testing::Values(&Data101, &Data102));

  TEST_P(ConvertBufferToTimeTest, TestINT64) {
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
    void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      StrConvertDoubleToString* pData = GetParam();
      dValue = pData->m_dValue;
      strValue = pData->m_strValue;
      lDividend = pData->m_lDividend;
    }

    void TearDown(void) override {
      // clearup
    }

  public:
    double dValue;
    CString strValue;
    long lDividend;
  };

  INSTANTIATE_TEST_CASE_P(TestConvertDoubleToString, ConvertDoubleToStringTest, testing::Values(&Data0, &Data1,
    &Data2, &Data3, &Data4, &Data5));

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
    void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      StrConvertLongToString* pData = GetParam();
      lValue = pData->m_lValue;
      strValue = pData->m_strValue;
      lDividend = pData->m_lDividend;
    }

    void TearDown(void) override {
      // clearup
    }

  public:
    long lValue;
    CString strValue;
    long lDividend;
  };

  INSTANTIATE_TEST_CASE_P(TestConvertLongToString, ConvertLongToStringTest, testing::Values(&Data10, &Data11,
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
    void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      StrConvertIntegerToString* pData = GetParam();
      iValue = pData->m_iValue;
      strValue = pData->m_strValue;
      lDividend = pData->m_lDividend;
    }

    void TearDown(void) override {
      // clearup
    }

  public:
    int iValue;
    CString strValue;
    long lDividend;
  };

  INSTANTIATE_TEST_CASE_P(TestConvertIntegerToString, ConvertIntegerToStringTest, testing::Values(&Data20, &Data21,
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
    void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      StrConvertINT64ToString* pData = GetParam();
      iValue = pData->m_iValue;
      strValue = pData->m_strValue;
      lDividend = pData->m_lDividend;
    }

    void TearDown(void) override {
      // clearup
    }

  public:
    INT64 iValue;
    CString strValue;
    long lDividend;
  };

  INSTANTIATE_TEST_CASE_P(TestConvertINT64ToString, ConvertINT64ToStringTest, testing::Values(&Data40, &Data41,
    &Data42, &Data43, &Data44, &Data45));

  TEST_P(ConvertINT64ToStringTest, TestINT64) {
    CString str = ConvertValueToString(iValue, lDividend);
    EXPECT_STREQ(str, strValue);
  }
}