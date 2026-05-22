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

	TEST_F(CContainerChinaStockTest, TestSetAllDayLineNeedMaintain) {
		vector<int> vEndDate;
		vEndDate.resize(m_containerChinaStock.Size());
		for (size_t i = 0; i < m_containerChinaStock.Size(); i++) {
			vEndDate[i] = m_containerChinaStock.GetStock(i)->GetDayLineEndDate();
		}
		EXPECT_TRUE(m_containerChinaStock.IsUpdateDayLine());
		EXPECT_EQ(m_containerChinaStock.GetStock(1)->GetDayLineEndDate(), 20250101);
		m_containerChinaStock.GetStock(1)->SetDayLineEndDate(20200101);
		m_containerChinaStock.ClearDayLineNeedUpdateStatus();

		m_containerChinaStock.SetDayLineNeedMaintain();

		EXPECT_TRUE(m_containerChinaStock.IsUpdateDayLine());
		EXPECT_EQ(m_containerChinaStock.GetStock(1)->GetDayLineEndDate(), 19900101);

		// 恢复原状
		for (auto i = 0; i < m_containerChinaStock.Size(); i++) {
			m_containerChinaStock.GetStock(i)->SetDayLineEndDate(vEndDate[i]);
		}
	}

	TEST_F(CContainerChinaStockTest, TestDeleteDuplicatedStockDB) {
		using namespace StockMarket;
		const auto& t = ChinaStockCode{};
		// Ensure no leftover test symbols
		auto db = GetStockMarketDB();
		db(remove_from(t).where(t.Symbol == std::string("DUPLICATE")));

		// Insert duplicate rows for the same Symbol
		db(insert_into(t).set(t.Symbol = std::string("DUPLICATE"), t.DisplaySymbol = std::string("TEST_DUP1")));
		db(insert_into(t).set(t.Symbol = std::string("DUPLICATE"), t.DisplaySymbol = std::string("TEST_DUP2")));
		db(insert_into(t).set(t.Symbol = std::string("DUPLICATE"), t.DisplaySymbol = std::string("TEST_DUP3")));

		// Ensure inserts are committed and visible to other connections
		db.execute("COMMIT");

		// Verify duplicates were inserted
		db = GetStockMarketDB(); // 执行完插入后，重新获取数据库连接，以确保看到最新的数据
		auto resBefore = db(select(all_of(t)).from(t).where(t.Symbol == std::string("DUPLICATE")));
		EXPECT_TRUE(resBefore.size() >= 3) << resBefore.size() << " rows found, expected at least 3";
		// Call the function under test
		m_containerChinaStock.DeleteDuplicatedStockDB();
		// Verify only one row remains for that symbol
		db = GetStockMarketDB();
		auto resAfter = db(select(all_of(t)).from(t).where(t.Symbol == std::string("DUPLICATE")));
		EXPECT_EQ(resAfter.size(), 1);

		// Clean up test data
		db = GetStockMarketDB(); // 执行完删除重复代码任务后，重新获取数据库连接，以确保看到最新的数据
		auto tx = start_transaction(db);
		db(remove_from(t).where(t.Symbol == std::string("DUPLICATE")));
		tx.commit();
	}
}
