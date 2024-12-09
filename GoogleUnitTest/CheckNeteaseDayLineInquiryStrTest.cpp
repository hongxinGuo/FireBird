#include"pch.h"

////#include"gtest/gtest.h"

import FireBird.System.Message;

#include"ChinaMarket.h"
#include"GeneralCheck.h"

namespace FireBirdTest {
	struct CheckNeteaseDayLineInquiryStrData {
		CheckNeteaseDayLineInquiryStrData(int count, const CString& Data) {
			m_iCount = count;
			m_strData = Data;
		}

	public:
		int m_iCount;
		CString m_strData;
	};

	CheckNeteaseDayLineInquiryStrData Data1(1, _T("0600000"));
	CheckNeteaseDayLineInquiryStrData Data2(2, _T("1000001"));
	// 无效上海股票代码
	CheckNeteaseDayLineInquiryStrData Data3(3, _T("0700000"));
	CheckNeteaseDayLineInquiryStrData Data4(4, _T("0400000"));
	CheckNeteaseDayLineInquiryStrData Data5(5, _T("0400000"));
	// 无效深圳股票代码
	CheckNeteaseDayLineInquiryStrData Data6(6, _T("1400000"));
	CheckNeteaseDayLineInquiryStrData Data7(7, _T("1400000"));
	CheckNeteaseDayLineInquiryStrData Data8(8, _T("1400000"));

	class CheckNeteaseDayLineInquiryStrTest : public::testing::TestWithParam<CheckNeteaseDayLineInquiryStrData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const CheckNeteaseDayLineInquiryStrData* pData = GetParam();
			m_iCount = pData->m_iCount;
			m_strCode = pData->m_strData;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.InnerSystemInfoSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		int m_iCount;
		CString m_strCode;
	};

	INSTANTIATE_TEST_SUITE_P(TestCheckNeteaseDayLineInquiryData, CheckNeteaseDayLineInquiryStrTest,
	                         testing::Values(&Data1, &Data2, &Data3, &Data4, &Data5, &Data6, &Data7, &Data8
	                         ));

	TEST_P(CheckNeteaseDayLineInquiryStrTest, TestCheck) {
		const bool fSucceed = gl_pChinaMarket->CheckValidOfNeteaseDayLineInquiringStr(m_strCode);
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
