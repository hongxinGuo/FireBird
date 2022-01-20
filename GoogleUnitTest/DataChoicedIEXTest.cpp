#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"DataChoicedStock.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CDataChoicedIEXTest : public ::testing::Test {
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
		CDataChoicedStock m_dataChoicedIEX;
	};

	TEST_F(CDataChoicedIEXTest, TestInitialize) {
		EXPECT_EQ(m_dataChoicedIEX.GetSize(), 0);
	}

	TEST_F(CDataChoicedIEXTest, TestLoad) {
		EXPECT_EQ(m_dataChoicedIEX.GetSize(), 0) << "初始未装载IEX代码";

		m_dataChoicedIEX.LoadDB();
		EXPECT_EQ(m_dataChoicedIEX.GetSize(), 4) << "默认状态下装载4个代码";

		CWorldStockPtr pIEX = m_dataChoicedIEX.GetStock(2);
		EXPECT_STREQ(pIEX->GetSymbol(), _T("AAL")) << "装载时没有排序，使用的是原始位置";
	}
}