#include"pch.h"

#include"GeneralCheck.h"

#include"DataChosenStock.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CDataChosenIEXTest : public ::testing::Test {
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
		CDataChosenStock m_dataChosenIEX;
	};

	TEST_F(CDataChosenIEXTest, TestInitialize) {
		EXPECT_EQ(m_dataChosenIEX.GetSize(), 0);
	}

	TEST_F(CDataChosenIEXTest, TestLoad) {
		EXPECT_EQ(m_dataChosenIEX.GetSize(), 0) << "��ʼδװ��IEX����";

		m_dataChosenIEX.LoadDB();
		EXPECT_EQ(m_dataChosenIEX.GetSize(), 4) << "Ĭ��״̬��װ��4������";

		CWorldStockPtr pIEX = m_dataChosenIEX.GetStock(2);
		EXPECT_STREQ(pIEX->GetSymbol(), _T("AAL")) << "װ��ʱû������ʹ�õ���ԭʼλ��";
	}
}