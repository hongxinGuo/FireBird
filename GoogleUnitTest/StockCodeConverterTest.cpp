#include"pch.h"

#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"GeneralCheck.h"

#include"StockCodeConverter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class StockCodeConverterTest : public ::testing::Test {
		virtual void SetUp(void) override {
			GeneralCheck();
		}
		virtual void TearDown(void) override {
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

	TEST_F(StockCodeConverterTest, TestXferSinaToStandred) {
		EXPECT_STREQ(XferSinaToStandred(_T("sh600001")), _T("600001.SS"));
		EXPECT_STREQ(XferSinaToStandred(_T("sz000001")), _T("000001.SZ"));
		EXPECT_STREQ(XferSinaToStandred(_T("sh60001")), _T("h60001.SS"));
		EXPECT_STREQ(XferSinaToStandred(_T("sz00001")), _T("z00001.SZ"));
	}

	TEST_F(StockCodeConverterTest, TestXferSinaToNetease) {
		EXPECT_STREQ(XferSinaToNetease(_T("sh600001")), _T("0600001"));
		EXPECT_STREQ(XferSinaToNetease(_T("sz000001")), _T("1000001"));
		EXPECT_STREQ(XferSinaToNetease(_T("sh60001")), _T("0h60001"));
		EXPECT_STREQ(XferSinaToNetease(_T("sz00001")), _T("1z00001"));
	}

	TEST_F(StockCodeConverterTest, TestXferNeteaseToStandred) {
		EXPECT_STREQ(XferNeteaseToStandred(_T("0600001")), _T("600001.SS"));
		EXPECT_STREQ(XferNeteaseToStandred(_T("1000001")), _T("000001.SZ"));
		EXPECT_STREQ(XferNeteaseToStandred(_T("060001")), _T("060001.SS"));
		EXPECT_STREQ(XferNeteaseToStandred(_T("100001")), _T("100001.SZ"));
	}

	TEST_F(StockCodeConverterTest, TestXferNeteaseToSina) {
		EXPECT_STREQ(XferNeteaseToSina(_T("0600001")), _T("sh600001"));
		EXPECT_STREQ(XferNeteaseToSina(_T("1000001")), _T("sz000001"));
		EXPECT_STREQ(XferNeteaseToSina(_T("060001")), _T("sh060001"));
		EXPECT_STREQ(XferNeteaseToSina(_T("100001")), _T("sz100001"));
	}

	TEST_F(StockCodeConverterTest, TestXferStandredToSina) {
		EXPECT_STREQ(XferStandredToSina(_T("600001.SS")), _T("sh600001"));
		EXPECT_STREQ(XferStandredToSina(_T("000001.SZ")), _T("sz000001"));
		EXPECT_STREQ(XferStandredToSina(_T("60001.SS")), _T("sh60001."));
		EXPECT_STREQ(XferStandredToSina(_T("00001.SZ")), _T("sz00001."));
	}

	TEST_F(StockCodeConverterTest, TestXferStandredToNetease) {
		EXPECT_STREQ(XferStandredToNetease(_T("600001.SS")), _T("0600001"));
		EXPECT_STREQ(XferStandredToNetease(_T("000001.SZ")), _T("1000001"));
		EXPECT_STREQ(XferStandredToNetease(_T("60001.SS")), _T("060001."));
		EXPECT_STREQ(XferStandredToNetease(_T("00001.SZ")), _T("100001."));
	}

	TEST_F(StockCodeConverterTest, TestXferStandredToTengxun) {
		EXPECT_STREQ(XferStandredToTengxun(_T("600001.SS")), _T("sh600001"));
		EXPECT_STREQ(XferStandredToTengxun(_T("000001.SZ")), _T("sz000001"));
		EXPECT_STREQ(XferStandredToTengxun(_T("60001.SS")), _T("sh60001."));
		EXPECT_STREQ(XferStandredToTengxun(_T("00001.SZ")), _T("sz00001."));
	}
}