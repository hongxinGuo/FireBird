#include"pch.h"

#include"GeneralCheck.h"

#include"ContainerChosenForex.h"
#include "FinnhubForexSymbol.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerChosenForexTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CContainerChosenForex m_containerChosenForex;
	};

	TEST_F(CContainerChosenForexTest, TestInitialize) {
		EXPECT_EQ(m_containerChosenForex.Size(), 0);
	}

	TEST_F(CContainerChosenForexTest, TestLoad) {
		EXPECT_EQ(m_containerChosenForex.Size(), 0) << "��ʼδװ��Forex����";

		m_containerChosenForex.LoadDB();
		EXPECT_EQ(m_containerChosenForex.Size(), 3) << "Ĭ��״̬��װ��3������";

		CForexSymbolPtr pForex = dynamic_pointer_cast<CFinnhubForexSymbol>(m_containerChosenForex.Get(2));
		EXPECT_STREQ(pForex->GetSymbol(), _T("OANDA:AUD_SGD")) << "װ��ʱû������ʹ�õ���ԭʼλ��";
	}
}
