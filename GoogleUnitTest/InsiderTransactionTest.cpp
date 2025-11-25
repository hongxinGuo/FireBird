#include"pch.h"

#include"GeneralCheck.h"

#include"InsiderTransaction.h"

namespace FireBirdTest {
	class CInsiderTransactionTest : public ::testing::Test {
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

	TEST_F(CInsiderTransactionTest, TestInitialize) {
		const CInsiderTransaction insiderTransaction;
		EXPECT_EQ(insiderTransaction.m_strSymbol, " ");
		EXPECT_EQ(insiderTransaction.m_strPersonName, " ");
		EXPECT_EQ(insiderTransaction.m_lShare, 0);
		EXPECT_EQ(insiderTransaction.m_lChange, 0);
		EXPECT_EQ(insiderTransaction.m_lFilingDate, 19800101);
		EXPECT_EQ(insiderTransaction.m_lTransactionDate, 19800101);
		EXPECT_DOUBLE_EQ(insiderTransaction.m_dTransactionPrice, 0.0);
	}

	TEST_F(CInsiderTransactionTest, TestAppend) {
		CSetInsiderTransaction setInsiderTransaction, setInsiderTransaction2;
		CInsiderTransaction insiderTransaction, insiderTransaction2;

		insiderTransaction.m_strSymbol = "AA.US";

		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		setInsiderTransaction.Open();
		setInsiderTransaction.m_pDatabase->BeginTrans();
		insiderTransaction.Append(setInsiderTransaction);
		setInsiderTransaction.m_pDatabase->CommitTrans();
		setInsiderTransaction.Close();

		insiderTransaction2.m_strSymbol = "AA.US2";
		setInsiderTransaction.m_strFilter = "[Symbol] = 'AA.US'";
		setInsiderTransaction.Open();
		setInsiderTransaction.m_pDatabase->BeginTrans();
		insiderTransaction.Load(setInsiderTransaction); // 装入第一个Insider Transaction信息：AA.US
		EXPECT_EQ(insiderTransaction.m_strSymbol, "AA.US");
		EXPECT_EQ(insiderTransaction.m_strPersonName, " ");
		EXPECT_EQ(insiderTransaction.m_lShare, 0);
		EXPECT_EQ(insiderTransaction.m_lChange, 0);
		EXPECT_EQ(insiderTransaction.m_lFilingDate, 19800101);
		EXPECT_EQ(insiderTransaction.m_lTransactionDate, 19800101);
		EXPECT_DOUBLE_EQ(insiderTransaction.m_dTransactionPrice, 0.0);

		insiderTransaction2.Update(setInsiderTransaction); // 更新此数据，将Symbol名称改为AA.US2
		setInsiderTransaction.m_pDatabase->CommitTrans();
		setInsiderTransaction.Close();

		// 恢复原状
		setInsiderTransaction2.m_strFilter = "[Symbol] = 'AA.US2'";
		setInsiderTransaction2.Open();
		setInsiderTransaction2.m_pDatabase->BeginTrans();
		while (!setInsiderTransaction2.IsEOF()) {
			EXPECT_TRUE(!setInsiderTransaction2.IsEOF()) << "此时已经存入了AA.US2";
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
		insiderTransaction.Load(setInsiderTransaction); // 装入第一个内部交易信息：A, a b c, ...
		EXPECT_EQ(insiderTransaction.m_strSymbol, "A");
		EXPECT_EQ(insiderTransaction.m_strPersonName, "a b c");
		EXPECT_EQ(insiderTransaction.m_lShare, 0);
		EXPECT_EQ(insiderTransaction.m_lChange, 0);
		EXPECT_EQ(insiderTransaction.m_lFilingDate, 19800101);
		EXPECT_EQ(insiderTransaction.m_lTransactionDate, 20210101);
		EXPECT_DOUBLE_EQ(insiderTransaction.m_dTransactionPrice, 0.0);
		setInsiderTransaction.Close();
	}
}
