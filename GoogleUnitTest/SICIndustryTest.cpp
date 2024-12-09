#include"pch.h"

////#include"gtest/gtest.h"

import FireBird.System.Configuration;

#include"GeneralCheck.h"

import FireBird.Type.SICIndustry;

namespace FireBirdTest {
	class CSICIndustryTest : public ::testing::Test {
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

	TEST_F(CSICIndustryTest, TestInitialize) {
		const CSICIndustry SICIndustry;
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

		ASSERT(!gl_systemConfiguration.IsWorkingMode());
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
