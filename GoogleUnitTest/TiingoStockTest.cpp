#include"pch.h"

#include"GeneralCheck.h"
#include "SetTiingoStockDayLine.h"

#include"TiingoStock.h"

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
		EXPECT_TRUE(IsTiingoStock(pTiingoStock));
		EXPECT_FALSE(IsTiingoStock(pStock));
		EXPECT_FALSE(IsTiingoStock(nullptr));
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
		auto pvDayLine = make_shared<vector<CTiingoDayLinePtr>>();
		CSetTiingoStockDayLine setDayLine;

		CTiingoDayLinePtr pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol("A");
		pDayLine->SetDate(20210101); // 这个需要添加进数据库
		pDayLine->SetClose(10010);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol("A");
		pDayLine->SetDate(20210102); // 这个需要添加进数据库
		pDayLine->SetClose(12345);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol("A");
		pDayLine->SetDate(20210107); // 这个数据库中有，无需添加
		pDayLine->SetClose(10020);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol("A");
		pDayLine->SetDate(20210123); // 这个需要添加进数据库
		pDayLine->SetClose(10030);
		pvDayLine->push_back(pDayLine);

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

	TEST_F(CTiingoStockTest, TestAdd52WeekLow) {
		EXPECT_FALSE(stock.Have52WeekLowDate(20000101)) << "初始时容器为空";

		stock.Add52WeekLow(20000101);
		stock.Add52WeekLow(20010101);

		EXPECT_FALSE(stock.Have52WeekLowDate(19800101));
		EXPECT_TRUE(stock.Have52WeekLowDate(20000101));
		EXPECT_TRUE(stock.Have52WeekLowDate(20010101));

		stock.Delete52WeekLow(19800101);
		EXPECT_TRUE(stock.Have52WeekLowDate(20000101));
		EXPECT_TRUE(stock.Have52WeekLowDate(20010101));

		stock.Delete52WeekLow(20000101);
		EXPECT_FALSE(stock.Have52WeekLowDate(20000101));
		EXPECT_TRUE(stock.Have52WeekLowDate(20010101));
	}

	TEST_F(CTiingoStockTest, TestAdd52WeekHigh) {
		EXPECT_FALSE(stock.Have52WeekHighDate(20000101)) << "初始时容器为空";

		stock.Add52WeekHigh(20000101);
		stock.Add52WeekHigh(20010101);

		EXPECT_FALSE(stock.Have52WeekHighDate(19800101));
		EXPECT_TRUE(stock.Have52WeekHighDate(20000101));
		EXPECT_TRUE(stock.Have52WeekHighDate(20010101));

		stock.Delete52WeekHigh(19800101);
		EXPECT_TRUE(stock.Have52WeekHighDate(20000101));
		EXPECT_TRUE(stock.Have52WeekHighDate(20010101));

		stock.Delete52WeekHigh(20000101);
		EXPECT_FALSE(stock.Have52WeekHighDate(20000101));
		EXPECT_TRUE(stock.Have52WeekHighDate(20010101));
	}

	TEST_F(CTiingoStockTest, TestUpdateRTData) {
		CTiingoIEXTopOfBookPtr pIEX = make_shared<CTiingoIEXTopOfBook>();
		pIEX->m_timeStamp = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::from_time_t(123456));
		pIEX->m_lOpen = 12340;
		pIEX->m_lHigh = 23450;
		pIEX->m_lLow = 1230;
		pIEX->m_lLastClose = 234560;
		pIEX->m_lNew = 230;
		pIEX->m_llVolume = 100000;
		stock.UpdateRTData(pIEX);
		EXPECT_EQ(stock.GetTransactionTime(), pIEX->m_timeStamp.time_since_epoch().count());
		EXPECT_EQ(stock.GetOpen(), pIEX->m_lOpen);
		EXPECT_EQ(stock.GetHigh(), pIEX->m_lHigh);
		EXPECT_EQ(stock.GetLow(), pIEX->m_lLow);
		EXPECT_EQ(stock.GetLastClose(), pIEX->m_lLastClose);
		EXPECT_EQ(stock.GetNew(), pIEX->m_lNew);
		EXPECT_EQ(stock.GetVolume(), pIEX->m_llVolume);
	}

	TEST_F(CTiingoStockTest, TestSave) {
		CTiingoStock stock2;
		CSetTiingoStock setTiingoStock;

		setTiingoStock.m_strFilter = "[Ticker] = '000001.US'";
		setTiingoStock.Open();
		EXPECT_TRUE(setTiingoStock.IsEOF());
		setTiingoStock.Close();

		stock.SetTiingoPermaTicker("aasdfasdfj");
		stock.SetSymbol("000001.US");
		stock.SetName("adkjkf");
		stock.SetActive(true);
		stock.SetIsADR(true);
		stock.SetSicCode(1234);
		stock.SetSicIndustry("defg");
		stock.SetSicSector("efg");
		stock.SetTiingoIndustry("ghi");
		stock.SetTiingoSector("defghijk");
		stock.SetReportingCurrency("US");
		stock.SetLocation("Irvine");
		stock.SetCompanyWebSite("ijk");
		stock.SetSECFilingWebSite("https://def.com");
		stock.SetCompanyFinancialStatementUpdateDate(20202020);
		stock.Add52WeekLow(20200101);
		stock.Add52WeekLow(20240101);
		stock.Add52WeekHigh(20000101);
		stock.Add52WeekHigh(20040101);

		setTiingoStock.Open();
		setTiingoStock.m_pDatabase->BeginTrans();
		stock.Append(setTiingoStock);
		setTiingoStock.m_pDatabase->CommitTrans();// 使用CommitTrans()后会真正更新数据库。
		setTiingoStock.Close();

		setTiingoStock.m_strFilter = "[Ticker] = '000001.US'";
		setTiingoStock.Open();
		setTiingoStock.m_pDatabase->BeginTrans();
		stock2.Load(setTiingoStock);
		setTiingoStock.Delete();
		setTiingoStock.m_pDatabase->CommitTrans(); // 必须使用CommitTrans()来真正删除数据库中的数据。
		setTiingoStock.Close();

		EXPECT_TRUE(stock.GetTiingoPermaTicker() == stock2.GetTiingoPermaTicker());
		EXPECT_TRUE(stock.GetSymbol() == stock2.GetSymbol());
		EXPECT_TRUE(stock.GetName() == stock2.GetName());
		EXPECT_TRUE(stock.IsActive() == stock2.IsActive());
		EXPECT_TRUE(stock.IsADR() == stock2.IsADR());
		EXPECT_EQ(stock.GetSicCode(), stock2.GetSicCode());
		EXPECT_TRUE(stock.GetSicIndustry() == stock2.GetSicIndustry());
		EXPECT_TRUE(stock.GetSicSector() == stock2.GetSicSector());
		EXPECT_TRUE(stock.GetTiingoIndustry() == stock2.GetTiingoIndustry());
		EXPECT_TRUE(stock.GetTiingoSector() == stock2.GetTiingoSector());
		EXPECT_TRUE(stock.GetReportingCurrency() == stock2.GetReportingCurrency());
		EXPECT_TRUE(stock.GetLocation() == stock2.GetLocation());
		EXPECT_TRUE(stock.GetCompanyWebSite() == stock2.GetCompanyWebSite());
		EXPECT_TRUE(stock.GetSECFilingWebSite() == stock2.GetSECFilingWebSite());
		EXPECT_EQ(stock.GetCompanyFinancialStatementUpdateDate(), stock2.GetCompanyFinancialStatementUpdateDate());
		EXPECT_TRUE(stock.Have52WeekLowDate(20200101));
		EXPECT_TRUE(stock.Have52WeekLowDate(20240101));
		EXPECT_TRUE(stock.Have52WeekHighDate(20000101));
		EXPECT_TRUE(stock.Have52WeekHighDate(20040101));
	}

	TEST_F(CTiingoStockTest, TestSaveDayLine) {
		auto pvDayLine = make_shared<vector<CTiingoDayLinePtr>>();
		CSetTiingoStockDayLine setDayLine;

		CTiingoDayLinePtr pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol("A");
		pDayLine->SetDate(19800101); // 这个日期早于数据库中的最早日期，需要添加进数据库
		pDayLine->SetClose(115);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol("A");
		pDayLine->SetDate(20210101); // 这个日期为新日期，需要添加进数据库
		pDayLine->SetClose(12340);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol("A");
		pDayLine->SetDate(20210107); // 这个数据库中有，无需添加
		pDayLine->SetClose(10020);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol("A");
		pDayLine->SetDate(20241111); // 这个日期为新日期，需要添加进数据库
		pDayLine->SetClose(135);
		pvDayLine->push_back(pDayLine);

		stock.SetSymbol("A");
		stock.SetDayLineEndDate(20210107);
		stock.UpdateDayLine(pvDayLine);

		stock.SaveDayLineDB();

		setDayLine.m_strFilter = "[Symbol] = 'A'";
		setDayLine.m_strSort = "[Date]";
		setDayLine.Open();
		setDayLine.m_pDatabase->BeginTrans();
		EXPECT_TRUE(setDayLine.m_Date == 19800101);
		EXPECT_STREQ(setDayLine.m_Close, _T("0.000115"));
		setDayLine.Delete();
		while (setDayLine.m_Date != 20210101) setDayLine.MoveNext();
		EXPECT_STREQ(setDayLine.m_Close, _T("0.012340"));
		setDayLine.Delete();
		while (setDayLine.m_Date != 20241111) setDayLine.MoveNext();
		EXPECT_TRUE(setDayLine.m_Date = 20241111);
		EXPECT_STREQ(setDayLine.m_Close, _T("0.000135"));
		setDayLine.Delete();
		setDayLine.m_pDatabase->CommitTrans();
		setDayLine.Close();
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
		EXPECT_EQ(volume, pvWeekLine->GetData(2289)->GetVolume()) << "成交量相等";
		EXPECT_EQ(amount, pvWeekLine->GetData(2289)->GetAmount()) << "成交金额相等";

		EXPECT_EQ(pvDayLine->GetData(11056)->GetDate(), pvWeekLine->GetData(2290)->GetDate()) << "周一的日期";
		EXPECT_EQ(pvDayLine->GetData(11060)->GetClose(), pvWeekLine->GetData(2290)->GetClose()) << "最后的收盘价相等";
		EXPECT_EQ(pvDayLine->GetData(11056)->GetOpen(), pvWeekLine->GetData(2290)->GetOpen()) << "周一的开盘价相等";
		EXPECT_EQ(228660000, pvWeekLine->GetData(2290)->GetHigh()) << "最高价";
		EXPECT_EQ(219710000, pvWeekLine->GetData(2290)->GetLow()) << "最低价";
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
		auto pvDayLine = make_shared<vector<CTiingoDayLinePtr>>();
		//CSetTiingoStockDayLine setDayLine;

		EXPECT_EQ(stock.GetDayLineSize(), 0);
		EXPECT_FALSE(stock.HaveNewDayLineData()) << "没有日线数据";

		CTiingoDayLinePtr pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol("A");
		pDayLine->SetDate(20210101); // 这个需要添加进数据库
		pDayLine->SetClose(10010);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol("A");
		pDayLine->SetDate(20210102); // 这个需要添加进数据库
		pDayLine->SetClose(12345);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol("A");
		pDayLine->SetDate(20210107); // 这个数据库中有，无需添加
		pDayLine->SetClose(10020);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol("A");
		pDayLine->SetDate(20210123); // 这个需要添加进数据库
		pDayLine->SetClose(10030);
		pvDayLine->push_back(pDayLine);

		stock.SetSymbol("A");
		stock.SetDayLineEndDate(20210107);
		stock.UpdateDayLine(pvDayLine);

		EXPECT_EQ(stock.GetDayLineSize(), 4);

		pDayLine = stock.GetDayLine(1);
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

		CSetTiingoCompanyFinancialState setState;

		setState.m_strFilter = "[Exchange] = 'Test'";
		setState.m_strSort = "[yearQuarter]";
		setState.Open();
		setState.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setState.IsEOF());
		EXPECT_EQ(setState.m_yearQuarter, 202001);
		setState.Delete();
		setState.MoveNext();
		EXPECT_FALSE(setState.IsEOF());
		EXPECT_EQ(setState.m_yearQuarter, 202305);
		setState.Delete();
		setState.MoveNext();
		EXPECT_FALSE(setState.IsEOF());
		EXPECT_EQ(setState.m_yearQuarter, 202404);
		setState.Delete();
		setState.MoveNext();
		EXPECT_TRUE(setState.IsEOF());

		setState.m_pDatabase->CommitTrans();
		setState.Close();
	}

	TEST_F(CTiingoStockTest, TestProcessDayLine) {
		//auto pStock = make_shared<CTiingoStock>();
		//pStock->SetSymbol("AAPL");
		//pStock->ProcessDayLine();

		auto pStock3 = make_shared<CTiingoStock>();
		pStock3->SetSymbol("ACI");
		pStock3->ProcessDayLine3();

		auto pStock2 = make_shared<CTiingoStock>();
		pStock2->SetSymbol("ACI");
		pStock2->ProcessDayLine2();

		EXPECT_TRUE(pStock3->m_v52WeekLow.size() == pStock2->m_v52WeekLow.size());
		EXPECT_TRUE(pStock3->m_v52WeekHigh.size() == pStock2->m_v52WeekHigh.size());
		//EXPECT_TRUE(pStock->m_v52WeekLow.size() == pStock2->m_v52WeekLow.size());
		//EXPECT_TRUE(pStock->m_v52WeekHigh.size() == pStock2->m_v52WeekHigh.size());

		for (int i = 0; i < pStock3->m_v52WeekLow.size(); i++) {
			EXPECT_TRUE(pStock3->m_v52WeekLow.at(i) == pStock2->m_v52WeekLow.at(i)) << i;
		}
		for (int i = 0; i < pStock3->m_v52WeekHigh.size(); i++) {
			EXPECT_TRUE(pStock3->m_v52WeekHigh.at(i) == pStock2->m_v52WeekHigh.at(i)) << i;
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
		pStock3->ProcessDayLine3();

		auto pStock2 = make_shared<CTiingoStock>();
		pStock2->SetSymbol("A");
		pStock2->ProcessDayLine2();

		EXPECT_TRUE(pStock3->m_v52WeekLow.size() == pStock2->m_v52WeekLow.size());
		EXPECT_TRUE(pStock3->m_v52WeekHigh.size() == pStock2->m_v52WeekHigh.size());
		//EXPECT_TRUE(pStock->m_v52WeekLow.size() == pStock2->m_v52WeekLow.size());
		//EXPECT_TRUE(pStock->m_v52WeekHigh.size() == pStock2->m_v52WeekHigh.size());

		for (int i = 0; i < pStock3->m_v52WeekLow.size(); i++) {
			EXPECT_TRUE(pStock3->m_v52WeekLow.at(i) == pStock2->m_v52WeekLow.at(i)) << i;
		}
		for (int i = 0; i < pStock3->m_v52WeekLow.size(); i++) {
			EXPECT_TRUE(pStock3->m_v52WeekHigh.at(i) == pStock2->m_v52WeekHigh.at(i)) << i;
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
}
