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
	static CDataStockSymbol s_dataStockSymbol;
	class CDataStockSectionTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			EXPECT_FALSE(s_dataStockSymbol.IsUpdateStockSection());
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
			EXPECT_FALSE(s_dataStockSymbol.IsUpdateStockSection());
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			// clearup
			EXPECT_FALSE(s_dataStockSymbol.IsUpdateStockSection());
			s_dataStockSymbol.SetStockSectionActiveFlag(0, true);
			s_dataStockSymbol.SetStockSectionActiveFlag(1, false);
			s_dataStockSymbol.SetUpdateStockSection(false);

			GeneralCheck();
		}

	protected:
	};

	TEST_F(CDataStockSectionTest, TestIsUpdateStockSection) {
		EXPECT_FALSE(s_dataStockSymbol.IsUpdateStockSection());
		s_dataStockSymbol.SetUpdateStockSection(true);
		EXPECT_TRUE(s_dataStockSymbol.IsUpdateStockSection());
		s_dataStockSymbol.SetUpdateStockSection(false);
		EXPECT_FALSE(s_dataStockSymbol.IsUpdateStockSection());
	}

	TEST_F(CDataStockSectionTest, TestIsStockSectionActive) {
		EXPECT_TRUE(s_dataStockSymbol.IsStockSectionActive(0));
		s_dataStockSymbol.SetStockSectionActiveFlag(0, true);
		EXPECT_TRUE(s_dataStockSymbol.IsStockSectionActive(0));
		s_dataStockSymbol.SetStockSectionActiveFlag(0, false);
		EXPECT_FALSE(s_dataStockSymbol.IsStockSectionActive(0));
	}

	TEST_F(CDataStockSectionTest, TestUpdateStockSection1) {
		EXPECT_FALSE(s_dataStockSymbol.IsStockSectionActive(1));
		EXPECT_TRUE(s_dataStockSymbol.UpdateStockSection(1));
		EXPECT_TRUE(s_dataStockSymbol.IsStockSectionActive(1));
		EXPECT_FALSE(s_dataStockSymbol.UpdateStockSection(1));
	}

	TEST_F(CDataStockSectionTest, TestUpdateStockSection2) {
		CString strShanghaiStock = _T("600601.SS");
		CString strShenzhenStock = _T("000001.SZ");
		long lIndex = 600;
		long lIndex2 = 1000;

		EXPECT_TRUE(s_dataStockSymbol.IsStockSectionActive(lIndex)) << "装载预设数据库后如此";
		s_dataStockSymbol.SetStockSectionActiveFlag(lIndex, false);
		EXPECT_TRUE(s_dataStockSymbol.UpdateStockSection(strShanghaiStock));
		EXPECT_TRUE(s_dataStockSymbol.IsStockSectionActive(lIndex));
		EXPECT_FALSE(s_dataStockSymbol.UpdateStockSection(strShanghaiStock));

		EXPECT_TRUE(s_dataStockSymbol.IsStockSectionActive(lIndex2)) << "装载预设数据库后如此";
		s_dataStockSymbol.SetStockSectionActiveFlag(lIndex2, false);
		EXPECT_TRUE(s_dataStockSymbol.UpdateStockSection(strShenzhenStock));
		EXPECT_TRUE(s_dataStockSymbol.IsStockSectionActive(lIndex2));
		EXPECT_FALSE(s_dataStockSymbol.UpdateStockSection(strShenzhenStock));
	}
}