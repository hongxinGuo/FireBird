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
		static void SetUpTestSuite() { // ��������ĳ�ʼ������
			SCOPED_TRACE("");
			GeneralCheck();
			s_pMockContainerChinaStock = make_shared<CMockContainerChinaStock>();
			s_pMockContainerChinaStock->LoadStockProfileDB();
			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
		}

		static void TearDownTestSuite() {
			s_pMockContainerChinaStock = nullptr;

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

	protected:
	};

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
}
