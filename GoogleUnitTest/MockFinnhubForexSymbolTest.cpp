///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试FinnhubForexSymbol的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"GeneralCheck.h"

#include"Thread.h"

#include"FinnhubForexSymbol.h"

#include"WebInquirer.h"

#include"MockFinnhubForexSymbol.h"
using namespace std;
using namespace testing;
#include<memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CMockFinnhubForexSymbolTest : public ::testing::Test
	{
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
			// clearup
			GeneralCheck();
		}
	};

	TEST_F(CMockFinnhubForexSymbolTest, TestThreadUpdateFinnhubForexSymbolDayLine) {
		CMockFinnhubForexSymbol symbol;
		vector<CDayLinePtr> vDayLine;
		CDayLinePtr pDayLine = make_shared<CDayLine>();

		pDayLine->SetClose(100);
		vDayLine.push_back(pDayLine);
		symbol.UpdateDayLine(vDayLine);
		symbol.SetSymbol(_T("abcdef"));

		EXPECT_THAT(symbol.GetDayLineSize(), Gt(0));

		EXPECT_CALL(symbol, UpdateDayLineDB()).Times(1);

		EXPECT_EQ(ThreadUpdateForexDayLineDB(&symbol), (UINT)38);

		EXPECT_THAT(symbol.GetDayLineSize(), Eq(0));
		EXPECT_THAT(gl_systemMessage.DayLineInfoSize(), Eq(1));
		EXPECT_STREQ(gl_systemMessage.PopDayLineInfoMessage(), _T("abcdef日线资料存储完成"));
	}
}