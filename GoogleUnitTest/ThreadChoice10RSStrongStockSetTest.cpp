#include"pch.h"

#include"GeneralCheck.h"
#include"MockChinaMarket.h"

#include"thread.h"

using namespace testing;

namespace FireBirdTest {
	CMockChinaMarketPtr s_pMarket3;

	class CThreadChoice10RSStrong1StockSet : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();

			s_pMarket3 = make_shared<CMockChinaMarket>();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();

			s_pMarket3 = nullptr;
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CThreadChoice10RSStrong1StockSet, TestThreadChoice10RSStrong1StockSet) {
		EXPECT_CALL(*s_pMarket3, Choice10RSStrong1StockSet())
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_EQ(ThreadChoice10RSStrong1StockSet(s_pMarket3), 101);
		EXPECT_TRUE(s_pMarket3->IsUpdateOptionDB());
		EXPECT_EQ(s_pMarket3->GetUpdatedDateFor10DaysRS1(), s_pMarket3->GetMarketDate());

		EXPECT_THAT(gl_systemMessage.InformationSize(), Gt(0));
		while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
	}

	class CThreadChoice10RSStrong2StockSet : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			s_pMarket3 = make_shared<CMockChinaMarket>();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();

			s_pMarket3 = nullptr;
		}

		void SetUp() override { }

		void TearDown() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CThreadChoice10RSStrong2StockSet, TestThreadChoice10RSStrong2StockSet) {
		EXPECT_CALL(*s_pMarket3, Choice10RSStrong2StockSet())
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_EQ(ThreadChoice10RSStrong2StockSet(s_pMarket3), 102);
		EXPECT_TRUE(s_pMarket3->IsUpdateOptionDB());
		EXPECT_EQ(s_pMarket3->GetUpdatedDateFor10DaysRS2(), s_pMarket3->GetMarketDate());

		EXPECT_THAT(gl_systemMessage.InformationSize(), Gt(0));
		while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
	}

	class CThreadChoice10RSStrongStockSet : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			s_pMarket3 = make_shared<CMockChinaMarket>();
		}

		static void TearDownTestSuite() {
			s_pMarket3 = nullptr;
		}

		void SetUp() override { }

		void TearDown() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		CRSReference RSReference;
	};

	TEST_F(CThreadChoice10RSStrongStockSet, TestThreadChoice10RSStrongStockSet) {
		EXPECT_THAT(gl_systemMessage.InformationSize(), 0) << gl_systemMessage.PopInformationMessage();

		EXPECT_CALL(*s_pMarket3, Choice10RSStrongStockSet(&RSReference, 2))
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_EQ(ThreadChoice10RSStrongStockSet(s_pMarket3, &RSReference, 2), 103);
		EXPECT_THAT(gl_systemMessage.InformationSize(), 2);
		while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
	}
}
