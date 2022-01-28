#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"DataStockSymbol.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CDataStockSectionTest : public ::testing::Test {
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
		CDataStockSymbol m_dataStockSection;
	};

	TEST_F(CDataStockSectionTest, TestIsUpdateStockSection) {
		EXPECT_FALSE(m_dataStockSection.IsUpdateStockSection());
		m_dataStockSection.SetUpdateStockSection(true);
		EXPECT_TRUE(m_dataStockSection.IsUpdateStockSection());
		m_dataStockSection.SetUpdateStockSection(false);
		EXPECT_FALSE(m_dataStockSection.IsUpdateStockSection());
	}
}