#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"DataStockSymbol.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	static CDataStockSymbolPtr s_pDataStockSymbol = nullptr;
	class CDataStockSymbolTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			s_pDataStockSymbol = make_shared<CDataStockSymbol>();

			GeneralCheck();
			EXPECT_FALSE(s_pDataStockSymbol->IsUpdateStockSection());
		}

		static void TearDownTestSuite(void) {
			EXPECT_FALSE(s_pDataStockSymbol->IsUpdateStockSection());
			GeneralCheck();

			s_pDataStockSymbol = nullptr;
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			// clearup
			EXPECT_FALSE(s_pDataStockSymbol->IsUpdateStockSection());
			s_pDataStockSymbol->SetStockSectionActiveFlag(0, true);
			s_pDataStockSymbol->SetStockSectionActiveFlag(1, false);
			s_pDataStockSymbol->SetUpdateStockSection(false);

			GeneralCheck();
		}

	protected:
	};

	TEST_F(CDataStockSymbolTest, TestIsUpdateStockSection) {
		EXPECT_FALSE(s_pDataStockSymbol->IsUpdateStockSection());
		s_pDataStockSymbol->SetUpdateStockSection(true);
		EXPECT_TRUE(s_pDataStockSymbol->IsUpdateStockSection());
		s_pDataStockSymbol->SetUpdateStockSection(false);
		EXPECT_FALSE(s_pDataStockSymbol->IsUpdateStockSection());
	}

	TEST_F(CDataStockSymbolTest, TestIsStockSectionActive) {
		EXPECT_TRUE(s_pDataStockSymbol->IsStockSectionActive(0));
		s_pDataStockSymbol->SetStockSectionActiveFlag(0, true);
		EXPECT_TRUE(s_pDataStockSymbol->IsStockSectionActive(0));
		s_pDataStockSymbol->SetStockSectionActiveFlag(0, false);
		EXPECT_FALSE(s_pDataStockSymbol->IsStockSectionActive(0));
	}

	TEST_F(CDataStockSymbolTest, TestUpdateStockSection1) {
		EXPECT_FALSE(s_pDataStockSymbol->IsStockSectionActive(1));
		EXPECT_TRUE(s_pDataStockSymbol->UpdateStockSection(1));
		EXPECT_TRUE(s_pDataStockSymbol->IsStockSectionActive(1));
		EXPECT_FALSE(s_pDataStockSymbol->UpdateStockSection(1));
	}

	TEST_F(CDataStockSymbolTest, TestUpdateStockSection2) {
		CString strShanghaiStock = _T("600601.SS");
		CString strShenzhenStock = _T("000001.SZ");
		long lIndex = 600;
		long lIndex2 = 1000;

		EXPECT_TRUE(s_pDataStockSymbol->IsStockSectionActive(lIndex)) << "装载预设数据库后如此";
		s_pDataStockSymbol->SetStockSectionActiveFlag(lIndex, false);
		EXPECT_TRUE(s_pDataStockSymbol->UpdateStockSection(strShanghaiStock));
		EXPECT_TRUE(s_pDataStockSymbol->IsStockSectionActive(lIndex));
		EXPECT_FALSE(s_pDataStockSymbol->UpdateStockSection(strShanghaiStock));

		EXPECT_TRUE(s_pDataStockSymbol->IsStockSectionActive(lIndex2)) << "装载预设数据库后如此";
		s_pDataStockSymbol->SetStockSectionActiveFlag(lIndex2, false);
		EXPECT_TRUE(s_pDataStockSymbol->UpdateStockSection(strShenzhenStock));
		EXPECT_TRUE(s_pDataStockSymbol->IsStockSectionActive(lIndex2));
		EXPECT_FALSE(s_pDataStockSymbol->UpdateStockSection(strShenzhenStock));
	}

	TEST_F(CDataStockSymbolTest, TestIsStockSymbol) {
		EXPECT_TRUE(s_pDataStockSymbol->IsStockSymbol(_T("600000.SS")));
		EXPECT_TRUE(s_pDataStockSymbol->IsStockSymbol(_T("900000.SS")));
		EXPECT_TRUE(s_pDataStockSymbol->IsStockSymbol(_T("688000.SS")));
		EXPECT_TRUE(s_pDataStockSymbol->IsStockSymbol(_T("000000.SZ")));
		EXPECT_TRUE(s_pDataStockSymbol->IsStockSymbol(_T("002000.SZ")));
		EXPECT_TRUE(s_pDataStockSymbol->IsStockSymbol(_T("001000.SZ")));
		EXPECT_FALSE(s_pDataStockSymbol->IsStockSymbol(_T("60000.SS")));
		EXPECT_FALSE(s_pDataStockSymbol->IsStockSymbol(_T("00000.SZ")));
	}

	TEST_F(CDataStockSymbolTest, TestIncreaseIndex) {
		long l = 0;
		EXPECT_EQ(s_pDataStockSymbol->IncreaseIndex(l, 100), 1);
		EXPECT_EQ(l, 1);
		EXPECT_EQ(s_pDataStockSymbol->IncreaseIndex(l, 100), 2);
		EXPECT_EQ(l, 2);
		l = 98;
		EXPECT_EQ(s_pDataStockSymbol->IncreaseIndex(l, 100), 99);
		EXPECT_EQ(l, 99);
		EXPECT_EQ(s_pDataStockSymbol->IncreaseIndex(l, 100), 0);
		EXPECT_EQ(l, 0);
	}

	TEST_F(CDataStockSymbolTest, TestAdd) {
		EXPECT_FALSE(s_pDataStockSymbol->IsStockSymbol(_T("800800.SS"))) << "没有这个数据段的股票代码";
		s_pDataStockSymbol->Add(_T("800800.SS"));
		EXPECT_TRUE(s_pDataStockSymbol->IsStockSymbol(_T("800800.SS")));

		// 恢复原状
		EXPECT_TRUE(s_pDataStockSymbol->Delete(_T("800800.SS")));
	}

	TEST_F(CDataStockSymbolTest, TestUpdateStockSectionDB) {
		EXPECT_FALSE(s_pDataStockSymbol->IsStockSectionActive(800));
		s_pDataStockSymbol->SetStockSectionActiveFlag(800, true);
		s_pDataStockSymbol->UpdateStockSectionDB();
		s_pDataStockSymbol->LoadStockSectionDB();
		EXPECT_TRUE(s_pDataStockSymbol->IsStockSectionActive(800));

		// 恢复原状
		s_pDataStockSymbol->SetStockSectionActiveFlag(800, false);
		s_pDataStockSymbol->UpdateStockSectionDB();
	}
}