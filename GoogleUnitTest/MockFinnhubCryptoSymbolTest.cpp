///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试FinnhubCryptoSymbol的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"GeneralCheck.h"

#include"Thread.h"

#include"FinnhubCryptoSymbol.h"
#include"MockFinnhubCryptoSymbol.h"
#include"DayLine.h"

using namespace testing;
#include<memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CMockFinnhubCryptoSymbolTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}
	};

	TEST_F(CMockFinnhubCryptoSymbolTest, TestThreadUpdateFinnhubCryptoSymbolDayLine) {
		CMockFinnhubCryptoSymbol symbol;
		vector<CDayLinePtr> vDayLine;
		CDayLinePtr pDayLine = make_shared<CDayLine>();

		pDayLine->SetClose(100);
		vDayLine.push_back(pDayLine);
		symbol.UpdateDayLine(vDayLine);
		symbol.SetSymbol(_T("abcdef"));

		EXPECT_THAT(symbol.GetDayLineSize(), Gt(0));

		EXPECT_CALL(symbol, UpdateDayLineDB()).Times(1);

		EXPECT_EQ(ThreadUpdateCryptoDayLineDB(&symbol), (UINT)53);

		EXPECT_THAT(symbol.GetDayLineSize(), Eq(0));
		EXPECT_THAT(gl_systemMessage.DayLineInfoSize(), Eq(1));
		EXPECT_STREQ(gl_systemMessage.PopDayLineInfoMessage(), _T("abcdef日线资料存储完成"));
	}
}
