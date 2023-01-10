#include"pch.h"

#include"GeneralCheck.h"

#include"TimeConvert.h"

#include"InsiderSentiment.h"
#include"InsiderTransaction.h"
#include"EPSSurprise.h"

#include"WorldStock.h"
#include"WorldMarket.h"

#include"SetWorldStockDayLine.h"
#include"SetInsiderTransaction.h"

using namespace testing;

namespace FireBirdTest {
	class CWorldStockTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) { GeneralCheck(); }

		static void TearDownTestSuite(void) { GeneralCheck(); }

		void SetUp(void) override { pStock = nullptr; }

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CWorldStockPtr pStock;
	};

	TEST_F(CWorldStockTest, TestResetAllUpdateDate) {
		CWorldStock stock;
		stock.ResetAllUpdateDate();
		EXPECT_EQ(stock.GetProfileUpdateDate(), 19800101);
		EXPECT_EQ(stock.GetCompanyNewsUpdateDate(), 19800102);
		EXPECT_EQ(stock.GetBasicFinancialUpdateDate(), 19800103);
		EXPECT_EQ(stock.GetLastRTDataUpdateDate(), 19800104);
		EXPECT_EQ(stock.GetPeerUpdateDate(), 19800105);
		EXPECT_EQ(stock.GetInsiderTransactionUpdateDate(), 19800106);
		EXPECT_EQ(stock.GetInsiderSentimentUpdateDate(), 19800107);
		EXPECT_EQ(stock.GetLastEPSSurpriseUpdateDate(), 19800108);

		EXPECT_EQ(stock.GetTiingoStatementUpdateDate(), 19800101);
		EXPECT_EQ(stock.GetTiingoDailyDataUpdateDate(), 19800102);
	}

	TEST_F(CWorldStockTest, TestGetRatio) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetRatio(), 1000) << "国际市场的股票价格，放大倍数为1000";
	}

	TEST_F(CWorldStockTest, TestGetExchangeCode) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetExchangeCode(), _T("US"));
		stock.SetExchangeCode(_T("SS"));
		EXPECT_STREQ(stock.GetExchangeCode(), _T("SS"));
	}

	TEST_F(CWorldStockTest, TestGetSymbol) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetSymbol(), _T(""));
		stock.SetSymbol(_T("600000.SS"));
		EXPECT_STREQ(stock.GetSymbol(), _T("600000.SS"));
	}

	TEST_F(CWorldStockTest, TestGetTransactionTime) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetTransactionTime(), 0);
		stock.SetTransactionTime(1010101010);
		EXPECT_EQ(stock.GetTransactionTime(), 1010101010);
	}

	TEST_F(CWorldStockTest, TestGetLastClose) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetLastClose(), 0);
		stock.SetLastClose(10101010);
		EXPECT_EQ(stock.GetLastClose(), 10101010);
	}

	TEST_F(CWorldStockTest, TestGetOpen) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetOpen(), 0);
		stock.SetOpen(10101);
		EXPECT_EQ(stock.GetOpen(), 10101);
	}

	TEST_F(CWorldStockTest, TestGetHigh) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetHigh(), 0);
		stock.SetHigh(19980101);
		EXPECT_EQ(stock.GetHigh(), 19980101);
	}

	TEST_F(CWorldStockTest, TestGetLow) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetLow(), 0);
		stock.SetLow(19980101);
		EXPECT_EQ(stock.GetLow(), 19980101);
	}

	TEST_F(CWorldStockTest, TestGetNew) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetNew(), 0);
		stock.SetNew(10101010);
		EXPECT_EQ(stock.GetNew(), 10101010);
	}

	TEST_F(CWorldStockTest, TestGetUpDown) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetUpDown(), 0);
		stock.SetUpDown(10101010);
		EXPECT_EQ(stock.GetUpDown(), 10101010);
	}

	TEST_F(CWorldStockTest, TestGetAmount) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetAmount(), 0);
		stock.SetAmount(1010101010101010);
		EXPECT_EQ(stock.GetAmount(), 1010101010101010);
	}

	TEST_F(CWorldStockTest, TestGetVolume) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetVolume(), 0);
		stock.SetVolume(10101010);
		EXPECT_EQ(stock.GetVolume(), 10101010);
	}

	TEST_F(CWorldStockTest, TestGetTotalValue) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetTotalValue(), 0);
		stock.SetTotalValue(10101010);
		EXPECT_EQ(stock.GetTotalValue(), 10101010);
	}

	TEST_F(CWorldStockTest, TestGetCurrentValue) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetCurrentValue(), 0);
		stock.SetCurrentValue(10101010);
		EXPECT_EQ(stock.GetCurrentValue(), 10101010);
	}

	TEST_F(CWorldStockTest, TestGetChangeHandRate) {
		CWorldStock stock;
		EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 0.0);
		stock.SetChangeHandRate(1.1);
		EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 1.1);
	}

	TEST_F(CWorldStockTest, TestGetUpDownRate) {
		CWorldStock stock;
		EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 0.0);
		stock.SetUpDownRate(1.1);
		EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 1.1);
	}

	TEST_F(CWorldStockTest, TestIsTodayNewStock) {
		CWorldStock stock;

		EXPECT_FALSE(stock.IsTodayNewStock());
		stock.SetTodayNewStock(true);
		EXPECT_TRUE(stock.IsTodayNewStock());
		stock.SetTodayNewStock(false);
		EXPECT_FALSE(stock.IsTodayNewStock());
	}

	TEST_F(CWorldStockTest, TestIsUpdateStockProfileDB) {
		CWorldStock stock;

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

	TEST_F(CWorldStockTest, TestIsUpdateProfileDBAndClearFlag) {
		CWorldStock stock;

		EXPECT_FALSE(stock.IsUpdateProfileDB());
		EXPECT_FALSE(stock.IsUpdateProfileDBAndClearFlag());
		stock.SetUpdateProfileDB(true);
		EXPECT_TRUE(stock.IsUpdateProfileDBAndClearFlag());
		EXPECT_FALSE(stock.IsUpdateProfileDB());
	}

	TEST_F(CWorldStockTest, TestIsClearDayLineVector) {
		CWorldStock stock;

		EXPECT_FALSE(stock.IsClearDayLineVector());
		stock.SetClearDayLineVector(true);
		EXPECT_TRUE(stock.IsClearDayLineVector());
		EXPECT_TRUE(stock.IsClearDayLineVectorAndClearFlag());
		EXPECT_FALSE(stock.IsClearDayLineVector());
		stock.SetClearDayLineVector(true);
		EXPECT_TRUE(stock.IsClearDayLineVector());
		stock.SetClearDayLineVector(false);
		EXPECT_FALSE(stock.IsClearDayLineVector());
	}

	TEST_F(CWorldStockTest, TestIsActive) {
		CWorldStock stock;
		EXPECT_FALSE(stock.IsActive());
		stock.SetActive(true);
		EXPECT_TRUE(stock.IsActive());
		stock.SetActive(false);
		EXPECT_FALSE(stock.IsActive());
	}

	TEST_F(CWorldStockTest, TestGetDayLineEndDate) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetDayLineEndDate(), 19800101);
		stock.SetDayLineEndDate(19980101);
		EXPECT_EQ(stock.GetDayLineEndDate(), 19980101);
	}

	TEST_F(CWorldStockTest, TestGetDayLineStartDate) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetDayLineStartDate(), 29900101);
		stock.SetDayLineStartDate(19980101);
		EXPECT_EQ(stock.GetDayLineStartDate(), 19980101);
	}

	TEST_F(CWorldStockTest, TestGetIPOStatus) {
		CWorldStock stock;
		EXPECT_TRUE(stock.IsNotChecked());
		stock.SetIPOStatus(255);
		EXPECT_EQ(stock.GetIPOStatus(), 255);
	}

	TEST_F(CWorldStockTest, TestIsNullStock) {
		CWorldStock stock;
		stock.SetIPOStatus(_STOCK_NULL_);
		EXPECT_TRUE(stock.IsNullStock());
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_FALSE(stock.IsNullStock());
	}

	TEST_F(CWorldStockTest, TestIsIPOed) {
		CWorldStock stock;
		stock.SetIPOStatus(_STOCK_IPOED_);
		EXPECT_TRUE(stock.IsIPOed());
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_FALSE(stock.IsIPOed());
	}

	TEST_F(CWorldStockTest, TestIsNotChecked) {
		CWorldStock stock;
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_TRUE(stock.IsNotChecked());
		stock.SetIPOStatus(_STOCK_DELISTED_);
		EXPECT_FALSE(stock.IsNotChecked());
	}

	TEST_F(CWorldStockTest, TestIsDelisted) {
		CWorldStock stock;
		stock.SetIPOStatus(_STOCK_DELISTED_);
		EXPECT_TRUE(stock.IsDelisted());
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_FALSE(stock.IsDelisted());
	}

	TEST_F(CWorldStockTest, TestIsNotYetList) {
		CWorldStock stock;
		stock.SetIPOStatus(_STOCK_NOT_YET_LIST_);
		EXPECT_TRUE(stock.IsNotYetList());
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_FALSE(stock.IsNotYetList());
	}

	TEST_F(CWorldStockTest, TestIsDayLineNeedUpdate) {
		CWorldStock stock;
		EXPECT_TRUE(stock.IsDayLineNeedUpdate());
		stock.SetDayLineNeedUpdate(false);
		EXPECT_FALSE(stock.IsDayLineNeedUpdate());
		stock.SetDayLineNeedUpdate(true);
		EXPECT_TRUE(stock.IsDayLineNeedUpdate());
	}

	TEST_F(CWorldStockTest, TestIsDayLineNeededSaving) {
		// 此两个函数是具备同步机制的，这里没有进行测试
		CWorldStock stock;
		EXPECT_FALSE(stock.IsDayLineNeedSaving());
		stock.SetDayLineNeedSaving(true);
		EXPECT_TRUE(stock.IsDayLineNeedSaving());
		stock.SetDayLineNeedSaving(false);
		EXPECT_FALSE(stock.IsDayLineNeedSaving());
		stock.SetDayLineNeedSaving(true);
		EXPECT_TRUE(stock.IsDayLineNeedSavingAndClearFlag());
		EXPECT_FALSE(stock.IsDayLineNeedSaving());
	}

	TEST_F(CWorldStockTest, TestGetProfileUpdateDate) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetProfileUpdateDate(), 19800101);
		stock.SetProfileUpdateDate(10101010);
		EXPECT_EQ(stock.GetProfileUpdateDate(), 10101010);
	}

	TEST_F(CWorldStockTest, TestGetCompanyNewsUpdateDate) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetCompanyNewsUpdateDate(), 19800102);
		stock.SetCompanyNewsUpdateDate(10101010);
		EXPECT_EQ(stock.GetCompanyNewsUpdateDate(), 10101010);
	}

	TEST_F(CWorldStockTest, TestGetBasicFinancialUpdateDate) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetBasicFinancialUpdateDate(), 19800103);
		stock.SetBasicFinancialUpdateDate(10101010);
		EXPECT_EQ(stock.GetBasicFinancialUpdateDate(), 10101010);
	}

	TEST_F(CWorldStockTest, TestGetLastRTDataUpdateDate) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetLastRTDataUpdateDate(), 19800104);
		stock.SetLastRTDataUpdateDate(10101010);
		EXPECT_EQ(stock.GetLastRTDataUpdateDate(), 10101010);
	}

	TEST_F(CWorldStockTest, TestPeerUpdateDate) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetPeerUpdateDate(), 19800105);
		stock.SetPeerUpdateDate(10101010);
		EXPECT_EQ(stock.GetPeerUpdateDate(), 10101010);
	}

	TEST_F(CWorldStockTest, TestInsiderTransactionUpdateDate) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetInsiderTransactionUpdateDate(), 19800106);
		stock.SetInsiderTransactionUpdateDate(10101010);
		EXPECT_EQ(stock.GetInsiderTransactionUpdateDate(), 10101010);
	}

	TEST_F(CWorldStockTest, TestGetInsiderSentimentUpdateDate) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetInsiderSentimentUpdateDate(), 19800107);
		stock.SetInsiderSentimentUpdateDate(10101010);
		EXPECT_EQ(stock.GetInsiderSentimentUpdateDate(), 10101010);
	}

	TEST_F(CWorldStockTest, TestLastEPSSurpriseUpdateDate) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetLastEPSSurpriseUpdateDate(), 19800108);
		stock.SetLastEPSSurpriseUpdateDate(10101010);
		EXPECT_EQ(stock.GetLastEPSSurpriseUpdateDate(), 10101010);
	}

	TEST_F(CWorldStockTest, TestIsProfileUpdated) {
		CWorldStock stock;
		EXPECT_FALSE(stock.IsCompanyProfileUpdated());
		stock.SetCompanyProfileUpdated(true);
		EXPECT_TRUE(stock.IsCompanyProfileUpdated());
	}

	TEST_F(CWorldStockTest, TestIsEPSSurpriseUpdated) {
		CWorldStock stock;
		EXPECT_FALSE(stock.IsEPSSurpriseUpdated());
		stock.SetEPSSurpriseUpdated(true);
		EXPECT_TRUE(stock.IsEPSSurpriseUpdated());
	}

	TEST_F(CWorldStockTest, TestIsEPSSurpriseNeedSave) {
		CWorldStock stock;
		EXPECT_FALSE(stock.IsEPSSurpriseNeedSave());
		stock.SetEPSSurpriseNeedSave(true);
		EXPECT_TRUE(stock.IsEPSSurpriseNeedSave());
		EXPECT_TRUE(stock.IsEPSSurpriseNeedSaveAndClearFlag());
		EXPECT_FALSE(stock.IsEPSSurpriseNeedSave());
	}

	TEST_F(CWorldStockTest, TestIsInsiderTransactionNeedUpdate) {
		CWorldStock stock;
		EXPECT_TRUE(stock.IsInsiderTransactionNeedUpdate());
		stock.SetInsiderTransactionNeedUpdate(false);
		EXPECT_FALSE(stock.IsInsiderTransactionNeedUpdate());
		stock.SetInsiderTransactionNeedUpdate(true);
		EXPECT_TRUE(stock.IsInsiderTransactionNeedUpdate());
	}

	TEST_F(CWorldStockTest, TestIsInsiderTransactionNeedSave) {
		CWorldStock stock;
		EXPECT_FALSE(stock.IsInsiderTransactionNeedSave());
		stock.SetInsiderTransactionNeedSave(true);
		EXPECT_TRUE(stock.IsInsiderTransactionNeedSave());
		EXPECT_TRUE(stock.IsInsiderTransactionNeedSaveAndClearFlag());
		EXPECT_FALSE(stock.IsInsiderTransactionNeedSave());
	}

	TEST_F(CWorldStockTest, TestIsInsiderSentimentNeedUpdate) {
		CWorldStock stock;
		EXPECT_TRUE(stock.IsInsiderSentimentNeedUpdate());
		stock.SetInsiderSentimentNeedUpdate(false);
		EXPECT_FALSE(stock.IsInsiderSentimentNeedUpdate());
		stock.SetInsiderSentimentNeedUpdate(true);
		EXPECT_TRUE(stock.IsInsiderSentimentNeedUpdate());
	}

	TEST_F(CWorldStockTest, TestIsInsiderSentimentNeedSave) {
		CWorldStock stock;
		EXPECT_FALSE(stock.IsInsiderSentimentNeedSave());
		stock.SetInsiderSentimentNeedSave(true);
		EXPECT_TRUE(stock.IsInsiderSentimentNeedSave());
		EXPECT_TRUE(stock.IsInsiderSentimentNeedSaveAndClearFlag());
		EXPECT_FALSE(stock.IsInsiderSentimentNeedSave());
	}

	TEST_F(CWorldStockTest, TestGetDescription) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetDescription(), _T(""));
		stock.SetDescription(_T("abcdef"));
		EXPECT_STREQ(stock.GetDescription(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetDisplaySymbol) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetDisplaySymbol(), _T(""));
		stock.SetDisplaySymbol(_T("abcdef"));
		EXPECT_STREQ(stock.GetDisplaySymbol(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetType) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetType(), _T(" "));
		stock.SetType(_T("abcdef"));
		EXPECT_STREQ(stock.GetType(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetMic) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetMic(), _T(" "));
		stock.SetMic(_T("abcdef"));
		EXPECT_STREQ(stock.GetMic(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetFigi) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetFigi(), _T(" "));
		stock.SetFigi(_T("abcdef"));
		EXPECT_STREQ(stock.GetFigi(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetCurrency) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetCurrency(), _T(" "));
		stock.SetCurrency(_T("abcdef"));
		EXPECT_STREQ(stock.GetCurrency(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetAddress) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetAddress(), _T(" "));
		stock.SetAddress(_T("abcdef"));
		EXPECT_STREQ(stock.GetAddress(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetCity) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetCity(), _T(" "));
		stock.SetCity(_T("abcdef"));
		EXPECT_STREQ(stock.GetCity(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetCountry) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetCountry(), _T(" "));
		stock.SetCountry(_T("abcdef"));
		EXPECT_STREQ(stock.GetCountry(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetCusip) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetCusip(), _T(" "));
		stock.SetCusip(_T("abcdef"));
		EXPECT_STREQ(stock.GetCusip(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetSedol) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetSedol(), _T(" "));
		stock.SetSedol(_T("abcdef"));
		EXPECT_STREQ(stock.GetSedol(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetEmployeeTotal) {
		CWorldStock stock;
		EXPECT_EQ(stock.GetEmployeeTotal(), 0);
		stock.SetEmployeeTotal(1234567);
		EXPECT_EQ(stock.GetEmployeeTotal(), 1234567);
	}

	TEST_F(CWorldStockTest, TestGetListedExchange) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetListedExchange(), _T(" "));
		stock.SetListedExchange(_T("abcdef"));
		EXPECT_STREQ(stock.GetListedExchange(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetGgroup) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetGgroup(), _T(" "));
		stock.SetGgroup(_T("abcdef"));
		EXPECT_STREQ(stock.GetGgroup(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetGind) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetGind(), _T(" "));
		stock.SetGind(_T("abcdef"));
		EXPECT_STREQ(stock.GetGind(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetGsector) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetGsector(), _T(" "));
		stock.SetGsector(_T("abcdef"));
		EXPECT_STREQ(stock.GetGsector(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetGsubind) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetGsubind(), _T(" "));
		stock.SetGsubind(_T("abcdef"));
		EXPECT_STREQ(stock.GetGsubind(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetIPODate) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetIPODate(), _T(" "));
		stock.SetIPODate(_T("abcdef"));
		EXPECT_STREQ(stock.GetIPODate(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetIsin) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetIsin(), _T(" "));
		stock.SetIsin(_T("abcdef"));
		EXPECT_STREQ(stock.GetIsin(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetMarketCapitalization) {
		CWorldStock stock;
		EXPECT_DOUBLE_EQ(stock.GetMarketCapitalization(), 0.0);
		stock.SetMarketCapitalization(10101.010);
		EXPECT_DOUBLE_EQ(stock.GetMarketCapitalization(), 10101.010);
	}

	TEST_F(CWorldStockTest, TestGetNaics) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetNaics(), _T(" "));
		stock.SetNaics(_T("abcdef"));
		EXPECT_STREQ(stock.GetNaics(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetNaicsNationalIndustry) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetNaicsNationalIndustry(), _T(" "));
		stock.SetNaicsNationalIndustry(_T("abcdef"));
		EXPECT_STREQ(stock.GetNaicsNationalIndustry(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetNaicsSector) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetNaicsSector(), _T(" "));
		stock.SetNaicsSector(_T("abcdef"));
		EXPECT_STREQ(stock.GetNaicsSector(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetNaicsSubsector) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetNaicsSubsector(), _T(" "));
		stock.SetNaicsSubsector(_T("abcdef"));
		EXPECT_STREQ(stock.GetNaicsSubsector(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetName) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetName(), _T(" "));
		stock.SetName(_T("abcdef"));
		EXPECT_STREQ(stock.GetName(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetPhone) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetPhone(), _T(" "));
		stock.SetPhone(_T("abcdef"));
		EXPECT_STREQ(stock.GetPhone(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetShareOutstanding) {
		CWorldStock stock;
		EXPECT_DOUBLE_EQ(stock.GetShareOutstanding(), 0.0);
		stock.SetShareOutstanding(10101.010);
		EXPECT_DOUBLE_EQ(stock.GetShareOutstanding(), 10101.010);
	}

	TEST_F(CWorldStockTest, TestGetState) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetState(), _T(" "));
		stock.SetState(_T("abcdef"));
		EXPECT_STREQ(stock.GetState(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetWebURL) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetWebURL(), _T(" "));
		stock.SetWebURL(_T("abcdef"));
		EXPECT_STREQ(stock.GetWebURL(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetLogo) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetLogo(), _T(" "));
		stock.SetLogo(_T("abcdef"));
		EXPECT_STREQ(stock.GetLogo(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetTicker) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetTicker(), _T(" "));
		stock.SetTicker(_T("abcdef"));
		EXPECT_STREQ(stock.GetTicker(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetFinnhubIndustry) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetFinnhubIndustry(), _T(" "));
		stock.SetFinnhubIndustry(_T("abcdef"));
		EXPECT_STREQ(stock.GetFinnhubIndustry(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetPeer) {
		CWorldStock stock;
		EXPECT_STREQ(stock.GetPeer(), _T(" "));
		stock.SetPeer(_T("abcdef"));
		EXPECT_STREQ(stock.GetPeer(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestIsUSMarket) {
		CWorldStock stock;
		stock.SetExchangeCode(_T("SS"));
		EXPECT_FALSE(stock.IsUSMarket());
		stock.SetExchangeCode(_T("US"));
		EXPECT_TRUE(stock.IsUSMarket());
	}

	TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus1) {
		CWorldStock stock;

		stock.SetDayLineNeedUpdate(true);
		stock.SetActive(false);
		for (int i = 1; i < 6; i++) {
			EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "非活跃股票工作日不更新日线\n";
			stock.SetDayLineNeedUpdate(true);
		}
	}

	TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus2) {
		CWorldStock stock;

		stock.SetDayLineNeedUpdate(true);
		stock.SetIPOStatus(_STOCK_NULL_);
		EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, 0)) << "无效股票不检查日线\n";
	}

	TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus3) {
		CWorldStock stock;

		stock.SetDayLineNeedUpdate(true);
		stock.SetIPOStatus(_STOCK_DELISTED_);
		for (int i = 0; i < 7; i++) {
			if (i == 4)
				EXPECT_TRUE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "摘牌股票只在星期四检查日线\n";
			else
				EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "摘牌股票只在星期四检查日线\n";
			stock.SetDayLineNeedUpdate(true);
		}
	}

	TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus4) {
		CWorldStock stock;

		stock.SetDayLineNeedUpdate(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetActive(true);
		stock.SetDayLineEndDate(gl_pWorldMarket->GetPrevDay(gl_pWorldMarket->GetMarketDate(), 100));
		EXPECT_TRUE(stock.CheckDayLineUpdateStatus(gl_pWorldMarket->GetMarketDate(), gl_pWorldMarket->GetMarketDate(), 0, 1));
		stock.SetDayLineEndDate(gl_pWorldMarket->GetPrevDay(stock.GetDayLineEndDate()));
		EXPECT_FALSE(stock.CheckDayLineUpdateStatus(gl_pWorldMarket->GetMarketDate(), gl_pWorldMarket->GetMarketDate(), 0, 1)) << "早于100天的股票不再更新日线";
	}

	TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus5) {
		CWorldStock stock;
		long lCurrentDay = gl_pWorldMarket->GetMarketDate();
		long lPrevDay = gl_pWorldMarket->GetPrevDay(lCurrentDay);

		stock.SetDayLineNeedUpdate(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetActive(true);
		stock.SetDayLineEndDate(lCurrentDay); // 本日交易日日线已接收
		for (int i = 1; i < 6; i++) {
			EXPECT_FALSE(stock.CheckDayLineUpdateStatus(lCurrentDay, lPrevDay, 170001, i)) << "时间晚于17时后，检查当天日线";
			stock.SetDayLineNeedUpdate(true); // 重置状态
		}
		stock.SetDayLineEndDate(lPrevDay); // 本日交易日日线尚未接收
		for (int i = 1; i < 6; i++) { EXPECT_TRUE(stock.CheckDayLineUpdateStatus(lCurrentDay, lPrevDay, 170001, i)) << "时间晚于17时后，检查当天日线"; }
	}

	TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus6) {
		CWorldStock stock;
		long lCurrentDay = gl_pWorldMarket->GetMarketDate();
		long lPrevDay = gl_pWorldMarket->GetPrevDay(lCurrentDay);

		stock.SetDayLineNeedUpdate(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetActive(true);
		stock.SetDayLineEndDate(gl_pWorldMarket->GetPrevDay(lCurrentDay)); // 上一交易日日线数据已接收
		for (int i = 1; i < 6; i++) {
			EXPECT_FALSE(stock.CheckDayLineUpdateStatus(lCurrentDay, lPrevDay, 170000, i)) << "时间不晚于17时，检查上一交易日日线 " << i;
			stock.SetDayLineNeedUpdate(true); // 重置之
		}
		stock.SetDayLineEndDate(gl_pWorldMarket->GetPrevDay(lCurrentDay, 2)); // 上一交易日日线数据未接收
		for (int i = 1; i < 6; i++) { EXPECT_TRUE(stock.CheckDayLineUpdateStatus(lCurrentDay, lPrevDay, 170000, i)) << "时间不晚于17时，检查上一交易日日线"; }
	}

	TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus7) {
		CWorldStock stock;
		long lCurrentDate = gl_pWorldMarket->GetMarketDate();
		long lPrevMonday = GetPrevMonday(lCurrentDate);

		stock.SetDayLineNeedUpdate(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetActive(true);
		stock.SetDayLineEndDate(gl_pWorldMarket->GetPrevDay(lPrevMonday, 3)); // 上一交易日日线数据已接收
		EXPECT_FALSE(stock.CheckDayLineUpdateStatus(gl_pWorldMarket->GetPrevDay(lPrevMonday, 2), gl_pWorldMarket->GetPrevDay(lPrevMonday, 3), 170000, 6)) << "周六，检查上一交易日日线";
		stock.SetDayLineNeedUpdate(true); // 重置之
		EXPECT_FALSE(stock.CheckDayLineUpdateStatus(gl_pWorldMarket->GetPrevDay(lPrevMonday, 1), gl_pWorldMarket->GetPrevDay(lPrevMonday, 3), 170000, 0)) << "周日，检查上一交易日日线";
	}

	TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus8) {
		CWorldStock stock;

		stock.SetDayLineNeedUpdate(true);
		stock.SetIPOStatus(_STOCK_NOT_YET_LIST_);
		for (int i = 0; i < 7; i++) {
			if (i == 4)
				EXPECT_TRUE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "摘牌股票只在星期四检查日线\n";
			else
				EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "摘牌股票只在星期四检查日线\n";
			stock.SetDayLineNeedUpdate(true);
		}
	}

	TEST_F(CWorldStockTest, TestCheckEPSSurpriseStatus) {
		CWorldStock stock;
		long lCurrentDate = 20200101;

		stock.SetEPSSurpriseUpdated(false);
		stock.SetIPOStatus(_STOCK_NULL_);
		stock.CheckEPSSurpriseStatus(lCurrentDate);
		EXPECT_TRUE(stock.IsEPSSurpriseUpdated());

		stock.SetEPSSurpriseUpdated(false);
		stock.SetIPOStatus(_STOCK_DELISTED_);
		stock.CheckEPSSurpriseStatus(lCurrentDate);
		EXPECT_TRUE(stock.IsEPSSurpriseUpdated());

		stock.SetEPSSurpriseUpdated(false);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetLastEPSSurpriseUpdateDate(19700101);
		stock.CheckEPSSurpriseStatus(lCurrentDate);
		EXPECT_TRUE(stock.IsEPSSurpriseUpdated());

		stock.SetEPSSurpriseUpdated(false);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetLastEPSSurpriseUpdateDate(20190819); // 不早于135天
		stock.CheckEPSSurpriseStatus(lCurrentDate);
		EXPECT_TRUE(stock.IsEPSSurpriseUpdated());

		stock.SetEPSSurpriseUpdated(false);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetLastEPSSurpriseUpdateDate(20190818); // 早于135天， 不早于225天
		stock.CheckEPSSurpriseStatus(lCurrentDate);
		EXPECT_FALSE(stock.IsEPSSurpriseUpdated());

		stock.SetEPSSurpriseUpdated(false);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetLastEPSSurpriseUpdateDate(20190521); // 早于135天， 不早于225天
		stock.CheckEPSSurpriseStatus(lCurrentDate);
		EXPECT_FALSE(stock.IsEPSSurpriseUpdated());

		stock.SetEPSSurpriseUpdated(false);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetLastEPSSurpriseUpdateDate(20190520); // 早于135天， 不早于225天
		stock.CheckEPSSurpriseStatus(lCurrentDate);
		EXPECT_TRUE(stock.IsEPSSurpriseUpdated());
	}

	TEST_F(CWorldStockTest, TestSaveDayLine) {
		CWorldStock stock;
		vector<CDayLinePtr> vDayLine;
		CDayLinePtr pDayLine;
		CSetWorldStockDayLine setDayLine;

		pDayLine = make_shared<CDayLine>();
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

	TEST_F(CWorldStockTest, TestUpdateDayLineDB1) {
		CWorldStock stock;

		EXPECT_FALSE(stock.IsDayLineNeedSaving());

		EXPECT_FALSE(stock.UpdateDayLineDB()) << "更新日线标识为假时，无需执行实际操作";
	}

	TEST_F(CWorldStockTest, TestUpdateDayLineDB2) {
		CWorldStock stock;

		EXPECT_FALSE(stock.IsDayLineNeedSaving());
		stock.SetDayLineNeedSaving(true); // 需要更新
		EXPECT_EQ(stock.GetDayLineSize(), 0);

		EXPECT_FALSE(stock.UpdateDayLineDB()) << "日线数据为零时，无需执行实际操作";
		EXPECT_FALSE(stock.IsDayLineNeedSaving()) << "更新标识已被重置为假";
	}

	TEST_F(CWorldStockTest, TestUpdateDayLineDB3) {
		CWorldStock stock;
		vector<CDayLinePtr> vDayLine;
		CDayLinePtr pDayLine;
		CSetWorldStockDayLine setDayLine;

		EXPECT_FALSE(stock.IsDayLineNeedSaving());
		stock.SetDayLineNeedSaving(true); // 需要更新
		EXPECT_EQ(stock.GetDayLineSize(), 0);

		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20200101); // 这个需要添加进数据库
		pDayLine->SetClose(10010);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210101); // 这个需要添加进数据库
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
		stock.SetDayLineStartDate(20200101); // 日线开始日期不晚于当前日线数据日期
		stock.SetDayLineEndDate(20210123); // 日线结束日期不早于当前日线数据日期
		stock.UpdateDayLine(vDayLine);

		EXPECT_FALSE(stock.UpdateDayLineDB()) << "无需执行实际操作";
		EXPECT_FALSE(stock.IsDayLineNeedSaving()) << "更新标识已被重置为假";
		EXPECT_EQ(stock.GetDayLineSize(), 0) << "已清除日线数据";

		stock.SetDayLineNeedSaving(true); // 需要更新
		stock.SetDayLineStartDate(20200102); // 日线开始日期晚于当前日线数据日期
		stock.SetDayLineEndDate(20210123); // 日线结束日期不早于当前日线数据日期
		stock.UpdateDayLine(vDayLine);

		EXPECT_TRUE(stock.UpdateDayLineDB()) << "执行实际操作";
		EXPECT_FALSE(stock.IsDayLineNeedSaving()) << "更新标识已被重置为假";
		EXPECT_EQ(stock.GetDayLineSize(), 0);
		EXPECT_EQ(stock.GetDayLineStartDate(), 20200101) << "日线开始日期已更新为较早日期";
		EXPECT_TRUE(stock.IsUpdateProfileDB());
		ASSERT_GE(gl_systemMessage.DayLineInfoSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopDayLineInfoMessage(), _T("A日线资料存储完成"));

		// 清除添加的数据，恢复原状
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

	TEST_F(CWorldStockTest, TestSaveInsiderTransaction) {
		//  测试数据库中只有4个数据，股票代码：A， 内部交易人员：a b c，
		CWorldStock stock;
		vector<CInsiderTransactionPtr> vInsiderTransaction;
		CInsiderTransactionPtr pInsiderTransaction;
		CSetInsiderTransaction setInsiderTransaction;

		pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = _T("B");
		pInsiderTransaction->m_strPersonName = _T("a b c");
		pInsiderTransaction->m_lTransactionDate = 20200101; // 这个股票代码不符，需要添加进数据库
		vInsiderTransaction.push_back(pInsiderTransaction);
		pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = _T("A");
		pInsiderTransaction->m_strPersonName = _T("a b c d");
		pInsiderTransaction->m_lTransactionDate = 20210101; // 这个内部交易人员名称不符，需要添加进数据库
		vInsiderTransaction.push_back(pInsiderTransaction);
		pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = _T("A");
		pInsiderTransaction->m_strPersonName = _T("a b c");
		pInsiderTransaction->m_lTransactionDate = 20210107;
		pInsiderTransaction->m_strTransactionCode = _T("M"); // 这个数据库中有，无需添加
		vInsiderTransaction.push_back(pInsiderTransaction);
		pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = _T("A");
		pInsiderTransaction->m_strPersonName = _T("a b c");
		pInsiderTransaction->m_lTransactionDate = 20210124; // 这个日期不符，需要添加进数据库
		vInsiderTransaction.push_back(pInsiderTransaction);
		pInsiderTransaction->m_strSymbol = _T("A");
		pInsiderTransaction->m_strPersonName = _T("a b c");
		pInsiderTransaction->m_strTransactionCode = _T("S"); // 这个交易类型不符，需要添加进数据库
		vInsiderTransaction.push_back(pInsiderTransaction);

		stock.SetSymbol(_T("A"));
		stock.SetInsiderTransactionUpdateDate(20210123);
		stock.UpdateInsiderTransaction(vInsiderTransaction);

		stock.SaveInsiderTransaction();

		setInsiderTransaction.m_strFilter = _T("[Symbol] = 'B'");
		setInsiderTransaction.Open();
		setInsiderTransaction.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderTransaction.IsEOF());
		setInsiderTransaction.Delete();
		setInsiderTransaction.m_pDatabase->CommitTrans();
		setInsiderTransaction.Close();

		setInsiderTransaction.m_strFilter = _T("[PersonName] = 'a b c d'");
		setInsiderTransaction.Open();
		setInsiderTransaction.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderTransaction.IsEOF());
		setInsiderTransaction.Delete();
		setInsiderTransaction.m_pDatabase->CommitTrans();
		setInsiderTransaction.Close();

		setInsiderTransaction.m_strFilter = _T("[TransactionDate] = '20210124'");
		setInsiderTransaction.Open();
		setInsiderTransaction.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderTransaction.IsEOF());
		setInsiderTransaction.Delete();
		setInsiderTransaction.m_pDatabase->CommitTrans();
		setInsiderTransaction.Close();

		setInsiderTransaction.m_strFilter = _T("[TransactionCode] = 'S'");
		setInsiderTransaction.Open();
		setInsiderTransaction.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderTransaction.IsEOF());
		setInsiderTransaction.Delete();
		setInsiderTransaction.m_pDatabase->CommitTrans();
		setInsiderTransaction.Close();
	}

	TEST_F(CWorldStockTest, TestSaveInsiderSentiment) {
		//  测试数据库中只有4个数据，股票代码：A，
		CWorldStock stock;
		vector<CInsiderSentimentPtr> vInsiderSentiment;
		CInsiderSentimentPtr pInsiderSentiment;
		CSetInsiderSentiment setInsiderSentiment;

		pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment->m_strSymbol = _T("B"); // 这个股票代码不符，不可能出现此种情况，故而不需要添加进数据库
		pInsiderSentiment->m_lDate = 20200101;
		vInsiderSentiment.push_back(pInsiderSentiment);
		pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment->m_strSymbol = _T("A");
		pInsiderSentiment->m_lDate = 20200101; // 这个数据库中有，无需添加
		vInsiderSentiment.push_back(pInsiderSentiment);
		pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment->m_strSymbol = _T("A");
		pInsiderSentiment->m_lDate = 20210101; // 这个日期不符，需要添加进数据库
		vInsiderSentiment.push_back(pInsiderSentiment);

		stock.SetSymbol(_T("A"));
		stock.SetInsiderSentimentUpdateDate(20210101);
		stock.UpdateInsiderSentiment(vInsiderSentiment);

		stock.SaveInsiderSentiment();

		setInsiderSentiment.m_strFilter = _T("[Symbol] = 'B'");
		setInsiderSentiment.Open();
		EXPECT_TRUE(setInsiderSentiment.IsEOF()) << "股票B没有存储进了数据库";
		setInsiderSentiment.Close();

		setInsiderSentiment.m_strFilter = _T("[Date] = '20210101'");
		setInsiderSentiment.Open();
		setInsiderSentiment.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderSentiment.IsEOF()) << "新日期存储进了数据库";
		setInsiderSentiment.Delete();
		setInsiderSentiment.m_pDatabase->CommitTrans();
		setInsiderSentiment.Close();
	}

	TEST_F(CWorldStockTest, TestUpdateCompanyNewsDB) {
		CWorldStock stock;
		auto pvCompanyNews = make_shared<vector<CCompanyNewsPtr>>();
		CCompanyNewsPtr pCompanyNews;
		CSetCompanyNews setCompanyNews;

		stock.SetSymbol(_T("RIG"));

		pCompanyNews = make_shared<CFinnhubCompanyNews>();
		pCompanyNews->m_strCompanySymbol = _T("RIG");
		pCompanyNews->m_llDateTime = 19800101;
		pCompanyNews->m_strCategory = _T("test");
		pCompanyNews->m_iNewsID = 4;
		pvCompanyNews->push_back(pCompanyNews);
		pCompanyNews = make_shared<CFinnhubCompanyNews>();
		pCompanyNews->m_strCompanySymbol = _T("RIG");
		pCompanyNews->m_llDateTime = 20200101;
		pCompanyNews->m_strCategory = _T("test");
		pCompanyNews->m_iNewsID = 5;
		pvCompanyNews->push_back(pCompanyNews);

		stock.UpdateCompanyNews(pvCompanyNews);
		stock.UpdateCompanyNewsDB();

		int iCount = 0;
		setCompanyNews.m_strFilter = _T("[category] = 'test'");
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

	TEST_F(CWorldStockTest, TestUpdateDayLine) {
		CWorldStock stock;
		vector<CDayLinePtr> vDayLine;
		CDayLinePtr pDayLine;
		CSetWorldStockDayLine setDayLine;

		pDayLine = make_shared<CDayLine>();
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
		stock.UpdateDayLineStartEndDate();
		EXPECT_EQ(stock.GetDayLineStartDate(), 20210101);
		EXPECT_EQ(stock.GetDayLineEndDate(), 20210123);
		EXPECT_TRUE(stock.IsUpdateProfileDB());

		stock.UnloadDayLine();
		EXPECT_EQ(stock.GetDayLineSize(), 0);
	}

	TEST_F(CWorldStockTest, TestUpdateDayLineStartEndDate) {
		CWorldStock stock;
		vector<CDayLinePtr> vDayLine;
		CDayLinePtr pDayLine;
		CSetWorldStockDayLine setDayLine;

		pDayLine = make_shared<CDayLine>();
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

	TEST_F(CWorldStockTest, TestUpdateEPSSurpriseDB1) {
		CWorldStock stock;
		vector<CEPSSurprisePtr> vEPS;
		auto pEPS = make_shared<CEPSSurprise>();

		EXPECT_TRUE(stock.UpdateEPSSurpriseDB()) << "没有数据，返回";
	}

	TEST_F(CWorldStockTest, TestUpdateEPSSurpriseDB2) {
		CWorldStock stock;
		vector<CEPSSurprisePtr> vEPS;
		auto pEPS = make_shared<CEPSSurprise>();

		pEPS->m_strSymbol = _T("600601.SS");
		pEPS->m_lDate = 20200101;
		pEPS->m_dActual = 1.0;
		pEPS->m_dEstimate = 1.1;
		vEPS.push_back(pEPS);
		pEPS = make_shared<CEPSSurprise>();
		pEPS->m_strSymbol = _T("600601.SS");
		pEPS->m_lDate = 20200401;
		pEPS->m_dActual = 2.0;
		pEPS->m_dEstimate = 2.1;
		vEPS.push_back(pEPS);
		stock.UpdateEPSSurprise(vEPS);

		stock.SetLastEPSSurpriseUpdateDate(20210101);
		EXPECT_FALSE(stock.UpdateEPSSurpriseDB()) << "没有新数据，返回假";
	}

	TEST_F(CWorldStockTest, TestUpdateEPSSurpriseDB3) {
		CWorldStock stock;
		vector<CEPSSurprisePtr> vEPS;
		auto pEPS = make_shared<CEPSSurprise>();

		pEPS->m_strSymbol = _T("600601.US");
		pEPS->m_lDate = 20200101;
		pEPS->m_dActual = 1.0;
		pEPS->m_dEstimate = 1.1;
		vEPS.push_back(pEPS);
		pEPS = make_shared<CEPSSurprise>();
		pEPS->m_strSymbol = _T("600601.US");
		pEPS->m_lDate = 20200401;
		pEPS->m_dActual = 2.0;
		pEPS->m_dEstimate = 2.1;
		vEPS.push_back(pEPS);
		stock.UpdateEPSSurprise(vEPS);

		stock.SetLastEPSSurpriseUpdateDate(20200101);
		EXPECT_FALSE(stock.IsUpdateProfileDB());
		EXPECT_TRUE(stock.UpdateEPSSurpriseDB()) << "将新数据存入数据库(存储了一个新的)";
		EXPECT_TRUE(stock.IsUpdateProfileDB());
		EXPECT_EQ(stock.GetLastEPSSurpriseUpdateDate(), 20200401);

		CSetEPSSurprise setEPSSurprise;
		int i = 0;
		setEPSSurprise.m_strFilter = _T("[Symbol] = '600601.US'");
		setEPSSurprise.Open();
		setEPSSurprise.m_pDatabase->BeginTrans();
		while (!setEPSSurprise.IsEOF()) {
			EXPECT_DOUBLE_EQ(setEPSSurprise.m_Actual, 2.0);
			EXPECT_DOUBLE_EQ(setEPSSurprise.m_Estimate, 2.1);
			EXPECT_EQ(setEPSSurprise.m_Date, 20200401);
			EXPECT_STREQ(setEPSSurprise.m_Symbol, _T("600601.US"));
			setEPSSurprise.Delete();
			setEPSSurprise.MoveNext();
			i++;
		}
		setEPSSurprise.m_pDatabase->CommitTrans();
		setEPSSurprise.Close();
		EXPECT_EQ(i, 1);
	}

	TEST_F(CWorldStockTest, TestIsNeedUpdateProfile) {
		CWorldStock stock;
		auto pTiingoStock = make_shared<CTiingoStock>();

		stock.SetSymbol(_T("asdjfkajsfkj"));

		pTiingoStock->m_strTiingoPermaTicker = _T("askfkasjf");
		pTiingoStock->m_strTicker = _T("asdjfkajsfkj");
		pTiingoStock->m_strName = _T("sdlfkjsdfkjg");
		pTiingoStock->m_fIsActive = true;
		pTiingoStock->m_fIsADR = true;
		pTiingoStock->m_iSICCode = 1234343445;
		pTiingoStock->m_strSICIndustry = _T("kjkjljkjgklj");
		pTiingoStock->m_strSICSector = _T("eiruir");
		pTiingoStock->m_strTiingoIndustry = _T("kdfjgkjfierj");
		pTiingoStock->m_strTiingoSector = _T("quwerpqi");
		pTiingoStock->m_strReportingCurrency = _T("asjdxcmsm");
		pTiingoStock->m_strLocation = _T("mcmvkmcvkv");
		pTiingoStock->m_strCompanyWebSite = _T(",mcvmcv");
		pTiingoStock->m_strSECFilingWebSite = _T(".,m,.cmv,.");
		pTiingoStock->m_lStatementUpdateDate = 457843759;
		pTiingoStock->m_lDailyDataUpdateDate = 234234;
		stock.UpdateStockProfile(pTiingoStock);

		EXPECT_FALSE(stock.IsNeedUpdateProfile(pTiingoStock));
	}

	TEST_F(CWorldStockTest, TestUpdateStockProfile) {
		CWorldStock stock;
		auto pTiingoStock = make_shared<CTiingoStock>();

		stock.SetSymbol(_T("asdjfkajsfkj"));

		pTiingoStock->m_strTiingoPermaTicker = _T("askfkasjf");
		pTiingoStock->m_strTicker = _T("asdjfkajsfkj");
		pTiingoStock->m_strName = _T("sdlfkjsdfkjg");
		pTiingoStock->m_fIsActive = true;
		pTiingoStock->m_fIsADR = true;
		pTiingoStock->m_iSICCode = 1234343445;
		pTiingoStock->m_strSICIndustry = _T("kjkjljkjgklj");
		pTiingoStock->m_strSICSector = _T("eiruir");
		pTiingoStock->m_strTiingoIndustry = _T("kdfjgkjfierj");
		pTiingoStock->m_strTiingoSector = _T("quwerpqi");
		pTiingoStock->m_strReportingCurrency = _T("asjdxcmsm");
		pTiingoStock->m_strLocation = _T("mcmvkmcvkv");
		pTiingoStock->m_strCompanyWebSite = _T(",mcvmcv");
		pTiingoStock->m_strSECFilingWebSite = _T(".,m,.cmv,.");
		pTiingoStock->m_lStatementUpdateDate = 457843759;
		pTiingoStock->m_lDailyDataUpdateDate = 234234;

		stock.UpdateStockProfile(pTiingoStock);

		EXPECT_STREQ(stock.GetTiingoPermaTicker(), pTiingoStock->m_strTiingoPermaTicker);
		EXPECT_STREQ(stock.GetSymbol(), pTiingoStock->m_strTicker);
		EXPECT_TRUE(stock.TiingoIsActive());
		EXPECT_TRUE(stock.IsADR());
		EXPECT_EQ(stock.GetSICCode(), pTiingoStock->m_iSICCode);
		EXPECT_STREQ(stock.GetSICIndustry(), pTiingoStock->m_strSICIndustry);
		EXPECT_STREQ(stock.GetSICSector(), pTiingoStock->m_strSICSector);
		EXPECT_STREQ(stock.GetTiingoIndustry(), pTiingoStock->m_strTiingoIndustry);
		EXPECT_STREQ(stock.GetSICSector(), pTiingoStock->m_strSICSector);
		EXPECT_STREQ(stock.GetCompanyWebSite(), pTiingoStock->m_strCompanyWebSite);
		EXPECT_STREQ(stock.GetSECFilingWebSite(), pTiingoStock->m_strSECFilingWebSite);
		EXPECT_EQ(stock.GetTiingoStatementUpdateDate(), pTiingoStock->m_lStatementUpdateDate);
		EXPECT_EQ(stock.GetTiingoDailyDataUpdateDate(), pTiingoStock->m_lDailyDataUpdateDate);
	}

	TEST_F(CWorldStockTest, TestHaveNewDayLineData) {
		CWorldStock stock;
		vector<CDayLinePtr> vDayLine;
		CDayLinePtr pDayLine;
		CSetWorldStockDayLine setDayLine;

		EXPECT_EQ(stock.GetDayLineSize(), 0);
		EXPECT_FALSE(stock.HaveNewDayLineData()) << "没有日线数据";

		pDayLine = make_shared<CDayLine>();
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

	TEST_F(CWorldStockTest, TestCheckPeerStatus) {
		CWorldStock stock;
		EXPECT_FALSE(stock.IsPeerUpdated());

		stock.SetPeerUpdated(true);
		stock.SetPeerUpdateDate(20200101);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.CheckPeerStatus(20200401); // 91天
		EXPECT_FALSE(stock.IsPeerUpdated()) << "九十一天需更新";
		stock.CheckPeerStatus(20200331); // 90天
		EXPECT_TRUE(stock.IsPeerUpdated());

		stock.SetPeerUpdated(false);
		stock.SetIPOStatus(_STOCK_DELISTED_);
		stock.CheckPeerStatus(20200331); // 90天
		EXPECT_TRUE(stock.IsPeerUpdated()) << "九十天内无需更新";
		stock.CheckPeerStatus(20200401); // 91天
		EXPECT_TRUE(stock.IsPeerUpdated()) << "摘牌股票无需更新Peer";
	}

	TEST_F(CWorldStockTest, TestHaveInsiderTransaction) {
		CWorldStock stock;
		auto pInsiderTransaction = make_shared<CInsiderTransaction>();

		EXPECT_FALSE(stock.HaveInsiderTransaction());
		stock.m_vInsiderTransaction.push_back(pInsiderTransaction);
		EXPECT_TRUE(stock.HaveInsiderTransaction());
		stock.UnloadInsiderTransaction();
		EXPECT_FALSE(stock.HaveInsiderTransaction());
	}

	TEST_F(CWorldStockTest, TestCheckInsiderTransactionStatus) {
		CWorldStock stock;
		EXPECT_TRUE(stock.IsInsiderTransactionNeedUpdate());

		stock.SetInsiderTransactionNeedUpdate(false);
		stock.SetInsiderTransactionUpdateDate(20200101);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.CheckInsiderTransactionStatus(20200201); // 31天
		EXPECT_TRUE(stock.IsInsiderTransactionNeedUpdate()) << "三十一天需更新";
		stock.CheckInsiderTransactionStatus(20200131); // 30天
		EXPECT_FALSE(stock.IsInsiderTransactionNeedUpdate());

		stock.SetInsiderTransactionNeedUpdate(true);
		stock.SetIPOStatus(_STOCK_DELISTED_);
		stock.CheckInsiderTransactionStatus(20200131); // 30天
		EXPECT_FALSE(stock.IsInsiderTransactionNeedUpdate()) << "三十天内无需更新";
		stock.CheckInsiderTransactionStatus(20200201); // 31天
		EXPECT_FALSE(stock.IsInsiderTransactionNeedUpdate()) << "摘牌股票无需更新InsiderTransaction";
	}

	TEST_F(CWorldStockTest, TestHaveInsiderSentiment) {
		CWorldStock stock;
		auto pInsiderSentiment = make_shared<CInsiderSentiment>();

		EXPECT_FALSE(stock.HaveInsiderSentiment());
		stock.m_vInsiderSentiment.push_back(pInsiderSentiment);
		EXPECT_TRUE(stock.HaveInsiderSentiment());
		stock.UnloadInsiderSentiment();
		EXPECT_FALSE(stock.HaveInsiderSentiment());
	}

	TEST_F(CWorldStockTest, TestCheckInsiderSentimentStatus) {
		CWorldStock stock;
		EXPECT_TRUE(stock.IsInsiderSentimentNeedUpdate());

		stock.SetInsiderSentimentNeedUpdate(false);
		stock.SetInsiderSentimentUpdateDate(20200101);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.CheckInsiderSentimentStatus(20200201); // 31天
		EXPECT_TRUE(stock.IsInsiderSentimentNeedUpdate()) << "三十一天需更新";
		stock.CheckInsiderSentimentStatus(20200131); // 30天
		EXPECT_FALSE(stock.IsInsiderSentimentNeedUpdate());

		stock.SetInsiderSentimentNeedUpdate(true);
		stock.SetIPOStatus(_STOCK_DELISTED_);
		stock.CheckInsiderSentimentStatus(20200131); // 30天
		EXPECT_FALSE(stock.IsInsiderSentimentNeedUpdate()) << "三十天内无需更新";
		stock.CheckInsiderSentimentStatus(20200201); // 31天
		EXPECT_FALSE(stock.IsInsiderSentimentNeedUpdate()) << "摘牌股票无需更新InsiderSentiment";
	}

	TEST_F(CWorldStockTest, TestGetFinnhubDayLineInquiryString) {
		CWorldStock stock;
		long lDate = 20200101;

		time_t tt = gl_pWorldMarket->TransferToUTCTime(lDate);
		time_t ttOld = tt - (time_t)(365) * 24 * 3600;

		char buffer[30];
		sprintf_s(buffer, _T("%I64i"), tt);
		CString strTime = buffer;
		sprintf_s(buffer, _T("%I64i"), ttOld);
		CString strTimeOld = buffer;
		CString strMiddle, strMiddle2;

		stock.SetSymbol(_T("600601.SS"));
		stock.SetDayLineEndDate(20180101); // 早于20190102
		strMiddle2 = _T("600601.SS&resolution=D");
		strMiddle2 += _T("&from=") + strTimeOld + _T("&to=") + strTime;
		EXPECT_STREQ(stock.GetFinnhubDayLineInquiryString(tt), strMiddle2) << "免费账户最多只能申请一年的日线数据";

		stock.SetSymbol(_T("600601.SS"));
		stock.SetDayLineEndDate(20190501); // 晚于20190102
		ttOld = gl_pWorldMarket->TransferToUTCTime(20190501);
		sprintf_s(buffer, _T("%I64i"), ttOld);
		strTimeOld = buffer;
		strMiddle = _T("600601.SS&resolution=D");
		strMiddle += _T("&from=") + strTimeOld + _T("&to=") + strTime;
		EXPECT_STREQ(stock.GetFinnhubDayLineInquiryString(tt), strMiddle2) << "检查一年的数据";
	}

	TEST_F(CWorldStockTest, TestGetTiingoDayLineInquiryString) {
		CWorldStock stock;
		CString strMiddle;

		stock.SetSymbol(_T("600601.SS"));
		stock.SetDayLineEndDate(20180101);
		strMiddle = _T("600601.SS/prices?&startDate=1980-1-1&endDate=2020-1-1");

		EXPECT_STREQ(stock.GetTiingoDayLineInquiryString(20200101), strMiddle);
	}

	TEST_F(CWorldStockTest, TestSave) {
		CWorldStock stock, stock2;
		CSetWorldStock setWorldStock;

		stock.SetSymbol(_T("000001.US"));
		stock.SetExchangeCode(_T("US"));
		stock.SetDescription(_T("US Stock"));
		stock.SetDisplaySymbol(_T("DisplaySymbol"));
		stock.SetType(_T("Ordinary share"));
		stock.SetMic(_T("abcdef"));
		stock.SetFigi(_T("12345678"));
		stock.SetCurrency(_T("US Dollar"));
		stock.SetAddress(_T("Irvine CA US"));
		stock.SetCity(_T("Irvine"));
		stock.SetCountry(_T("US"));
		stock.SetCusip(_T("abcd"));
		stock.SetSedol(_T("a1b2"));
		stock.SetListedExchange(_T("NY Market"));
		stock.SetGgroup(_T("a2b3"));
		stock.SetGind(_T("Steel"));
		stock.SetGsector(_T("company"));
		stock.SetGsubind(_T("C2B3"));
		stock.SetIPODate(_T("20202020"));
		stock.SetIsin(_T("B2C4"));
		stock.SetNaics(_T("bcdefgh"));
		stock.SetNaicsNationalIndustry(_T("USA"));
		stock.SetNaicsSector(_T("Alumium"));
		stock.SetNaicsSubsector(_T("abc"));
		stock.SetName(_T("Unknown"));
		stock.SetPhone(_T("001000000"));
		stock.SetState(_T("CA"));
		stock.SetTicker(_T("000001.US"));
		stock.SetWebURL(_T("http://abc.com"));
		stock.SetLogo(_T("abc.jpg"));
		stock.SetFinnhubIndustry(_T("UK"));
		stock.SetPeer(_T("abdef"));
		stock.SetShareOutstanding(1.14);
		stock.SetEmployeeTotal(101023);
		stock.SetMarketCapitalization(34324.234);

		stock.SetTiingoPermaTicker(_T("aasdfasdfj"));
		stock.TiingoSetActive(true);
		stock.SetADR(true);
		stock.SetSICCode(1234);
		stock.SetSICIndustry(_T("defg"));
		stock.SetSICSector(_T("efg"));
		stock.SetTiingoIndustry(_T("ghi"));
		stock.SetTiingoSector(_T("defghijk"));
		stock.SetCompanyWebSite(_T("ijk"));
		stock.SetSECFilingWebSite(_T("https://def.com"));

		stock.SetProfileUpdateDate(20000102);
		stock.SetLastRTDataUpdateDate(20000103);
		stock.SetLastEPSSurpriseUpdateDate(20000104);
		stock.SetDayLineStartDate(19700101);
		stock.SetDayLineEndDate(19700102);
		stock.SetPeerUpdateDate(20000105);
		stock.SetInsiderTransactionUpdateDate(20000106);
		stock.SetInsiderSentimentUpdateDate(20000107);
		stock.SetCompanyNewsUpdateDate(20000108);
		stock.SetBasicFinancialUpdateDate(20000109);
		stock.SetTiingoDailyDataUpdateDate(20202020);
		stock.SetTiingoStatementUpdateDate(20201220);

		setWorldStock.Open();
		setWorldStock.m_pDatabase->BeginTrans();
		stock.Append(setWorldStock);
		setWorldStock.m_pDatabase->CommitTrans();
		setWorldStock.Close();

		setWorldStock.m_strFilter = _T("[Symbol] = '000001.US'");
		setWorldStock.Open();
		stock2.Load(setWorldStock);
		setWorldStock.m_pDatabase->BeginTrans();
		while (!setWorldStock.IsEOF()) {
			setWorldStock.Delete();
			setWorldStock.MoveNext();
		}
		setWorldStock.m_pDatabase->CommitTrans();
		setWorldStock.Close();

		EXPECT_STREQ(stock.GetSymbol(), stock2.GetSymbol());
		EXPECT_STREQ(stock.GetDescription(), stock2.GetDescription());
		EXPECT_STREQ(stock.GetExchangeCode(), stock2.GetExchangeCode());
		EXPECT_STREQ(stock.GetDisplaySymbol(), stock2.GetDisplaySymbol());
		EXPECT_STREQ(stock.GetType(), stock2.GetType());
		EXPECT_STREQ(stock.GetMic(), stock2.GetMic());
		EXPECT_STREQ(stock.GetFigi(), stock2.GetFigi());
		EXPECT_STREQ(stock.GetCurrency(), stock2.GetCurrency());
		EXPECT_STREQ(stock.GetAddress(), stock2.GetAddress());
		EXPECT_STREQ(stock.GetCity(), stock2.GetCity());
		EXPECT_STREQ(stock.GetCountry(), stock2.GetCountry());
		EXPECT_STREQ(stock.GetCusip(), stock2.GetCusip());
		EXPECT_STREQ(stock.GetSedol(), stock2.GetSedol());
		EXPECT_EQ(stock.GetEmployeeTotal(), stock2.GetEmployeeTotal());
		EXPECT_STREQ(stock.GetListedExchange(), stock2.GetListedExchange());
		EXPECT_STREQ(stock.GetGgroup(), stock2.GetGgroup());
		EXPECT_STREQ(stock.GetGsector(), stock2.GetGsector());
		EXPECT_STREQ(stock.GetGsubind(), stock2.GetGsubind());
		EXPECT_STREQ(stock.GetIPODate(), stock2.GetIPODate());
		EXPECT_STREQ(stock.GetIsin(), stock2.GetIsin());
		EXPECT_DOUBLE_EQ(stock.GetMarketCapitalization(), stock2.GetMarketCapitalization());
		EXPECT_STREQ(stock.GetNaics(), stock2.GetNaics());
		EXPECT_STREQ(stock.GetNaicsNationalIndustry(), stock2.GetNaicsNationalIndustry());
		EXPECT_STREQ(stock.GetNaicsSector(), stock2.GetNaicsSector());
		EXPECT_STREQ(stock.GetNaicsSubsector(), stock2.GetNaicsSubsector());
		EXPECT_STREQ(stock.GetName(), stock2.GetName());
		EXPECT_STREQ(stock.GetPhone(), stock2.GetPhone());
		EXPECT_DOUBLE_EQ(stock.GetShareOutstanding(), stock2.GetShareOutstanding());
		EXPECT_STREQ(stock.GetState(), stock2.GetState());
		EXPECT_STREQ(stock.GetTicker(), stock2.GetTicker());
		EXPECT_STREQ(stock.GetWebURL(), stock2.GetWebURL());
		EXPECT_STREQ(stock.GetLogo(), stock2.GetLogo());
		EXPECT_STREQ(stock.GetPeer(), stock2.GetPeer());
		EXPECT_EQ(stock.GetProfileUpdateDate(), stock2.GetProfileUpdateDate());
		EXPECT_EQ(stock.GetDayLineStartDate(), stock2.GetDayLineStartDate());
		EXPECT_EQ(stock.GetDayLineEndDate(), stock2.GetDayLineEndDate());
		EXPECT_EQ(stock.GetLastRTDataUpdateDate(), stock2.GetLastRTDataUpdateDate());
		EXPECT_EQ(stock.GetLastEPSSurpriseUpdateDate(), stock2.GetLastEPSSurpriseUpdateDate());
		EXPECT_EQ(stock.GetIPOStatus(), stock2.GetIPOStatus());
		EXPECT_STREQ(stock.GetTiingoPermaTicker(), stock2.GetTiingoPermaTicker());
		EXPECT_TRUE(stock.TiingoIsActive() == stock2.TiingoIsActive());
		EXPECT_TRUE(stock.IsADR() == stock2.IsADR());
		EXPECT_EQ(stock.GetSICCode(), stock2.GetSICCode());
		EXPECT_STREQ(stock.GetSICIndustry(), stock2.GetSICIndustry());
		EXPECT_STREQ(stock.GetSICSector(), stock2.GetSICSector());
		EXPECT_STREQ(stock.GetCompanyWebSite(), stock2.GetCompanyWebSite());
		EXPECT_STREQ(stock.GetSECFilingWebSite(), stock2.GetSECFilingWebSite());
		EXPECT_EQ(stock.GetProfileUpdateDate(), stock2.GetProfileUpdateDate());
		EXPECT_EQ(stock.GetLastRTDataUpdateDate(), stock2.GetLastRTDataUpdateDate());
		EXPECT_EQ(stock.GetLastEPSSurpriseUpdateDate(), stock2.GetLastEPSSurpriseUpdateDate());
		EXPECT_EQ(stock.GetDayLineStartDate(), stock2.GetDayLineStartDate());
		EXPECT_EQ(stock.GetDayLineEndDate(), stock2.GetDayLineEndDate());
		EXPECT_EQ(stock.GetPeerUpdateDate(), stock2.GetPeerUpdateDate());
		EXPECT_EQ(stock.GetInsiderTransactionUpdateDate(), stock2.GetInsiderTransactionUpdateDate());
		EXPECT_EQ(stock.GetInsiderSentimentUpdateDate(), stock2.GetInsiderSentimentUpdateDate());
		EXPECT_EQ(stock.GetTiingoDailyDataUpdateDate(), stock2.GetTiingoDailyDataUpdateDate());
		EXPECT_EQ(stock.GetTiingoStatementUpdateDate(), stock2.GetTiingoStatementUpdateDate());
	}

	TEST_F(CWorldStockTest, TestCheckCompanyNewsUpdated) {
		CWorldStock stock;
		stock.SetCompanyNewsUpdateDate(20220101);
		EXPECT_TRUE(stock.CheckCompanyNewsUpdateStatus(20220102));
		stock.SetCompanyNewsUpdated(false);
		EXPECT_TRUE(stock.CheckCompanyNewsUpdateStatus(20220107)) << "只有六天";
		stock.SetCompanyNewsUpdated(false);
		EXPECT_FALSE(stock.CheckCompanyNewsUpdateStatus(20220108)) << "每周检查一次公司新闻";
		stock.SetCompanyNewsUpdated(false);
		EXPECT_FALSE(stock.CheckCompanyNewsUpdateStatus(20220110));
	}
}
