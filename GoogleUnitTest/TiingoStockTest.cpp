#include"pch.h"

#include"GeneralCheck.h"
#include "SetTiingoStockDayLine.h"

#include"TiingoStock.h"
#include "TimeConvert.h"
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
		EXPECT_FALSE(tiingo.m_fIsActive);
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
		EXPECT_EQ(tiingo.GetCompanyFinancialStatementUpdateDate(), 19800101);
		EXPECT_EQ(tiingo.GetDailyDataUpdateDate(), 19800101);
	}

	TEST_F(CTiingoStockTest, TestResetAllUpdateDate) {
		stock.ResetAllUpdateDate();
		EXPECT_EQ(stock.GetCompanyProfileUpdateDate(), 19800101);
		EXPECT_EQ(stock.GetDailyDataUpdateDate(), 19800101);
		EXPECT_EQ(stock.GetDayLineUpdateDate(), 19800101);
		EXPECT_EQ(stock.GetCompanyFinancialStatementUpdateDate(), 19800101);
	}

	TEST_F(CTiingoStockTest, TestGetRatio) {
		EXPECT_EQ(stock.GetRatio(), 1000) << "国际市场的股票价格，放大倍数为1000";
	}

	TEST_F(CTiingoStockTest, TestGetExchangeCode) {
		EXPECT_STREQ(stock.GetExchangeCode(), _T(""));
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
		EXPECT_EQ(stock.GetDayLineEndDate(), 0);
		stock.SetDayLineEndDate(19980101);
		EXPECT_EQ(stock.GetDayLineEndDate(), 19980101);
	}

	TEST_F(CTiingoStockTest, TestGetDayLineStartDate) {
		EXPECT_EQ(stock.GetDayLineStartDate(), 29900101);
		stock.SetDayLineStartDate(19980101);
		EXPECT_EQ(stock.GetDayLineStartDate(), 19980101);
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

	TEST_F(CTiingoStockTest, TestGetProfileUpdateDate) {
		EXPECT_EQ(stock.GetCompanyProfileUpdateDate(), 19800101);
		stock.SetCompanyProfileUpdateDate(10101016);
		EXPECT_EQ(stock.GetCompanyProfileUpdateDate(), 10101016);
	}

	TEST_F(CTiingoStockTest, TestUpdateDayLineStartEndDate) {
		vector<CDayLinePtr> vDayLine;
		CSetTiingoStockDayLine setDayLine;

		CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210101); // 这个需要添加进数据库
		pDayLine->SetClose(10010);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210102); // 这个需要添加进数据库
		pDayLine->SetClose(12345);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210107); // 这个数据库中有，无需添加
		pDayLine->SetClose(10020);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210123); // 这个需要添加进数据库
		pDayLine->SetClose(10030);
		vDayLine.push_back(pDayLine);

		stock.SetSymbol(_T("A"));
		stock.SetDayLineEndDate(20210107);
		stock.UpdateDayLine(vDayLine);

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

	TEST_F(CTiingoStockTest, TestCheckCheckDayLineUpdateStatus1) {
		stock.SetUpdateDayLine(true);
		stock.SetActive(false);
		for (int i = 1; i < 6; i++) {
			EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "非活跃股票工作日不更新日线\n";
			stock.SetUpdateDayLine(true);
		}
	}

	TEST_F(CTiingoStockTest, TestCheckCheckDayLineUpdateStatus2) {
		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_NULL_);
		EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, 0)) << "无效股票不检查日线\n";
	}

	TEST_F(CTiingoStockTest, TestCheckCheckDayLineUpdateStatus3) {
		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_DELISTED_);
		for (int i = 0; i < 7; i++) {
			if (i == 3)
				EXPECT_TRUE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "摘牌股票只在星期三检查日线\n";
			else
				EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "摘牌股票只在星期三检查日线\n";
			stock.SetUpdateDayLine(true);
		}
	}

	TEST_F(CTiingoStockTest, TestCheckCheckDayLineUpdateStatus4) {
		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetActive(true);
		stock.SetDayLineEndDate(GetPrevDay(gl_pWorldMarket->GetMarketDate(), 100));
		EXPECT_TRUE(stock.CheckDayLineUpdateStatus(gl_pWorldMarket->GetMarketDate(), gl_pWorldMarket->GetMarketDate(), 0, 1));
		stock.SetDayLineEndDate(GetPrevDay(stock.GetDayLineEndDate()));
		EXPECT_FALSE(stock.CheckDayLineUpdateStatus(gl_pWorldMarket->GetMarketDate(), gl_pWorldMarket->GetMarketDate(), 0, 1)) << "早于100天的股票不再更新日线";
	}

	TEST_F(CTiingoStockTest, TestCheckCheckDayLineUpdateStatus5) {
		const long lCurrentDay = gl_pWorldMarket->GetMarketDate();
		const long lPrevDay = GetPrevDay(lCurrentDay);

		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetActive(true);
		stock.SetDayLineEndDate(lCurrentDay); // 本日交易日日线已接收
		for (int i = 1; i < 6; i++) {
			EXPECT_FALSE(stock.CheckDayLineUpdateStatus(lCurrentDay, lPrevDay, 170001, i)) << "时间晚于17时后，检查当天日线";
			stock.SetUpdateDayLine(true); // 重置状态
		}
		stock.SetDayLineEndDate(lPrevDay); // 本日交易日日线尚未接收
		for (int i = 1; i < 6; i++) { EXPECT_TRUE(stock.CheckDayLineUpdateStatus(lCurrentDay, lPrevDay, 170001, i)) << "时间晚于17时后，检查当天日线"; }
	}

	TEST_F(CTiingoStockTest, TestCheckCheckDayLineUpdateStatus6) {
		const long lCurrentDay = gl_pWorldMarket->GetMarketDate();
		const long lPrevDay = GetPrevDay(lCurrentDay);

		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetActive(true);
		stock.SetDayLineEndDate(GetPrevDay(lCurrentDay)); // 上一交易日日线数据已接收
		for (int i = 1; i < 6; i++) {
			EXPECT_FALSE(stock.CheckDayLineUpdateStatus(lCurrentDay, lPrevDay, 170000, i)) << "时间不晚于17时，检查上一交易日日线 " << i;
			stock.SetUpdateDayLine(true); // 重置之
		}
		stock.SetDayLineEndDate(GetPrevDay(lCurrentDay, 2)); // 上一交易日日线数据未接收
		for (int i = 1; i < 6; i++) { EXPECT_TRUE(stock.CheckDayLineUpdateStatus(lCurrentDay, lPrevDay, 170000, i)) << "时间不晚于17时，检查上一交易日日线"; }
	}

	TEST_F(CTiingoStockTest, TestCheckCheckDayLineUpdateStatus7) {
		const long lCurrentDate = gl_pWorldMarket->GetMarketDate();
		const long lPrevMonday = GetPrevMonday(lCurrentDate);

		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetActive(true);
		stock.SetDayLineEndDate(GetPrevDay(lPrevMonday, 3)); // 上一交易日日线数据已接收
		EXPECT_FALSE(stock.CheckDayLineUpdateStatus(GetPrevDay(lPrevMonday, 2), GetPrevDay(lPrevMonday, 3), 170000, 6)) << "周六，检查上一交易日日线";
		stock.SetUpdateDayLine(true); // 重置之
		EXPECT_FALSE(stock.CheckDayLineUpdateStatus(GetPrevDay(lPrevMonday, 1), GetPrevDay(lPrevMonday, 3), 170000, 0)) << "周日，检查上一交易日日线";
	}

	TEST_F(CTiingoStockTest, TestCheckCheckDayLineUpdateStatus8) {
		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_NOT_YET_LIST_);
		for (int i = 0; i < 7; i++) {
			if (i == 3)
				EXPECT_TRUE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "摘牌股票只在星期三检查日线\n";
			else
				EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "摘牌股票只在星期三检查日线\n";
			stock.SetUpdateDayLine(true);
		}
	}

	TEST_F(CTiingoStockTest, TestSave) {
		CTiingoStock stock, stock2;
		CSetTiingoStock setTiingoStock;

		stock.m_strTiingoPermaTicker = _T("aasdfasdfj");
		stock.SetSymbol(_T("000001.US"));
		stock.m_strName = _T("adkjkf");
		stock.m_fIsActive = true;
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
		stock.SetDailyDataUpdateDate(10101010);

		setTiingoStock.Open();
		stock.Append(setTiingoStock);
		setTiingoStock.Close();

		setTiingoStock.m_strFilter = _T("[Ticker] = '000001.US'");
		setTiingoStock.Open();
		stock2.Load(setTiingoStock);
		setTiingoStock.Delete();
		setTiingoStock.Close();

		EXPECT_STREQ(stock.m_strTiingoPermaTicker, stock2.m_strTiingoPermaTicker);
		EXPECT_STREQ(stock.GetSymbol(), stock2.GetSymbol());
		EXPECT_STREQ(stock.m_strName, stock2.m_strName);
		EXPECT_TRUE(stock.m_fIsActive == stock2.m_fIsActive);
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
		EXPECT_EQ(stock.GetDailyDataUpdateDate(), stock2.GetDailyDataUpdateDate());
	}

	TEST_F(CTiingoStockTest, TestSaveDayLine) {
		vector<CDayLinePtr> vDayLine;
		CSetTiingoStockDayLine setDayLine;

		CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20200101); // 这个日期早于数据库中的最早日期，需要添加进数据库
		pDayLine->SetClose(10010);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210101); // 这个日期为新日期，需要添加进数据库
		pDayLine->SetClose(12345);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210107); // 这个数据库中有，无需添加
		pDayLine->SetClose(10020);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210123); // 这个日期为新日期，需要添加进数据库
		pDayLine->SetClose(10030);
		vDayLine.push_back(pDayLine);

		stock.SetSymbol(_T("A"));
		stock.SetDayLineEndDate(20210107);
		stock.UpdateDayLine(vDayLine);

		stock.SaveDayLine();

		setDayLine.m_strFilter = _T("[Symbol] = 'A'");
		setDayLine.m_strSort = _T("[Date]");
		setDayLine.Open();
		setDayLine.m_pDatabase->BeginTrans();
		EXPECT_TRUE(setDayLine.m_Date == 20200101);
		EXPECT_STREQ(setDayLine.m_Close, _T("10.010"));
		setDayLine.Delete();
		while (setDayLine.m_Date != 20210101) setDayLine.MoveNext();
		EXPECT_STREQ(setDayLine.m_Close, _T("12.345"));
		setDayLine.Delete();
		while (setDayLine.m_Date != 20210123) setDayLine.MoveNext();
		EXPECT_TRUE(setDayLine.m_Date = 20210123);
		EXPECT_STREQ(setDayLine.m_Close, _T("10.030"));
		setDayLine.Delete();
		setDayLine.m_pDatabase->CommitTrans();
		setDayLine.Close();
	}

	TEST_F(CTiingoStockTest, TestHaveNewDayLineData) {
		vector<CDayLinePtr> vDayLine;
		//CSetTiingoStockDayLine setDayLine;

		EXPECT_EQ(stock.GetDayLineSize(), 0);
		EXPECT_FALSE(stock.HaveNewDayLineData()) << "没有日线数据";

		CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210101); // 这个需要添加进数据库
		pDayLine->SetClose(10010);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210102); // 这个需要添加进数据库
		pDayLine->SetClose(12345);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210107); // 这个数据库中有，无需添加
		pDayLine->SetClose(10020);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210123); // 这个需要添加进数据库
		pDayLine->SetClose(10030);
		vDayLine.push_back(pDayLine);

		stock.SetSymbol(_T("A"));
		stock.SetDayLineEndDate(20210107);
		stock.UpdateDayLine(vDayLine);

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
}
