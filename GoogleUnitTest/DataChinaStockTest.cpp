#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"DataChinaStock.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CDataChinaStockTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			// clearu
			GeneralCheck();
		}

	protected:
		CDataChinaStock m_dataChinaStock;
	};

	TEST_F(CDataChinaStockTest, TestGetUpDownRate) {
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("10.0"), _T("0.0009")), 0.0) << _T("LastCloseС��0.001ʱ����0");
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("11.0"), _T("10.0")), 0.1);
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("10.5"), _T("10.0")), 0.05);
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("9.0"), _T("10.0")), -0.1);
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("11.11"), _T("10.0")), 0.0) << _T("����0.11ʱ����0");
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("8.89"), _T("10.0")), 0.0) << _T("С��-0.11ʱ����0");
	}

	TEST_F(CDataChinaStockTest, TestGetNeteaseDayLineDataInquiringIndex) {
		EXPECT_EQ(m_dataChinaStock.GetNeteaseDayLineDataInquiringIndex(), 0);
		m_dataChinaStock.SetNeteaseDayLineDataInquiringIndex(100);
		EXPECT_EQ(m_dataChinaStock.GetNeteaseDayLineDataInquiringIndex(), 100);
	}

	TEST_F(CDataChinaStockTest, TestGetNeteaseRTDataInquiringIndex) {
		EXPECT_EQ(m_dataChinaStock.GetNeteaseRTDataInquiringIndex(), 0);
		m_dataChinaStock.SetNeteaseRTDataInquiringIndex(100);
		EXPECT_EQ(m_dataChinaStock.GetNeteaseRTDataInquiringIndex(), 100);
	}

	TEST_F(CDataChinaStockTest, TestGetSinaRTDataInquiringIndex) {
		EXPECT_EQ(m_dataChinaStock.GetSinaRTDataInquiringIndex(), 0);
		m_dataChinaStock.SetSinaRTDataInquiringIndex(100);
		EXPECT_EQ(m_dataChinaStock.GetSinaRTDataInquiringIndex(), 100);
	}
	TEST_F(CDataChinaStockTest, TestGetTengxunRTDataInquiringIndex) {
		EXPECT_EQ(m_dataChinaStock.GetTengxunRTDataInquiringIndex(), 0);
		m_dataChinaStock.SetTengxunRTDataInquiringIndex(100);
		EXPECT_EQ(m_dataChinaStock.GetTengxunRTDataInquiringIndex(), 100);
	}
}