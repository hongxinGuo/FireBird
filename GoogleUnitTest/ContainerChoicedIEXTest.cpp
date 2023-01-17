#include"pch.h"

#include"GeneralCheck.h"

#include"ContainerChosenStock.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerChosenIEXTest : public Test {
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
		CContainerChosenStock m_dataChosenIEX;
	};

	TEST_F(CContainerChosenIEXTest, TestInitialize) {
		EXPECT_EQ(m_dataChosenIEX.Size(), 0);
	}

	TEST_F(CContainerChosenIEXTest, TestLoad) {
		EXPECT_EQ(m_dataChosenIEX.Size(), 0) << "��ʼδװ��IEX����";

		m_dataChosenIEX.LoadDB();
		EXPECT_EQ(m_dataChosenIEX.Size(), 4) << "Ĭ��״̬��װ��4������";

		CWorldStockPtr pIEX = dynamic_pointer_cast<CWorldStock>(m_dataChosenIEX.Get(2));
		EXPECT_STREQ(pIEX->GetSymbol(), _T("AAL")) << "װ��ʱû������ʹ�õ���ԭʼλ��";
	}
}
