#include"pch.h"

#include"GeneralCheck.h"

#include"DataChosenForex.h"

using namespace testing;

namespace FireBirdTest {
	class CDataChosenForexTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override { }

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CDataChosenForex m_dataChosenForex;
	};

	TEST_F(CDataChosenForexTest, TestInitialize) {
		EXPECT_EQ(m_dataChosenForex.GetSize(), 0);
	}

	TEST_F(CDataChosenForexTest, TestLoad) {
		EXPECT_EQ(m_dataChosenForex.GetSize(), 0) << "��ʼδװ��Forex����";

		m_dataChosenForex.LoadDB();
		EXPECT_EQ(m_dataChosenForex.GetSize(), 3) << "Ĭ��״̬��װ��3������";

		CForexSymbolPtr pForex = m_dataChosenForex.GetForex(2);
		EXPECT_STREQ(pForex->GetSymbol(), _T("OANDA:AUD_SGD")) << "װ��ʱû������ʹ�õ���ԭʼλ��";
	}
}
