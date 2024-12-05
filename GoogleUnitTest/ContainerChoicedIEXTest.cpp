#include"pch.h"

//#include"gtest/gtest.h"

#include"GeneralCheck.h"

#include"FinnhubStock.h"
#include"ContainerChosenStock.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerChosenIEXTest : public Test {
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
		CContainerChosenStock m_dataChosenIEX;
	};

	TEST_F(CContainerChosenIEXTest, TestInitialize) {
		EXPECT_EQ(m_dataChosenIEX.Size(), 0);
	}

	TEST_F(CContainerChosenIEXTest, TestLoad) {
		EXPECT_EQ(m_dataChosenIEX.Size(), 0) << "��ʼδװ��IEX����";

		m_dataChosenIEX.LoadDB();
		EXPECT_EQ(m_dataChosenIEX.Size(), 4) << "Ĭ��״̬��װ��4������";

		const CFinnhubStockPtr pIEX = dynamic_pointer_cast<CFinnhubStock>(m_dataChosenIEX.Get(2));
		EXPECT_STREQ(pIEX->GetSymbol(), _T("AAL")) << "װ��ʱû������ʹ�õ���ԭʼλ��";
	}
}
