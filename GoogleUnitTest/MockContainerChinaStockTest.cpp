#include"pch.h"

#include"GeneralCheck.h"

#include"MockContainerChinaStock.h"

#include<memory>

#include "TimeConvert.h"
using std::make_shared;

using namespace testing;

namespace FireBirdTest {
	CMockContainerChinaStockPtr s_pMockContainerChinaStock;

	class CMockContainerChinaStockTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) { // 本测试类的初始化函数
			GeneralCheck();
			s_pMockContainerChinaStock = make_shared<CMockContainerChinaStock>();
			s_pMockContainerChinaStock->LoadStockProfileDB();
			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
		}

		static void TearDownTestSuite() {
			s_pMockContainerChinaStock = nullptr;

			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			// clearUp

			GeneralCheck();
		}

	protected:
	};

	TEST_F(CMockContainerChinaStockTest, TestSaveDayLineData) {
		EXPECT_FALSE(s_pMockContainerChinaStock->IsDayLineNeedSaving());

		s_pMockContainerChinaStock->GetStock(0)->SetDayLineNeedSaving(true);
		s_pMockContainerChinaStock->GetStock(10)->SetDayLineNeedSaving(true);
		EXPECT_EQ(s_pMockContainerChinaStock->GetStock(0)->GetDayLineSize(), 0);
		const CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->m_lDate = GetNextDay(s_pMockContainerChinaStock->GetStock(10)->GetDayLineEndDate()); // 确保是新数据
		s_pMockContainerChinaStock->GetStock(10)->StoreDayLine(pDayLine);

		EXPECT_CALL(*s_pMockContainerChinaStock, CreateThreadSaveDayLineBasicInfo).Times(1);

		EXPECT_TRUE(s_pMockContainerChinaStock->SaveDayLineData()) << "存储了一个";

		EXPECT_FALSE(s_pMockContainerChinaStock->GetStock(0)->IsDayLineNeedSaving());
		EXPECT_FALSE(s_pMockContainerChinaStock->GetStock(10)->IsDayLineNeedSaving());
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 1);
		gl_systemMessage.PopDayLineInfoMessage();
	}

	TEST_F(CMockContainerChinaStockTest, TestGetActiveStockSize) {
		for (size_t l = 0; l < s_pMockContainerChinaStock->GetLoadedStockSize(); l++) {
			const auto pStock = s_pMockContainerChinaStock->GetStock(l);
			pStock->SetActive(false);
		}
		EXPECT_EQ(s_pMockContainerChinaStock->GetActiveStockSize(), 0);

		s_pMockContainerChinaStock->GetStock(0)->SetActive(true);
		s_pMockContainerChinaStock->GetStock(10)->SetActive(true);

		EXPECT_EQ(s_pMockContainerChinaStock->GetActiveStockSize(), 2);
	}

	TEST_F(CMockContainerChinaStockTest, TestSetDayLineNeedUpdate) {
		for (size_t l = 0; l < s_pMockContainerChinaStock->GetLoadedStockSize(); l++) {
			const auto pStock = s_pMockContainerChinaStock->GetStock(l);
			pStock->SetDayLineNeedUpdate(false);
		}

		s_pMockContainerChinaStock->SetDayLineNeedUpdate();

		for (size_t l = 0; l < s_pMockContainerChinaStock->GetLoadedStockSize(); l++) {
			const auto pStock = s_pMockContainerChinaStock->GetStock(l);
			EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
		}
	}

	TEST_F(CMockContainerChinaStockTest, TestSetDayLineNeedMaintain) {
		for (size_t l = 0; l < s_pMockContainerChinaStock->GetLoadedStockSize(); l++) {
			const auto pStock = s_pMockContainerChinaStock->GetStock(l);
			pStock->SetDayLineNeedUpdate(false);
			pStock->SetDayLineEndDate(20200101);
		}

		s_pMockContainerChinaStock->SetDayLineNeedMaintain();

		for (size_t l = 0; l < s_pMockContainerChinaStock->GetLoadedStockSize(); l++) {
			const auto pStock = s_pMockContainerChinaStock->GetStock(l);
			EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
			EXPECT_EQ(pStock->GetDayLineEndDate(), 19900101);
		}
	}

	TEST_F(CMockContainerChinaStockTest, TestIsAStock) {
		const auto pStock = make_shared<CChinaStock>();
		pStock->SetSymbol(_T("600000.SS"));
		EXPECT_TRUE(s_pMockContainerChinaStock->IsAStock(pStock));
		pStock->SetSymbol(_T("600000.SA"));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(pStock));
		pStock->SetSymbol(_T("000001.SZ"));
		EXPECT_TRUE(s_pMockContainerChinaStock->IsAStock(pStock));
		pStock->SetSymbol(_T("10001.SZ"));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(pStock));
	}

	TEST_F(CMockContainerChinaStockTest, TestIsAStock2) {
		EXPECT_TRUE(s_pMockContainerChinaStock->IsAStock(_T("600000.SS")));
		EXPECT_TRUE(s_pMockContainerChinaStock->IsAStock(_T("601198.SS")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("602102.SS")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("603604.SS")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("604604.SS")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("605604.SS")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("606604.SS")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("607604.SS")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("608604.SS")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("609604.SS")));

		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("600000.SZ")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("000001.SA")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("000001.AZ")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("200001.SZ")));
		EXPECT_TRUE(s_pMockContainerChinaStock->IsAStock(_T("000001.SZ")));
		EXPECT_TRUE(s_pMockContainerChinaStock->IsAStock(_T("002389.SZ")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("003389.SZ")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("004389.SZ")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("005389.SZ")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("006389.SZ")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("007389.SZ")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("008389.SZ")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("009389.SZ")));
		EXPECT_FALSE(s_pMockContainerChinaStock->IsAStock(_T("001389.SZ")));
	}
}
