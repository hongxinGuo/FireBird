#include"pch.h"

#include"GeneralCheck.h"

#include"DataChoicedForex.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CDataChoicedForexTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			// clearu
			GeneralCheck();
		}

	protected:
		CDataChoicedForex m_dataChoicedForex;
	};

	TEST_F(CDataChoicedForexTest, TestInitialize) {
		EXPECT_EQ(m_dataChoicedForex.GetSize(), 0);
	}

	TEST_F(CDataChoicedForexTest, TestLoad) {
		EXPECT_EQ(m_dataChoicedForex.GetSize(), 0) << "��ʼδװ��Forex����";

		m_dataChoicedForex.LoadDB();
		EXPECT_EQ(m_dataChoicedForex.GetSize(), 3) << "Ĭ��״̬��װ��3������";

		CForexSymbolPtr pForex = m_dataChoicedForex.GetForex(2);
		EXPECT_STREQ(pForex->GetSymbol(), _T("OANDA:AUD_SGD")) << "װ��ʱû������ʹ�õ���ԭʼλ��";
	}
}