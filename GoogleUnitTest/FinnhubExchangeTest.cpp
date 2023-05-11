#include"pch.h"

#include"GeneralCheck.h"

#include"FinnhubStockExchange.h"

namespace FireBirdTest {
	class CFinnhubExchangeTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
			SCOPED_TRACE(""); GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE(""); GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE(""); GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE(""); GeneralCheck();
		}
	};

	TEST_F(CFinnhubExchangeTest, TestInitialize) {
		const CFinnhubStockExchange FinnhubExchange;
		EXPECT_STREQ(FinnhubExchange.m_strCode, _T(" "));
		EXPECT_STREQ(FinnhubExchange.m_strName, _T(" "));
		EXPECT_STREQ(FinnhubExchange.m_strMic, _T(" "));
		EXPECT_STREQ(FinnhubExchange.m_strTimeZone, _T(" "));
		EXPECT_STREQ(FinnhubExchange.m_strHour, _T(" "));
		EXPECT_STREQ(FinnhubExchange.m_strCloseDate, _T(" "));
		EXPECT_STREQ(FinnhubExchange.m_strCountry, _T(""));
		EXPECT_STREQ(FinnhubExchange.m_strSource, _T(""));
		EXPECT_FALSE(FinnhubExchange.m_fUpdated);
	}

	TEST_F(CFinnhubExchangeTest, TestIsUpdated) {
		CFinnhubStockExchange finnhubExchange;

		EXPECT_FALSE(finnhubExchange.IsUpdated());
		finnhubExchange.SetUpdated(true);
		EXPECT_TRUE(finnhubExchange.IsUpdated());
	}

	TEST_F(CFinnhubExchangeTest, TestAppend) {
		CSetFinnhubStockExchange setFinnhubExchange, setFinnhubExchange2;
		CFinnhubStockExchange FinnhubExchange, FinnhubExchange2;

		FinnhubExchange.m_strCode = _T("AA");
		FinnhubExchange.m_strName = _T("aaa");
		FinnhubExchange.m_strMic = _T("abdc");
		FinnhubExchange.m_strTimeZone = _T("Beijing");
		FinnhubExchange.m_strHour = _T("10101010");
		FinnhubExchange.m_strCloseDate = _T("20202020");
		FinnhubExchange.m_strCountry = _T("dfe");
		FinnhubExchange.m_strSource = _T("abc");
		FinnhubExchange.m_fUpdated = true;

		ASSERT(!gl_systemStatus.IsWorkingMode());
		setFinnhubExchange.Open();
		setFinnhubExchange.m_pDatabase->BeginTrans();
		FinnhubExchange.Append(setFinnhubExchange);
		setFinnhubExchange.m_pDatabase->CommitTrans();
		setFinnhubExchange.Close();

		setFinnhubExchange2.m_strFilter = _T("[Code] = 'AA'");
		setFinnhubExchange2.Open();
		setFinnhubExchange2.m_pDatabase->BeginTrans();
		EXPECT_TRUE(!setFinnhubExchange2.IsEOF()) << "此时已经存入了AA";
		FinnhubExchange2.Load(setFinnhubExchange2);
		EXPECT_STREQ(FinnhubExchange.m_strCode, _T("AA"));
		EXPECT_STREQ(FinnhubExchange.m_strName, _T("aaa"));
		EXPECT_STREQ(FinnhubExchange.m_strMic, _T("abdc"));
		EXPECT_STREQ(FinnhubExchange.m_strTimeZone, _T("Beijing"));
		EXPECT_STREQ(FinnhubExchange.m_strHour, _T("10101010"));
		EXPECT_STREQ(FinnhubExchange.m_strCloseDate, _T("20202020"));
		EXPECT_STREQ(FinnhubExchange.m_strCountry, _T("dfe"));
		EXPECT_STREQ(FinnhubExchange.m_strSource, _T("abc"));
		EXPECT_TRUE(FinnhubExchange.m_fUpdated);
		setFinnhubExchange2.Delete();
		setFinnhubExchange2.m_pDatabase->CommitTrans();
		setFinnhubExchange2.Close();
	}
}
