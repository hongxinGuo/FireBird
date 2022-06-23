#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoIndustry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CTiingoIndustryTest : public ::testing::Test
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

	TEST_F(CTiingoIndustryTest, TestInitialize) {
		CTiingoIndustry TiingoIndustry;
		EXPECT_STREQ(TiingoIndustry.m_strIndustry, _T(" "));
		EXPECT_STREQ(TiingoIndustry.m_strSector, _T(" "));
		EXPECT_FALSE(TiingoIndustry.m_fUpdated);
	}

	TEST_F(CTiingoIndustryTest, TestIsUpdated) {
		CTiingoIndustry finnhubExchange;

		EXPECT_FALSE(finnhubExchange.IsUpdated());
		finnhubExchange.SetUpdated(true);
		EXPECT_TRUE(finnhubExchange.IsUpdated());
	}

	TEST_F(CTiingoIndustryTest, TestAppend) {
		CSetTiingoIndustry setTiingoIndustry, setTiingoIndustry2;
		CTiingoIndustry TiingoIndustry, TiingoIndustry2;

		TiingoIndustry.m_strIndustry = _T("aaa");
		TiingoIndustry.m_strSector = _T("abdc");
		TiingoIndustry.m_fUpdated = true;

		ASSERT(!gl_systemStatus.IsNormalMode());
		setTiingoIndustry.Open();
		setTiingoIndustry.m_pDatabase->BeginTrans();
		TiingoIndustry.Append(setTiingoIndustry);
		setTiingoIndustry.m_pDatabase->CommitTrans();
		setTiingoIndustry.Close();

		setTiingoIndustry2.m_strFilter = _T("[Industry] = 'aaa'");
		setTiingoIndustry2.Open();
		setTiingoIndustry2.m_pDatabase->BeginTrans();
		EXPECT_TRUE(!setTiingoIndustry2.IsEOF()) << "此时已经存入了AA";
		TiingoIndustry2.Load(setTiingoIndustry2);
		EXPECT_STREQ(TiingoIndustry.m_strIndustry, _T("aaa"));
		EXPECT_STREQ(TiingoIndustry.m_strSector, _T("abdc"));
		EXPECT_TRUE(TiingoIndustry.m_fUpdated);
		setTiingoIndustry2.Delete();
		setTiingoIndustry2.m_pDatabase->CommitTrans();
		setTiingoIndustry2.Close();
	}
}