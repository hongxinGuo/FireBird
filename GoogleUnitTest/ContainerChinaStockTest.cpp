#include"pch.h"

#include"GeneralCheck.h"
#include"ChinaMarket.h"

#include"ContainerChinaStock.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerChinaStockTest : public Test {
	protected:
		static void SetUpTestSuite() {
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
			CChinaStockPtr pStock = nullptr;
			for (int i = 0; i < 100; i++) {
				pStock = gl_dataContainerChinaStock.GetStock(i);
				m_containerChinaStock.Add(pStock);
			}
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CContainerChinaStock m_containerChinaStock;
	};

	TEST_F(CContainerChinaStockTest, TestGetUpDownRate) {
		EXPECT_DOUBLE_EQ(m_containerChinaStock.GetUpDownRate(_T("10.0"), _T("0.0009")), 0.0) << _T("LastClose小于0.001时返回0");
		EXPECT_DOUBLE_EQ(m_containerChinaStock.GetUpDownRate(_T("11.0"), _T("10.0")), 0.1);
		EXPECT_DOUBLE_EQ(m_containerChinaStock.GetUpDownRate(_T("10.5"), _T("10.0")), 0.05);
		EXPECT_DOUBLE_EQ(m_containerChinaStock.GetUpDownRate(_T("9.0"), _T("10.0")), -0.1);
		EXPECT_DOUBLE_EQ(m_containerChinaStock.GetUpDownRate(_T("11.11"), _T("10.0")), 0.0) << _T("大于0.11时返回0");
		EXPECT_DOUBLE_EQ(m_containerChinaStock.GetUpDownRate(_T("8.89"), _T("10.0")), 0.0) << _T("小于-0.11时返回0");
	}

	TEST_F(CContainerChinaStockTest, TestGetNeteaseRTDataInquiringIndex) {
		EXPECT_EQ(m_containerChinaStock.GetNeteaseRTDataInquiringIndex(), 0);
		m_containerChinaStock.SetNeteaseRTDataInquiringIndex(100);
		EXPECT_EQ(m_containerChinaStock.GetNeteaseRTDataInquiringIndex(), 100);
	}

	TEST_F(CContainerChinaStockTest, TestGetSinaRTDataInquiringIndex) {
		EXPECT_EQ(m_containerChinaStock.GetSinaRTDataInquiringIndex(), 0);
		m_containerChinaStock.SetSinaRTDataInquiringIndex(100);
		EXPECT_EQ(m_containerChinaStock.GetSinaRTDataInquiringIndex(), 100);
	}

	TEST_F(CContainerChinaStockTest, TestGetTengxunRTDataInquiringIndex) {
		EXPECT_EQ(m_containerChinaStock.GetTengxunRTDataInquiringIndex(), 0);
		m_containerChinaStock.SetTengxunRTDataInquiringIndex(100);
		EXPECT_EQ(m_containerChinaStock.GetTengxunRTDataInquiringIndex(), 100);
	}

	TEST_F(CContainerChinaStockTest, TestGetNextIndex) {
		long l = 0;
		EXPECT_EQ(m_containerChinaStock.GetNextIndex(l), 1);
		EXPECT_EQ(l, 0);
		l++;
		EXPECT_EQ(m_containerChinaStock.GetNextIndex(l), 2);
		EXPECT_EQ(l, 1);
		l = 98;
		EXPECT_EQ(m_containerChinaStock.GetNextIndex(l), 99);
		EXPECT_EQ(l, 98);
		l++;
		EXPECT_EQ(m_containerChinaStock.GetNextIndex(l), 0) << "默认的Stock数为100";
		EXPECT_EQ(l, 99);
	}

	TEST_F(CContainerChinaStockTest, TestSortStock) {
		const auto pStock = make_shared<CChinaStock>();
		pStock->SetSymbol(_T("0.A")); // 
		m_containerChinaStock.Add(pStock);
		EXPECT_STREQ(m_containerChinaStock.GetStock(m_containerChinaStock.Size() - 1)->GetSymbol(), _T("0.A"));

		m_containerChinaStock.Sort();
		EXPECT_STREQ(m_containerChinaStock.GetStock(0)->GetSymbol(), _T("0.A")) << "0.A位于第一位";

		m_containerChinaStock.Delete(pStock);
	}

	TEST_F(CContainerChinaStockTest, TestSetUpdateAllDayLine) {
		for (int i = 0; i < m_containerChinaStock.GetLoadedStockSize(); i++) {
			EXPECT_TRUE(m_containerChinaStock.GetStock(i)->IsUpdateDayLine());
		}
		EXPECT_TRUE(m_containerChinaStock.IsUpdateDayLine());
		m_containerChinaStock.ClearDayLineNeedUpdateStatus();

		m_containerChinaStock.SetUpdateDayLine();

		EXPECT_TRUE(m_containerChinaStock.IsUpdateDayLine());

		for (int i = 0; i < m_containerChinaStock.GetLoadedStockSize(); i++) {
			EXPECT_TRUE(m_containerChinaStock.GetStock(i)->IsUpdateDayLine());
		}
	}

	TEST_F(CContainerChinaStockTest, TestSetAllDayLineNeedMaintain) {
		vector<int> vEndDate;
		vEndDate.resize(m_containerChinaStock.Size());
		for (int i = 0; i < m_containerChinaStock.Size(); i++) {
			vEndDate[i] = m_containerChinaStock.GetStock(i)->GetDayLineEndDate();
		}
		EXPECT_TRUE(m_containerChinaStock.IsUpdateDayLine());
		EXPECT_EQ(m_containerChinaStock.GetStock(1)->GetDayLineEndDate(), 20210430);
		m_containerChinaStock.GetStock(1)->SetDayLineEndDate(20200101);
		m_containerChinaStock.ClearDayLineNeedUpdateStatus();

		m_containerChinaStock.SetDayLineNeedMaintain();

		EXPECT_TRUE(m_containerChinaStock.IsUpdateDayLine());
		EXPECT_EQ(m_containerChinaStock.GetStock(1)->GetDayLineEndDate(), 19900101);

		// 恢复原状
		for (int i = 0; i < m_containerChinaStock.Size(); i++) {
			m_containerChinaStock.GetStock(i)->SetDayLineEndDate(vEndDate[i]);
		}
	}
}
