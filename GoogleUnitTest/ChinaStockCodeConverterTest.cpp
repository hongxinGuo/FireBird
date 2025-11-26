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

	TEST_F(StockCodeConverterTest, _T(TestIsShanghaiExchange)) {
		EXPECT_TRUE(IsShanghaiExchange("SS"));
		EXPECT_TRUE(IsShanghaiExchange("600001.SS"));
		EXPECT_TRUE(IsShanghaiExchange("000001.SS"));
		EXPECT_TRUE(IsShanghaiExchange("399399.SS"));
		EXPECT_TRUE(IsShanghaiExchange("abcSS"));
		EXPECT_FALSE(IsShanghaiExchange("SZ"));
	}

	TEST_F(StockCodeConverterTest, TEXT(TestIsShenzhenExchange)) {
		EXPECT_TRUE(IsShenzhenExchange("SZ"));
		EXPECT_TRUE(IsShenzhenExchange("600001.SZ"));
		EXPECT_TRUE(IsShenzhenExchange("000001.SZ"));
		EXPECT_TRUE(IsShenzhenExchange("399399.SZ"));
		EXPECT_TRUE(IsShenzhenExchange("abcSZ"));
		EXPECT_FALSE(IsShenzhenExchange("SS"));
	}

	TEST_F(StockCodeConverterTest, TestGetStockExchange) {
		EXPECT_EQ(GetStockExchange("600000.SS"), "SS");
		EXPECT_EQ(GetStockExchange("600000.SZ"), "SZ");
		EXPECT_EQ(GetStockExchange("600SA"), "SA");
		EXPECT_EQ(GetStockExchange("SS"), "SS");
	}

	TEST_F(StockCodeConverterTest, TestGetStockSymbol) {
		EXPECT_EQ(GetStockSymbol("600001.SS"), "600001");
		EXPECT_EQ(GetStockSymbol("600000.SZ"), "600000");
		EXPECT_EQ(GetStockSymbol("600SA"), "60");
		EXPECT_EQ(GetStockSymbol("600.SS"), "600");
	}

	TEST_F(StockCodeConverterTest, TestCreateStockCode) {
		EXPECT_EQ(CreateStockCode("SS", "600000"), "600000.SS");
		EXPECT_EQ(CreateStockCode("SZ", "000001"), "000001.SZ");
		EXPECT_EQ(CreateStockCode("SS", "6000"), "6000.SS");
		EXPECT_EQ(CreateStockCode("SZ", ""), ".SZ");
	}

	TEST_F(StockCodeConverterTest, TestXferSinaToStandard) {
		string str2;
		string str = "sh600001";
		string_view sv = str;
		EXPECT_EQ(XferSinaToStandard(str), "600001.SS");
		EXPECT_EQ(XferSinaToStandard(sv), "600001.SS");
		str = "sz000001";
		sv = str;
		EXPECT_EQ(XferSinaToStandard(str), "000001.SZ");
		EXPECT_EQ(XferSinaToStandard(sv), "000001.SZ");
		str = "sh60001";
		EXPECT_EQ(XferSinaToStandard(str), "h60001.SS");
		str = "sz00001";
		EXPECT_EQ(XferSinaToStandard(str), "z00001.SZ");
		str = "ss600001";
		sv = str;
		EXPECT_THROW(str2 = XferSinaToStandard(str), std::exception);
		EXPECT_THROW(str2 = XferSinaToStandard(sv), std::exception);
	}

	TEST_F(StockCodeConverterTest, TestXferSinaToNetease) {
		EXPECT_EQ(XferSinaToNetease("sh600001"), "0600001");
		EXPECT_EQ(XferSinaToNetease("sz000001"), "1000001");
		EXPECT_EQ(XferSinaToNetease("sh60001"), "0h60001");
		EXPECT_EQ(XferSinaToNetease("sz00001"), "1z00001");
		EXPECT_THROW(string str = XferSinaToNetease("ss600001"), exception);
	}

	TEST_F(StockCodeConverterTest, TestXferNeteaseToStandard) {
		string str = "0600001";
		EXPECT_EQ(XferNeteaseToStandard(str), "600001.SS");
		str = "1000001";
		EXPECT_EQ(XferNeteaseToStandard(str), "000001.SZ");
		str = "060001";
		EXPECT_THROW(string str1 = XferNeteaseToStandard(str), exception);
		str = "100001";
		EXPECT_THROW(string str1 = XferNeteaseToStandard(str), exception);
		str = "200001";
		EXPECT_THROW(string str1 = XferNeteaseToStandard(str), exception);
	}

	TEST_F(StockCodeConverterTest, TestXferNeteaseToStandard2) {
		string str = "0600001";
		EXPECT_EQ(XferNeteaseToStandard(string_view(str)), "600001.SS");
		str = "1000001";
		EXPECT_EQ(XferNeteaseToStandard(string_view(str)), "000001.SZ");
		str = "060001";
		EXPECT_THROW(string str1 = XferNeteaseToStandard(str), exception);
		str = "100001";
		EXPECT_THROW(string str1 = XferNeteaseToStandard(str), exception);
		str = "200001";
		EXPECT_THROW(string str1 = XferNeteaseToStandard(str), exception);
	}

	TEST_F(StockCodeConverterTest, TestXferNeteaseToSina) {
		EXPECT_EQ(XferNeteaseToSina("0600001"), "sh600001");
		EXPECT_EQ(XferNeteaseToSina("1000001"), "sz000001");
		EXPECT_EQ(XferNeteaseToSina("060001"), "sh060001");
		EXPECT_EQ(XferNeteaseToSina("100001"), "sz100001");
		EXPECT_THROW(string str = XferNeteaseToSina("300001"), exception);
	}

	TEST_F(StockCodeConverterTest, TestXferStandardToSina) {
		EXPECT_EQ(XferStandardToSina("600001.SS"), "sh600001");
		EXPECT_EQ(XferStandardToSina("000001.SZ"), "sz000001");
		EXPECT_EQ(XferStandardToSina("60001.SS"), "sh60001.");
		EXPECT_EQ(XferStandardToSina("00001.SZ"), "sz00001.");
		EXPECT_THROW(string str = XferStandardToSina("000001.SA"), exception);
		EXPECT_THROW(string str = XferStandardToSina("000001.AS"), exception);
	}

	TEST_F(StockCodeConverterTest, TestXferStandardToNetease) {
		EXPECT_EQ(XferStandardToNetease("600001.SS"), "0600001");
		EXPECT_EQ(XferStandardToNetease("000001.SZ"), "1000001");
		EXPECT_EQ(XferStandardToNetease("60001.SS"), "060001.");
		EXPECT_EQ(XferStandardToNetease("00001.SZ"), "100001.");
		EXPECT_THROW(string str = XferStandardToNetease("000001.SA"), exception);
		EXPECT_THROW(string str = XferStandardToNetease("000001.AS"), exception);
	}

	TEST_F(StockCodeConverterTest, TestXferStandardToTengxun) {
		EXPECT_EQ(XferStandardToTengxun("600001.SS"), "sh600001");
		EXPECT_EQ(XferStandardToTengxun("000001.SZ"), "sz000001");
		EXPECT_EQ(XferStandardToTengxun("60001.SS"), "sh60001.");
		EXPECT_EQ(XferStandardToTengxun("00001.SZ"), "sz00001.");
		EXPECT_THROW(string str = XferStandardToTengxun("000001.SA"), exception);
		EXPECT_THROW(string str = XferStandardToTengxun("000001.AS"), exception);
	}
}
