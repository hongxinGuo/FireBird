#include"pch.h"

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
		EXPECT_EQ(companyNews.m_DateTime.time_since_epoch().count(), 0);
		EXPECT_EQ(companyNews.m_iNewsID, 0);

		EXPECT_EQ(companyNews.m_strCompanySymbol, "");
		EXPECT_EQ(companyNews.m_strCategory, "");
		EXPECT_EQ(companyNews.m_strRelatedSymbol, "");
		EXPECT_EQ(companyNews.m_strHeadLine, "");
		EXPECT_EQ(companyNews.m_strImage, "");
		EXPECT_EQ(companyNews.m_strSource, "");
		EXPECT_EQ(companyNews.m_strSummary, "");
		EXPECT_EQ(companyNews.m_strURL, "");
	}
}
