#include"pch.h"

#include"GeneralCheck.h"

#include"ContainerStockSymbol.h"
#include"ChinaStockCodeConverter.h"

#include<memory>
using std::make_shared;

using namespace testing;

namespace FireBirdTest {
	static CContainerStockSymbolPtr s_pDataStockSymbol = nullptr;

	class CDataStockSymbolTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			s_pDataStockSymbol = make_shared<CContainerStockSymbol>();

			EXPECT_FALSE(s_pDataStockSymbol->IsUpdateStockSection());
			EXPECT_EQ(s_pDataStockSymbol->Size(), 14000) << "默认装入的StockSymbol数为14000";
		}

		static void TearDownTestSuite() {
			EXPECT_FALSE(s_pDataStockSymbol->IsUpdateStockSection());

			s_pDataStockSymbol = nullptr;
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			EXPECT_FALSE(s_pDataStockSymbol->IsUpdateStockSection());
			s_pDataStockSymbol->SetStockSectionActiveFlag(0, true);
			s_pDataStockSymbol->SetStockSectionActiveFlag(1, false);
			s_pDataStockSymbol->SetUpdateStockSection(false);

			SCOPED_TRACE("");
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
		const CString strShanghaiStock = _T("600601.SS");
		const CString strShenzhenStock = _T("000001.SZ");
		constexpr long lIndex = 600;
		constexpr long lIndex2 = 1000;

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

	TEST_F(CDataStockSymbolTest, TestGetNextIndex) {
		long l = 0;
		EXPECT_EQ(s_pDataStockSymbol->GetNextIndex(l), 1);
		EXPECT_EQ(l, 0);
		l++;
		EXPECT_EQ(s_pDataStockSymbol->GetNextIndex(l), 2);
		EXPECT_EQ(l, 1);
		l = 14000 - 2;
		EXPECT_EQ(s_pDataStockSymbol->GetNextIndex(l), 14000 - 1);
		EXPECT_EQ(l, 14000 - 2);
		l++;
		EXPECT_EQ(s_pDataStockSymbol->GetNextIndex(l), 0) << "默认装入的StockSymbol数为14000";
		EXPECT_EQ(l, 13999);
	}

	TEST_F(CDataStockSymbolTest, TestAdd) {
		EXPECT_FALSE(s_pDataStockSymbol->IsStockSymbol(_T("800800.SS"))) << "没有这个数据段的股票代码";
		s_pDataStockSymbol->Add(_T("800800.SS"));
		EXPECT_TRUE(s_pDataStockSymbol->IsStockSymbol(_T("800800.SS")));

		// ??????????
		EXPECT_TRUE(s_pDataStockSymbol->Delete(_T("800800.SS")));
	}

	TEST_F(CDataStockSymbolTest, TestUpdateStockSectionDB) {
		EXPECT_FALSE(s_pDataStockSymbol->IsStockSectionActive(800));
		s_pDataStockSymbol->SetStockSectionActiveFlag(800, true);
		s_pDataStockSymbol->UpdateStockSectionDB();
		s_pDataStockSymbol->LoadStockSectionDB();
		EXPECT_TRUE(s_pDataStockSymbol->IsStockSectionActive(800));

		// ??????????
		s_pDataStockSymbol->SetStockSectionActiveFlag(800, false);
		s_pDataStockSymbol->UpdateStockSectionDB();
	}

	TEST_F(CDataStockSymbolTest, TestGetNextStockInquiringMiddleStr) {
		long i = 0;
		EXPECT_STREQ(s_pDataStockSymbol->GetNextStockInquiringMiddleStr(i, _T(","), 2, XferStandardToSina), _T("sh000000,sh000001"));
		EXPECT_EQ(i, 2);

		i = 2;
		EXPECT_STREQ(s_pDataStockSymbol->GetNextStockInquiringMiddleStr(i, _T("."), 3, XferStandardToSina), _T("sh000002.sh000003.sh000004"));
		EXPECT_EQ(i, 5);

		i = 5;
		EXPECT_STREQ(s_pDataStockSymbol->GetNextStockInquiringMiddleStr(i, _T(","), 3, XferStandardToNetease), _T("0000005,0000006,0000007"));
		EXPECT_EQ(i, 8);
	}

	TEST_F(CDataStockSymbolTest, TestGetNextSinaStockInquiringMiddleStr) {
		long i = 2;
		EXPECT_STREQ(s_pDataStockSymbol->GetNextSinaStockInquiringMiddleStr(i), _T("sh000000,sh000001")) << "起始位置为零";

		i = 3;
		EXPECT_STREQ(s_pDataStockSymbol->GetNextSinaStockInquiringMiddleStr(i), _T("sh000002,sh000003,sh000004")) << "起始位置为2";;
	}

	TEST_F(CDataStockSymbolTest, TestGetNextTengxunStockInquiringMiddleStr) {
		long i = 2;
		EXPECT_STREQ(s_pDataStockSymbol->GetNextTengxunStockInquiringMiddleStr(i), _T("sh000000,sh000001")) << "起始位置为零";

		i = 3;
		EXPECT_STREQ(s_pDataStockSymbol->GetNextTengxunStockInquiringMiddleStr(i), _T("sh000002,sh000003,sh000004")) << "起始位置为2";
	}

	TEST_F(CDataStockSymbolTest, TestGetNextNeteaseStockInquiringMiddleStr) {
		long i = 2;
		EXPECT_STREQ(s_pDataStockSymbol->GetNextNeteaseStockInquiringMiddleStr(i), _T("0000000,0000001")) << "起始位置为零";

		i = 3;
		EXPECT_STREQ(s_pDataStockSymbol->GetNextNeteaseStockInquiringMiddleStr(i), _T("0000002,0000003,0000004")) << "起始位置为2";;
	}
}
