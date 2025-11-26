#include"pch.h"

#include"GeneralCheck.h"

#include"Country.h"

namespace FireBirdTest {
	class CCountryTest : public ::testing::Test {
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

	TEST_F(CCountryTest, TestInitialize) {
		const CCountry country;
		EXPECT_EQ(country.m_strCode2.compare(" "), 0);
		EXPECT_EQ(country.m_strCode3.compare(" "), 0);
		EXPECT_EQ(country.m_strCodeNo.compare(" "), 0);
		EXPECT_EQ(country.m_strCountry.compare(" "), 0);
		EXPECT_EQ(country.m_strCurrency.compare(" "), 0);
		EXPECT_EQ(country.m_strCurrencyCode.compare(" "), 0);
	}

	TEST_F(CCountryTest, TestAppend) {
		CSetCountry setCountry, setCountry2;
		CCountry country;

		country.m_strCode2 = "AA";

		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		setCountry.Open();
		setCountry.m_pDatabase->BeginTrans();
		country.Append(setCountry);
		setCountry.m_pDatabase->CommitTrans();
		setCountry.Close();

		setCountry2.m_strFilter = "[Code2] = 'AA'";
		setCountry2.Open();
		setCountry2.m_pDatabase->BeginTrans();
		while (!setCountry2.IsEOF()) {
			EXPECT_TRUE(!setCountry2.IsEOF()) << "此时已经存入了AA";
			setCountry2.Delete();
			setCountry2.MoveNext();
		}
		setCountry2.m_pDatabase->CommitTrans();
		setCountry2.Close();
	}

	TEST_F(CCountryTest, TestLoad) {
		CSetCountry setCountry;
		CCountry country;

		setCountry.Open();
		country.Load(setCountry); // 装入第一个国家的信息：AL, ALB, 8, Albania, Lek, All
		EXPECT_EQ(country.m_strCode2.compare("AL"), 0);
		EXPECT_EQ(country.m_strCode3.compare("ALB"), 0);
		EXPECT_EQ(country.m_strCodeNo.compare("8"), 0);
		EXPECT_EQ(country.m_strCountry.compare("Albania"), 0);
		EXPECT_EQ(country.m_strCurrency.compare("Lek"), 0);
		EXPECT_EQ(country.m_strCurrencyCode.compare("ALL"), 0);
		setCountry.Close();
	}
}
