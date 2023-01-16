#include"pch.h"

#include"GeneralCheck.h"

#include"ContainerChosenForex.h"
#include "FinnhubForexSymbol.h"

using namespace testing;

namespace FireBirdTest {
	class CDataChosenForexTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CDataChosenForex m_dataChosenForex;
	};

	TEST_F(CDataChosenForexTest, TestInitialize) {
		EXPECT_EQ(m_dataChosenForex.Size(), 0);
	}

	TEST_F(CDataChosenForexTest, TestLoad) {
		EXPECT_EQ(m_dataChosenForex.Size(), 0) << "��ʼδװ��Forex����";

		m_dataChosenForex.LoadDB();
		EXPECT_EQ(m_dataChosenForex.Size(), 3) << "Ĭ��״̬��װ��3������";

		CForexSymbolPtr pForex = dynamic_pointer_cast<CFinnhubForexSymbol>(m_dataChosenForex.Get(2));
		EXPECT_STREQ(pForex->GetSymbol(), _T("OANDA:AUD_SGD")) << "װ��ʱû������ʹ�õ���ԭʼλ��";
	}
}
