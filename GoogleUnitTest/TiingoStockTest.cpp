#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoStock.h"
#include "dataBaseConnector.h"

namespace FireBirdTest {
	class CTiingoStockTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
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

		CTiingoStock stock;
	};

	TEST_F(CTiingoStockTest, TestIsTiingoStock) {
		CTiingoStockPtr pTiingoStock = make_shared<CTiingoStock>();
		CVirtualStockPtr pStock;
		CChinaStockPtr pChinaStock = make_shared<CChinaStock>();
		CFinnhubStockPtr pFinnhubStock = make_shared<CFinnhubStock>();
		EXPECT_TRUE(IsTiingoStock(pTiingoStock));
		EXPECT_FALSE(IsTiingoStock(pStock));
		EXPECT_FALSE(IsTiingoStock(pChinaStock));
		EXPECT_FALSE(IsTiingoStock(pFinnhubStock));
		EXPECT_FALSE(IsTiingoStock(nullptr));

		pStock = pTiingoStock;
		EXPECT_TRUE(IsTiingoStock(pStock));
		pStock = pChinaStock;
		EXPECT_FALSE(IsTiingoStock(pStock));
	}

	TEST_F(CTiingoStockTest, TestInitialize) {
		CTiingoStock tiingo;

		EXPECT_EQ(tiingo.GetTiingoPermaTicker(), "");
		EXPECT_EQ(tiingo.GetSymbol(), "");
		EXPECT_EQ(tiingo.GetName(), "");
		EXPECT_FALSE(tiingo.IsActive());
		EXPECT_FALSE(tiingo.IsADR());
		EXPECT_EQ(tiingo.GetSicCode(), 0);
		EXPECT_EQ(tiingo.GetSicIndustry(), "");
		EXPECT_EQ(tiingo.GetSicSector(), "");
		EXPECT_EQ(tiingo.GetTiingoIndustry(), "");
		EXPECT_EQ(tiingo.GetTiingoSector(), "");
		EXPECT_EQ(tiingo.GetReportingCurrency(), "");
		EXPECT_EQ(tiingo.GetLocation(), "");
		EXPECT_EQ(tiingo.GetCompanyWebSite(), "");
		EXPECT_EQ(tiingo.GetSECFilingWebSite(), "");
		EXPECT_EQ(stock.GetCompanyFinancialStatementUpdateDate(), 19800101);
		EXPECT_EQ(stock.GetDailyUpdateDate(), 19800101);
		EXPECT_EQ(stock.GetDayLineEndDate(), 19800101);
		EXPECT_EQ(stock.GetDayLineStartDate(), 29900101);
		EXPECT_EQ(stock.GetStatementLastUpdatedDate(), 0);
	}

	TEST_F(CTiingoStockTest, TestResetAllUpdateDate) {
		stock.ResetAllUpdateDate();
		EXPECT_EQ(stock.GetCompanyFinancialStatementUpdateDate(), 19800101);
		EXPECT_EQ(stock.GetDailyUpdateDate(), 19800101);
		EXPECT_EQ(stock.GetDayLineEndDate(), 19800101);
		EXPECT_EQ(stock.GetDayLineStartDate(), 29900101);
		EXPECT_EQ(stock.GetStatementLastUpdatedDate(), 0);
		EXPECT_EQ(stock.GetDayLineProcessDate(), 19800101);
	}

	TEST_F(CTiingoStockTest, TestGetRatio) {
		EXPECT_EQ(stock.GetRatio(), 1000000) << "Tiingo.com的股票价格，放大倍数为1000000";
	}

	TEST_F(CTiingoStockTest, TestGetExchangeCode) {
		EXPECT_EQ(stock.GetExchangeCode(), "US");
		stock.SetExchangeCode("SS");
		EXPECT_EQ(stock.GetExchangeCode(), "SS");
	}

	TEST_F(CTiingoStockTest, TestGetSymbol) {
		EXPECT_EQ(stock.GetSymbol(), "");
		stock.SetSymbol("600000.SS");
		EXPECT_EQ(stock.GetSymbol(), "600000.SS");
	}

	TEST_F(CTiingoStockTest, TestGetTransactionTime) {
		EXPECT_EQ(stock.GetTransactionTime(), 0);
		stock.SetTransactionTime(1010101010);
		EXPECT_EQ(stock.GetTransactionTime(), 1010101010);
	}

	TEST_F(CTiingoStockTest, TestGetLastClose) {
		EXPECT_EQ(stock.GetLastClose(), 0);
		stock.SetLastClose(10101010);
		EXPECT_EQ(stock.GetLastClose(), 10101010);
	}

	TEST_F(CTiingoStockTest, TestGetOpen) {
		EXPECT_EQ(stock.GetOpen(), 0);
		stock.SetOpen(10101);
		EXPECT_EQ(stock.GetOpen(), 10101);
	}

	TEST_F(CTiingoStockTest, TestGetHigh) {
		EXPECT_EQ(stock.GetHigh(), 0);
		stock.SetHigh(19980101);
		EXPECT_EQ(stock.GetHigh(), 19980101);
	}

	TEST_F(CTiingoStockTest, TestGetLow) {
		EXPECT_EQ(stock.GetLow(), 0);
		stock.SetLow(19980101);
		EXPECT_EQ(stock.GetLow(), 19980101);
	}

	TEST_F(CTiingoStockTest, TestGetNew) {
		EXPECT_EQ(stock.GetNew(), 0);
		stock.SetNew(10101010);
		EXPECT_EQ(stock.GetNew(), 10101010);
	}

	TEST_F(CTiingoStockTest, TestGetUpDown) {
		EXPECT_EQ(stock.GetUpDown(), 0);
		stock.SetUpDown(10101010);
		EXPECT_EQ(stock.GetUpDown(), 10101010);
	}

	TEST_F(CTiingoStockTest, TestGetAmount) {
		EXPECT_EQ(stock.GetAmount(), 0);
		stock.SetAmount(1010101010101010);
		EXPECT_EQ(stock.GetAmount(), 1010101010101010);
	}

	TEST_F(CTiingoStockTest, TestGetVolume) {
		EXPECT_EQ(stock.GetVolume(), 0);
		stock.SetVolume(10101010);
		EXPECT_EQ(stock.GetVolume(), 10101010);
	}

	TEST_F(CTiingoStockTest, TestGetTotalValue) {
		EXPECT_EQ(stock.GetTotalValue(), 0);
		stock.SetTotalValue(10101010);
		EXPECT_EQ(stock.GetTotalValue(), 10101010);
	}

	TEST_F(CTiingoStockTest, TestGetCurrentValue) {
		EXPECT_EQ(stock.GetCurrentValue(), 0);
		stock.SetCurrentValue(10101010);
		EXPECT_EQ(stock.GetCurrentValue(), 10101010);
	}

	TEST_F(CTiingoStockTest, TestGetChangeHandRate) {
		EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 0.0);
		stock.SetChangeHandRate(1.1);
		EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 1.1);
	}

	TEST_F(CTiingoStockTest, TestGetUpDownRate) {
		EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 0.0);
		stock.SetUpDownRate(1.1);
		EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 1.1);
	}

	TEST_F(CTiingoStockTest, TestIsUpdateFinancialState) {
		EXPECT_TRUE(stock.IsUpdateFinancialState());

		stock.SetUpdateFinancialState(false);
		EXPECT_FALSE(stock.IsUpdateFinancialState());
	}

	TEST_F(CTiingoStockTest, TestIsUpdateFinancialStateDB) {
		EXPECT_FALSE(stock.IsUpdateFinancialStateDB());

		stock.SetUpdateFinancialStateDB(true);
		EXPECT_TRUE(stock.IsUpdateFinancialStateDB());
	}

	TEST_F(CTiingoStockTest, TestIsTodayNewStock) {
		EXPECT_FALSE(stock.IsTodayNewStock());
		stock.SetTodayNewStock(true);
		EXPECT_TRUE(stock.IsTodayNewStock());
		stock.SetTodayNewStock(false);
		EXPECT_FALSE(stock.IsTodayNewStock());
	}

	TEST_F(CTiingoStockTest, TestIsUpdateStockProfileDB) {
		EXPECT_FALSE(stock.IsUpdateProfileDB());
		stock.SetUpdateProfileDB(true);
		EXPECT_TRUE(stock.IsUpdateProfileDB());
		stock.SetUpdateProfileDB(false);
		EXPECT_FALSE(stock.IsUpdateProfileDB());
	}

	TEST_F(CTiingoStockTest, TestIsUpdateCompanyNewsDB) {
		EXPECT_FALSE(stock.IsUpdateCompanyNewsDB());
		stock.SetUpdateCompanyNewsDB(true);
		EXPECT_TRUE(stock.IsUpdateCompanyNewsDB());
		stock.SetUpdateCompanyNewsDB(false);
		EXPECT_FALSE(stock.IsUpdateCompanyNewsDB());
	}

	TEST_F(CTiingoStockTest, TestIsActive) {
		EXPECT_FALSE(stock.IsActive());
		stock.SetActive(true);
		EXPECT_TRUE(stock.IsActive());
		stock.SetActive(false);
		EXPECT_FALSE(stock.IsActive());
	}

	TEST_F(CTiingoStockTest, TestGetDayLineEndDate) {
		EXPECT_EQ(stock.GetDayLineEndDate(), 19800101);
		stock.SetDayLineEndDate(19980101);
		EXPECT_EQ(stock.GetDayLineEndDate(), 19980101);
	}

	TEST_F(CTiingoStockTest, TestGetDayLineStartDate) {
		EXPECT_EQ(stock.GetDayLineStartDate(), 29900101);
		stock.SetDayLineStartDate(19980101);
		EXPECT_EQ(stock.GetDayLineStartDate(), 19980101);
	}

	TEST_F(CTiingoStockTest, TestGetStatementLastUpdatedDate) {
		EXPECT_EQ(stock.GetStatementLastUpdatedDate(), 0);
		stock.SetStatementLastUpdatedDate(19990101);
		EXPECT_EQ(stock.GetStatementLastUpdatedDate(), 19990101);
	}

	TEST_F(CTiingoStockTest, TestGetCompanyFinancialStatementUpdateDate) {
		EXPECT_EQ(stock.GetCompanyFinancialStatementUpdateDate(), 19800101);
		stock.SetCompanyFinancialStatementUpdateDate(19930101);
		EXPECT_EQ(stock.GetCompanyFinancialStatementUpdateDate(), 19930101);
	}

	TEST_F(CTiingoStockTest, TestGetDayLineProcessDate) {
		EXPECT_EQ(stock.GetDayLineProcessDate(), 19800101);
		stock.SetDayLineProcessDate(19930101);
		EXPECT_EQ(stock.GetDayLineProcessDate(), 19930101);
	}

	TEST_F(CTiingoStockTest, TestGetIPOStatus) {
		EXPECT_TRUE(stock.IsNotChecked());
		stock.SetIPOStatus(255);
		EXPECT_EQ(stock.GetIPOStatus(), 255);
	}

	TEST_F(CTiingoStockTest, TestIsNullStock) {
		stock.SetIPOStatus(_STOCK_NULL_);
		EXPECT_TRUE(stock.IsNullStock());
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_FALSE(stock.IsNullStock());
	}

	TEST_F(CTiingoStockTest, TestIsIPOed) {
		stock.SetIPOStatus(_STOCK_IPOED_);
		EXPECT_TRUE(stock.IsIPOed());
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_FALSE(stock.IsIPOed());
	}

	TEST_F(CTiingoStockTest, TestIsNotChecked) {
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_TRUE(stock.IsNotChecked());
		stock.SetIPOStatus(_STOCK_DELISTED_);
		EXPECT_FALSE(stock.IsNotChecked());
	}

	TEST_F(CTiingoStockTest, TestIsDelisted) {
		stock.SetIPOStatus(_STOCK_DELISTED_);
		EXPECT_TRUE(stock.IsDelisted());
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_FALSE(stock.IsDelisted());
	}

	TEST_F(CTiingoStockTest, TestIsNotYetList) {
		stock.SetIPOStatus(_STOCK_NOT_YET_LIST_);
		EXPECT_TRUE(stock.IsNotYetList());
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_FALSE(stock.IsNotYetList());
	}

	TEST_F(CTiingoStockTest, TestIsUpdateDayLine) {
		EXPECT_TRUE(stock.IsUpdateDayLine());
		stock.SetUpdateDayLine(false);
		EXPECT_FALSE(stock.IsUpdateDayLine());
		stock.SetUpdateDayLine(true);
		EXPECT_TRUE(stock.IsUpdateDayLine());
	}

	TEST_F(CTiingoStockTest, TestIsDayLineNeededSaving) {
		// 此两个函数是具备同步机制的，这里没有进行测试
		EXPECT_FALSE(stock.IsUpdateDayLineDB());
		stock.SetUpdateDayLineDB(true);
		EXPECT_TRUE(stock.IsUpdateDayLineDB());
		stock.SetUpdateDayLineDB(false);
		EXPECT_FALSE(stock.IsUpdateDayLineDB());
	}

	TEST_F(CTiingoStockTest, TestUpdateDayLineStartEndDate) {
		auto pvDayLine = make_shared<vector<CTiingoCandleLine>>();

		CTiingoCandleLine dayLine;
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210101); // 这个需要添加进数据库
		dayLine.SetClose(10010);
		pvDayLine->push_back(dayLine);
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210102); // 这个需要添加进数据库
		dayLine.SetClose(12345);
		pvDayLine->push_back(dayLine);
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210107); // 这个数据库中有，无需添加
		dayLine.SetClose(10020);
		pvDayLine->push_back(dayLine);
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210123); // 这个需要添加进数据库
		dayLine.SetClose(10030);
		pvDayLine->push_back(dayLine);

		stock.SetSymbol("A");
		stock.SetDayLineEndDate(20210107);
		stock.UpdateDayLine(pvDayLine);

		stock.SetUpdateProfileDB(false);
		stock.SetDayLineStartDate(20210107);
		stock.SetDayLineEndDate(20210108);
		stock.UpdateDayLineStartEndDate();
		EXPECT_EQ(stock.GetDayLineStartDate(), 20210101);
		EXPECT_EQ(stock.GetDayLineEndDate(), 20210123);
		EXPECT_TRUE(stock.IsUpdateProfileDB());

		stock.SetUpdateProfileDB(false);
		stock.SetDayLineStartDate(20210101);
		stock.SetDayLineEndDate(20210110);
		stock.UpdateDayLineStartEndDate();
		EXPECT_EQ(stock.GetDayLineStartDate(), 20210101);
		EXPECT_EQ(stock.GetDayLineEndDate(), 20210123);
		EXPECT_TRUE(stock.IsUpdateProfileDB());

		stock.SetUpdateProfileDB(false);
		stock.SetDayLineStartDate(20210102);
		stock.SetDayLineEndDate(20210210);
		stock.UpdateDayLineStartEndDate();
		EXPECT_EQ(stock.GetDayLineStartDate(), 20210101);
		EXPECT_EQ(stock.GetDayLineEndDate(), 20210210);
		EXPECT_TRUE(stock.IsUpdateProfileDB());

		stock.SetUpdateProfileDB(false);
		stock.SetDayLineStartDate(20210101);
		stock.SetDayLineEndDate(20210210);
		stock.UpdateDayLineStartEndDate();
		EXPECT_EQ(stock.GetDayLineStartDate(), 20210101);
		EXPECT_EQ(stock.GetDayLineEndDate(), 20210210);
		EXPECT_FALSE(stock.IsUpdateProfileDB());
	}

	TEST_F(CTiingoStockTest, TestGetSetStockSplit) {
		EXPECT_EQ(stock.GetStockSplitCount(), 0);
		CStockSplit p;
		p.SetDate(20200101);
		p.SetRatio(2.0);
		stock.AddStockSplit(p);
		EXPECT_EQ(stock.GetStockSplitCount(), 1);
		EXPECT_EQ(stock.GetStockSplit(0).GetDate(), 20200101);
		EXPECT_DOUBLE_EQ(stock.GetStockSplit(0).GetRatio(), 2.0);
		p.SetDate(20210101);
		p.SetRatio(3.0);
		stock.AddStockSplit(p);
		EXPECT_EQ(stock.GetStockSplitCount(), 2);
		EXPECT_EQ(stock.GetStockSplit(1).GetDate(), 20210101);
		EXPECT_DOUBLE_EQ(stock.GetStockSplit(1).GetRatio(), 3.0);
		p.SetDate(20190101);
		p.SetRatio(1.5);
		stock.AddStockSplit(p);
		EXPECT_EQ(stock.GetStockSplitCount(), 3);
		EXPECT_EQ(stock.GetStockSplit(0).GetDate(), 20190101);
		EXPECT_DOUBLE_EQ(stock.GetStockSplit(0).GetRatio(), 1.5);
		EXPECT_EQ(stock.GetStockSplit(1).GetDate(), 20200101);
		EXPECT_DOUBLE_EQ(stock.GetStockSplit(1).GetRatio(), 2.0);
		EXPECT_EQ(stock.GetStockSplit(2).GetDate(), 20210101);
		EXPECT_DOUBLE_EQ(stock.GetStockSplit(2).GetRatio(), 3.0);
	}

	TEST_F(CTiingoStockTest, TestAdd52WeekLow) {
		EXPECT_FALSE(stock.Have52WeekLowDate(20000101)) << "初始时容器为空";

		stock.Add52WeekLowDate(20000101);
		stock.Add52WeekLowDate(20010101);

		EXPECT_FALSE(stock.Have52WeekLowDate(19800101));
		EXPECT_TRUE(stock.Have52WeekLowDate(20000101));
		EXPECT_TRUE(stock.Have52WeekLowDate(20010101));

		stock.Delete52WeekLowDate(19800101);
		EXPECT_TRUE(stock.Have52WeekLowDate(20000101));
		EXPECT_TRUE(stock.Have52WeekLowDate(20010101));

		stock.Delete52WeekLowDate(20000101);
		EXPECT_FALSE(stock.Have52WeekLowDate(20000101));
		EXPECT_TRUE(stock.Have52WeekLowDate(20010101));
	}

	TEST_F(CTiingoStockTest, TestAdd52WeekHigh) {
		EXPECT_FALSE(stock.Have52WeekHighDate(20000101)) << "初始时容器为空";

		stock.Add52WeekHighDate(20000101);
		stock.Add52WeekHighDate(20010101);

		EXPECT_FALSE(stock.Have52WeekHighDate(19800101));
		EXPECT_TRUE(stock.Have52WeekHighDate(20000101));
		EXPECT_TRUE(stock.Have52WeekHighDate(20010101));

		stock.Delete52WeekHighDate(19800101);
		EXPECT_TRUE(stock.Have52WeekHighDate(20000101));
		EXPECT_TRUE(stock.Have52WeekHighDate(20010101));

		stock.Delete52WeekHighDate(20000101);
		EXPECT_FALSE(stock.Have52WeekHighDate(20000101));
		EXPECT_TRUE(stock.Have52WeekHighDate(20010101));
	}

	TEST_F(CTiingoStockTest, TestUpdateRTData) {
		CTiingoIEXTopOfBook IEX;
		IEX.m_timeStamp = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::from_time_t(123456));
		IEX.m_lOpen = 12340;
		IEX.m_lHigh = 23450;
		IEX.m_lLow = 1230;
		IEX.m_lLastClose = 234560;
		IEX.m_lNew = 230;
		IEX.m_llVolume = 100000;
		stock.UpdateRTData(IEX);
		EXPECT_EQ(stock.GetTransactionTime(), IEX.m_timeStamp.time_since_epoch().count());
		EXPECT_EQ(stock.GetOpen(), IEX.m_lOpen);
		EXPECT_EQ(stock.GetHigh(), IEX.m_lHigh);
		EXPECT_EQ(stock.GetLow(), IEX.m_lLow);
		EXPECT_EQ(stock.GetLastClose(), IEX.m_lLastClose);
		EXPECT_EQ(stock.GetNew(), IEX.m_lNew);
		EXPECT_EQ(stock.GetVolume(), IEX.m_llVolume);
	}

	TEST_F(CTiingoStockTest, TestSaveDayLine) {
		using namespace StockMarket;
		const auto& t = TiingoStockDayline{};
		{
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);

			// 清除可能的测试遗留数据（如果有的话），以保证后续操作的正确性。
			db(remove_from(t).where(t.Symbol == "A" && t.Date == 19800101));
			db(remove_from(t).where(t.Symbol == "A" && t.Date == 20210101));
			db(remove_from(t).where(t.Symbol == "A" && t.Date == 20241111));
			tx.commit();
		}
		auto pvDayLine = make_shared<vector<CTiingoCandleLine>>();

		CTiingoCandleLine dayLine;
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(19800101); // 这个日期早于数据库中的最早日期，需要添加进数据库
		dayLine.SetClose(115);
		dayLine.SetRatio(stock.GetRatio());
		pvDayLine->push_back(dayLine);
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210101); // 这个日期为新日期，需要添加进数据库
		dayLine.SetClose(12340);
		dayLine.SetRatio(stock.GetRatio());
		pvDayLine->push_back(dayLine);
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210107); // 这个数据库中有，无需添加
		dayLine.SetClose(10020);
		dayLine.SetRatio(stock.GetRatio());
		pvDayLine->push_back(dayLine);
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20241111); // 这个日期为新日期，需要添加进数据库
		dayLine.SetClose(135);
		dayLine.SetRatio(stock.GetRatio());
		pvDayLine->push_back(dayLine);

		stock.SetSymbol("A");
		stock.SetDayLineEndDate(20210107);
		stock.UpdateDayLine(pvDayLine);

		stock.SaveDayLineDB();

		{
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);

			// 清除可能的测试遗留数据（如果有的话），以保证后续操作的正确性。
			auto result = db(select(all_of(t)).from(t).where(t.Symbol == "A" && t.Date == 19800101));
			EXPECT_TRUE(result.size() == 1);
			auto& row = result.front();
			double value = row.Close;
			EXPECT_DOUBLE_EQ(value, 0.000115);
			result = db(select(all_of(t)).from(t).where(t.Symbol == "A" && t.Date == 20210101));
			EXPECT_TRUE(result.size() == 1);
			auto& row2 = result.front();
			value = row2.Close;
			EXPECT_DOUBLE_EQ(value, 0.012340);
			result = db(select(all_of(t)).from(t).where(t.Symbol == "A" && t.Date == 20241111));
			EXPECT_TRUE(result.size() == 1);
			auto& row3 = result.front();
			value = row3.Close;
			EXPECT_DOUBLE_EQ(value, 0.000135);

			db(remove_from(t).where(t.Symbol == "A" && t.Date == 19800101));
			db(remove_from(t).where(t.Symbol == "A" && t.Date == 20210101));
			db(remove_from(t).where(t.Symbol == "A" && t.Date == 20241111));
			tx.commit();
		}
	}

	TEST_F(CTiingoStockTest, TestCreateWeekLine) {
		stock.SetSymbol("AAPL");
		stock.LoadDayLineDB();
		stock.CreateWeekLine();
		auto pvDayLine = stock.DayLine();
		auto pvWeekLine = stock.WeekLine();

		EXPECT_EQ(pvDayLine->Size(), 11061);
		EXPECT_EQ(pvWeekLine->Size(), 2291);
		EXPECT_EQ(pvDayLine->GetData(11052)->GetDate(), pvWeekLine->GetData(2289)->GetDate()) << pvDayLine->GetData(11052)->GetDate();
		EXPECT_EQ(pvDayLine->GetData(11055)->GetClose(), pvWeekLine->GetData(2289)->GetClose()) << pvDayLine->GetData(11055)->GetDate();
		EXPECT_EQ(pvDayLine->GetData(11052)->GetOpen(), pvWeekLine->GetData(2289)->GetOpen()) << "周一的开盘价相等";
		EXPECT_EQ(234325000, pvWeekLine->GetData(2289)->GetHigh()) << "最高价";
		EXPECT_EQ(220270000, pvWeekLine->GetData(2289)->GetLow()) << "最低价";
		INT64 volume = 0, amount = 0;
		for (int i = 0; i < 4; i++) {
			volume += pvDayLine->GetData(11052 + i)->GetVolume();
			amount += pvDayLine->GetData(11051 + i)->GetAmount();
		}
		auto weekLine = pvWeekLine->GetData(2289);
		EXPECT_EQ(volume, pvWeekLine->GetData(2289)->GetVolume()) << "成交量相等";
		EXPECT_EQ(amount, pvWeekLine->GetData(2289)->GetAmount()) << "成交金额相等";

		EXPECT_EQ(pvDayLine->GetData(11056)->GetDate(), pvWeekLine->GetData(2290)->GetDate()) << "周一的日期";
		EXPECT_EQ(pvDayLine->GetData(11060)->GetClose(), pvWeekLine->GetData(2290)->GetClose()) << "最后的收盘价相等";
		EXPECT_EQ(pvDayLine->GetData(11056)->GetOpen(), pvWeekLine->GetData(2290)->GetOpen()) << "周一的开盘价相等";
		EXPECT_EQ(228660000, pvWeekLine->GetData(2290)->GetHigh()) << "最高价";
		EXPECT_EQ(219710000, pvWeekLine->GetData(2290)->GetLow()) << "复权调整后的最低价";
		volume = 0;
		amount = 0;
		for (int i = 0; i < 5; i++) {
			volume += pvDayLine->GetData(11056 + i)->GetVolume();
			amount += pvDayLine->GetData(11056 + i)->GetAmount();
		}
		EXPECT_EQ(volume, pvWeekLine->GetData(2290)->GetVolume()) << "成交量相等";
		EXPECT_EQ(amount, pvWeekLine->GetData(2290)->GetAmount()) << "成交金额相等";
	}

	TEST_F(CTiingoStockTest, TestHaveNewDayLineData) {
		auto pvDayLine = make_shared<vector<CTiingoCandleLine>>();
		//CSetTiingoStockDayLine setDayLine;

		EXPECT_EQ(stock.GetDayLineSize(), 0);
		EXPECT_FALSE(stock.HaveNewDayLineData()) << "没有日线数据";

		CTiingoCandleLine dayLine;
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210101); // 这个需要添加进数据库
		dayLine.SetClose(10010);
		pvDayLine->push_back(dayLine);
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210102); // 这个需要添加进数据库
		dayLine.SetClose(12345);
		pvDayLine->push_back(dayLine);
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210107); // 这个数据库中有，无需添加
		dayLine.SetClose(10020);
		pvDayLine->push_back(dayLine);
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210123); // 这个需要添加进数据库
		dayLine.SetClose(10030);
		pvDayLine->push_back(dayLine);

		stock.SetSymbol("A");
		stock.SetDayLineEndDate(20210107);
		stock.UpdateDayLine(pvDayLine);

		EXPECT_EQ(stock.GetDayLineSize(), 4);

		auto pDayLine = stock.GetDayLine(1);
		EXPECT_EQ(pDayLine->GetDate(), 20210102);

		stock.SetUpdateProfileDB(false);
		stock.SetDayLineStartDate(20210107);
		stock.SetDayLineEndDate(20210108);
		EXPECT_TRUE(stock.HaveNewDayLineData());

		stock.SetUpdateProfileDB(false);
		stock.SetDayLineStartDate(20210101);
		stock.SetDayLineEndDate(20210110);
		EXPECT_TRUE(stock.HaveNewDayLineData());

		stock.SetUpdateProfileDB(false);
		stock.SetDayLineStartDate(20210102);
		stock.SetDayLineEndDate(20210210);
		EXPECT_TRUE(stock.HaveNewDayLineData());

		stock.SetUpdateProfileDB(false);
		stock.SetDayLineStartDate(20210101);
		stock.SetDayLineEndDate(20210210);
		EXPECT_FALSE(stock.HaveNewDayLineData());
	}

	TEST_F(CTiingoStockTest, TestProcessDayLine) {
		//auto pStock = make_shared<CTiingoStock>();
		//pStock->SetSymbol("AAPL");
		//pStock->ProcessDayLine();

		auto pStock3 = make_shared<CTiingoStock>();
		pStock3->SetSymbol("ACI");
		pStock3->ProcessDayLine3(19800101);

		auto pStock2 = make_shared<CTiingoStock>();
		pStock2->SetSymbol("ACI");
		pStock2->ProcessDayLine2(19800101);

		EXPECT_TRUE(pStock3->m_v52WeekLowDate.size() == pStock2->m_v52WeekLowDate.size());
		EXPECT_TRUE(pStock3->m_v52WeekHighDate.size() == pStock2->m_v52WeekHighDate.size());
		//EXPECT_TRUE(pStock->m_v52WeekLow.size() == pStock2->m_v52WeekLow.size());
		//EXPECT_TRUE(pStock->m_v52WeekHigh.size() == pStock2->m_v52WeekHigh.size());

		for (size_t i = 0; i < pStock3->m_v52WeekLowDate.size(); i++) {
			EXPECT_TRUE(pStock3->m_v52WeekLowDate.at(i) == pStock2->m_v52WeekLowDate.at(i)) << i;
		}
		for (size_t i = 0; i < pStock3->m_v52WeekHighDate.size(); i++) {
			EXPECT_TRUE(pStock3->m_v52WeekHighDate.at(i) == pStock2->m_v52WeekHighDate.at(i)) << i;
		}
		/*
		for (int i = 0; i < pStock->m_v52WeekLow.size(); i++) {
			EXPECT_TRUE(pStock->m_v52WeekLow.at(i) == pStock2->m_v52WeekLow.at(i)) << i;
		}
		for (int i = 0; i < pStock->m_v52WeekLow.size(); i++) {
			EXPECT_TRUE(pStock->m_v52WeekHigh.at(i) == pStock2->m_v52WeekHigh.at(i)) << i;
		}
		*/
		string str = pStock2->GetSymbol();

		//
	}

	TEST_F(CTiingoStockTest, TestProcessDayLine2) {
		//auto pStock = make_shared<CTiingoStock>();
		//pStock->SetSymbol("A"));
		//pStock->ProcessDayLine();

		auto pStock3 = make_shared<CTiingoStock>();
		pStock3->SetSymbol("A");
		pStock3->ProcessDayLine3(19800101);

		auto pStock2 = make_shared<CTiingoStock>();
		pStock2->SetSymbol("A");
		pStock2->ProcessDayLine2(19800101);

		EXPECT_TRUE(pStock3->m_v52WeekLowDate.size() == pStock2->m_v52WeekLowDate.size());
		EXPECT_TRUE(pStock3->m_v52WeekHighDate.size() == pStock2->m_v52WeekHighDate.size());
		//EXPECT_TRUE(pStock->m_v52WeekLow.size() == pStock2->m_v52WeekLow.size());
		//EXPECT_TRUE(pStock->m_v52WeekHigh.size() == pStock2->m_v52WeekHigh.size());

		for (size_t i = 0; i < pStock3->m_v52WeekLowDate.size(); i++) {
			EXPECT_TRUE(pStock3->m_v52WeekLowDate.at(i) == pStock2->m_v52WeekLowDate.at(i)) << i;
		}
		for (size_t i = 0; i < pStock3->m_v52WeekLowDate.size(); i++) {
			EXPECT_TRUE(pStock3->m_v52WeekHighDate.at(i) == pStock2->m_v52WeekHighDate.at(i)) << i;
		}
		/*
		for (int i = 0; i < pStock->m_v52WeekLow.size(); i++) {
			EXPECT_TRUE(pStock->m_v52WeekLow.at(i) == pStock2->m_v52WeekLow.at(i)) << i;
		}
		for (int i = 0; i < pStock->m_v52WeekLow.size(); i++) {
			EXPECT_TRUE(pStock->m_v52WeekHigh.at(i) == pStock2->m_v52WeekHigh.at(i)) << i;
		}
		*/
		string str = pStock2->GetSymbol();

		//
	}

	TEST_F(CTiingoStockTest, TestRebuildStockSplitDB) {
		stock.ClearStockSplit();
		EXPECT_EQ(stock.GetStockSplitCount(), 0);

		stock.SetSymbol("AAPL");

		stock.RebuildStockSplitDB();

		EXPECT_EQ(stock.GetStockSplitCount(), 5);
		EXPECT_EQ(stock.GetStockSplit(0).GetDate(), 19870616);
		EXPECT_EQ(stock.GetStockSplit(1).GetDate(), 20000621);
		EXPECT_EQ(stock.GetStockSplit(2).GetDate(), 20050228);
		EXPECT_EQ(stock.GetStockSplit(3).GetDate(), 20140609);
		EXPECT_EQ(stock.GetStockSplit(4).GetDate(), 20200831);
		EXPECT_DOUBLE_EQ(stock.GetStockSplit(0).GetRatio(), 2.0);
		EXPECT_DOUBLE_EQ(stock.GetStockSplit(1).GetRatio(), 2.0);
		EXPECT_DOUBLE_EQ(stock.GetStockSplit(2).GetRatio(), 2.0);
		EXPECT_DOUBLE_EQ(stock.GetStockSplit(3).GetRatio(), 7.0);
		EXPECT_DOUBLE_EQ(stock.GetStockSplit(4).GetRatio(), 4.0);
	}

	TEST_F(CTiingoStockTest, TestUpdateFinancialStateDB) {
		stock.SetSymbol("AAPL");
		CTiingoCompanyFinancialStatesPtr pvState = make_shared<vector<CTiingoCompanyFinancialStatePtr>>();
		CTiingoCompanyFinancialStatePtr pState = make_shared<CTiingoCompanyFinancialState>();
		pState->m_symbol = "AAPL";
		pState->m_yearQuarter = 202001; // 早于最早的数据
		pState->m_exchange = "Test"; // 虚假交易所代码，用于删除
		pvState->push_back(pState);
		pState = make_shared<CTiingoCompanyFinancialState>();
		pState->m_symbol = "AAPL";
		pState->m_yearQuarter = 202301; // 存在于集合中
		pState->m_exchange = "Test";
		pvState->push_back(pState);
		pState = make_shared<CTiingoCompanyFinancialState>();
		pState->m_symbol = "AAPL";
		pState->m_yearQuarter = 202305; // 居中新数据
		pState->m_exchange = "Test";
		pvState->push_back(pState);
		pState = make_shared<CTiingoCompanyFinancialState>();
		pState->m_symbol = "AAPL";
		pState->m_yearQuarter = 202404; // 晚于最新数据
		pState->m_exchange = "Test";
		pvState->push_back(pState);

		stock.UpdateFinancialState(pvState);

		stock.UpdateFinancialStateDB();

		{
			using namespace StockMarket;
			const auto& t = TiingoCompanyFinancialState{};
			auto db = gl_dbStockMarket.get();
			auto tx = start_transaction(db);
			//auto result = db(select(count(all_of(table))).from(table).where(table.Symbol == "AAPL" && table.Exchange == "Test").order_by(table.YearQuarter.asc()));
			//Note: Resharper误报，无法识别result的类型，实际为vector<TiingoCompanyFinancialState>
			//resharper disable all
			auto result = db(select(all_of(t)).from(t).where(t.Symbol == "AAPL" && t.Exchange == "Test").order_by(t.YearQuarter.asc())); //NOLINT
			//resharper restore all
			size_t rows = result.size();
			EXPECT_TRUE(rows == 3);
			auto& row1 = result.front();
			EXPECT_EQ(row1.YearQuarter.value(), 202001);
			result.pop_front();
			auto& row2 = result.front();
			EXPECT_EQ(row2.YearQuarter.value(), 202305);
			result.pop_front();
			auto& row3 = result.front();
			EXPECT_EQ(row3.YearQuarter.value(), 202404);
			db(remove_from(t).where(t.Symbol == "AAPL" && t.Exchange == "Test"));
			tx.commit();
		}
	}
}
