#include"pch.h"

#include"GeneralCheck.h"

#include "FinnhubForex.h"
#include"ContainerChosenForex.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerChosenForexTest : public Test {
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
		CContainerChosenForex m_containerChosenForex;
	};

	TEST_F(CContainerChosenForexTest, TestInitialize) {
		EXPECT_EQ(m_containerChosenForex.Size(), 0);
	}

	TEST_F(CContainerChosenForexTest, TestLoad) {
		EXPECT_EQ(m_containerChosenForex.Size(), 0) << "��ʼδװ��Forex����";

		m_containerChosenForex.LoadDB();
		EXPECT_EQ(m_containerChosenForex.Size(), 3) << "Ĭ��״̬��װ��3������";

		const CForexSymbolPtr pForex = m_containerChosenForex.GetForexSymbol(2);
		EXPECT_STREQ(pForex->GetSymbol(), _T("OANDA:AUD_SGD")) << "װ��ʱû������ʹ�õ���ԭʼλ��";
	}
}
