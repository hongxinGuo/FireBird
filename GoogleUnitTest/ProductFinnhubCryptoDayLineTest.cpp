#include"pch.h"

#include <gtest/gtest.h>

#include "ContainerFinnhubCrypto.h"
#include"GeneralCheck.h"
#include"WorldMarket.h"
#include"ProductFinnhubCryptoDayLine.h"
#include "SystemMessage.h"
#include "FinnhubCrypto.h"
#include"DayLine.h"

#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CProductFinnhubCryptoDayLineTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CProductFinnhubCryptoDayLine cryptoDayLine;
	};

	TEST_F(CProductFinnhubCryptoDayLineTest, TestInitialize) {
		EXPECT_EQ(cryptoDayLine.GetIndex(), 0);
		EXPECT_EQ(cryptoDayLine.GetInquiryFunction(), "https://finnhub.io/api/v1/crypto/candle?symbol=");
	}

	TEST_F(CProductFinnhubCryptoDayLineTest, TestCreatMessage) {
		cryptoDayLine.SetIndex(1);
		EXPECT_EQ(cryptoDayLine.CreateMessage()->front(),
		          (cryptoDayLine.GetInquiryFunction() + gl_dataFinnhubCryptoSymbol.GetItem(1)->GetFinnhubDayLineInquiryParam(GetUTCTime())));
	}

	TEST_F(CProductFinnhubCryptoDayLineTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	namespace {
		// 格式不对(缺开始的‘{’），无法顺利Parser
		Test_FinnhubWebData finnhubWebData221(1, "BINANCE:USDTUAH", R"("c":[1.10159,1.10784],"h":[1.10278,1.10889],"l":[1.09806,1.10023],"o":[1.10051,1.10228],"s":"ok","t":[1574978400,1575237600],"v":[36521,47505]})");
		// 没有s项
		Test_FinnhubWebData finnhubWebData222_1(11, "BINANCE:USDTUAH", R"({"c":[1.10159,1.10784],"h":[1.10278,1.10889],"l":[1.09806,1.10023],"o":[1.10051,1.10228],"a":"ok","t":[1574978400,1575237600],"v":[36521,47505]})");
		// s项报告非ok
		Test_FinnhubWebData finnhubWebData222(2, "BINANCE:USDTUAH", R"({"c":[1.10159,1.10784],"h":[1.10278,1.10889],"l":[1.09806,1.10023],"o":[1.10051,1.10228],"s":"not ok","t":[1574978400,1575237600],"v":[36521,47505]})");
		// s项报告no data
		Test_FinnhubWebData finnhubWebData223(3, "BINANCE:USDTUAH", R"({"c":[1.10159,1.10784],"h":[1.10278,1.10889],"l":[1.09806,1.10023],"o":[1.10051,1.10228],"s":"no_data","a":[1574978400,1575237600],"v":[36521,47505]})");
		// 数据缺乏t项
		Test_FinnhubWebData finnhubWebData224(4, "BINANCE:USDTUAH", R"({"c":[1.10159,1.10784],"h":[1.10278,1.10889],"l":[1.09806,1.10023],"o":[1.10051,1.10228],"s":"ok","a":[1574978400,1575237600],"v":[36521,47505]})");
		// 缺乏c项。不影响结果
		Test_FinnhubWebData finnhubWebData225(5, "BINANCE:USDTUAH", R"({"a":[1.10159,1.10784],"h":[1.10278,1.10889],"l":[1.09806,1.10023],"o":[1.10051,1.10228],"s":"ok","t":[1574978400,1575237600],"v":[36521,47505]})");
		// 缺乏h项
		Test_FinnhubWebData finnhubWebData226(6, "BINANCE:USDTUAH", R"({"c":[1.10159,1.10784],"a":[1.10278,1.10889],"l":[1.09806,1.10023],"o":[1.10051,1.10228],"s":"ok","t":[1574978400,1575237600],"v":[36521,47505]})");
		// 缺乏l项
		Test_FinnhubWebData finnhubWebData227(7, "BINANCE:USDTUAH", R"({"c":[1.10159,1.10784],"h":[1.10278,1.10889],"a":[1.09806,1.10023],"o":[1.10051,1.10228],"s":"ok","t":[1574978400,1575237600],"v":[36521,47505]})");
		// 缺乏o项
		Test_FinnhubWebData finnhubWebData228(8, "BINANCE:USDTUAH", R"({"c":[1.10159,1.10784],"h":[1.10278,1.10889],"l":[1.09806,1.10023],"a":[1.10051,1.10228],"s":"ok","t":[1574978400,1575237600],"v":[36521,47505]})");
		// 缺乏v项
		Test_FinnhubWebData finnhubWebData229(9, "BINANCE:USDTUAH", R"({"c":[1.10159,1.10784],"h":[1.10278,1.10889],"l":[1.09806,1.10023],"o":[1.10051,1.10228],"s":"ok","t":[1574978400,1575237600],"a":[36521,47505]})");
		// 正确的数据
		Test_FinnhubWebData finnhubWebData230(10, "BINANCE:USDTUAH", R"({"c":[1.10159,1.10784],"h":[1.10278,1.10889],"l":[1.09806,1.10023],"o":[1.10051,1.10228],"s":"ok","t":[1574978400,1575237600],"v":[36521,47505]})");
	}

	class ParseFinnhubCryptoCandleTest : public::testing::TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_index = pData->m_index;
			EXPECT_TRUE(gl_dataFinnhubCryptoSymbol.IsSymbol(pData->m_strSymbol));
			m_text = pData->m_data;
			m_finnhubCryptoDayLine.Test_checkAccessRight_(m_text);

			m_pvDayLine = nullptr;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		int m_index;
		CDayLinesPtr m_pvDayLine;
		string m_text;
		CProductFinnhubCryptoDayLine m_finnhubCryptoDayLine;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubCryptoCandle1,
	                         ParseFinnhubCryptoCandleTest,
	                         testing::Values(&finnhubWebData221, &finnhubWebData222_1, &finnhubWebData222, &finnhubWebData223, &finnhubWebData224, &finnhubWebData225,
		                         &finnhubWebData226, &finnhubWebData227, &finnhubWebData228, &finnhubWebData229, &finnhubWebData230));

	TEST_P(ParseFinnhubCryptoCandleTest, TestParseFinnhubCryptoCandle0) {
		string strMessage;

		m_pvDayLine = m_finnhubCryptoDayLine.Parse(m_text);
		switch (m_index) {
		case 1: // 格式不对
			EXPECT_EQ(m_pvDayLine->size(), 0);
			break;
		case 2: // s项报告not ok
			EXPECT_EQ(m_pvDayLine->size(), 0);
			strMessage = "日线返回值不为ok";
			EXPECT_EQ(gl_systemMessage.PopErrorMessage(), strMessage);
			break;
		case 3: // s项报告 no data
			EXPECT_EQ(m_pvDayLine->size(), 0);
			break;
		case 4: //数据缺乏t项
			EXPECT_EQ(m_pvDayLine->size(), 0);
			break;
		case 5:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 6:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 7:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 8:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 9:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 10:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 11: // 没有s项
			EXPECT_EQ(m_pvDayLine->size(), 0);
			break;
		default:
			break;
		}
	}

}
