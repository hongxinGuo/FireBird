#include"pch.h"

#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"GeneralCheck.h"

#include"ConvertToString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	struct StrConvertDoubleToString {
		StrConvertDoubleToString(double dValue, CString CValueOfPeriod, long lDividend) {
			m_dValue = dValue;
			m_strValue = CValueOfPeriod;
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
			ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
			StrConvertDoubleToString* pData = GetParam();
			dValue = pData->m_dValue;
			CValueOfPeriod = pData->m_strValue;
			lDividend = pData->m_lDividend;
		}

		virtual void TearDown(void) override {
			// clearup
		}

	public:
		double dValue;
		CString CValueOfPeriod;
		long lDividend;
	};

	INSTANTIATE_TEST_SUITE_P(TestConvertDoubleToString, ConvertDoubleToStringTest, testing::Values(&Data0, &Data1, &Data2, &Data3, &Data4
		, &Data5));

	TEST_P(ConvertDoubleToStringTest, TestDouble) {
		CString str = ConvertValueToString(dValue, lDividend);
		EXPECT_STREQ(str, CValueOfPeriod);
	}

	struct StrConvertLongToString {
		StrConvertLongToString(long lValue, CString CValueOfPeriod, long lDividend) {
			m_lValue = lValue;
			m_strValue = CValueOfPeriod;
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
			ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
			StrConvertLongToString* pData = GetParam();
			lValue = pData->m_lValue;
			CValueOfPeriod = pData->m_strValue;
			lDividend = pData->m_lDividend;
		}

		virtual void TearDown(void) override {
			// clearup
		}

	public:
		long lValue;
		CString CValueOfPeriod;
		long lDividend;
	};

	INSTANTIATE_TEST_SUITE_P(TestConvertLongToString, ConvertLongToStringTest, testing::Values(&Data10, &Data11,
		&Data12, &Data13, &Data14, &Data15));

	TEST_P(ConvertLongToStringTest, TestLong) {
		CString str = ConvertValueToString(lValue, lDividend);
		EXPECT_STREQ(str, CValueOfPeriod);
	}

	struct StrConvertIntegerToString {
		StrConvertIntegerToString(int iValue, CString CValueOfPeriod, long lDividend) {
			m_iValue = iValue;
			m_strValue = CValueOfPeriod;
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
			ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
			StrConvertIntegerToString* pData = GetParam();
			iValue = pData->m_iValue;
			CValueOfPeriod = pData->m_strValue;
			lDividend = pData->m_lDividend;
		}

		virtual void TearDown(void) override {
			// clearup
		}

	public:
		int iValue;
		CString CValueOfPeriod;
		long lDividend;
	};

	INSTANTIATE_TEST_SUITE_P(TestConvertIntegerToString, ConvertIntegerToStringTest, testing::Values(&Data20, &Data21,
		&Data22, &Data23, &Data24, &Data25));

	TEST_P(ConvertIntegerToStringTest, TestInteger) {
		CString str = ConvertValueToString(iValue, lDividend);
		EXPECT_STREQ(str, CValueOfPeriod);
	}

	struct StrConvertINT64ToString {
		StrConvertINT64ToString(INT64 iValue, CString CValueOfPeriod, long lDividend) {
			m_iValue = iValue;
			m_strValue = CValueOfPeriod;
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
			ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
			StrConvertINT64ToString* pData = GetParam();
			iValue = pData->m_iValue;
			CValueOfPeriod = pData->m_strValue;
			lDividend = pData->m_lDividend;
		}

		virtual void TearDown(void) override {
			// clearup
		}

	public:
		INT64 iValue;
		CString CValueOfPeriod;
		long lDividend;
	};

	INSTANTIATE_TEST_SUITE_P(TestConvertINT64ToString, ConvertINT64ToStringTest, testing::Values(&Data40, &Data41,
		&Data42, &Data43, &Data44, &Data45));

	TEST_P(ConvertINT64ToStringTest, TestINT64) {
		CString str = ConvertValueToString(iValue, lDividend);
		EXPECT_STREQ(str, CValueOfPeriod);
	}

	class CRSReferenceTest : public ::testing::Test {
		virtual void SetUp(void) override {
		}
		virtual void TearDown(void) override {
			gl_systemStatus.SetWorkingMode(false);
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