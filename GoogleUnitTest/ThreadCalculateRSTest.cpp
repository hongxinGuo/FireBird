#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"
#include"MockChinaStock.h"

#include"thread.h"

using namespace testing;

namespace FireBirdTest {
	class CThreadCalculateRSTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			gl_pChinaMarket->ResetCurrentStock();
			gl_pChinaMarket->SetCurrentStockChanged(false);
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();

			gl_pChinaMarket->SetCurrentStock(_T("600000.SS"));
			pMockStock = make_shared<CMockChinaStock>();

			//恢复原状
			while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardCurrentMarketTask();
		}

		void TearDown(void) override {
			gl_pChinaMarket->SetCurrentStock(nullptr);
			//恢复原状
			while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardCurrentMarketTask();

			pMockStock = nullptr;
			vStock.resize(0);
			gl_systemStatus.SetExitingSystem(false);
			GeneralCheck();
		}

		CMockChinaStockPtr pMockStock;
		vector<CChinaStockPtr> vStock;
		CRSReference ref;
	};

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrongStock1) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(false);
		pMockStock->SetSymbol(_T("600602.SS"));
		pMockStock->SetActive(true);

		EXPECT_CALL(*pMockStock, Calculate10RSStrongStockSet(&ref))
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_EQ(ThreadCalculate10RSStrongStock(&vStock, &ref, pMockStock), static_cast<UINT>(104));
		EXPECT_EQ(vStock.size(), 1);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "与当前所选股票不一致时，则卸载已加载的日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrongStock2) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(false);
		pMockStock->SetSymbol(_T("600000.SS")); // 与默认当前所选股票相同
		pMockStock->SetActive(true);
		EXPECT_CALL(*pMockStock, Calculate10RSStrongStockSet(&ref))
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_EQ(ThreadCalculate10RSStrongStock(&vStock, &ref, pMockStock), static_cast<UINT>(104));
		EXPECT_EQ(vStock.size(), 1);
		EXPECT_TRUE(pMockStock->IsDayLineLoaded()) << "与当前所选股票一致时，则不卸载已加载的日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrongStock3) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(true);
		pMockStock->SetSymbol(_T("600000.SS")); // 与默认当前所选股票相同
		pMockStock->SetActive(true);
		EXPECT_CALL(*pMockStock, Calculate10RSStrongStockSet(&ref))
		.Times(0);
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(0);
		EXPECT_EQ(ThreadCalculate10RSStrongStock(&vStock, &ref, pMockStock), static_cast<UINT>(104));
		EXPECT_EQ(vStock.size(), 0);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "没有加载日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrongStock4) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(false);
		pMockStock->SetSymbol(_T("600000.SS")); // 与默认当前所选股票相同
		pMockStock->SetActive(false);
		EXPECT_CALL(*pMockStock, Calculate10RSStrongStockSet(&ref))
		.Times(0);
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(0);
		EXPECT_EQ(ThreadCalculate10RSStrongStock(&vStock, &ref, pMockStock), static_cast<UINT>(104));
		EXPECT_EQ(vStock.size(), 0);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "没有加载日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrongStock5) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(false);
		pMockStock->SetSymbol(_T("300001.SZ")); // 非A股
		pMockStock->SetActive(true);
		EXPECT_CALL(*pMockStock, Calculate10RSStrongStockSet(&ref))
		.Times(0);
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(0);
		EXPECT_EQ(ThreadCalculate10RSStrongStock(&vStock, &ref, pMockStock), static_cast<UINT>(104));
		EXPECT_EQ(vStock.size(), 0);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "没有加载日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrongStock6) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(false);
		pMockStock->SetSymbol(_T("600001.SS"));
		pMockStock->SetActive(true);
		pMockStock->SetDayLineLoaded(true); // 日线数据已加载
		EXPECT_CALL(*pMockStock, Calculate10RSStrongStockSet(&ref))
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(0);
		EXPECT_EQ(ThreadCalculate10RSStrongStock(&vStock, &ref, pMockStock), static_cast<UINT>(104));
		EXPECT_EQ(vStock.size(), 1) << pMockStock->GetSymbol();
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "日线数据已卸载";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrong1Stock1) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(false);
		pMockStock->SetSymbol(_T("600666.SS"));
		pMockStock->SetActive(true);
		EXPECT_CALL(*pMockStock, Calculate10RSStrong1StockSet())
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_EQ(ThreadCalculate10RSStrong1Stock(&vStock, pMockStock), static_cast<UINT>(105));
		EXPECT_EQ(vStock.size(), 1);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "与当前所选股票不一致时，则卸载已加载的日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrong1Stock2) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(false);
		pMockStock->SetSymbol(_T("600000.SS")); // 与默认当前所选股票相同
		pMockStock->SetActive(true);
		EXPECT_CALL(*pMockStock, Calculate10RSStrong1StockSet())
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_EQ(ThreadCalculate10RSStrong1Stock(&vStock, pMockStock), static_cast<UINT>(105));
		EXPECT_EQ(vStock.size(), 1);
		EXPECT_TRUE(pMockStock->IsDayLineLoaded()) << "与当前所选股票一致时，则不卸载已加载的日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrong1Stock3) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(true);
		pMockStock->SetSymbol(_T("600000.SS")); // 与默认当前所选股票相同
		pMockStock->SetActive(true);
		EXPECT_CALL(*pMockStock, Calculate10RSStrong1StockSet())
		.Times(0);
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(0);
		EXPECT_EQ(ThreadCalculate10RSStrong1Stock(&vStock, pMockStock), static_cast<UINT>(105));
		EXPECT_EQ(vStock.size(), 0);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "没有加载日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrong1Stock4) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(false);
		pMockStock->SetSymbol(_T("600000.SS")); // 与默认当前所选股票相同
		pMockStock->SetActive(false);
		EXPECT_CALL(*pMockStock, Calculate10RSStrong1StockSet())
		.Times(0);
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(0);
		EXPECT_EQ(ThreadCalculate10RSStrong1Stock(&vStock, pMockStock), static_cast<UINT>(105));
		EXPECT_EQ(vStock.size(), 0);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "没有加载日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrong1Stock5) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(false);
		pMockStock->SetSymbol(_T("300001.SZ")); // 非A股
		pMockStock->SetActive(true);
		EXPECT_CALL(*pMockStock, Calculate10RSStrong1StockSet())
		.Times(0);
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(0);
		EXPECT_EQ(ThreadCalculate10RSStrong1Stock(&vStock, pMockStock), static_cast<UINT>(105));
		EXPECT_EQ(vStock.size(), 0);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "没有加载日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrong1Stock6) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(false);
		pMockStock->SetSymbol(_T("600001.SS"));
		pMockStock->SetActive(true);
		pMockStock->SetDayLineLoaded(true); // 日线数据已加载
		EXPECT_CALL(*pMockStock, Calculate10RSStrong1StockSet())
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(0);
		EXPECT_EQ(ThreadCalculate10RSStrong1Stock(&vStock, pMockStock), static_cast<UINT>(105));
		EXPECT_EQ(vStock.size(), 1) << pMockStock->GetSymbol();
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "日线数据已卸载";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrong2Stock1) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(false);
		pMockStock->SetSymbol(_T("600666.SS"));
		pMockStock->SetActive(true);
		EXPECT_CALL(*pMockStock, Calculate10RSStrong2StockSet())
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_EQ(ThreadCalculate10RSStrong2Stock(&vStock, pMockStock), static_cast<UINT>(106));
		EXPECT_EQ(vStock.size(), 1);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "与当前所选股票不一致时，则卸载已加载的日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrong2Stock2) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(false);
		pMockStock->SetSymbol(_T("600000.SS")); // 与默认当前所选股票相同
		pMockStock->SetActive(true);
		EXPECT_CALL(*pMockStock, Calculate10RSStrong2StockSet())
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_EQ(ThreadCalculate10RSStrong2Stock(&vStock, pMockStock), static_cast<UINT>(106));
		EXPECT_EQ(vStock.size(), 1);
		EXPECT_TRUE(pMockStock->IsDayLineLoaded()) << "与当前所选股票一致时，则不卸载已加载的日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrong2Stock3) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(true); // 退出系统标识为真
		pMockStock->SetSymbol(_T("600000.SS")); // 与默认当前所选股票相同
		pMockStock->SetActive(true);
		EXPECT_CALL(*pMockStock, Calculate10RSStrong2StockSet())
		.Times(0);
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(0);
		EXPECT_EQ(ThreadCalculate10RSStrong2Stock(&vStock, pMockStock), static_cast<UINT>(106));
		EXPECT_EQ(vStock.size(), 0);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "没有加载日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrong2Stock4) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(false);
		pMockStock->SetSymbol(_T("600000.SS")); // 与默认当前所选股票相同
		pMockStock->SetActive(false);
		EXPECT_CALL(*pMockStock, Calculate10RSStrong2StockSet())
		.Times(0);
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(0);
		EXPECT_EQ(ThreadCalculate10RSStrong2Stock(&vStock, pMockStock), static_cast<UINT>(106));
		EXPECT_EQ(vStock.size(), 0);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "没有加载日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrong2Stock5) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(false);
		pMockStock->SetSymbol(_T("300001.SZ")); // 非A股
		pMockStock->SetActive(true);
		EXPECT_CALL(*pMockStock, Calculate10RSStrong2StockSet())
		.Times(0);
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(0);
		EXPECT_EQ(ThreadCalculate10RSStrong2Stock(&vStock, pMockStock), static_cast<UINT>(106));
		EXPECT_EQ(vStock.size(), 0);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "没有加载日线数据";

		// 恢复原状
		gl_pChinaMarket->SetCurrentStock(nullptr);
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrong2Stock6) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(false);
		pMockStock->SetSymbol(_T("600001.SS"));
		pMockStock->SetActive(true);
		pMockStock->SetDayLineLoaded(true); // 日线数据已加载
		EXPECT_CALL(*pMockStock, Calculate10RSStrong2StockSet())
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
		.Times(0);
		EXPECT_EQ(ThreadCalculate10RSStrong2Stock(&vStock, pMockStock), static_cast<UINT>(106));
		EXPECT_EQ(vStock.size(), 1);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "日线数据已卸载";
	}
}
