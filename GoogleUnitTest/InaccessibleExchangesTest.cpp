#include"pch.h"

#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"GeneralCheck.h"

#include"FinnhubInaccessibleExchange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CInaccessibleExchangesTest : public ::testing::Test {
		virtual void SetUp(void) override {
			GeneralCheck();

			vExchanges.push_back(_T("US"));
			vExchanges.push_back(_T("SS"));
			vExchanges.push_back(_T("SZ"));
			inaccessibleExchange.Assign(_T("COMPANY_NEWS"), 1, vExchanges);
		}
		virtual void TearDown(void) override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}
	protected:
		vector<string> vExchanges;
		CInaccessibleExchanges inaccessibleExchange;
	};

	TEST_F(CInaccessibleExchangesTest, TestAssign) {
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("US"))) << "Ĭ�ϰ���US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SZ"))) << "Ĭ�ϰ���US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SS"))) << "Ĭ�ϰ���US";
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("FA"))) << "Ĭ�ϲ�����FA";
		vector<string> v{ _T("US2"), _T("SZ2"), _T("SS2") };
		inaccessibleExchange.Assign(_T("Another"), 2, v);

		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("US2")));
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SZ2")));
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SS2")));
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("US")));
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("SZ")));
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("SS")));
	}

	TEST_F(CInaccessibleExchangesTest, TestAddExchange) {
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("FA"))) << "Ĭ�ϲ�����FA";
		inaccessibleExchange.AddExchange(_T("FA"));
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("FA"))) << "��Ӻ�Ͱ���FA��";
	}

	TEST_F(CInaccessibleExchangesTest, TestDeleteExchange) {
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("US"))) << "Ĭ�ϰ���US";
		inaccessibleExchange.DeleteExchange(_T("US"));
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("US"))) << "ɾ����Ͳ�����US��";
	}

	TEST_F(CInaccessibleExchangesTest, TestHaveExchange) {
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("US"))) << "Ĭ�ϰ���US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SZ"))) << "Ĭ�ϰ���US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SS"))) << "Ĭ�ϰ���US";
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("FA"))) << "Ĭ�ϲ�����FA";
	}
}

namespace StockAnalysisTest {
	class CFinnhubInaccessibleExchangeTest : public ::testing::Test {
		virtual void SetUp(void) override {
			GeneralCheck();
		}
		virtual void TearDown(void) override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}
	protected:
		CFinnhubInaccessibleExchange inaccessibleExchange;
	};
}