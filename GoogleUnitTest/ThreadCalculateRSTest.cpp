#include"pch.h"

#include"globedef.h"
#include"MockChinaStock.h"

#include"thread.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CThreadCalculateRSTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			gl_pChinaMarket->SetCurrentStock(_T("600000.SS"));
		}

		static void TearDownTestSuite(void) {
			gl_pChinaMarket->ResetCurrentStock();
			gl_pChinaMarket->SetCurrentStockChanged(false);
			EXPECT_FALSE(gl_fExitingSystem);
		}

		virtual void SetUp(void) override {
			pMockStock = make_shared<CMockChinaStock>();
		}

		virtual void TearDown(void) override {
			pMockStock = nullptr;
			vStock.resize(0);
			gl_fExitingSystem = false;
		}
		CMockChinaStockPtr pMockStock;
		vector<CChinaStockPtr> vStock;
		CRSReference ref;
	};

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrongStock) {
		gl_fExitingCalculatingRS = false;
		gl_fExitingSystem = false;
		pMockStock->SetSymbol(_T("600602.SS"));
		pMockStock->SetActive(true);

		EXPECT_CALL(*pMockStock, Calculate10RSStrongStockSet(&ref))
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_EQ(ThreadCalculate10RSStrongStock(&vStock, &ref, pMockStock), (UINT)104);
		EXPECT_EQ(vStock.size(), 1);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "与当前所选股票不一致时，则卸载已加载的日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrong1Stock) {
		gl_fExitingCalculatingRS = false;
		gl_fExitingSystem = false;
		pMockStock->SetSymbol(_T("600601.SS"));
		pMockStock->SetActive(true);
		EXPECT_CALL(*pMockStock, Calculate10RSStrong1StockSet())
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_EQ(ThreadCalculate10RSStrong1Stock(&vStock, pMockStock), (UINT)105);
		EXPECT_EQ(vStock.size(), 1);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "与当前所选股票不一致时，则卸载已加载的日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrong2Stock1) {
		gl_fExitingCalculatingRS = false;
		gl_fExitingSystem = false;
		pMockStock->SetSymbol(_T("600666.SS"));
		pMockStock->SetActive(true);
		EXPECT_CALL(*pMockStock, Calculate10RSStrong2StockSet())
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_EQ(ThreadCalculate10RSStrong2Stock(&vStock, pMockStock), (UINT)106);
		EXPECT_EQ(vStock.size(), 1);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "与当前所选股票不一致时，则卸载已加载的日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrong2Stock2) {
		gl_fExitingCalculatingRS = false;
		gl_fExitingSystem = false;
		pMockStock->SetSymbol(_T("600000.SS")); // 与默认当前所选股票相同
		pMockStock->SetOffset(gl_pChinaMarket->GetStock(_T("600000.SS"))->GetOffset()); // 设置此变量以防止函数报错。
		pMockStock->SetActive(true);
		EXPECT_CALL(*pMockStock, Calculate10RSStrong2StockSet())
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_EQ(ThreadCalculate10RSStrong2Stock(&vStock, pMockStock), (UINT)106);
		EXPECT_EQ(vStock.size(), 1);
		EXPECT_TRUE(pMockStock->IsDayLineLoaded()) << "与当前所选股票一致时，则不卸载已加载的日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrong2Stock3) {
		gl_fExitingCalculatingRS = false;
		gl_fExitingSystem = true;
		pMockStock->SetSymbol(_T("600000.SS")); // 与默认当前所选股票相同
		pMockStock->SetActive(true);
		EXPECT_CALL(*pMockStock, Calculate10RSStrong2StockSet())
			.Times(0);
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
			.Times(0);
		EXPECT_EQ(ThreadCalculate10RSStrong2Stock(&vStock, pMockStock), (UINT)106);
		EXPECT_EQ(vStock.size(), 0);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "没有加载日线数据";
	}

	TEST_F(CThreadCalculateRSTest, TestThreadCalculate10RSStrong2Stock4) {
		gl_fExitingCalculatingRS = false;
		gl_fExitingSystem = false;
		pMockStock->SetSymbol(_T("600000.SS")); // 与默认当前所选股票相同
		pMockStock->SetActive(false);
		EXPECT_CALL(*pMockStock, Calculate10RSStrong2StockSet())
			.Times(0);
		EXPECT_CALL(*pMockStock, LoadDayLine(pMockStock->GetSymbol()))
			.Times(0);
		EXPECT_EQ(ThreadCalculate10RSStrong2Stock(&vStock, pMockStock), (UINT)106);
		EXPECT_EQ(vStock.size(), 0);
		EXPECT_FALSE(pMockStock->IsDayLineLoaded()) << "没有加载日线数据";
	}
}