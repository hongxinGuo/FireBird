#include"pch.h"
#include"globedef.h"

#include"Country.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
  class CCountryTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) { // 本测试类的初始化函数
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
    EXPECT_STREQ(country.m_strCode2, _T("AL"));
    EXPECT_STREQ(country.m_strCode3, _T("ALB"));
    EXPECT_STREQ(country.m_strCodeNo, _T("8"));
    EXPECT_STREQ(country.m_strCountry, _T("Albania"));
    EXPECT_STREQ(country.m_strCurrency, _T("Lek"));
    EXPECT_STREQ(country.m_strCurrencyCode, _T("ALL"));
    setCountry.Close();
  }
}