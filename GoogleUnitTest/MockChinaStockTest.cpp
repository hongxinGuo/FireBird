#include"pch.h"

#include"Thread.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

#include"MockChinaStock.h"

#include"GeneralCheck.h"

using namespace testing;

namespace FireBirdTest {
	class CMockChinaStockTest : public Test {
	protected:
		static void SetUpTestSuite() {
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			GeneralCheck();
		}

		void SetUp() override {
			GeneralCheck();

			pStock = make_shared<CMockChinaStock>();
			gl_pChinaMarket->CalculateTime();
		}

		void TearDown() override {
			// clearUp
			gl_systemStatus.SetExitingSystem(false);
			gl_pChinaMarket->CalculateTime();
			gl_pChinaMarket->SetUpdateOptionDB(false);

			GeneralCheck();
		}

	protected:
		CMockChinaStockPtr pStock;
		CMockChinaStock stock;
	};

	TEST_F(CMockChinaStockTest, TestShowCurrentTransaction) {
		EXPECT_CALL(*pStock, ReportGuadanTransaction())
		.Times(0);
		pStock->ShowCurrentTransaction();
		EXPECT_EQ(pStock->GetCurrentTransactionVolume(), 0);
		pStock->SetCurrentTransactionVolume(1);
		EXPECT_CALL(*pStock, ReportGuadanTransaction())
		.Times(1);
		pStock->ShowCurrentTransaction();
	}

	TEST_F(CMockChinaStockTest, TestShowCurrentInformationOfCancelingGuadan) {
		EXPECT_CALL(*pStock, ReportGuadan())
		.Times(1);
		pStock->ShowCurrentInformationOfCancelingGuadan();
	}

	TEST_F(CMockChinaStockTest, TestThreadSaveDayLineOfOneStock) {
		EXPECT_CALL(*pStock, SaveDayLineBasicInfo)
		.Times(0);
		pStock->SetDayLineLoaded(true);
		pStock->SetSymbol(_T("601111.SS"));
		gl_systemStatus.SetExitingSystem(true);
		EXPECT_EQ(ThreadSaveDayLineBasicInfo(pStock.get()), static_cast<UINT>(15));
		EXPECT_TRUE(pStock->IsDayLineLoaded());
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 0);

		EXPECT_CALL(*pStock, SaveDayLineBasicInfo)
		.Times(1)
		.WillOnce(Return(false));
		pStock->SetDayLineLoaded(true);
		pStock->SetSymbol(_T("601111.SS"));
		gl_systemStatus.SetExitingSystem(false);
		EXPECT_EQ(ThreadSaveDayLineBasicInfo(pStock.get()), static_cast<UINT>(15));
		EXPECT_FALSE(pStock->IsDayLineLoaded()) << "存储时不涉及卸载日线数据\n";
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 0);

		EXPECT_CALL(*pStock, SaveDayLineBasicInfo)
		.Times(1)
		.WillOnce(Return(true));
		pStock->SetDayLineLoaded(true);
		pStock->SetSymbol(_T("601111.SS"));
		gl_systemStatus.SetExitingSystem(false);
		EXPECT_EQ(ThreadSaveDayLineBasicInfo(pStock.get()), static_cast<UINT>(15));
		EXPECT_FALSE(pStock->IsDayLineLoaded()) << "存储时不涉及卸载日线数据\n";
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 1);
		CString str = gl_systemMessage.PopDayLineInfoMessage();
		EXPECT_STREQ(str, _T("601111.SS日线资料存储完成"));
	}

	TEST_F(CMockChinaStockTest, TestThreadLoadDayLine) {
		auto pDayLine = make_shared<CDayLine>();
		pStock->StoreDayLine(pDayLine);
		InSequence seq;
		EXPECT_CALL(*pStock, LoadDayLine)
		.Times(1);
		pStock->SetDayLineLoaded(false);
		EXPECT_EQ(ThreadLoadDayLine(pStock.get()), static_cast<UINT>(16));
		EXPECT_TRUE(pStock->IsDayLineLoaded());
		EXPECT_EQ(pStock->GetDayLineSize(), 0) << _T("存储日线数据后清空队列\n");
	}

	TEST_F(CMockChinaStockTest, TestThreadLoadWeekLine) {
		auto pWeekLine = make_shared<CWeekLine>();
		pStock->StoreWeekLine(pWeekLine);
		EXPECT_CALL(*pStock, LoadWeekLine)
		.Times(1);
		pStock->SetWeekLineLoaded(false);
		EXPECT_EQ(ThreadLoadWeekLine(pStock.get()), static_cast<UINT>(29));
		EXPECT_TRUE(pStock->IsWeekLineLoaded());
		EXPECT_EQ(pStock->GetWeekLineSize(), 0) << _T("存储周线数据后清空队列\n");
	}

	TEST_F(CMockChinaStockTest, TestThreadBuildWeekLineOfStock) {
		gl_systemStatus.SetExitingSystem(true);
		EXPECT_CALL(*pStock, BuildWeekLine(_)).Times(0);
		EXPECT_EQ(ThreadBuildWeekLineOfStock(pStock.get(), 20200101), 26);

		gl_systemStatus.SetExitingSystem(false);
		EXPECT_CALL(*pStock, BuildWeekLine(20200101)).Times(1);
		EXPECT_EQ(ThreadBuildWeekLineOfStock(pStock.get(), 20200101), 26);
	}
}
