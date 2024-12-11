#include"pch.h"

 

#include"SystemData.h"

#include"GeneralCheck.h"
import FireBird.Set.TiingoStockDayLine;
import FireBird.Type.TiingoIEXTopOfBook; 
import FireBird.Type.TiingoCompanyFinancialState;

import FireBird.Stock.TiingoStock;

#include "WorldMarket.h"

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

	TEST_F(CTiingoStockTest, TestInitialize) {
		CTiingoStock tiingo;

		EXPECT_STREQ(tiingo.m_strTiingoPermaTicker, _T(""));
		EXPECT_STREQ(tiingo.GetSymbol(), _T(""));
		EXPECT_STREQ(tiingo.m_strName, _T(""));
		EXPECT_FALSE(tiingo.IsActive());
		EXPECT_FALSE(tiingo.m_fIsADR);
		EXPECT_EQ(tiingo.m_iSicCode, 0);
		EXPECT_STREQ(tiingo.m_strSicIndustry, _T(""));
		EXPECT_STREQ(tiingo.m_strSicSector, _T(""));
		EXPECT_STREQ(tiingo.m_strTiingoIndustry, _T(""));
		EXPECT_STREQ(tiingo.m_strTiingoSector, _T(""));
		EXPECT_STREQ(tiingo.m_strReportingCurrency, _T(""));
		EXPECT_STREQ(tiingo.m_strLocation, _T(""));
		EXPECT_STREQ(tiingo.m_strCompanyWebSite, _T(""));
		EXPECT_STREQ(tiingo.m_strSECFilingWebSite, _T(""));
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
		EXPECT_EQ(stock.GetRatio(), 10000) << "Tiingo.com的股票价格，放大倍数为10000";
	}

	TEST_F(CTiingoStockTest, TestGetExchangeCode) {
		EXPECT_STREQ(stock.GetExchangeCode(), _T("US"));
		stock.SetExchangeCode(_T("SS"));
		EXPECT_STREQ(stock.GetExchangeCode(), _T("SS"));
	}

	TEST_F(CTiingoStockTest, TestGetSymbol) {
		EXPECT_STREQ(stock.GetSymbol(), _T(""));
		stock.SetSymbol(_T("600000.SS"));
		EXPECT_STREQ(stock.GetSymbol(), _T("600000.SS"));
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
		EXPECT_TRUE(stock.IsUpdateProfileDBAndClearFlag());
		EXPECT_FALSE(stock.IsUpdateProfileDB());
		stock.SetUpdateProfileDB(true);
		EXPECT_TRUE(stock.IsUpdateProfileDB());
		stock.SetUpdateProfileDB(false);
		EXPECT_FALSE(stock.IsUpdateProfileDB());
	}

	TEST_F(CTiingoStockTest, TestIsUpdateProfileDBAndClearFlag) {
		EXPECT_FALSE(stock.IsUpdateProfileDB());
		EXPECT_FALSE(stock.IsUpdateProfileDBAndClearFlag());
		stock.SetUpdateProfileDB(true);
		EXPECT_TRUE(stock.IsUpdateProfileDBAndClearFlag());
		EXPECT_FALSE(stock.IsUpdateProfileDB());
	}

	TEST_F(CTiingoStockTest, TestIsUpdateCompanyNewsDB) {
		EXPECT_FALSE(stock.IsUpdateCompanyNewsDB());
		stock.SetUpdateCompanyNewsDB(true);
		EXPECT_TRUE(stock.IsUpdateCompanyNewsDB());
		EXPECT_TRUE(stock.IsUpdateCompanyNewsDBAndClearFlag());
		EXPECT_FALSE(stock.IsUpdateCompanyNewsDB());
		stock.SetUpdateCompanyNewsDB(true);
		EXPECT_TRUE(stock.IsUpdateCompanyNewsDB());
		stock.SetUpdateCompanyNewsDB(false);
		EXPECT_FALSE(stock.IsUpdateCompanyNewsDB());
	}

	TEST_F(CTiingoStockTest, TestIsUpdateCompanyNewsDBAndClearFlag) {
		EXPECT_FALSE(stock.IsUpdateCompanyNewsDB());
		EXPECT_FALSE(stock.IsUpdateCompanyNewsDBAndClearFlag());
		stock.SetUpdateCompanyNewsDB(true);
		EXPECT_TRUE(stock.IsUpdateCompanyNewsDBAndClearFlag());
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
		stock.SetUpdateDayLineDB(true);
		EXPECT_TRUE(stock.IsUpdateDayLineDBAndClearFlag());
		EXPECT_FALSE(stock.IsUpdateDayLineDB());
	}

	TEST_F(CTiingoStockTest, TestUpdateDayLineStartEndDate) {
		auto pvDayLine = make_shared<vector<CTiingoDayLinePtr>>();
		CSetTiingoStockDayLine setDayLine;

		CTiingoDayLinePtr pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210101); // 这个需要添加进数据库
		pDayLine->SetClose(10010);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210102); // 这个需要添加进数据库
		pDayLine->SetClose(12345);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210107); // 这个数据库中有，无需添加
		pDayLine->SetClose(10020);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210123); // 这个需要添加进数据库
		pDayLine->SetClose(10030);
		pvDayLine->push_back(pDayLine);

		stock.SetSymbol(_T("A"));
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
		pIEX->m_llTimestamp = 123456;
		pIEX->m_lOpen = 12340;
		pIEX->m_lHigh = 23450;
		pIEX->m_lLow = 1230;
		pIEX->m_lLastClose = 234560;
		pIEX->m_lNew = 230;
		pIEX->m_llVolume = 100000;
		stock.UpdateRTData(pIEX);
		EXPECT_EQ(stock.GetTransactionTime(), pIEX->m_llTimestamp);
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

		setTiingoStock.m_strFilter = _T("[Ticker] = '000001.US'");
		setTiingoStock.Open();
		EXPECT_TRUE(setTiingoStock.IsEOF()) << setTiingoStock.m_TiingoPermaTicker;
		setTiingoStock.Close();

		stock.m_strTiingoPermaTicker = _T("aasdfasdfj");
		stock.SetSymbol(_T("000001.US"));
		stock.m_strName = _T("adkjkf");
		stock.SetActive(true);
		stock.m_fIsADR = true;
		stock.m_iSicCode = 1234;
		stock.m_strSicIndustry = _T("defg");
		stock.m_strSicSector = _T("efg");
		stock.m_strTiingoIndustry = _T("ghi");
		stock.m_strTiingoSector = _T("defghijk");
		stock.m_strReportingCurrency = _T("US");
		stock.m_strLocation = _T("Irvine");
		stock.m_strCompanyWebSite = _T("ijk");
		stock.m_strSECFilingWebSite = _T("https://def.com");
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

		setTiingoStock.m_strFilter = _T("[Ticker] = '000001.US'");
		setTiingoStock.Open();
		setTiingoStock.m_pDatabase->BeginTrans();
		stock2.Load(setTiingoStock);
		setTiingoStock.Delete();
		setTiingoStock.m_pDatabase->CommitTrans(); // 必须使用CommitTrans()来真正删除数据库中的数据。
		setTiingoStock.Close();

		EXPECT_STREQ(stock.m_strTiingoPermaTicker, stock2.m_strTiingoPermaTicker);
		EXPECT_STREQ(stock.GetSymbol(), stock2.GetSymbol());
		EXPECT_STREQ(stock.m_strName, stock2.m_strName);
		EXPECT_TRUE(stock.IsActive() == stock2.IsActive());
		EXPECT_TRUE(stock.m_fIsADR == stock2.m_fIsADR);
		EXPECT_EQ(stock.m_iSicCode, stock2.m_iSicCode);
		EXPECT_STREQ(stock.m_strSicIndustry, stock2.m_strSicIndustry);
		EXPECT_STREQ(stock.m_strSicSector, stock2.m_strSicSector);
		EXPECT_STREQ(stock.m_strTiingoIndustry, stock2.m_strTiingoIndustry);
		EXPECT_STREQ(stock.m_strTiingoSector, stock2.m_strTiingoSector);
		EXPECT_STREQ(stock.m_strReportingCurrency, stock2.m_strReportingCurrency);
		EXPECT_STREQ(stock.m_strLocation, stock2.m_strLocation);
		EXPECT_STREQ(stock.m_strCompanyWebSite, stock2.m_strCompanyWebSite);
		EXPECT_STREQ(stock.m_strSECFilingWebSite, stock2.m_strSECFilingWebSite);
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
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(19800101); // 这个日期早于数据库中的最早日期，需要添加进数据库
		pDayLine->SetClose(115);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210101); // 这个日期为新日期，需要添加进数据库
		pDayLine->SetClose(12340);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210107); // 这个数据库中有，无需添加
		pDayLine->SetClose(10020);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20241111); // 这个日期为新日期，需要添加进数据库
		pDayLine->SetClose(135);
		pvDayLine->push_back(pDayLine);

		stock.SetSymbol(_T("A"));
		stock.SetDayLineEndDate(20210107);
		stock.UpdateDayLine(pvDayLine);

		stock.SaveDayLineDB();

		setDayLine.m_strFilter = _T("[Symbol] = 'A'");
		setDayLine.m_strSort = _T("[Date]");
		setDayLine.Open();
		setDayLine.m_pDatabase->BeginTrans();
		EXPECT_TRUE(setDayLine.m_Date == 19800101);
		EXPECT_STREQ(setDayLine.m_Close, _T("0.0115"));
		setDayLine.Delete();
		while (setDayLine.m_Date != 20210101) setDayLine.MoveNext();
		EXPECT_STREQ(setDayLine.m_Close, _T("1.2340"));
		setDayLine.Delete();
		while (setDayLine.m_Date != 20241111) setDayLine.MoveNext();
		EXPECT_TRUE(setDayLine.m_Date = 20241111);
		EXPECT_STREQ(setDayLine.m_Close, _T("0.0135"));
		setDayLine.Delete();
		setDayLine.m_pDatabase->CommitTrans();
		setDayLine.Close();
	}

	TEST_F(CTiingoStockTest, TestHaveNewDayLineData) {
		auto pvDayLine = make_shared<vector<CTiingoDayLinePtr>>();
		//CSetTiingoStockDayLine setDayLine;

		EXPECT_EQ(stock.GetDayLineSize(), 0);
		EXPECT_FALSE(stock.HaveNewDayLineData()) << "没有日线数据";

		CTiingoDayLinePtr pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210101); // 这个需要添加进数据库
		pDayLine->SetClose(10010);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210102); // 这个需要添加进数据库
		pDayLine->SetClose(12345);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210107); // 这个数据库中有，无需添加
		pDayLine->SetClose(10020);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210123); // 这个需要添加进数据库
		pDayLine->SetClose(10030);
		pvDayLine->push_back(pDayLine);

		stock.SetSymbol(_T("A"));
		stock.SetDayLineEndDate(20210107);
		stock.UpdateDayLine(pvDayLine);

		EXPECT_EQ(stock.GetDayLineSize(), 4);

		pDayLine = stock.GetDayLine(1);
		EXPECT_EQ(pDayLine->GetMarketDate(), 20210102);

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
		stock.SetSymbol(_T("AAPL"));
		CTiingoCompanyFinancialStatesPtr pvState = make_shared<vector<CTiingoCompanyFinancialStatePtr>>();
		CTiingoCompanyFinancialStatePtr pState = make_shared<CTiingoCompanyFinancialState>();
		pState->m_symbol = _T("AAPL");
		pState->m_yearQuarter = 202001; // 早于最早的数据
		pState->m_exchange = _T("Test"); // 虚假交易所代码，用于删除
		pvState->push_back(pState);
		pState = make_shared<CTiingoCompanyFinancialState>();
		pState->m_symbol = _T("AAPL");
		pState->m_yearQuarter = 202301; // 存在于集合中
		pState->m_exchange = _T("Test");
		pvState->push_back(pState);
		pState = make_shared<CTiingoCompanyFinancialState>();
		pState->m_symbol = _T("AAPL");
		pState->m_yearQuarter = 202305; // 居中新数据
		pState->m_exchange = _T("Test");
		pvState->push_back(pState);
		pState = make_shared<CTiingoCompanyFinancialState>();
		pState->m_symbol = _T("AAPL");
		pState->m_yearQuarter = 202404; // 晚于最新数据
		pState->m_exchange = _T("Test");
		pvState->push_back(pState);

		stock.UpdateFinancialState(pvState);

		stock.UpdateFinancialStateDB();

		CSetTiingoCompanyFinancialState setState;

		setState.m_strFilter = _T("[Exchange] = 'Test'");
		setState.m_strSort = _T("[yearQuarter]");
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
		//pStock->SetSymbol(_T("AAPL"));
		//pStock->ProcessDayLine();

		auto pStock3 = make_shared<CTiingoStock>();
		pStock3->SetSymbol(_T("ACI"));
		pStock3->ProcessDayLine3();

		auto pStock2 = make_shared<CTiingoStock>();
		pStock2->SetSymbol(_T("ACI"));
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
		CString str = pStock2->GetSymbol();

		//
	}

	TEST_F(CTiingoStockTest, TestProcessDayLine2) {
		//auto pStock = make_shared<CTiingoStock>();
		//pStock->SetSymbol(_T("A"));
		//pStock->ProcessDayLine();

		auto pStock3 = make_shared<CTiingoStock>();
		pStock3->SetSymbol(_T("A"));
		pStock3->ProcessDayLine3();

		auto pStock2 = make_shared<CTiingoStock>();
		pStock2->SetSymbol(_T("A"));
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
		CString str = pStock2->GetSymbol();

		//
	}
}
