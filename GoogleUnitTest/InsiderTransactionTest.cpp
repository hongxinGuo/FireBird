#include"pch.h"
#include"globedef.h"

#include"InsiderTransaction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
  class CInsiderTransactionTest : public ::testing::Test
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

  TEST_F(CInsiderTransactionTest, TestInitialize) {
    CInsiderTransaction insiderTransaction;
    EXPECT_STREQ(insiderTransaction.m_strSymbol, _T(" "));
    EXPECT_STREQ(insiderTransaction.m_strPersonName, _T(" "));
    EXPECT_EQ(insiderTransaction.m_lShare, 0);
    EXPECT_EQ(insiderTransaction.m_lChange, 0);
    EXPECT_EQ(insiderTransaction.m_lFilingDate, 19800101);
    EXPECT_EQ(insiderTransaction.m_lTransactionDate, 19800101);
    EXPECT_DOUBLE_EQ(insiderTransaction.m_dTransactionPrice, 0.0);
  }

  TEST_F(CInsiderTransactionTest, TestAppend) {
    CSetInsiderTransaction setInsiderTransaction, setInsiderTransaction2;
    CInsiderTransaction insiderTransaction;

    insiderTransaction.m_strSymbol = _T("AA.US");

    ASSERT(!gl_fNormalMode);
    setInsiderTransaction.Open();
    setInsiderTransaction.m_pDatabase->BeginTrans();
    insiderTransaction.Append(setInsiderTransaction);
    setInsiderTransaction.m_pDatabase->CommitTrans();
    setInsiderTransaction.Close();

    setInsiderTransaction.m_strFilter = _T("[Symbol] = 'AA.US'");
    setInsiderTransaction.Open();
    insiderTransaction.Load(setInsiderTransaction); // װ���һ��Insider Transaction��Ϣ��AA.US
    EXPECT_STREQ(insiderTransaction.m_strSymbol, _T("AA.US"));
    EXPECT_STREQ(insiderTransaction.m_strPersonName, _T(" "));
    EXPECT_EQ(insiderTransaction.m_lShare, 0);
    EXPECT_EQ(insiderTransaction.m_lChange, 0);
    EXPECT_EQ(insiderTransaction.m_lFilingDate, 19800101);
    EXPECT_EQ(insiderTransaction.m_lTransactionDate, 19800101);
    EXPECT_DOUBLE_EQ(insiderTransaction.m_dTransactionPrice, 0.0);
    setInsiderTransaction.Close();

    setInsiderTransaction2.m_strFilter = _T("[Symbol] = 'AA.US'");
    setInsiderTransaction2.Open();
    setInsiderTransaction2.m_pDatabase->BeginTrans();
    while (!setInsiderTransaction2.IsEOF()) {
      EXPECT_TRUE(!setInsiderTransaction2.IsEOF()) << "��ʱ�Ѿ�������AA.US";
      setInsiderTransaction2.Delete();
      setInsiderTransaction2.MoveNext();
    }
    setInsiderTransaction2.m_pDatabase->CommitTrans();
    setInsiderTransaction2.Close();
  }

  TEST_F(CInsiderTransactionTest, TestLoad) {
    CSetInsiderTransaction setInsiderTransaction;
    CInsiderTransaction insiderTransaction;

    setInsiderTransaction.Open();
    insiderTransaction.Load(setInsiderTransaction); // װ���һ���ڲ�������Ϣ��A, a b c, ...
    EXPECT_STREQ(insiderTransaction.m_strSymbol, _T("A"));
    EXPECT_STREQ(insiderTransaction.m_strPersonName, _T("a b c"));
    EXPECT_EQ(insiderTransaction.m_lShare, 0);
    EXPECT_EQ(insiderTransaction.m_lChange, 0);
    EXPECT_EQ(insiderTransaction.m_lFilingDate, 19800101);
    EXPECT_EQ(insiderTransaction.m_lTransactionDate, 20210101);
    EXPECT_DOUBLE_EQ(insiderTransaction.m_dTransactionPrice, 0.0);
    setInsiderTransaction.Close();
  }
}