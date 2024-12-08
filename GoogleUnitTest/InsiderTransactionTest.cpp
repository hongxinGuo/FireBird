#include"pch.h"

////#include"gtest/gtest.h"

#include"SystemConfiguration.h"

#include"GeneralCheck.h"

#include"InsiderTransaction.h"

namespace FireBirdTest {
	class CInsiderTransactionTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // ��������ĳ�ʼ������
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

	TEST_F(CInsiderTransactionTest, TestInitialize) {
		const CInsiderTransaction insiderTransaction;
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
		CInsiderTransaction insiderTransaction, insiderTransaction2;

		insiderTransaction.m_strSymbol = _T("AA.US");

		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		setInsiderTransaction.Open();
		setInsiderTransaction.m_pDatabase->BeginTrans();
		insiderTransaction.Append(setInsiderTransaction);
		setInsiderTransaction.m_pDatabase->CommitTrans();
		setInsiderTransaction.Close();

		insiderTransaction2.m_strSymbol = _T("AA.US2");
		setInsiderTransaction.m_strFilter = _T("[Symbol] = 'AA.US'");
		setInsiderTransaction.Open();
		setInsiderTransaction.m_pDatabase->BeginTrans();
		insiderTransaction.Load(setInsiderTransaction); // װ���һ��Insider Transaction��Ϣ��AA.US
		EXPECT_STREQ(insiderTransaction.m_strSymbol, _T("AA.US"));
		EXPECT_STREQ(insiderTransaction.m_strPersonName, _T(" "));
		EXPECT_EQ(insiderTransaction.m_lShare, 0);
		EXPECT_EQ(insiderTransaction.m_lChange, 0);
		EXPECT_EQ(insiderTransaction.m_lFilingDate, 19800101);
		EXPECT_EQ(insiderTransaction.m_lTransactionDate, 19800101);
		EXPECT_DOUBLE_EQ(insiderTransaction.m_dTransactionPrice, 0.0);

		insiderTransaction2.Update(setInsiderTransaction); // ���´����ݣ���Symbol���Ƹ�ΪAA.US2
		setInsiderTransaction.m_pDatabase->CommitTrans();
		setInsiderTransaction.Close();

		// �ָ�ԭ״
		setInsiderTransaction2.m_strFilter = _T("[Symbol] = 'AA.US2'");
		setInsiderTransaction2.Open();
		setInsiderTransaction2.m_pDatabase->BeginTrans();
		while (!setInsiderTransaction2.IsEOF()) {
			EXPECT_TRUE(!setInsiderTransaction2.IsEOF()) << "��ʱ�Ѿ�������AA.US2";
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
