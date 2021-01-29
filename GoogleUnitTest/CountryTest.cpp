#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"Country.h"

namespace StockAnalysisTest {
  class CCountryTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) { // ��������ĳ�ʼ������
      ASSERT_FALSE(gl_fNormalMode);
    }

    static void TearDownTestSuite() {
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
    }
  };

  TEST_F(CCountryTest, TestInitialize) {
    CCountry country;
    EXPECT_STREQ(country.m_strCode2, _T(" "));
    EXPECT_STREQ(country.m_strCode3, _T(" "));
    EXPECT_STREQ(country.m_strCodeNo, _T(" "));
    EXPECT_STREQ(country.m_strCountry, _T(" "));
    EXPECT_STREQ(country.m_strCurrency, _T(" "));
    EXPECT_STREQ(country.m_strCurrencyCode, _T(" "));
  }

  TEST_F(CCountryTest, TestAppend) {
    CSetCountry setCountry, setCountry2;
    CCountry country;

    country.m_strCode2 = _T("AA");

    ASSERT(!gl_fNormalMode);
    setCountry.Open();
    setCountry.m_pDatabase->BeginTrans();
    country.Append(setCountry);
    setCountry.m_pDatabase->CommitTrans();
    setCountry.Close();

    setCountry2.m_strFilter = _T("[Code2] = 'AA'");
    setCountry2.Open();
    EXPECT_TRUE(!setCountry2.IsEOF()) << "��ʱ�Ѿ�������AA";
    setCountry2.Delete();
    setCountry2.Close();
  }
}