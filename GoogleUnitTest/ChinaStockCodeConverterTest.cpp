#include"pch.h"

#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"GeneralCheck.h"

#include"ChinaStockCodeConverter.h"

namespace FireBirdTest {
	class StockCodeConverterTest : public ::testing::Test {
		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}
	};

	TEST_F(StockCodeConverterTest, TestIsShanghaiExchange) {
		EXPECT_TRUE(IsShanghaiExchange(_T("SS")));
		EXPECT_TRUE(IsShanghaiExchange(_T("600001.SS")));
		EXPECT_TRUE(IsShanghaiExchange(_T("000001.SS")));
		EXPECT_TRUE(IsShanghaiExchange(_T("399399.SS")));
		EXPECT_TRUE(IsShanghaiExchange(_T("abcSS")));
		EXPECT_FALSE(IsShanghaiExchange(_T("SZ")));
	}

	TEST_F(StockCodeConverterTest, TestIsShenzhenExchange) {
		EXPECT_TRUE(IsShenzhenExchange(_T("SZ")));
		EXPECT_TRUE(IsShenzhenExchange(_T("600001.SZ")));
		EXPECT_TRUE(IsShenzhenExchange(_T("000001.SZ")));
		EXPECT_TRUE(IsShenzhenExchange(_T("399399.SZ")));
		EXPECT_TRUE(IsShenzhenExchange(_T("abcSZ")));
		EXPECT_FALSE(IsShenzhenExchange(_T("SS")));
	}

	TEST_F(StockCodeConverterTest, TestGetStockExchange) {
		EXPECT_STREQ(GetStockExchange(_T("600000.SS")), _T("SS"));
		EXPECT_STREQ(GetStockExchange(_T("600000.SZ")), _T("SZ"));
		EXPECT_STREQ(GetStockExchange(_T("600SA")), _T("SA"));
		EXPECT_STREQ(GetStockExchange(_T("SS")), _T("SS"));
		EXPECT_STREQ(GetStockExchange(_T("S")), _T("S"));
	}

	TEST_F(StockCodeConverterTest, TestGetStockSymbol) {
		EXPECT_STREQ(GetStockSymbol(_T("600001.SS")), _T("600001"));
		EXPECT_STREQ(GetStockSymbol(_T("600000.SZ")), _T("600000"));
		EXPECT_STREQ(GetStockSymbol(_T("600SA")), _T("60"));
		EXPECT_STREQ(GetStockSymbol(_T("600.SS")), _T("600"));
	}

	TEST_F(StockCodeConverterTest, TestCreateStockCode) {
		EXPECT_STREQ(CreateStockCode(_T("SS"), _T("600000")), _T("600000.SS"));
		EXPECT_STREQ(CreateStockCode(_T("SZ"), _T("000001")), _T("000001.SZ"));
		EXPECT_STREQ(CreateStockCode(_T("SS"), _T("6000")), _T("6000.SS"));
		EXPECT_STREQ(CreateStockCode(_T("SZ"), _T("")), _T(".SZ"));
	}

	TEST_F(StockCodeConverterTest, TestXferSinaToStandard) {
		EXPECT_STREQ(XferSinaToStandard(_T("sh600001")), _T("600001.SS"));
		EXPECT_STREQ(XferSinaToStandard(_T("sz000001")), _T("000001.SZ"));
		EXPECT_STREQ(XferSinaToStandard(_T("sh60001")), _T("h60001.SS"));
		EXPECT_STREQ(XferSinaToStandard(_T("sz00001")), _T("z00001.SZ"));
	}

	TEST_F(StockCodeConverterTest, TestXferSinaToNetease) {
		EXPECT_STREQ(XferSinaToNetease(_T("sh600001")), _T("0600001"));
		EXPECT_STREQ(XferSinaToNetease(_T("sz000001")), _T("1000001"));
		EXPECT_STREQ(XferSinaToNetease(_T("sh60001")), _T("0h60001"));
		EXPECT_STREQ(XferSinaToNetease(_T("sz00001")), _T("1z00001"));
	}

	TEST_F(StockCodeConverterTest, TestXferNeteaseToStandard) {
		EXPECT_STREQ(XferNeteaseToStandard(_T("0600001")), _T("600001.SS"));
		EXPECT_STREQ(XferNeteaseToStandard(_T("1000001")), _T("000001.SZ"));
		EXPECT_STREQ(XferNeteaseToStandard(_T("060001")), _T("060001.SS"));
		EXPECT_STREQ(XferNeteaseToStandard(_T("100001")), _T("100001.SZ"));
	}

	TEST_F(StockCodeConverterTest, TestXferNeteaseToSina) {
		EXPECT_STREQ(XferNeteaseToSina(_T("0600001")), _T("sh600001"));
		EXPECT_STREQ(XferNeteaseToSina(_T("1000001")), _T("sz000001"));
		EXPECT_STREQ(XferNeteaseToSina(_T("060001")), _T("sh060001"));
		EXPECT_STREQ(XferNeteaseToSina(_T("100001")), _T("sz100001"));
	}

	TEST_F(StockCodeConverterTest, TestXferStandardToSina) {
		EXPECT_STREQ(XferStandardToSina(_T("600001.SS")), _T("sh600001"));
		EXPECT_STREQ(XferStandardToSina(_T("000001.SZ")), _T("sz000001"));
		EXPECT_STREQ(XferStandardToSina(_T("60001.SS")), _T("sh60001."));
		EXPECT_STREQ(XferStandardToSina(_T("00001.SZ")), _T("sz00001."));
	}

	TEST_F(StockCodeConverterTest, TestXferStandardToNetease) {
		EXPECT_STREQ(XferStandardToNetease(_T("600001.SS")), _T("0600001"));
		EXPECT_STREQ(XferStandardToNetease(_T("000001.SZ")), _T("1000001"));
		EXPECT_STREQ(XferStandardToNetease(_T("60001.SS")), _T("060001."));
		EXPECT_STREQ(XferStandardToNetease(_T("00001.SZ")), _T("100001."));
	}

	TEST_F(StockCodeConverterTest, TestXferStandardToTengxun) {
		EXPECT_STREQ(XferStandardToTengxun(_T("600001.SS")), _T("sh600001"));
		EXPECT_STREQ(XferStandardToTengxun(_T("000001.SZ")), _T("sz000001"));
		EXPECT_STREQ(XferStandardToTengxun(_T("60001.SS")), _T("sh60001."));
		EXPECT_STREQ(XferStandardToTengxun(_T("00001.SZ")), _T("sz00001."));
	}
}
