#include"pch.h"

#include"GeneralCheck.h"

#include"EPSSurprise.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CEPSSurpriseTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) { // 本测试类的初始化函数
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
		}
	};

	TEST_F(CEPSSurpriseTest, TestInitialize) {
		CEPSSurprise EPSSurprise;
		EXPECT_STREQ(EPSSurprise.m_strSymbol, _T(" "));
		EXPECT_EQ(EPSSurprise.m_lDate, 0);
		EXPECT_DOUBLE_EQ(EPSSurprise.m_dActual, 0.0);
		EXPECT_DOUBLE_EQ(EPSSurprise.m_dEstimate, 0.0);
	}

	TEST_F(CEPSSurpriseTest, TestAppend) {
		CSetEPSSurprise setEPSSurprise, setEPSSurprise2;
		CEPSSurprise EPSSurprise, EPSSurprise2;

		EPSSurprise.m_strSymbol = _T("AAAAA");
		EPSSurprise.m_dActual = 1.2;
		EPSSurprise.m_dEstimate = 1.0;
		EPSSurprise.m_lDate = 20202020;

		ASSERT(!gl_systemStatus.IsWorkingMode());
		setEPSSurprise.Open();
		setEPSSurprise.m_pDatabase->BeginTrans();
		EPSSurprise.Append(setEPSSurprise);
		setEPSSurprise.m_pDatabase->CommitTrans();
		setEPSSurprise.Close();

		setEPSSurprise2.m_strFilter = _T("[Symbol] = 'AAAAA'");
		setEPSSurprise2.Open();
		EXPECT_TRUE(!setEPSSurprise2.IsEOF()) << "此时已经存入了ABCDEF";
		EXPECT_DOUBLE_EQ(setEPSSurprise2.m_Actual, 1.2);
		EPSSurprise2.Load(setEPSSurprise2);
		EXPECT_STREQ(EPSSurprise2.m_strSymbol, _T("AAAAA"));
		EXPECT_DOUBLE_EQ(EPSSurprise2.m_dActual, 1.2);
		EXPECT_DOUBLE_EQ(EPSSurprise2.m_dEstimate, 1.0);
		EXPECT_EQ(EPSSurprise2.m_lDate, 20202020);
		setEPSSurprise2.m_pDatabase->BeginTrans();
		while (!setEPSSurprise2.IsEOF()) {
			setEPSSurprise2.Delete();
			setEPSSurprise2.MoveNext();
		}
		setEPSSurprise2.m_pDatabase->CommitTrans();
		setEPSSurprise2.Close();
	}
}