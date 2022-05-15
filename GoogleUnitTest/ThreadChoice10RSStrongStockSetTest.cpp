#include"pch.h"

#include"globedef.h"
#include"SystemMessage.h"

#include"GeneralCheck.h"
#include"MockChinaMarket.h"

#include"thread.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CThreadChoice10RSStrong1StockSet : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			GeneralCheck();
		}

		CMockChinaMarket market;
	};

	TEST_F(CThreadChoice10RSStrong1StockSet, TestThreadChoice10RSStrong1StockSet) {
		EXPECT_CALL(market, Choice10RSStrong1StockSet())
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_EQ(ThreadChoice10RSStrong1StockSet(&market), (UINT)101);
		EXPECT_TRUE(market.IsUpdateOptionDB());
		EXPECT_EQ(market.GetUpdatedDateFor10DaysRS1(), market.GetMarketDate());

		EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), Gt(0));
		while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
	}

	class CThreadChoice10RSStrong2StockSet : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			GeneralCheck();
		}

		CMockChinaMarket market;
	};

	TEST_F(CThreadChoice10RSStrong2StockSet, TestThreadChoice10RSStrong2StockSet) {
		EXPECT_CALL(market, Choice10RSStrong2StockSet())
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_EQ(ThreadChoice10RSStrong2StockSet(&market), (UINT)102);
		EXPECT_TRUE(market.IsUpdateOptionDB());
		EXPECT_EQ(market.GetUpdatedDateFor10DaysRS2(), market.GetMarketDate());

		EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), Gt(0));
		while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
	}

	class CThreadChoice10RSStrongStockSet : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			GeneralCheck();
		}

		CMockChinaMarket market;
		CRSReference RSReference;
	};

	TEST_F(CThreadChoice10RSStrongStockSet, TestThreadChoice10RSStrongStockSet) {
		EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0) << gl_systemMessage.PopInformationMessage();

		EXPECT_CALL(market, Choice10RSStrongStockSet(&RSReference, 2))
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_EQ(ThreadChoice10RSStrongStockSet(&market, &RSReference, 2), (UINT)103);
		EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 2);
		while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
	}
}