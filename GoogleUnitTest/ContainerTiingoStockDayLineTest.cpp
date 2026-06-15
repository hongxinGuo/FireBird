#include"pch.h"

#include"GeneralCheck.h"

#include"DayLine.h"
#include"ContainerTiingoStockDayLine.h"
#include "dataBaseConnector.h"
#include "ProductTiingoStockDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerTiingoStockDayLineTest : public ::testing::Test {
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
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CContainerTiingoStockDayLine m_dataTiingoStockDayLine;
	};

	TEST_F(CContainerTiingoStockDayLineTest, LoadDB2_LoadsInsertedRow) {
		CTiingoStock stock;
		using namespace StockMarket;
		const auto& t = TiingoStockDayline{};

		// unique symbol to avoid collisions
		const string uniqueSymbol = "UT_TI_" + to_string(std::chrono::system_clock::now().time_since_epoch().count());

		// sample values
		const chrono::local_days date = toLocalDays(20250101);
		const string exchange = "NASDAQ";
		const double lastClose = 123.45;
		const double open = 120.0;
		const double high = 125.0;
		const double low = 119.0;
		const double close = 124.0;
		const double splitFactor = 1.0;
		const double dividend = 0.0;
		const double upAndDown = 1.0;
		const INT64 volume = 1000;
		const INT64 amount = 124000000;
		const double upDownRate = 0.8;
		const double changeHandRate = 0.1;
		const INT64 totalValue = 1000000;
		const INT64 currentValue = 100000;
		const double rs = 50.0;
		const double rsIndex = 1.0;
		const double rsBackup = 0.0;

		// Insert a deterministic row for the test
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		db(insert_into(t).set(
			t.Date = toFormattedDate(date),
			t.Exchange = exchange,
			t.Symbol = uniqueSymbol,
			t.LastClose = lastClose,
			t.Open = open,
			t.High = high,
			t.Low = low,
			t.Close = close,
			t.SplitFactor = splitFactor,
			t.Dividend = dividend,
			t.UpAndDown = upAndDown,
			t.Volume = volume,
			t.Amount = amount,
			t.UpDownRate = upDownRate,
			t.ChangeHandRate = changeHandRate,
			t.TotalValue = totalValue,
			t.CurrentValue = currentValue
		));
		tx.commit();

		// Load via container
		CContainerTiingoStockDayLine container;
		container.LoadDB(uniqueSymbol);
		EXPECT_GT(container.Size(), 0u);

		// verify first row matches inserted data
		auto p = container.GetData(0);
		ASSERT_NE(p, nullptr);
		EXPECT_EQ(p->GetDate(), date);
		EXPECT_EQ(p->GetExchange(), exchange);
		EXPECT_EQ(p->GetStockSymbol(), uniqueSymbol);
		EXPECT_DOUBLE_EQ(static_cast<double>(p->GetOpen()) / stock.GetRatio(), open);
		EXPECT_DOUBLE_EQ(static_cast<double>(p->GetHigh()) / stock.GetRatio(), high);
		EXPECT_DOUBLE_EQ(static_cast<double>(p->GetLow()) / stock.GetRatio(), low);
		EXPECT_DOUBLE_EQ(static_cast<double>(p->GetClose()) / stock.GetRatio(), close);
		EXPECT_DOUBLE_EQ(static_cast<double>(p->GetLastClose()) / stock.GetRatio(), lastClose);
		EXPECT_DOUBLE_EQ(p->GetSplitFactor(), splitFactor);
		EXPECT_DOUBLE_EQ(p->GetDividend(), dividend);
		EXPECT_DOUBLE_EQ(p->GetUpDown(), upAndDown);
		EXPECT_EQ(p->GetVolume(), volume);
		EXPECT_EQ(p->GetAmount(), amount);
		EXPECT_DOUBLE_EQ(p->GetUpDownRate(), upDownRate);
		EXPECT_DOUBLE_EQ(p->GetChangeHandRate(), changeHandRate);
		EXPECT_EQ(p->GetTotalValue(), totalValue);
		EXPECT_EQ(p->GetCurrentValue(), currentValue);

		// Cleanup: delete test rows
		auto db2 = gl_dbStockMarket.get();
		auto tx2 = sqlpp::start_transaction(db2);
		db2(remove_from(t).where(t.Symbol == uniqueSymbol));
		tx2.commit();
	}

	TEST_F(CContainerTiingoStockDayLineTest, TestSaveDB) {
		vector<CDayLine> vDayLine;
		CTiingoCandleLinesPtr dayLinesPtr = make_shared<vector<CTiingoCandleLine>>();

		CTiingoCandleLine dayLine;
		dayLine.SetDate(20241103); // 测试库中没有，插入
		dayLine.SetStockSymbol("A");
		dayLine.SetClose(100);
		dayLinesPtr->push_back(dayLine);
		dayLine.SetDate(20241107); // 测试库中有
		dayLine.SetStockSymbol("A");
		dayLine.SetClose(100);
		dayLinesPtr->push_back(dayLine);
		dayLine.SetDate(20241111); // 测试库中的数据最新日期为20241108，此日期位于其后
		dayLine.SetStockSymbol("A");
		dayLine.SetClose(100);
		dayLinesPtr->push_back(dayLine);
		m_dataTiingoStockDayLine.UpdateData(dayLinesPtr);

		m_dataTiingoStockDayLine.SaveDB("A");

		m_dataTiingoStockDayLine.LoadDB("A");
		EXPECT_EQ(m_dataTiingoStockDayLine.GetData(m_dataTiingoStockDayLine.Size() - 1)->GetDate(),
		          toLocalDays(20241111)) << "新存储数据位于最后";

		// 恢复原状
		using namespace StockMarket;
		const auto& t = TiingoStockDayline{};

		auto db = gl_dbStockMarket.get();
		auto tx = start_transaction(db);

		db(remove_from(t).where(t.Symbol == "A" && t.Date == 20241103));
		db(remove_from(t).where(t.Symbol == "A" && t.Date == 20241111));

		tx.commit();
	}

	TEST_F(CContainerTiingoStockDayLineTest, TestSplitAdjust) {
		m_dataTiingoStockDayLine.LoadDB("AAPL");

		auto data = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(20250102));
		auto data2 = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(20200831)); // 最近一次4拆股
		EXPECT_EQ(data2->GetLastClose(), 499230000 / 4);
		EXPECT_EQ(data2->GetClose(), 129039999);
		data2 = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(20200828)); // 最近一次4拆股前一天
		EXPECT_EQ(data2->GetLastClose(), 500040000 / 4);
		EXPECT_EQ(data2->GetClose(), 499230000/4);
		EXPECT_EQ(data2->GetHigh(), 505770000/4);
		EXPECT_EQ(data2->GetLow(), 498310000/4);
		EXPECT_EQ(data2->GetOpen(), 504050000/4);
		auto data3 = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(20140609)); // 最近一次7拆股
		EXPECT_EQ(data3->GetLastClose(), 645570000/28);
		EXPECT_EQ(data3->GetClose(), 93700000/4);
		EXPECT_EQ(data3->GetHigh(), 23470000);
		auto data4 = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(20050228)); // 2拆股
		EXPECT_EQ(data4->GetLastClose(), 88990000 / 56);
		EXPECT_EQ(data4->GetClose(), 44860000/28);
		EXPECT_EQ(data4->GetLow(), 1570000);
		auto data5 = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(20000621)); // 2拆股
		EXPECT_EQ(data5->GetLastClose(), 101250000 / 112);
		EXPECT_EQ(data5->GetClose(), 55630000/56);
		EXPECT_EQ(data5->GetHigh(), 1016785);
		data2 = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(19870617)); // 拆股后第一天
		EXPECT_EQ(data2->GetLastClose(), 41500000/112);
		EXPECT_EQ(data2->GetClose(), 40500000 / 112);
		EXPECT_EQ(data2->GetHigh(), 42500000 / 112);
		EXPECT_EQ(data2->GetLow(), 40000000 / 112);
		EXPECT_EQ(data2->GetOpen(), 41500000 / 112);
		auto data6 = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(19870616)); // 2拆股
		EXPECT_EQ(data6->GetLastClose(), 78500000 / 224);
		EXPECT_EQ(data6->GetClose(), 41500000 / 112);
		EXPECT_EQ(data6->GetOpen(), 370535);
		data2 = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(19801212)); // 上市第一天
		EXPECT_EQ(data2->GetLastClose(), 0);
		EXPECT_EQ(data2->GetClose(), 28750000 / 224);
		EXPECT_EQ(data2->GetHigh(), 28870000 / 224);
		EXPECT_EQ(data2->GetLow(), 28750000 / 224);
		EXPECT_EQ(data2->GetOpen(), 28750000 / 224);
	}

	TEST_F(CContainerTiingoStockDayLineTest, TestSplitAdjust2) {
		m_dataTiingoStockDayLine.LoadDB("AYRO");

		auto data = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(20250102));
		auto data2 = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(20250626)); // 1/16
		EXPECT_EQ(data2->GetLastClose(), 430000 * 16);
		EXPECT_EQ(data2->GetClose(), 7638600);
		data2 = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(20230918)); // 1/8
		EXPECT_EQ(data2->GetLastClose(), 485400 * 16 * 8);
		EXPECT_EQ(data2->GetClose(), 2930000 * 16);
		EXPECT_EQ(data2->GetHigh(), 3603100 * 16);
		EXPECT_EQ(data2->GetLow(), 2900000 * 16);
		EXPECT_EQ(data2->GetOpen(), 3340000 * 16);
		auto data3 = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(20200529)); // 1/5
		EXPECT_EQ(data3->GetLastClose(), 780000 * 16 * 8 * 5);
		EXPECT_EQ(data3->GetClose(), 3050000 * 16 * 8);
		EXPECT_EQ(data3->GetHigh(), 4099999 * 16 * 8);
		auto data4 = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(20190311)); // 1/6
		EXPECT_EQ(data4->GetLastClose(), 2035159296); // 530000 * 16 * 8 * 5 * 6的近似值
		long long ll = 4050000LL * 16 * 8 * 5;
		EXPECT_EQ(data4->GetClose(), ll);
		ll = 3030000LL * 16 * 8 * 5;
		EXPECT_EQ(data4->GetLow(), ll);
		auto data5 = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(20180131)); // 1/4
		EXPECT_EQ(data5->GetClose(), 13439731205); // 3500000LL * 16 * 8 * 5 * 6
		EXPECT_EQ(data5->GetHigh(), 18393232135); //4790000LL * 16 * 8 * 5 * 6
		EXPECT_EQ(data5->GetLastClose(), 15820483588);// 1030000LL * 16 * 8 * 5 * 6 * 4;
		data2 = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(20180130)); // 上市第一天
		EXPECT_EQ(data2->GetLastClose(), 0);
		EXPECT_EQ(data2->GetClose(), 15820483590); //1030000 * 16 * 5 * 6 * 4
		EXPECT_EQ(data2->GetHigh(), 19660406791); // 1280000 * 16 * 5 * 6 * 4
		EXPECT_EQ(data2->GetLow(), 15359692806); // 1000000 * 16 * 5 * 6 * 4
		EXPECT_EQ(data2->GetOpen(), 16742065158); // 109000LL * 16 * 5 * 6 * 4
	}

	TEST_F(CContainerTiingoStockDayLineTest, TestSplitAdjust3) {
		m_dataTiingoStockDayLine.LoadDB("BEEM");

		auto data = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(20250102));
		auto data2 = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(20190416)); // 1/50
		EXPECT_EQ(data2->GetLastClose(), 165000 * 50);
		EXPECT_EQ(data2->GetClose(), 5440000);
		EXPECT_EQ(data2->GetHigh(), 5500000);
		EXPECT_EQ(data2->GetLow(), 5060000);
		EXPECT_EQ(data2->GetOpen(), 5410000);
		data2 = m_dataTiingoStockDayLine.GetDayLine(toLocalDays(20100430)); // 上市第一天，0.69并股
		EXPECT_EQ(data2->GetLastClose(), 0);
		EXPECT_EQ(data2->GetClose(), 350000 * 50);
		EXPECT_EQ(data2->GetHigh(), 350000 * 50);
		EXPECT_EQ(data2->GetLow(), 250000 * 50);
		EXPECT_EQ(data2->GetOpen(), 260000 * 50);
	}
}
