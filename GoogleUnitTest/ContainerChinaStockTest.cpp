#include"pch.h"

#include"GeneralCheck.h"
#include"ChinaMarket.h"

#include"ContainerChinaStock.h"

using namespace testing;

namespace FireBirdTest {
	class CDataChinaStockTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			CChinaStockPtr pStock = nullptr;

			for (int i = 0; i < 100; i++) {
				pStock = gl_pChinaMarket->GetStock(i);
				m_dataChinaStock.Add(pStock);
			}
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CContainerChinaStock m_dataChinaStock;
	};

	TEST_F(CDataChinaStockTest, TestGetUpDownRate) {
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("10.0"), _T("0.0009")), 0.0) << _T("LastClose小于0.001时返回0");
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("11.0"), _T("10.0")), 0.1);
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("10.5"), _T("10.0")), 0.05);
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("9.0"), _T("10.0")), -0.1);
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("11.11"), _T("10.0")), 0.0) << _T("大于0.11时返回0");
		EXPECT_DOUBLE_EQ(m_dataChinaStock.GetUpDownRate(_T("8.89"), _T("10.0")), 0.0) << _T("小于-0.11时返回0");
	}

	TEST_F(CDataChinaStockTest, TestGetNeteaseDayLineDataInquiringIndex) {
		EXPECT_EQ(m_dataChinaStock.GetNeteaseDayLineDataInquiringIndex(), 0);
		m_dataChinaStock.SetNeteaseDayLineDataInquiringIndex(100);
		EXPECT_EQ(m_dataChinaStock.GetNeteaseDayLineDataInquiringIndex(), 100);
	}

	TEST_F(CDataChinaStockTest, TestGetNeteaseRTDataInquiringIndex) {
		EXPECT_EQ(m_dataChinaStock.GetNeteaseRTDataInquiringIndex(), 0);
		m_dataChinaStock.SetNeteaseRTDataInquiringIndex(100);
		EXPECT_EQ(m_dataChinaStock.GetNeteaseRTDataInquiringIndex(), 100);
	}

	TEST_F(CDataChinaStockTest, TestGetSinaRTDataInquiringIndex) {
		EXPECT_EQ(m_dataChinaStock.GetSinaRTDataInquiringIndex(), 0);
		m_dataChinaStock.SetSinaRTDataInquiringIndex(100);
		EXPECT_EQ(m_dataChinaStock.GetSinaRTDataInquiringIndex(), 100);
	}

	TEST_F(CDataChinaStockTest, TestGetTengxunRTDataInquiringIndex) {
		EXPECT_EQ(m_dataChinaStock.GetTengxunRTDataInquiringIndex(), 0);
		m_dataChinaStock.SetTengxunRTDataInquiringIndex(100);
		EXPECT_EQ(m_dataChinaStock.GetTengxunRTDataInquiringIndex(), 100);
	}

	TEST_F(CDataChinaStockTest, TestGetNextIndex) {
		long l = 0;
		EXPECT_EQ(m_dataChinaStock.GetNextIndex(l), 1);
		EXPECT_EQ(l, 0);
		l++;
		EXPECT_EQ(m_dataChinaStock.GetNextIndex(l), 2);
		EXPECT_EQ(l, 1);
		l = 98;
		EXPECT_EQ(m_dataChinaStock.GetNextIndex(l), 99);
		EXPECT_EQ(l, 98);
		l++;
		EXPECT_EQ(m_dataChinaStock.GetNextIndex(l), 0) << "默认的Stock数为100";
		EXPECT_EQ(l, 99);
	}

	TEST_F(CDataChinaStockTest, TestSortStock) {
		const auto pStock = make_shared<CChinaStock>();
		pStock->SetSymbol(_T("0.A")); // 
		m_dataChinaStock.Add(pStock);
		EXPECT_STREQ(m_dataChinaStock.GetStock(m_dataChinaStock.Size() - 1)->GetSymbol(), _T("0.A"));

		m_dataChinaStock.Sort();
		EXPECT_STREQ(m_dataChinaStock.GetStock(0)->GetSymbol(), _T("0.A")) << "0.A位于第一位";

		m_dataChinaStock.Delete(pStock);
	}
}
