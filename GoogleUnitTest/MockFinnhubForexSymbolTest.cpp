///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试FinnhubForexSymbol的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"GeneralCheck.h"

#include"Thread.h"
#include"DayLine.h"

#include"FinnhubForexSymbol.h"
#include"MockFinnhubForexSymbol.h"

using namespace testing;

#include<memory>
using std::make_shared;

namespace FireBirdTest {
	class CMockFinnhubForexSymbolTest : public ::testing::Test {
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
	};

	TEST_F(CMockFinnhubForexSymbolTest, TestThreadUpdateFinnhubForexSymbolDayLine) {
		CMockFinnhubForexSymbolPtr pSymbol = make_shared<CMockFinnhubForexSymbol>();
		vector<CDayLinePtr> vDayLine;
		const auto pDayLine = make_shared<CDayLine>();

		pDayLine->SetClose(100);
		vDayLine.push_back(pDayLine);
		pSymbol->UpdateDayLine(vDayLine);
		pSymbol->SetSymbol(_T("abcdef"));

		EXPECT_THAT(pSymbol->GetDayLineSize(), Gt(0));

		EXPECT_CALL(*pSymbol, UpdateDayLineDB()).Times(1);

		EXPECT_EQ(ThreadUpdateForexDayLineDB(pSymbol), 38);

		EXPECT_THAT(pSymbol->GetDayLineSize(), Eq(0));
		EXPECT_THAT(gl_systemMessage.DayLineInfoSize(), Eq(1));
		EXPECT_STREQ(gl_systemMessage.PopDayLineInfoMessage(), _T("abcdef日线资料存储完成"));
	}
}
