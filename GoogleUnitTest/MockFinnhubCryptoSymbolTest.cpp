///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试FinnhubCryptoSymbol的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"

#include"Thread.h"

#include"FinnhubCryptoSymbol.h"

#include"WebInquirer.h"

#include"MockFinnhubCryptoSymbol.h"
using namespace std;
using namespace testing;
#include<memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CMockFinnhubCryptoSymbolTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			ASSERT_FALSE(gl_fNormalMode);
		}

		static void TearDownTestSuite(void) {
		}

		virtual void SetUp(void) override {
			EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0) << gl_systemMessage.PopInformationMessage();
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 0) << gl_systemMessage.PopDayLineInfoMessage();
		}

		virtual void TearDown(void) override {
			// clearup

			while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
			while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
			while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
		}
	};

	TEST_F(CMockFinnhubCryptoSymbolTest, TestThreadUpdateFinnhubCryptoSymbolDayLine) {
		CMockFinnhubCryptoSymbol symbol;
		vector<CDayLinePtr> vDayLine;
		CDayLinePtr pDayLine = make_shared<CDayLine>();

		vDayLine.push_back(pDayLine);
		symbol.UpdateDayLine(vDayLine);
		symbol.SetSymbol(_T("abcdef"));

		EXPECT_THAT(symbol.GetDayLineSize(), Gt(0));

		EXPECT_CALL(symbol, UpdateDayLineDB()).Times(1);

		EXPECT_EQ(ThreadUpdateCryptoDayLineDB(&symbol), (UINT)53);

		EXPECT_THAT(symbol.GetDayLineSize(), Eq(0));
		EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), Eq(1));
		EXPECT_STREQ(gl_systemMessage.PopDayLineInfoMessage(), _T("abcdef日线资料存储完成"));
	}
}