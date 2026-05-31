#include"pch.h"

#include"GeneralCheck.h"

#include"ContainerChinaStock.h"
#include "dataBaseConnector.h"

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
		CContainerChinaStock m_containerChinaStock2;
	};

	TEST_F(CContainerChinaStockTest, TestGetUpDownRate) {
		EXPECT_DOUBLE_EQ(m_containerChinaStock.GetUpDownRate("10.0", "0.0009"), 0.0) << "LastClose小于0.001时返回0";
		EXPECT_DOUBLE_EQ(m_containerChinaStock.GetUpDownRate("11.0", "10.0"), 0.1);
		EXPECT_DOUBLE_EQ(m_containerChinaStock.GetUpDownRate("10.5", "10.0"), 0.05);
		EXPECT_DOUBLE_EQ(m_containerChinaStock.GetUpDownRate("9.0", "10.0"), -0.1);
		EXPECT_DOUBLE_EQ(m_containerChinaStock.GetUpDownRate("11.11", "10.0"), 0.0) << "大于0.11时返回0";
		EXPECT_DOUBLE_EQ(m_containerChinaStock.GetUpDownRate("8.89", "10.0"), 0.0) << "小于-0.11时返回0";
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
		pStock->SetSymbol("0.A"); // 
		m_containerChinaStock.Add(pStock);
		EXPECT_EQ(m_containerChinaStock.GetStock(m_containerChinaStock.Size() - 1)->GetSymbol(), "0.A");

		m_containerChinaStock.Sort();
		EXPECT_EQ(m_containerChinaStock.GetStock(0)->GetSymbol(), "0.A") << "0.A位于第一位";

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

	TEST_F(CContainerChinaStockTest, TestLoadProfileDB) {
		m_containerChinaStock2.Reset();
		using namespace StockMarket;
		const auto& t = ChinaStockProfile{};
		{
			auto db = gl_dbStockMarket.get();
			auto tx = start_transaction(db);

			db(sqlpp::insert_into(t).set(t.Symbol = "000001.SS", t.Exchange = "Test")); // 重复代码，用于测试，此时代码总数是5702
			tx.commit();
		}
		m_containerChinaStock2.LoadProfileDB();
		EXPECT_EQ(m_containerChinaStock2.Size(), 5701); // 2024-06-01中国A股总共有5701只股票（包括退市股票）。如果数据库中有重复的股票代码，则会被删除，所以最终装载的股票数应该是5701。
		{
			auto db = gl_dbStockMarket.get();
			auto tx = start_transaction(db);

			auto result = db(sqlpp::select(all_of(t)).from(t).where(t.Symbol == "000001.SS"));
			tx.commit();
			size_t rows = result.size();
			EXPECT_EQ(rows, 1) << "数据库中应该只有一条000001.SS的记录";
			auto& row = result.front();
			EXPECT_EQ(row.Symbol.value(), "000001.SS");
			EXPECT_EQ(row.ID.value(), 1);
		}
		m_containerChinaStock2.Reset();
	}
}
