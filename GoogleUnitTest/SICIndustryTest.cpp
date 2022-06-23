#include"pch.h"

#include"GeneralCheck.h"

#include"SICIndustry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CSICIndustryTest : public ::testing::Test
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

	TEST_F(CSICIndustryTest, TestInitialize) {
		CSICIndustry SICIndustry;
		EXPECT_EQ(SICIndustry.m_lCode, 0);
		EXPECT_STREQ(SICIndustry.m_strIndustry, _T(" "));
		EXPECT_STREQ(SICIndustry.m_strSector, _T(" "));
		EXPECT_FALSE(SICIndustry.m_fUpdated);
	}

	TEST_F(CSICIndustryTest, TestIsUpdated) {
		CSICIndustry finnhubExchange;

		EXPECT_FALSE(finnhubExchange.IsUpdated());
		finnhubExchange.SetUpdated(true);
		EXPECT_TRUE(finnhubExchange.IsUpdated());
	}

	TEST_F(CSICIndustryTest, TestAppend) {
		CSetSICIndustry setSICIndustry, setSICIndustry2;
		CSICIndustry SICIndustry, SICIndustry2;

		SICIndustry.m_lCode = 1234;
		SICIndustry.m_strIndustry = _T("aaa");
		SICIndustry.m_strSector = _T("abdc");
		SICIndustry.m_fUpdated = true;

		ASSERT(!gl_systemStatus.IsNormalMode());
		setSICIndustry.Open();
		setSICIndustry.m_pDatabase->BeginTrans();
		SICIndustry.Append(setSICIndustry);
		setSICIndustry.m_pDatabase->CommitTrans();
		setSICIndustry.Close();

		setSICIndustry2.m_strFilter = _T("[Code] = 1234");
		setSICIndustry2.Open();
		setSICIndustry2.m_pDatabase->BeginTrans();
		EXPECT_TRUE(!setSICIndustry2.IsEOF()) << "此时已经存入了AA";
		SICIndustry2.Load(setSICIndustry2);
		EXPECT_EQ(SICIndustry.m_lCode, 1234);
		EXPECT_STREQ(SICIndustry.m_strIndustry, _T("aaa"));
		EXPECT_STREQ(SICIndustry.m_strSector, _T("abdc"));
		EXPECT_TRUE(SICIndustry.m_fUpdated);
		setSICIndustry2.Delete();
		setSICIndustry2.m_pDatabase->CommitTrans();
		setSICIndustry2.Close();
	}
}