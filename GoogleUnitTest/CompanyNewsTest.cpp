#include"pch.h"

//#include"gtest/gtest.h"

#include"SystemConfiguration.h"

#include"GeneralCheck.h"

#include"FinnhubCompanyNews.h"

namespace FireBirdTest {
	class CCompanyNewsTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
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
	};

	TEST_F(CCompanyNewsTest, TestInitialize) {
		const CFinnhubCompanyNews companyNews;
		EXPECT_EQ(companyNews.m_llDateTime, 0);
		EXPECT_EQ(companyNews.m_iNewsID, 0);

		EXPECT_STREQ(companyNews.m_strCompanySymbol, _T(""));
		EXPECT_STREQ(companyNews.m_strCategory, _T(""));
		EXPECT_STREQ(companyNews.m_strRelatedSymbol, _T(""));
		EXPECT_STREQ(companyNews.m_strHeadLine, _T(""));
		EXPECT_STREQ(companyNews.m_strImage, _T(""));
		EXPECT_STREQ(companyNews.m_strSource, _T(""));
		EXPECT_STREQ(companyNews.m_strSummary, _T(""));
		EXPECT_STREQ(companyNews.m_strURL, _T(""));
	}

	TEST_F(CCompanyNewsTest, TestAppend) {
		CSetCompanyNews setCompanyNews, setCompanyNews2;
		CFinnhubCompanyNews companyNews;

		companyNews.m_strCompanySymbol = _T("AA");

		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		setCompanyNews.Open();
		setCompanyNews.m_pDatabase->BeginTrans();
		companyNews.Append(setCompanyNews);
		setCompanyNews.m_pDatabase->CommitTrans();
		setCompanyNews.Close();

		setCompanyNews2.m_strFilter = _T("[Symbol] = 'AA'");
		setCompanyNews2.Open();
		setCompanyNews2.m_pDatabase->BeginTrans();
		while (!setCompanyNews2.IsEOF()) {
			EXPECT_TRUE(!setCompanyNews2.IsEOF()) << "此时已经存入了AA";
			setCompanyNews2.Delete();
			setCompanyNews2.MoveNext();
		}
		setCompanyNews2.m_pDatabase->CommitTrans();
		setCompanyNews2.Close();
	}

	TEST_F(CCompanyNewsTest, TestLoad) {
		CSetCompanyNews setCompanyNews;
		CFinnhubCompanyNews companyNews;

		setCompanyNews.Open();
		companyNews.Load(setCompanyNews); // 装入第一个国家的信息：AL, ALB, 8, Albania, Lek, All
		EXPECT_EQ(companyNews.m_llDateTime, 10101010);
		EXPECT_EQ(companyNews.m_iNewsID, 1);

		EXPECT_STREQ(companyNews.m_strCompanySymbol, _T("AAPL"));
		EXPECT_STREQ(companyNews.m_strCategory, _T("company news"));
		EXPECT_STREQ(companyNews.m_strRelatedSymbol, _T("AAPL"));
		EXPECT_STREQ(companyNews.m_strHeadLine, _T("Head line"));
		EXPECT_STREQ(companyNews.m_strImage, _T("abc"));
		EXPECT_STREQ(companyNews.m_strSource, _T("cde"));
		EXPECT_STREQ(companyNews.m_strSummary, _T("efg"));
		EXPECT_STREQ(companyNews.m_strURL, _T("https:://"));
		setCompanyNews.Close();
	}
}
