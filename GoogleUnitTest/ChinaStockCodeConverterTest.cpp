#include"pch.h"

#include"WorldMarket.h"

#include"GeneralCheck.h"

#include"ChinaStockCodeConverter.h"

namespace FireBirdTest {
	class StockCodeConverterTest : public testing::Test {
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			gl_systemConfiguration.SetWorkingMode(false);

			SCOPED_TRACE("");
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
		EXPECT_EQ(GetStockExchange(_T("600000.SS")), _T("SS"));
		EXPECT_EQ(GetStockExchange(_T("600000.SZ")), _T("SZ"));
		EXPECT_EQ(GetStockExchange(_T("600SA")), _T("SA"));
		EXPECT_EQ(GetStockExchange(_T("SS")), _T("SS"));
	}

	TEST_F(StockCodeConverterTest, TestGetStockSymbol) {
		EXPECT_EQ(GetStockSymbol(_T("600001.SS")), _T("600001"));
		EXPECT_EQ(GetStockSymbol(_T("600000.SZ")), _T("600000"));
		EXPECT_EQ(GetStockSymbol(_T("600SA")), _T("60"));
		EXPECT_EQ(GetStockSymbol(_T("600.SS")), _T("600"));
	}

	TEST_F(StockCodeConverterTest, TestCreateStockCode) {
		EXPECT_EQ(CreateStockCode(_T("SS"), _T("600000")), _T("600000.SS"));
		EXPECT_EQ(CreateStockCode(_T("SZ"), _T("000001")), _T("000001.SZ"));
		EXPECT_EQ(CreateStockCode(_T("SS"), _T("6000")), _T("6000.SS"));
		EXPECT_EQ(CreateStockCode(_T("SZ"), _T("")), _T(".SZ"));
	}

	TEST_F(StockCodeConverterTest, TestXferSinaToStandard) {
		string str2;
		string str = "sh600001";
		string_view sv = str;
		EXPECT_EQ(XferSinaToStandard(str), _T("600001.SS"));
		EXPECT_EQ(XferSinaToStandard(sv), _T("600001.SS"));
		str = _T("sz000001");
		sv = str;
		EXPECT_EQ(XferSinaToStandard(str), _T("000001.SZ"));
		EXPECT_EQ(XferSinaToStandard(sv), _T("000001.SZ"));
		str = _T("sh60001");
		EXPECT_EQ(XferSinaToStandard(str), _T("h60001.SS"));
		str = _T("sz00001");
		EXPECT_EQ(XferSinaToStandard(str), _T("z00001.SZ"));
		str = "ss600001";
		sv = str;
		EXPECT_THROW(str2 = XferSinaToStandard(str), std::exception);
		EXPECT_THROW(str2 = XferSinaToStandard(sv), std::exception);
	}

	TEST_F(StockCodeConverterTest, TestXferSinaToNetease) {
		EXPECT_EQ(XferSinaToNetease(_T("sh600001")), _T("0600001"));
		EXPECT_EQ(XferSinaToNetease(_T("sz000001")), _T("1000001"));
		EXPECT_EQ(XferSinaToNetease(_T("sh60001")), _T("0h60001"));
		EXPECT_EQ(XferSinaToNetease(_T("sz00001")), _T("1z00001"));
		EXPECT_THROW(string str = XferSinaToNetease(_T("ss600001")), exception);
	}

	TEST_F(StockCodeConverterTest, TestXferNeteaseToStandard) {
		string str = _T("0600001");
		EXPECT_EQ(XferNeteaseToStandard(str), _T("600001.SS"));
		str = _T("1000001");
		EXPECT_EQ(XferNeteaseToStandard(str), _T("000001.SZ"));
		str = _T("060001");
		EXPECT_THROW(string str1 = XferNeteaseToStandard(str), exception);
		str = _T("100001");
		EXPECT_THROW(string str1 = XferNeteaseToStandard(str), exception);
		str = _T("200001");
		EXPECT_THROW(string str1 = XferNeteaseToStandard(str), exception);
	}

	TEST_F(StockCodeConverterTest, TestXferNeteaseToStandard2) {
		string str = _T("0600001");
		EXPECT_EQ(XferNeteaseToStandard(string_view(str)), _T("600001.SS"));
		str = _T("1000001");
		EXPECT_EQ(XferNeteaseToStandard(string_view(str)), _T("000001.SZ"));
		str = _T("060001");
		EXPECT_THROW(string str1 = XferNeteaseToStandard(str), exception);
		str = _T("100001");
		EXPECT_THROW(string str1 = XferNeteaseToStandard(str), exception);
		str = _T("200001");
		EXPECT_THROW(string str1 = XferNeteaseToStandard(str), exception);
	}

	TEST_F(StockCodeConverterTest, TestXferNeteaseToSina) {
		EXPECT_EQ(XferNeteaseToSina(_T("0600001")), _T("sh600001"));
		EXPECT_EQ(XferNeteaseToSina(_T("1000001")), _T("sz000001"));
		EXPECT_EQ(XferNeteaseToSina(_T("060001")), _T("sh060001"));
		EXPECT_EQ(XferNeteaseToSina(_T("100001")), _T("sz100001"));
		EXPECT_THROW(string str = XferNeteaseToSina(_T("300001")), exception);
	}

	TEST_F(StockCodeConverterTest, TestXferStandardToSina) {
		EXPECT_EQ(XferStandardToSina(_T("600001.SS")), _T("sh600001"));
		EXPECT_EQ(XferStandardToSina(_T("000001.SZ")), _T("sz000001"));
		EXPECT_EQ(XferStandardToSina(_T("60001.SS")), _T("sh60001."));
		EXPECT_EQ(XferStandardToSina(_T("00001.SZ")), _T("sz00001."));
		EXPECT_THROW(string str = XferStandardToSina(_T("000001.SA")), exception);
		EXPECT_THROW(string str = XferStandardToSina(_T("000001.AS")), exception);
	}

	TEST_F(StockCodeConverterTest, TestXferStandardToNetease) {
		EXPECT_EQ(XferStandardToNetease(_T("600001.SS")), _T("0600001"));
		EXPECT_EQ(XferStandardToNetease(_T("000001.SZ")), _T("1000001"));
		EXPECT_EQ(XferStandardToNetease(_T("60001.SS")), _T("060001."));
		EXPECT_EQ(XferStandardToNetease(_T("00001.SZ")), _T("100001."));
		EXPECT_THROW(string str = XferStandardToNetease(_T("000001.SA")), exception);
		EXPECT_THROW(string str = XferStandardToNetease(_T("000001.AS")), exception);
	}

	TEST_F(StockCodeConverterTest, TestXferStandardToTengxun) {
		EXPECT_EQ(XferStandardToTengxun(_T("600001.SS")), _T("sh600001"));
		EXPECT_EQ(XferStandardToTengxun(_T("000001.SZ")), _T("sz000001"));
		EXPECT_EQ(XferStandardToTengxun(_T("60001.SS")), _T("sh60001."));
		EXPECT_EQ(XferStandardToTengxun(_T("00001.SZ")), _T("sz00001."));
		EXPECT_THROW(string str = XferStandardToTengxun(_T("000001.SA")), exception);
		EXPECT_THROW(string str = XferStandardToTengxun(_T("000001.AS")), exception);
	}
}
