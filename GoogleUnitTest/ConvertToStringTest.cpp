#include"pch.h"

//#include"gtest/gtest.h"

#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"GeneralCheck.h"

#include"ConvertToString.h"

namespace FireBirdTest {
	struct StrConvertDoubleToString {
		StrConvertDoubleToString(double dValue, const CString& CValueOfPeriod, long lDividend) {
			m_dValue = dValue;
			m_strValue = CValueOfPeriod;
			m_lDividend = lDividend;
		}

	public:
		double m_dValue;
		CString m_strValue;
		long m_lDividend;
	};

	StrConvertDoubleToString Data0(10.234, _T("10.2340"), 1);
	StrConvertDoubleToString Data1(-110.2343, _T("-110.2343"), 1);
	StrConvertDoubleToString Data2(1210.2346, _T("1210.2346"), 1);
	StrConvertDoubleToString Data3(-10234, _T("-10.2340"), 1000);
	StrConvertDoubleToString Data4(1, _T("0.0001"), 10000);
	StrConvertDoubleToString Data5(-12102346, _T("-1210.2346"), 10000);

	class ConvertDoubleToStringTest : public testing::TestWithParam<StrConvertDoubleToString*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
			const StrConvertDoubleToString* pData = GetParam();
			dValue = pData->m_dValue;
			CValueOfPeriod = pData->m_strValue;
			lDividend = pData->m_lDividend;
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		double dValue;
		CString CValueOfPeriod;
		long lDividend;
	};

	INSTANTIATE_TEST_SUITE_P(TestConvertDoubleToString, ConvertDoubleToStringTest, testing::Values(&Data0, &Data1, &Data2, &Data3, &Data4
		                         , &Data5));

	TEST_P(ConvertDoubleToStringTest, TestDouble) {
		const CString str = ConvertValueToString(dValue, lDividend);
		EXPECT_STREQ(str, CValueOfPeriod);
	}

	struct StrConvertLongToString {
		StrConvertLongToString(long lValue, const CString& CValueOfPeriod, long lDividend) {
			m_lValue = lValue;
			m_strValue = CValueOfPeriod;
			m_lDividend = lDividend;
		}

	public:
		long m_lValue;
		CString m_strValue;
		long m_lDividend;
	};

	StrConvertLongToString Data10(10234, _T("10234.0000"), 1);
	StrConvertLongToString Data11(-11023.43, _T("-11023.0000"), 1);
	StrConvertLongToString Data12(12102346, _T("12102346.0000"), 1);
	StrConvertLongToString Data13(-10234, _T("-10.2340"), 1000);
	StrConvertLongToString Data14(1102344, _T("110.2344"), 10000);
	StrConvertLongToString Data15(-12102346, _T("-1210.2346"), 10000);
	/*
	StrConvertLongToString Data6
	StrConvertLongToString Data7
	StrConvertLongToString Data8
	StrConvertLongToString Data9
	StrConvertLongToString Data10
	StrConvertLongToString Data11
	StrConvertLongToString Data12
	*/

	class ConvertLongToStringTest : public testing::TestWithParam<StrConvertLongToString*> {
	protected:
		void SetUp() override {
			ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
			const StrConvertLongToString* pData = GetParam();
			lValue = pData->m_lValue;
			CValueOfPeriod = pData->m_strValue;
			lDividend = pData->m_lDividend;
		}

		void TearDown() override {
			// clearUp
		}

	public:
		long lValue;
		CString CValueOfPeriod;
		long lDividend;
	};

	INSTANTIATE_TEST_SUITE_P(TestConvertLongToString, ConvertLongToStringTest, testing::Values(&Data10, &Data11,
		                         &Data12, &Data13, &Data14, &Data15));

	TEST_P(ConvertLongToStringTest, TestLong) {
		const CString str = ConvertValueToString(lValue, lDividend);
		EXPECT_STREQ(str, CValueOfPeriod);
	}

	struct StrConvertIntegerToString {
		StrConvertIntegerToString(int iValue, const CString& CValueOfPeriod, long lDividend) {
			m_iValue = iValue;
			m_strValue = CValueOfPeriod;
			m_lDividend = lDividend;
		}

	public:
		int m_iValue;
		CString m_strValue;
		long m_lDividend;
	};

	StrConvertIntegerToString Data20(10234, _T("10234.0000"), 1);
	StrConvertIntegerToString Data21(-11023.43, _T("-11023.0000"), 1);
	StrConvertIntegerToString Data22(12102346, _T("12102346.0000"), 1);
	StrConvertIntegerToString Data23(-10234, _T("-10.2340"), 1000);
	StrConvertIntegerToString Data24(1102344, _T("110.2344"), 10000);
	StrConvertIntegerToString Data25(-12102346, _T("-1210.2346"), 10000);
	/*
	StrConvertIntegerToString Data26
	StrConvertIntegerToString Data27
	StrConvertIntegerToString Data28
	StrConvertIntegerToString Data29
	StrConvertIntegerToString Data30
	StrConvertIntegerToString Data31
	StrConvertIntegerToString Data32
	*/

	class ConvertIntegerToStringTest : public testing::TestWithParam<StrConvertIntegerToString*> {
	protected:
		void SetUp() override {
			ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
			const StrConvertIntegerToString* pData = GetParam();
			iValue = pData->m_iValue;
			CValueOfPeriod = pData->m_strValue;
			lDividend = pData->m_lDividend;
		}

		void TearDown() override {
			// clearUp
		}

	public:
		int iValue;
		CString CValueOfPeriod;
		long lDividend;
	};

	INSTANTIATE_TEST_SUITE_P(TestConvertIntegerToString, ConvertIntegerToStringTest, testing::Values(&Data20, &Data21,
		                         &Data22, &Data23, &Data24, &Data25));

	TEST_P(ConvertIntegerToStringTest, TestInteger) {
		const CString str = ConvertValueToString(iValue, lDividend);
		EXPECT_STREQ(str, CValueOfPeriod);
	}

	struct StrConvertINT64ToString {
		StrConvertINT64ToString(INT64 iValue, const CString& CValueOfPeriod, long lDividend) {
			m_iValue = iValue;
			m_strValue = CValueOfPeriod;
			m_lDividend = lDividend;
		}

	public:
		INT64 m_iValue;
		CString m_strValue;
		long m_lDividend;
	};

	StrConvertINT64ToString Data40(10234, _T("10234.0000"), 1);
	StrConvertINT64ToString Data41(-11023.43, _T("-11023.0000"), 1);
	StrConvertINT64ToString Data42(12102346, _T("12102346.0000"), 1);
	StrConvertINT64ToString Data43(-10234, _T("-10.2340"), 1000);
	StrConvertINT64ToString Data44(1102344, _T("110.2344"), 10000);
	StrConvertINT64ToString Data45(-12102346, _T("-1210.2346"), 10000);
	/*
	StrConvertINT64ToString Data46
	StrConvertINT64ToString Data47
	StrConvertINT64ToString Data48
	StrConvertINT64ToString Data49
	StrConvertINT64ToString Data50
	StrConvertINT64ToString Data51
	StrConvertINT64ToString Data52
	*/

	class ConvertINT64ToStringTest : public testing::TestWithParam<StrConvertINT64ToString*> {
	protected:
		void SetUp() override {
			ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
			const StrConvertINT64ToString* pData = GetParam();
			iValue = pData->m_iValue;
			CValueOfPeriod = pData->m_strValue;
			lDividend = pData->m_lDividend;
		}

		void TearDown() override {
			// clearUp
		}

	public:
		INT64 iValue;
		CString CValueOfPeriod;
		long lDividend;
	};

	INSTANTIATE_TEST_SUITE_P(TestConvertINT64ToString, ConvertINT64ToStringTest, testing::Values(&Data40, &Data41,
		                         &Data42, &Data43, &Data44, &Data45));

	TEST_P(ConvertINT64ToStringTest, TestINT64) {
		const CString str = ConvertValueToString(iValue, lDividend);
		EXPECT_STREQ(str, CValueOfPeriod);
	}

	class CRSReferenceTest : public testing::Test {
		void SetUp() override {}

		void TearDown() override {
			gl_systemConfiguration.SetWorkingMode(false);
		}
	};

	TEST_F(CRSReferenceTest, TestInitialize) {
		constexpr CRSReference RSReference;
		EXPECT_FALSE(RSReference.m_fActive);
		for (int i = 0; i < 4; i++) {
			EXPECT_EQ(RSReference.m_lStrongDayLength[i], 0);
			EXPECT_EQ(RSReference.m_lDayLength[i], 0);
			EXPECT_DOUBLE_EQ(RSReference.m_dRSStrong[i], 50.0);
		}
	}

	TEST(FormatToMKTest, TestFormatToMK) {
		CString str = FormatToMK(123);
		EXPECT_STREQ(str, _T(" 123"));
		str = FormatToMK(12 * 1024);
		EXPECT_STREQ(str, _T("  12K"));
		str = FormatToMK(12 * 1024 * 1024);
		EXPECT_STREQ(str, _T("  12M"));
		str = FormatToMK(static_cast<long>(12) * 1024 * 1024);
		EXPECT_STREQ(str, _T("  12M"));
	}
}
