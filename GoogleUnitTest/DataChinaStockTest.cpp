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
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("10.0"), _T("0.0009")), 0.0) << _T("LastClose小于0.001时返回0");
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("11.0"), _T("10.0")), 0.1);
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("10.5"), _T("10.0")), 0.05);
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("9.0"), _T("10.0")), -0.1);
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("11.11"), _T("10.0")), 0.0) << _T("大于0.11时返回0");
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("8.89"), _T("10.0")), 0.0) << _T("小于-0.11时返回0");
	}
}