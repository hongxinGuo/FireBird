#include"pch.h"

////#include"gtest/gtest.h"

#include"GeneralCheck.h"
#include"MockChinaMarket.h"

 

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

		void SetUp() override {}

		void TearDown() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

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

		void SetUp() override {}

		void TearDown() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		CRSReference RSReference;
	};
}
