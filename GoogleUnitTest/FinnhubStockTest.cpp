#include"pch.h"

#include "dataBaseConnector.h"
#include"GeneralCheck.h"

#include"TimeConvert.h"

#include"InsiderSentiment.h"
#include"InsiderTransaction.h"
#include"EPSSurprise.h"

#include"FinnhubStock.h"
#include"WorldMarket.h"

#include"SetFinnhubStockDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubStockTest : public Test {
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
			pStock = nullptr;
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CFinnhubStockPtr pStock;
		CFinnhubStock stock;
	};

	TEST_F(CFinnhubStockTest, TestResetAllUpdateDate) {
		stock.ResetAllUpdateDate();
		EXPECT_EQ(stock.GetProfileUpdateDate(), 19800101);
		EXPECT_EQ(stock.GetCompanyNewsUpdateDate(), 19800101);
		EXPECT_EQ(stock.GetBasicFinancialUpdateDate(), 19800101);
		EXPECT_EQ(stock.GetLastRTDataUpdateDate(), 19800101);
		EXPECT_EQ(stock.GetPeerUpdateDate(), 19800101);
		EXPECT_EQ(stock.GetInsiderTransactionUpdateDate(), 19800101);
		EXPECT_EQ(stock.GetInsiderSentimentUpdateDate(), 19800101);
		EXPECT_EQ(stock.GetLastEPSSurpriseUpdateDate(), 19800101);
	}

	TEST_F(CFinnhubStockTest, TestGetRatio) {
		EXPECT_EQ(stock.GetRatio(), 1000) << "国际市场的股票价格，放大倍数为1000";
	}

	TEST_F(CFinnhubStockTest, TestGetExchangeCode) {
		EXPECT_EQ(stock.GetExchangeCode(), "US");
		stock.SetExchangeCode("SS");
		EXPECT_EQ(stock.GetExchangeCode(), "SS");
	}

	TEST_F(CFinnhubStockTest, TestGetSymbol) {
		EXPECT_EQ(stock.GetSymbol(), "");
		stock.SetSymbol("600000.SS");
		EXPECT_EQ(stock.GetSymbol(), "600000.SS");
	}

	TEST_F(CFinnhubStockTest, TestGetTransactionTime) {
		EXPECT_EQ(stock.GetTransactionTime(), 0);
		stock.SetTransactionTime(1010101010);
		EXPECT_EQ(stock.GetTransactionTime(), 1010101010);
	}

	TEST_F(CFinnhubStockTest, TestGetLastClose) {
		EXPECT_EQ(stock.GetLastClose(), 0);
		stock.SetLastClose(10101010);
		EXPECT_EQ(stock.GetLastClose(), 10101010);
	}

	TEST_F(CFinnhubStockTest, TestGetOpen) {
		EXPECT_EQ(stock.GetOpen(), 0);
		stock.SetOpen(10101);
		EXPECT_EQ(stock.GetOpen(), 10101);
	}

	TEST_F(CFinnhubStockTest, TestGetHigh) {
		EXPECT_EQ(stock.GetHigh(), 0);
		stock.SetHigh(19980101);
		EXPECT_EQ(stock.GetHigh(), 19980101);
	}

	TEST_F(CFinnhubStockTest, TestGetLow) {
		EXPECT_EQ(stock.GetLow(), 0);
		stock.SetLow(19980101);
		EXPECT_EQ(stock.GetLow(), 19980101);
	}

	TEST_F(CFinnhubStockTest, TestGetNew) {
		EXPECT_EQ(stock.GetNew(), 0);
		stock.SetNew(10101010);
		EXPECT_EQ(stock.GetNew(), 10101010);
	}

	TEST_F(CFinnhubStockTest, TestGetUpDown) {
		EXPECT_EQ(stock.GetUpDown(), 0);
		stock.SetUpDown(10101010);
		EXPECT_EQ(stock.GetUpDown(), 10101010);
	}

	TEST_F(CFinnhubStockTest, TestGetAmount) {
		EXPECT_EQ(stock.GetAmount(), 0);
		stock.SetAmount(1010101010101010);
		EXPECT_EQ(stock.GetAmount(), 1010101010101010);
	}

	TEST_F(CFinnhubStockTest, TestGetVolume) {
		EXPECT_EQ(stock.GetVolume(), 0);
		stock.SetVolume(10101010);
		EXPECT_EQ(stock.GetVolume(), 10101010);
	}

	TEST_F(CFinnhubStockTest, TestGetTotalValue) {
		EXPECT_EQ(stock.GetTotalValue(), 0);
		stock.SetTotalValue(10101010);
		EXPECT_EQ(stock.GetTotalValue(), 10101010);
	}

	TEST_F(CFinnhubStockTest, TestGetCurrentValue) {
		EXPECT_EQ(stock.GetCurrentValue(), 0);
		stock.SetCurrentValue(10101010);
		EXPECT_EQ(stock.GetCurrentValue(), 10101010);
	}

	TEST_F(CFinnhubStockTest, TestGetChangeHandRate) {
		EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 0.0);
		stock.SetChangeHandRate(1.1);
		EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 1.1);
	}

	TEST_F(CFinnhubStockTest, TestGetUpDownRate) {
		EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 0.0);
		stock.SetUpDownRate(1.1);
		EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 1.1);
	}

	TEST_F(CFinnhubStockTest, TestIsTodayNewStock) {
		EXPECT_FALSE(stock.IsTodayNewStock());
		stock.SetTodayNewStock(true);
		EXPECT_TRUE(stock.IsTodayNewStock());
		stock.SetTodayNewStock(false);
		EXPECT_FALSE(stock.IsTodayNewStock());
	}

	TEST_F(CFinnhubStockTest, TestIsUpdateStockProfileDB) {
		EXPECT_FALSE(stock.IsUpdateProfileDB());
		stock.SetUpdateProfileDB(true);
		EXPECT_TRUE(stock.IsUpdateProfileDB());
		stock.SetUpdateProfileDB(false);
		EXPECT_FALSE(stock.IsUpdateProfileDB());
	}

	TEST_F(CFinnhubStockTest, TestIsUpdateCompanyNewsDB) {
		EXPECT_FALSE(stock.IsUpdateCompanyNewsDB());
		stock.SetUpdateCompanyNewsDB(true);
		EXPECT_TRUE(stock.IsUpdateCompanyNewsDB());
		stock.SetUpdateCompanyNewsDB(false);
		EXPECT_FALSE(stock.IsUpdateCompanyNewsDB());
	}

	TEST_F(CFinnhubStockTest, TestIsActive) {
		EXPECT_FALSE(stock.IsActive());
		stock.SetActive(true);
		EXPECT_TRUE(stock.IsActive());
		stock.SetActive(false);
		EXPECT_FALSE(stock.IsActive());
	}

	TEST_F(CFinnhubStockTest, TestGetDayLineEndDate) {
		EXPECT_EQ(stock.GetDayLineEndDate(), 19800101);
		stock.SetDayLineEndDate(19980102);
		EXPECT_EQ(stock.GetDayLineEndDate(), 19980102);
	}

	TEST_F(CFinnhubStockTest, TestGetDayLineStartDate) {
		EXPECT_EQ(stock.GetDayLineStartDate(), 29900101);
		stock.SetDayLineStartDate(19980101);
		EXPECT_EQ(stock.GetDayLineStartDate(), 19980101);
	}

	TEST_F(CFinnhubStockTest, TestGetIPOStatus) {
		EXPECT_TRUE(stock.IsNotChecked());
		stock.SetIPOStatus(255);
		EXPECT_EQ(stock.GetIPOStatus(), 255);
	}

	TEST_F(CFinnhubStockTest, TestIsNullStock) {
		stock.SetIPOStatus(_STOCK_NULL_);
		EXPECT_TRUE(stock.IsNullStock());
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_FALSE(stock.IsNullStock());
	}

	TEST_F(CFinnhubStockTest, TestIsIPOed) {
		stock.SetIPOStatus(_STOCK_IPOED_);
		EXPECT_TRUE(stock.IsIPOed());
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_FALSE(stock.IsIPOed());
	}

	TEST_F(CFinnhubStockTest, TestIsNotChecked) {
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_TRUE(stock.IsNotChecked());
		stock.SetIPOStatus(_STOCK_DELISTED_);
		EXPECT_FALSE(stock.IsNotChecked());
	}

	TEST_F(CFinnhubStockTest, TestIsDelisted) {
		stock.SetIPOStatus(_STOCK_DELISTED_);
		EXPECT_TRUE(stock.IsDelisted());
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_FALSE(stock.IsDelisted());
	}

	TEST_F(CFinnhubStockTest, TestIsNotYetList) {
		stock.SetIPOStatus(_STOCK_NOT_YET_LIST_);
		EXPECT_TRUE(stock.IsNotYetList());
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_FALSE(stock.IsNotYetList());
	}

	TEST_F(CFinnhubStockTest, TestIsUpdateDayLine) {
		EXPECT_TRUE(stock.IsUpdateDayLine());
		stock.SetUpdateDayLine(false);
		EXPECT_FALSE(stock.IsUpdateDayLine());
		stock.SetUpdateDayLine(true);
		EXPECT_TRUE(stock.IsUpdateDayLine());
	}

	TEST_F(CFinnhubStockTest, TestIsDayLineNeededSaving) {
		// 此两个函数是具备同步机制的，这里没有进行测试
		EXPECT_FALSE(stock.IsUpdateDayLineDB());
		stock.SetUpdateDayLineDB(true);
		EXPECT_TRUE(stock.IsUpdateDayLineDB());
		stock.SetUpdateDayLineDB(false);
		EXPECT_FALSE(stock.IsUpdateDayLineDB());
	}

	TEST_F(CFinnhubStockTest, TestGetProfileUpdateDate) {
		EXPECT_EQ(stock.GetProfileUpdateDate(), 19800101);
		stock.SetProfileUpdateDate(10101016);
		EXPECT_EQ(stock.GetProfileUpdateDate(), 10101016);
	}

	TEST_F(CFinnhubStockTest, TestGetCompanyNewsUpdateDate) {
		EXPECT_EQ(stock.GetCompanyNewsUpdateDate(), 19800101);
		stock.SetCompanyNewsUpdateDate(10101017);
		EXPECT_EQ(stock.GetCompanyNewsUpdateDate(), 10101017);
	}

	TEST_F(CFinnhubStockTest, TestGetBasicFinancialUpdateDate) {
		EXPECT_EQ(stock.GetBasicFinancialUpdateDate(), 19800101);
		stock.SetBasicFinancialUpdateDate(10101018);
		EXPECT_EQ(stock.GetBasicFinancialUpdateDate(), 10101018);
	}

	TEST_F(CFinnhubStockTest, TestGetLastRTDataUpdateDate) {
		EXPECT_EQ(stock.GetLastRTDataUpdateDate(), 19800101);
		stock.SetLastRTDataUpdateDate(10101011);
		EXPECT_EQ(stock.GetLastRTDataUpdateDate(), 10101011);
	}

	TEST_F(CFinnhubStockTest, TestPeerUpdateDate) {
		EXPECT_EQ(stock.GetPeerUpdateDate(), 19800101);
		stock.SetPeerUpdateDate(10101012);
		EXPECT_EQ(stock.GetPeerUpdateDate(), 10101012);
	}

	TEST_F(CFinnhubStockTest, TestInsiderTransactionUpdateDate) {
		EXPECT_EQ(stock.GetInsiderTransactionUpdateDate(), 19800101);
		stock.SetInsiderTransactionUpdateDate(10101013);
		EXPECT_EQ(stock.GetInsiderTransactionUpdateDate(), 10101013);
	}

	TEST_F(CFinnhubStockTest, TestGetInsiderSentimentUpdateDate) {
		EXPECT_EQ(stock.GetInsiderSentimentUpdateDate(), 19800101);
		stock.SetInsiderSentimentUpdateDate(10101014);
		EXPECT_EQ(stock.GetInsiderSentimentUpdateDate(), 10101014);
	}

	TEST_F(CFinnhubStockTest, TestLastEPSSurpriseUpdateDate) {
		EXPECT_EQ(stock.GetLastEPSSurpriseUpdateDate(), 19800101);
		stock.SetLastEPSSurpriseUpdateDate(10101015);
		EXPECT_EQ(stock.GetLastEPSSurpriseUpdateDate(), 10101015);
	}

	TEST_F(CFinnhubStockTest, TestIsUpdateCompanyProfile) {
		EXPECT_TRUE(stock.IsUpdateCompanyProfile());
		stock.SetUpdateCompanyProfile(false);
		EXPECT_FALSE(stock.IsUpdateCompanyProfile());
	}

	TEST_F(CFinnhubStockTest, TestIsUpdateEPSSurprise) {
		EXPECT_TRUE(stock.IsUpdateEPSSurprise());
		stock.SetUpdateEPSSurprise(false);
		EXPECT_FALSE(stock.IsUpdateEPSSurprise());
	}

	TEST_F(CFinnhubStockTest, TestIsUpdateEPSSurpriseDB) {
		EXPECT_FALSE(stock.IsUpdateEPSSurpriseDB());
		stock.SetUpdateEPSSurpriseDB(true);
		EXPECT_TRUE(stock.IsUpdateEPSSurpriseDB());
	}

	TEST_F(CFinnhubStockTest, TestIsUpdateSECFilingsDB) {
		EXPECT_FALSE(stock.IsUpdateSECFilingsDB());
		stock.SetUpdateSECFilingsDB(true);
		EXPECT_TRUE(stock.IsUpdateSECFilingsDB());
		stock.SetUpdateSECFilingsDB(false);
		EXPECT_FALSE(stock.IsUpdateSECFilingsDB());
	}

	TEST_F(CFinnhubStockTest, TestIsUpdateInsiderTransaction) {
		EXPECT_TRUE(stock.IsUpdateInsiderTransaction());
		stock.SetUpdateInsiderTransaction(false);
		EXPECT_FALSE(stock.IsUpdateInsiderTransaction());
		stock.SetUpdateInsiderTransaction(true);
		EXPECT_TRUE(stock.IsUpdateInsiderTransaction());
	}

	TEST_F(CFinnhubStockTest, TestIsInsiderTransactionNeedSave) {
		EXPECT_FALSE(stock.IsUpdateInsiderTransactionDB());
		stock.SetUpdateInsiderTransactionDB(true);
		EXPECT_TRUE(stock.IsUpdateInsiderTransactionDB());
	}

	TEST_F(CFinnhubStockTest, TestIsUpdateInsiderSentiment) {
		EXPECT_TRUE(stock.IsUpdateInsiderSentiment());
		stock.SetUpdateInsiderSentiment(false);
		EXPECT_FALSE(stock.IsUpdateInsiderSentiment());
		stock.SetUpdateInsiderSentiment(true);
		EXPECT_TRUE(stock.IsUpdateInsiderSentiment());
	}

	TEST_F(CFinnhubStockTest, TestIsInsiderSentimentNeedSave) {
		EXPECT_FALSE(stock.IsUpdateInsiderSentimentDB());
		stock.SetUpdateInsiderSentimentDB(true);
		EXPECT_TRUE(stock.IsUpdateInsiderSentimentDB());
	}

	TEST_F(CFinnhubStockTest, TestGetDescription) {
		EXPECT_EQ(stock.GetDescription(), "");
		stock.SetDescription("abcdef");
		EXPECT_EQ(stock.GetDescription(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetDisplaySymbol) {
		EXPECT_EQ(stock.GetDisplaySymbol(), "");
		stock.SetDisplaySymbol("abcdef");
		EXPECT_EQ(stock.GetDisplaySymbol(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetType) {
		EXPECT_EQ(stock.GetType(), " ");
		stock.SetType("abcdef");
		EXPECT_EQ(stock.GetType(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetMic) {
		EXPECT_EQ(stock.GetMic(), " ");
		stock.SetMic("abcdef");
		EXPECT_EQ(stock.GetMic(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetFigi) {
		EXPECT_EQ(stock.GetFigi(), " ");
		stock.SetFigi("abcdef");
		EXPECT_EQ(stock.GetFigi(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetCurrency) {
		EXPECT_EQ(stock.GetCurrency(), " ");
		stock.SetCurrency("abcdef");
		EXPECT_EQ(stock.GetCurrency(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetAddress) {
		EXPECT_EQ(stock.GetAddress(), " ");
		stock.SetAddress("abcdef");
		EXPECT_EQ(stock.GetAddress(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetCity) {
		EXPECT_EQ(stock.GetCity(), " ");
		stock.SetCity("abcdef");
		EXPECT_EQ(stock.GetCity(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetCountry) {
		EXPECT_EQ(stock.GetCountry(), " ");
		stock.SetCountry("abcdef");
		EXPECT_EQ(stock.GetCountry(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetCusip) {
		EXPECT_EQ(stock.GetCusip(), " ");
		stock.SetCusip("abcdef");
		EXPECT_EQ(stock.GetCusip(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetSedol) {
		EXPECT_EQ(stock.GetSedol(), " ");
		stock.SetSedol("abcdef");
		EXPECT_EQ(stock.GetSedol(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetEmployeeTotal) {
		EXPECT_EQ(stock.GetEmployeeTotal(), 0);
		stock.SetEmployeeTotal(1234567);
		EXPECT_EQ(stock.GetEmployeeTotal(), 1234567);
	}

	TEST_F(CFinnhubStockTest, TestGetGgroup) {
		EXPECT_EQ(stock.GetGgroup(), " ");
		stock.SetGgroup("abcdef");
		EXPECT_EQ(stock.GetGgroup(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetGind) {
		EXPECT_EQ(stock.GetGind(), " ");
		stock.SetGind("abcdef");
		EXPECT_EQ(stock.GetGind(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetGsector) {
		EXPECT_EQ(stock.GetGsector(), " ");
		stock.SetGsector("abcdef");
		EXPECT_EQ(stock.GetGsector(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetGsubind) {
		EXPECT_EQ(stock.GetGsubind(), " ");
		stock.SetGsubind("abcdef");
		EXPECT_EQ(stock.GetGsubind(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetIPODate) {
		EXPECT_EQ(stock.GetIPODate(), " ");
		stock.SetIPODate("abcdef");
		EXPECT_EQ(stock.GetIPODate(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetIsin) {
		EXPECT_EQ(stock.GetIsin(), " ");
		stock.SetIsin("abcdef");
		EXPECT_EQ(stock.GetIsin(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetMarketCapitalization) {
		EXPECT_DOUBLE_EQ(stock.GetMarketCapitalization(), 0.0);
		stock.SetMarketCapitalization(10101.010);
		EXPECT_DOUBLE_EQ(stock.GetMarketCapitalization(), 10101.010);
	}

	TEST_F(CFinnhubStockTest, TestGetNaics) {
		EXPECT_EQ(stock.GetNaics(), " ");
		stock.SetNaics("abcdef");
		EXPECT_EQ(stock.GetNaics(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetNaicsNationalIndustry) {
		EXPECT_EQ(stock.GetNaicsNationalIndustry(), " ");
		stock.SetNaicsNationalIndustry("abcdef");
		EXPECT_EQ(stock.GetNaicsNationalIndustry(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetNaicsSector) {
		EXPECT_EQ(stock.GetNaicsSector(), " ");
		stock.SetNaicsSector("abcdef");
		EXPECT_EQ(stock.GetNaicsSector(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetNaicsSubsector) {
		EXPECT_EQ(stock.GetNaicsSubsector(), " ");
		stock.SetNaicsSubsector("abcdef");
		EXPECT_EQ(stock.GetNaicsSubsector(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetName) {
		EXPECT_EQ(stock.GetName(), " ");
		stock.SetName("abcdef");
		EXPECT_EQ(stock.GetName(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetPhone) {
		EXPECT_EQ(stock.GetPhone(), " ");
		stock.SetPhone("abcdef");
		EXPECT_EQ(stock.GetPhone(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetShareOutstanding) {
		EXPECT_DOUBLE_EQ(stock.GetShareOutstanding(), 0.0);
		stock.SetShareOutstanding(10101.010);
		EXPECT_DOUBLE_EQ(stock.GetShareOutstanding(), 10101.010);
	}

	TEST_F(CFinnhubStockTest, TestGetState) {
		EXPECT_EQ(stock.GetState(), " ");
		stock.SetState("abcdef");
		EXPECT_EQ(stock.GetState(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetWebURL) {
		EXPECT_EQ(stock.GetWebURL(), " ");
		stock.SetWebURL("abcdef");
		EXPECT_EQ(stock.GetWebURL(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetLogo) {
		EXPECT_EQ(stock.GetLogo(), " ");
		stock.SetLogo("abcdef");
		EXPECT_EQ(stock.GetLogo(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetTicker) {
		EXPECT_EQ(stock.GetTicker(), " ");
		stock.SetTicker("abcdef");
		EXPECT_EQ(stock.GetTicker(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetFinnhubIndustry) {
		EXPECT_EQ(stock.GetFinnhubIndustry(), " ");
		stock.SetFinnhubIndustry("abcdef");
		EXPECT_EQ(stock.GetFinnhubIndustry(), "abcdef");
	}

	TEST_F(CFinnhubStockTest, TestGetPeer) {
		EXPECT_TRUE(stock.GetPeer().empty());
		const nlohmannJson jSon = nlohmannJson::parse(R"(["abcdef","000001.SS"])");
		stock.SetPeer(jSon);
		const nlohmannJson jSon2 = stock.GetPeer();
		const string s1 = jSon2.dump();
		EXPECT_EQ(s1, "[\"abcdef\",\"000001.SS\"]");
	}

	TEST_F(CFinnhubStockTest, TestIsUSMarket) {
		stock.SetExchangeCode("SS");
		EXPECT_FALSE(stock.IsUSMarket());
		stock.SetExchangeCode("US");
		EXPECT_TRUE(stock.IsUSMarket());
	}

	TEST_F(CFinnhubStockTest, TestCheckCheckDayLineUpdateStatus1) {
		stock.SetUpdateDayLine(true);
		stock.SetActive(false);
		for (int i = 1; i < 6; i++) {
			EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "非活跃股票工作日不更新日线\n";
			stock.SetUpdateDayLine(true);
		}
	}

	TEST_F(CFinnhubStockTest, TestCheckCheckDayLineUpdateStatus2) {
		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_NULL_);
		EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, 0)) << "无效股票不检查日线\n";
	}

	TEST_F(CFinnhubStockTest, TestCheckCheckDayLineUpdateStatus3) {
		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_DELISTED_);
		for (int i = 0; i < 7; i++) {
			if (i == 4)
				EXPECT_TRUE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "摘牌股票只在星期四检查日线\n";
			else
				EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "摘牌股票只在星期四检查日线\n";
			stock.SetUpdateDayLine(true);
		}
	}

	TEST_F(CFinnhubStockTest, TestCheckCheckDayLineUpdateStatus4) {
		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetActive(true);
		stock.SetDayLineEndDate(GetPrevDay(gl_pWorldMarket->GetMarketDate(), 100));
		EXPECT_TRUE(stock.CheckDayLineUpdateStatus(gl_pWorldMarket->GetMarketDate(), gl_pWorldMarket->GetMarketDate(), 0, 1));
		stock.SetDayLineEndDate(GetPrevDay(stock.GetDayLineEndDate()));
		EXPECT_FALSE(stock.CheckDayLineUpdateStatus(gl_pWorldMarket->GetMarketDate(), gl_pWorldMarket->GetMarketDate(), 0, 1)) << "早于100天的股票不再更新日线";
	}

	TEST_F(CFinnhubStockTest, TestCheckCheckDayLineUpdateStatus5) {
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

	TEST_F(CFinnhubStockTest, TestCheckCheckDayLineUpdateStatus6) {
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

	TEST_F(CFinnhubStockTest, TestCheckCheckDayLineUpdateStatus7) {
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

	TEST_F(CFinnhubStockTest, TestCheckCheckDayLineUpdateStatus8) {
		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_NOT_YET_LIST_);
		for (int i = 0; i < 7; i++) {
			if (i == 4)
				EXPECT_TRUE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "摘牌股票只在星期四检查日线\n";
			else
				EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "摘牌股票只在星期四检查日线\n";
			stock.SetUpdateDayLine(true);
		}
	}

	TEST_F(CFinnhubStockTest, TestCheckEPSSurpriseStatus) {
		constexpr long lCurrentDate = 20200101;

		stock.SetUpdateEPSSurprise(true);
		stock.SetIPOStatus(_STOCK_NULL_);
		stock.CheckEPSSurpriseStatus(lCurrentDate);
		EXPECT_FALSE(stock.IsUpdateEPSSurprise());

		stock.SetUpdateEPSSurprise(true);
		stock.SetIPOStatus(_STOCK_DELISTED_);
		stock.CheckEPSSurpriseStatus(lCurrentDate);
		EXPECT_FALSE(stock.IsUpdateEPSSurprise());

		stock.SetUpdateEPSSurprise(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetLastEPSSurpriseUpdateDate(19700101);
		stock.CheckEPSSurpriseStatus(lCurrentDate);
		EXPECT_FALSE(stock.IsUpdateEPSSurprise());

		stock.SetUpdateEPSSurprise(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetLastEPSSurpriseUpdateDate(20191003); // 不早于90天
		stock.CheckEPSSurpriseStatus(lCurrentDate);
		EXPECT_FALSE(stock.IsUpdateEPSSurprise());

		stock.SetUpdateEPSSurprise(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetLastEPSSurpriseUpdateDate(20191002); // 早于90天， 不早于900天
		stock.CheckEPSSurpriseStatus(lCurrentDate);
		EXPECT_TRUE(stock.IsUpdateEPSSurprise());

		stock.SetUpdateEPSSurprise(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetLastEPSSurpriseUpdateDate(20160521); // 早于900天
		stock.CheckEPSSurpriseStatus(lCurrentDate);
		EXPECT_FALSE(stock.IsUpdateEPSSurprise());
	}

	TEST_F(CFinnhubStockTest, TestCheckSECFilingsStatus) {
		constexpr long lCurrentDate = 20200101;

		stock.SetUpdateSECFilings(true);
		stock.SetIPOStatus(_STOCK_NULL_);
		stock.CheckSECFilingsStatus(lCurrentDate);
		EXPECT_FALSE(stock.IsUpdateSECFilings());

		stock.SetUpdateSECFilings(true);
		stock.SetIPOStatus(_STOCK_DELISTED_);
		stock.CheckSECFilingsStatus(lCurrentDate);
		EXPECT_FALSE(stock.IsUpdateSECFilings());

		stock.SetUpdateSECFilings(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetSECFilingsUpdateDate(20191202); // 不早于30天
		stock.CheckSECFilingsStatus(lCurrentDate);
		EXPECT_FALSE(stock.IsUpdateSECFilings());

		stock.SetUpdateSECFilings(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetSECFilingsUpdateDate(20191201); // 早于30天
		stock.CheckSECFilingsStatus(lCurrentDate);
		EXPECT_TRUE(stock.IsUpdateSECFilings());
	}

	TEST_F(CFinnhubStockTest, TestSaveDayLine) {
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();
		CSetFinnhubStockDayLine setDayLine;

		CDayLine dayLine;
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(19990101); // 这个日期早于数据库中的最早日期，需要添加进数据库
		dayLine.SetClose(10010);
		pvDayLine->push_back(dayLine);
		dayLine.Reset();
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210101); // 这个日期为新日期，需要添加进数据库
		dayLine.SetClose(12345);
		pvDayLine->push_back(dayLine);
		dayLine.Reset();
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210123); // 这个日期为新日期，需要添加进数据库
		dayLine.SetClose(10030);
		pvDayLine->push_back(dayLine);

		stock.SetSymbol("A");
		stock.SetDayLineEndDate(20210107);
		stock.UpdateDayLine(pvDayLine);

		stock.SaveDayLineDB();

		setDayLine.m_strFilter = "[Symbol] = 'A'";
		setDayLine.m_strSort = "[Date]";
		setDayLine.Open();
		setDayLine.m_pDatabase->BeginTrans();
		EXPECT_TRUE(setDayLine.m_Date == 19990101);
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

	TEST_F(CFinnhubStockTest, TestUpdateDayLineDB1) {
		EXPECT_FALSE(stock.IsUpdateDayLineDB());
		EXPECT_FALSE(stock.UpdateDayLineDB()) << "更新日线标识为假时，无需执行实际操作";
	}

	TEST_F(CFinnhubStockTest, TestUpdateDayLineDB2) {
		EXPECT_FALSE(stock.IsUpdateDayLineDB());
		stock.SetUpdateDayLineDB(true); // 需要更新
		EXPECT_EQ(stock.GetDayLineSize(), 0);

		EXPECT_FALSE(stock.UpdateDayLineDB()) << "日线数据为零时，无需执行实际操作";
		EXPECT_FALSE(stock.IsUpdateDayLineDB()) << "更新标识已被重置为假";
	}

	TEST_F(CFinnhubStockTest, TestUpdateDayLineDB3) {
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();
		CSetFinnhubStockDayLine setDayLine;

		EXPECT_FALSE(stock.IsUpdateDayLineDB());
		stock.SetUpdateDayLineDB(true); // 需要更新
		EXPECT_EQ(stock.GetDayLineSize(), 0);

		CDayLine dayLine;
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20200101); // 这个需要添加进数据库
		dayLine.SetClose(10010);
		pvDayLine->push_back(dayLine);
		dayLine = CDayLine();
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210101); // 这个需要添加进数据库
		dayLine.SetClose(12345);
		pvDayLine->push_back(dayLine);
		dayLine = CDayLine();
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210107); // 这个数据库中有，无需添加
		dayLine.SetClose(10020);
		pvDayLine->push_back(dayLine);
		dayLine = CDayLine();
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210123); // 这个需要添加进数据库
		dayLine.SetClose(10030);
		pvDayLine->push_back(dayLine);

		stock.SetSymbol("A");
		stock.SetDayLineStartDate(20200101); // 日线开始日期不晚于当前日线数据日期
		stock.SetDayLineEndDate(20210123); // 日线结束日期不早于当前日线数据日期
		stock.UpdateDayLine(pvDayLine);

		EXPECT_FALSE(stock.UpdateDayLineDB()) << "无需执行实际操作";
		EXPECT_FALSE(stock.IsUpdateDayLineDB()) << "更新标识已被重置为假";
		EXPECT_EQ(stock.GetDayLineSize(), 0) << "已清除日线数据";

		stock.SetUpdateDayLineDB(true); // 需要更新
		stock.SetDayLineStartDate(20200102); // 日线开始日期晚于当前日线数据日期
		stock.SetDayLineEndDate(20210123); // 日线结束日期不早于当前日线数据日期
		stock.UpdateDayLine(pvDayLine);

		EXPECT_TRUE(stock.UpdateDayLineDB()) << "执行实际操作";
		EXPECT_FALSE(stock.IsUpdateDayLineDB()) << "更新标识已被重置为假";
		EXPECT_EQ(stock.GetDayLineSize(), 0);
		EXPECT_EQ(stock.GetDayLineStartDate(), 20200101) << "日线开始日期已更新为较早日期";
		EXPECT_TRUE(stock.IsUpdateProfileDB());
		ASSERT_GE(gl_systemMessage.DayLineInfoSize(), 1);
		EXPECT_EQ(gl_systemMessage.PopDayLineInfoMessage(), "A日线资料存储完成");

		// 清除添加的数据，恢复原状
		setDayLine.m_strFilter = "[Symbol] = 'A'";
		setDayLine.m_strSort = "[Date]";
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

	TEST_F(CFinnhubStockTest, TestUpdateInsiderTransactionDB) {
		//  测试数据库中只有4个数据，股票代码：A， 内部交易人员：a b c，
		CInsiderTransactionsPtr pvInsiderTransaction = make_shared<vector<CInsiderTransaction>>();

		CInsiderTransaction insiderTransaction;
		insiderTransaction.m_strSymbol = "A";
		insiderTransaction.m_strPersonName = "a b c d";
		insiderTransaction.m_lTransactionDate = 20210101; // 这个内部交易人员名称不符，需要添加进数据库
		pvInsiderTransaction->push_back(insiderTransaction);
		insiderTransaction.m_strSymbol = "A";
		insiderTransaction.m_strPersonName = "a b c";
		insiderTransaction.m_lTransactionDate = 20210107;
		insiderTransaction.m_strTransactionCode = "M"; // 这个数据库中有，无需添加
		pvInsiderTransaction->push_back(insiderTransaction);
		insiderTransaction.m_strSymbol = "A";
		insiderTransaction.m_strPersonName = "a b c";
		insiderTransaction.m_lTransactionDate = 20210124; // 这个日期较新，需要添加进数据库
		pvInsiderTransaction->push_back(insiderTransaction);
		insiderTransaction.m_strSymbol = "A";
		insiderTransaction.m_lTransactionDate = 20210107;
		insiderTransaction.m_strPersonName = "a b c";
		insiderTransaction.m_strTransactionCode = "S"; // 这个交易类型不符，需要添加进数据库
		pvInsiderTransaction->push_back(insiderTransaction);

		stock.SetSymbol("A");
		stock.SetInsiderTransactionUpdateDate(20210123);
		stock.UpdateInsiderTransaction(pvInsiderTransaction);

		stock.UpdateInsiderTransactionDB();

		using namespace StockMarket;
		const auto& t = FinnhubInsiderTransaction{};
		{
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);

			auto result = db(select(all_of(t)).from(t).where(t.PersonName == "a b c d"));
			size_t rows = result.size();
			EXPECT_EQ(rows, 1);
			db(sqlpp::remove_from(t).where(t.PersonName == "a b c d"));
			tx.commit();
		}

		{
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);

			auto result = db(select(all_of(t)).from(t).where(t.TransactionDate == 20210124));
			size_t rows = result.size();
			EXPECT_EQ(rows, 1);
			db(sqlpp::remove_from(t).where(t.TransactionDate == 20210124));
			tx.commit();
		}

		{
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);

			auto result = db(select(all_of(t)).from(t).where(t.TransactionCode == "S"));
			size_t rows = result.size();
			EXPECT_EQ(rows, 1);
			db(sqlpp::remove_from(t).where(t.TransactionCode == "S"));
			tx.commit();
		}
	}

	TEST_F(CFinnhubStockTest, TestSaveInsiderSentiment) {
		//  测试数据库中只有4个数据，股票代码：A，
		CInsiderSentimentsPtr pvInsiderSentiment = make_shared<vector<CInsiderSentiment>>();
		CSetInsiderSentiment setInsiderSentiment;

		CInsiderSentiment insiderSentiment;
		insiderSentiment.m_strSymbol = "B"; // 这个股票代码不符，不可能出现此种情况，故而不需要添加进数据库
		insiderSentiment.m_lDate = 20200101;
		pvInsiderSentiment->push_back(insiderSentiment);
		insiderSentiment.m_strSymbol = "A";
		insiderSentiment.m_lDate = 20200101; // 这个数据库中有，无需添加
		pvInsiderSentiment->push_back(insiderSentiment);
		insiderSentiment.m_strSymbol = "A";
		insiderSentiment.m_lDate = 20210101; // 这个日期不符，需要添加进数据库
		pvInsiderSentiment->push_back(insiderSentiment);

		stock.SetSymbol("A");
		stock.SetInsiderSentimentUpdateDate(20210101);
		stock.UpdateInsiderSentiment(pvInsiderSentiment);

		stock.UpdateInsiderSentimentDB();

		setInsiderSentiment.m_strFilter = "[Symbol] = 'B'";
		setInsiderSentiment.Open();
		EXPECT_TRUE(setInsiderSentiment.IsEOF()) << "股票B没有存储进了数据库";
		setInsiderSentiment.Close();

		setInsiderSentiment.m_strFilter = "[Date] = '20210101'";
		setInsiderSentiment.Open();
		setInsiderSentiment.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderSentiment.IsEOF()) << "新日期存储进了数据库";
		setInsiderSentiment.Delete();
		setInsiderSentiment.m_pDatabase->CommitTrans();
		setInsiderSentiment.Close();
	}

	TEST_F(CFinnhubStockTest, TestUpdateCompanyNewsDB) {
		const auto pvCompanyNews = make_shared<vector<CFinnhubCompanyNews>>();
		CSetCompanyNews setCompanyNews;

		stock.SetSymbol("RIG");

		CFinnhubCompanyNews companyNews;
		companyNews.m_strCompanySymbol = "RIG";
		companyNews.m_llDateTime = 19800101;
		companyNews.m_strCategory = "test";
		companyNews.m_iNewsID = 4;
		pvCompanyNews->push_back(companyNews);
		companyNews.m_strCompanySymbol = "RIG";
		companyNews.m_llDateTime = 20200101;
		companyNews.m_strCategory = "test";
		companyNews.m_iNewsID = 5;
		pvCompanyNews->push_back(companyNews);

		stock.UpdateCompanyNews(pvCompanyNews);
		stock.UpdateCompanyNewsDB();

		int iCount = 0;
		setCompanyNews.m_strFilter = "[category] = 'test'";
		setCompanyNews.Open();
		setCompanyNews.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setCompanyNews.IsEOF()) << "已经加入了两个category为test的数据";
		while (!setCompanyNews.IsEOF()) {
			setCompanyNews.Delete();
			setCompanyNews.MoveNext();
			iCount++;
		}
		setCompanyNews.m_pDatabase->CommitTrans();
		setCompanyNews.Close();
		EXPECT_EQ(iCount, 2);
	}

	TEST_F(CFinnhubStockTest, TestUpdateDayLine) {
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();
		CSetFinnhubStockDayLine setDayLine;

		CDayLine dayLine;
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210101); // 这个需要添加进数据库
		dayLine.SetClose(10010);
		pvDayLine->push_back(dayLine);
		dayLine = CDayLine();
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210102); // 这个需要添加进数据库
		dayLine.SetClose(12345);
		pvDayLine->push_back(dayLine);
		dayLine = CDayLine();
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210107); // 这个数据库中有，无需添加
		dayLine.SetClose(10020);
		pvDayLine->push_back(dayLine);
		dayLine = CDayLine();
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210123); // 这个需要添加进数据库
		dayLine.SetClose(10030);
		pvDayLine->push_back(dayLine);

		stock.SetSymbol("A");
		stock.SetDayLineEndDate(20210107);
		stock.UpdateDayLine(pvDayLine);

		EXPECT_EQ(stock.GetDayLineSize(), 4);

		CDayLine* pDayLine = stock.GetDayLine(1);
		EXPECT_EQ(pDayLine->GetDate(), 20210102);

		stock.SetUpdateProfileDB(false);
		stock.SetDayLineStartDate(20210107);
		stock.SetDayLineEndDate(20210108);
		stock.UpdateDayLineStartEndDate();
		EXPECT_EQ(stock.GetDayLineStartDate(), 20210101);
		EXPECT_EQ(stock.GetDayLineEndDate(), 20210123);
		EXPECT_TRUE(stock.IsUpdateProfileDB());

		stock.UnloadDayLine();
		EXPECT_EQ(stock.GetDayLineSize(), 0);
	}

	TEST_F(CFinnhubStockTest, TestUpdateDayLineStartEndDate) {
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();
		CSetFinnhubStockDayLine setDayLine;

		CDayLine dayLine;
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210101); // 这个需要添加进数据库
		dayLine.SetClose(10010);
		pvDayLine->push_back(dayLine);
		dayLine = CDayLine();
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210102); // 这个需要添加进数据库
		dayLine.SetClose(12345);
		pvDayLine->push_back(dayLine);
		dayLine = CDayLine();
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210107); // 这个数据库中有，无需添加
		dayLine.SetClose(10020);
		pvDayLine->push_back(dayLine);
		dayLine = CDayLine();
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

	TEST_F(CFinnhubStockTest, TestUpdateEPSSurpriseDB1) {
		vector<CEPSSurprisePtr> vEPS;
		auto pEPS = make_shared<CEPSSurprise>();

		EXPECT_TRUE(stock.UpdateEPSSurpriseDB()) << "没有数据，返回";
	}

	TEST_F(CFinnhubStockTest, TestUpdateEPSSurpriseDB2) {
		CEPSSurprisesPtr pvEPS = make_shared<vector<CEPSSurprise>>();

		CEPSSurprise EPS;

		EPS.m_strSymbol = "600601.SS";
		EPS.m_lDate = 20200101;
		EPS.m_dActual = 1.0;
		EPS.m_dEstimate = 1.1;
		pvEPS->push_back(EPS);
		EPS.m_strSymbol = "600601.SS";
		EPS.m_lDate = 20200401;
		EPS.m_dActual = 2.0;
		EPS.m_dEstimate = 2.1;
		pvEPS->push_back(EPS);
		stock.UpdateEPSSurprise(pvEPS);

		stock.SetLastEPSSurpriseUpdateDate(20210101);
		EXPECT_FALSE(stock.UpdateEPSSurpriseDB()) << "没有新数据，返回假";
	}

	TEST_F(CFinnhubStockTest, TestUpdateEPSSurpriseDB3) {
		CEPSSurprisesPtr pvEPS = make_shared<vector<CEPSSurprise>>();

		CEPSSurprise EPS;

		EPS.m_strSymbol = "600601.US";
		EPS.m_lDate = 20200101;
		EPS.m_dActual = 1.0;
		EPS.m_dEstimate = 1.1;
		pvEPS->push_back(EPS);
		EPS.m_strSymbol = "600601.US";
		EPS.m_lDate = 20200401;
		EPS.m_dActual = 2.0;
		EPS.m_dEstimate = 2.1;
		pvEPS->push_back(EPS);
		stock.UpdateEPSSurprise(pvEPS);

		stock.SetLastEPSSurpriseUpdateDate(20200101);
		EXPECT_FALSE(stock.IsUpdateProfileDB());
		EXPECT_TRUE(stock.UpdateEPSSurpriseDB()) << "将新数据存入数据库(存储了一个新的)";
		EXPECT_TRUE(stock.IsUpdateProfileDB());
		EXPECT_EQ(stock.GetLastEPSSurpriseUpdateDate(), 20200401);

		using namespace StockMarket;
		const auto& t = FinnhubStockEstimatesEpsSurprise{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		auto result = db(select(all_of(t)).from(t).where(t.Symbol == "600601.US"));
		int rows = result.size();
		EXPECT_EQ(rows, 1);
		auto& row = result.front();
		EXPECT_DOUBLE_EQ(row.Actual, 2.0);
		EXPECT_DOUBLE_EQ(row.Estimate, 2.1);
		int date = row.Date;
		EXPECT_EQ(date, 20200401);
		EXPECT_EQ(row.Symbol, "600601.US");

		db(sqlpp::remove_from(t).where(t.Symbol == "600601.US"));
		tx.commit();
	}

	TEST_F(CFinnhubStockTest, TestUpdateSECFilingsDB1) {
		CSECFilingsPtr pvSECFilings = make_shared<vector<CSECFiling>>();
		CSECFiling SECFiling;
		SECFiling.m_strSymbol = "MFI"; // 已存在代码
		SECFiling.m_strAccessNumber = "0"; // 新存取号，比原有的都小
		pvSECFilings->push_back(SECFiling);
		SECFiling.m_strSymbol = "MFI"; // 已存在代码
		SECFiling.m_strAccessNumber = "0000814133-03-000033"; // 已存在存取号
		pvSECFilings->push_back(SECFiling);
		SECFiling.m_strSymbol = "MFI"; // 已存在代码
		SECFiling.m_strAccessNumber = "1000950135-08-002549"; // 新存取号，比原有的都大
		pvSECFilings->push_back(SECFiling);

		stock.SetSymbol("MFI");
		stock.SetSECFilings(pvSECFilings);
		stock.SetUpdateSECFilingsDB(true);
		stock.SetUpdateSECFilings(false);

		EXPECT_TRUE(stock.UpdateSECFilingsDB());

		CSetSECFilings setSECFilings;
		setSECFilings.m_strFilter = "[symbol] = 'MFI'";
		setSECFilings.m_strSort = "[accessNumber]";
		setSECFilings.Open();
		setSECFilings.m_pDatabase->BeginTrans();
		EXPECT_TRUE(setSECFilings.m_AccessNumber.Compare(_T("0")) == 0);
		setSECFilings.Delete();
		for (int i = 0; i < 11; i++) {
			setSECFilings.MoveNext(); // 测试库中原有10个，加上新存入的存取号为"0"这个。
		}
		EXPECT_FALSE(setSECFilings.IsEOF());
		EXPECT_STREQ(setSECFilings.m_AccessNumber, _T("1000950135-08-002549"));
		setSECFilings.Delete();
		setSECFilings.MoveNext();
		EXPECT_TRUE(setSECFilings.IsEOF());
		setSECFilings.m_pDatabase->CommitTrans();
		setSECFilings.Close();
	}

	TEST_F(CFinnhubStockTest, TestHaveNewDayLineData) {
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();
		CSetFinnhubStockDayLine setDayLine;

		EXPECT_EQ(stock.GetDayLineSize(), 0);
		EXPECT_FALSE(stock.HaveNewDayLineData()) << "没有日线数据";

		CDayLine dayLine;
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210101); // 这个需要添加进数据库
		dayLine.SetClose(10010);
		pvDayLine->push_back(dayLine);
		dayLine = CDayLine();
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210102); // 这个需要添加进数据库
		dayLine.SetClose(12345);
		pvDayLine->push_back(dayLine);
		dayLine = CDayLine();
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210107); // 这个数据库中有，无需添加
		dayLine.SetClose(10020);
		pvDayLine->push_back(dayLine);
		dayLine = CDayLine();
		dayLine.SetStockSymbol("A");
		dayLine.SetDate(20210123); // 这个需要添加进数据库
		dayLine.SetClose(10030);
		pvDayLine->push_back(dayLine);

		stock.SetSymbol("A");
		stock.SetDayLineEndDate(20210107);
		stock.UpdateDayLine(pvDayLine);

		EXPECT_EQ(stock.GetDayLineSize(), 4);

		CDayLine* pDayLine = stock.GetDayLine(1);
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

	TEST_F(CFinnhubStockTest, TestCheckPeerStatus) {
		EXPECT_TRUE(stock.IsUpdatePeer());

		stock.SetUpdatePeer(false);
		stock.SetPeerUpdateDate(20200101);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.CheckPeerStatus(20200401); // 91天
		EXPECT_TRUE(stock.IsUpdatePeer()) << "九十一天需更新";
		stock.CheckPeerStatus(20200331); // 90天
		EXPECT_FALSE(stock.IsUpdatePeer());

		stock.SetUpdatePeer(true);
		stock.SetIPOStatus(_STOCK_DELISTED_);
		stock.CheckPeerStatus(20200331); // 90天
		EXPECT_FALSE(stock.IsUpdatePeer()) << "九十天内无需更新";
		stock.CheckPeerStatus(20200401); // 91天
		EXPECT_FALSE(stock.IsUpdatePeer()) << "摘牌股票无需更新Peer";
	}

	TEST_F(CFinnhubStockTest, TestHaveInsiderTransaction) {
		CInsiderTransaction insiderTransaction;

		EXPECT_FALSE(stock.HaveInsiderTransaction());
		stock.m_vInsiderTransaction.push_back(insiderTransaction);
		EXPECT_TRUE(stock.HaveInsiderTransaction());
		stock.UnloadInsiderTransaction();
		EXPECT_FALSE(stock.HaveInsiderTransaction());
	}

	TEST_F(CFinnhubStockTest, TestCheckInsiderTransactionStatus) {
		EXPECT_TRUE(stock.IsUpdateInsiderTransaction());

		stock.SetUpdateInsiderTransaction(false);
		stock.SetInsiderTransactionUpdateDate(20200101);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.CheckInsiderTransactionStatus(20200201); // 31天
		EXPECT_TRUE(stock.IsUpdateInsiderTransaction()) << "三十一天需更新";
		stock.CheckInsiderTransactionStatus(20200131); // 30天
		EXPECT_FALSE(stock.IsUpdateInsiderTransaction());

		stock.SetUpdateInsiderTransaction(true);
		stock.SetIPOStatus(_STOCK_DELISTED_);
		stock.CheckInsiderTransactionStatus(20200131); // 30天
		EXPECT_FALSE(stock.IsUpdateInsiderTransaction()) << "三十天内无需更新";
		stock.CheckInsiderTransactionStatus(20200201); // 31天
		EXPECT_FALSE(stock.IsUpdateInsiderTransaction()) << "摘牌股票无需更新InsiderTransaction";
	}

	TEST_F(CFinnhubStockTest, TestHaveInsiderSentiment) {
		CInsiderSentiment insiderSentiment;

		EXPECT_FALSE(stock.HaveInsiderSentiment());
		stock.m_pvInsiderSentiment = make_shared<vector<CInsiderSentiment>>();
		stock.m_pvInsiderSentiment->push_back(insiderSentiment);
		EXPECT_TRUE(stock.HaveInsiderSentiment());
		stock.ClearInsiderSentiment();
		EXPECT_FALSE(stock.HaveInsiderSentiment());
	}

	TEST_F(CFinnhubStockTest, TestCheckInsiderSentimentStatus) {
		EXPECT_TRUE(stock.IsUpdateInsiderSentiment());

		stock.SetUpdateInsiderSentiment(false);
		stock.SetInsiderSentimentUpdateDate(20200101);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.CheckInsiderSentimentStatus(20200201); // 31天
		EXPECT_TRUE(stock.IsUpdateInsiderSentiment()) << "三十一天需更新";
		stock.CheckInsiderSentimentStatus(20200131); // 30天
		EXPECT_FALSE(stock.IsUpdateInsiderSentiment());

		stock.SetUpdateInsiderSentiment(true);
		stock.SetIPOStatus(_STOCK_DELISTED_);
		stock.CheckInsiderSentimentStatus(20200131); // 30天
		EXPECT_FALSE(stock.IsUpdateInsiderSentiment()) << "三十天内无需更新";
		stock.CheckInsiderSentimentStatus(20200201); // 31天
		EXPECT_FALSE(stock.IsUpdateInsiderSentiment()) << "摘牌股票无需更新InsiderSentiment";
	}

	TEST_F(CFinnhubStockTest, TestGetFinnhubDayLineInquiryParam) {
		constexpr long lDate = 20200101;

		const time_t tt = gl_pWorldMarket->TransferToUTCTime(lDate);
		time_t ttOld = tt - static_cast<time_t>(365) * 24 * 3600;

		string sTime = fmt::format("{:Ld}", tt);
		string sTimeOld = fmt::format("{:Ld}", ttOld);

		stock.SetSymbol("600601.SS");
		stock.SetDayLineEndDate(20180101); // 早于20190102
		string sMiddle2 = "600601.SS&resolution=D&from=" + sTimeOld + "&to=" + sTime;
		EXPECT_TRUE(stock.GetFinnhubDayLineInquiryParam(tt) == sMiddle2) << "免费账户最多只能申请一年的日线数据";
		EXPECT_EQ(stock.GetFinnhubDayLineInquiryParam(tt), sMiddle2) << "免费账户最多只能申请一年的日线数据";

		stock.SetSymbol("600601.SS");
		stock.SetDayLineEndDate(20190501); // 晚于20190102
		ttOld = gl_pWorldMarket->TransferToUTCTime(20190501);
		sTimeOld = fmt::format("{:Ld}", ttOld);
		string sParam = "600601.SS&resolution=D&from=" + sTimeOld + "&to=" + sTime;
		EXPECT_TRUE(stock.GetFinnhubDayLineInquiryParam(tt) == sMiddle2) << "检查一年的数据";
	}

	TEST_F(CFinnhubStockTest, TestGetTiingoDayLineInquiryParam) {
		stock.SetSymbol("600601.SS");
		const string strParam = "600601.SS/prices?&startDate=2018-1-1&endDate=2020-1-1";

		EXPECT_TRUE(stock.GetTiingoDayLineInquiryParam(20180101, 20200101) == strParam);
	}

	TEST_F(CFinnhubStockTest, TestGetTiingoDayLineInquiryParam2) {
		stock.SetSymbol("600601.SS");
		const string strParam = "600601.SS/prices?&startDate=1980-1-1&endDate=2020-1-1";

		EXPECT_TRUE(stock.GetTiingoDayLineInquiryParam(19800101, 20200101) == strParam);
	}

	TEST_F(CFinnhubStockTest, TestCheckCompanyNewsUpdated) {
		stock.SetShareOutstanding(1);
		stock.SetMarketCapitalization(2);
		stock.SetCompanyNewsUpdateDate(20220101);
		EXPECT_FALSE(stock.CheckCompanyNewsUpdateStatus(20220102));
		stock.SetUpdateCompanyNews(true);
		EXPECT_FALSE(stock.CheckCompanyNewsUpdateStatus(20220107)) << "只有六天";
		stock.SetUpdateCompanyNews(true);
		EXPECT_TRUE(stock.CheckCompanyNewsUpdateStatus(20220108)) << "每周检查一次公司新闻";
		stock.SetUpdateCompanyNews(true);
		EXPECT_TRUE(stock.CheckCompanyNewsUpdateStatus(20220110));
	}

	TEST_F(CFinnhubStockTest, TestCheckCompanyNewsUpdated2) {
		stock.SetShareOutstanding(0);
		stock.SetMarketCapitalization(0);
		stock.SetCompanyNewsUpdateDate(20220101);
		EXPECT_FALSE(stock.CheckCompanyNewsUpdateStatus(20220102));
		stock.SetUpdateCompanyNews(true);
		EXPECT_FALSE(stock.CheckCompanyNewsUpdateStatus(20220131)) << "只有六天";
		stock.SetUpdateCompanyNews(true);
		EXPECT_TRUE(stock.CheckCompanyNewsUpdateStatus(20220201)) << "未上市或退市股票每月检查一次公司新闻";
		stock.SetUpdateCompanyNews(true);
		EXPECT_TRUE(stock.CheckCompanyNewsUpdateStatus(20220202));
	}
}
