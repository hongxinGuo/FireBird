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
	class CWorldStockTest : public Test {
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
		CWorldStockPtr pStock;
		CWorldStock stock;
	};

	TEST_F(CWorldStockTest, TestResetAllUpdateDate) {
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

	TEST_F(CWorldStockTest, TestGetRatio) {
		EXPECT_EQ(stock.GetRatio(), 1000) << "�����г��Ĺ�Ʊ�۸񣬷Ŵ���Ϊ1000";
	}

	TEST_F(CWorldStockTest, TestGetExchangeCode) {
		EXPECT_STREQ(stock.GetExchangeCode(), _T("US"));
		stock.SetExchangeCode(_T("SS"));
		EXPECT_STREQ(stock.GetExchangeCode(), _T("SS"));
	}

	TEST_F(CWorldStockTest, TestGetSymbol) {
		EXPECT_STREQ(stock.GetSymbol(), _T(""));
		stock.SetSymbol(_T("600000.SS"));
		EXPECT_STREQ(stock.GetSymbol(), _T("600000.SS"));
	}

	TEST_F(CWorldStockTest, TestGetTransactionTime) {
		EXPECT_EQ(stock.GetTransactionTime(), 0);
		stock.SetTransactionTime(1010101010);
		EXPECT_EQ(stock.GetTransactionTime(), 1010101010);
	}

	TEST_F(CWorldStockTest, TestGetLastClose) {
		EXPECT_EQ(stock.GetLastClose(), 0);
		stock.SetLastClose(10101010);
		EXPECT_EQ(stock.GetLastClose(), 10101010);
	}

	TEST_F(CWorldStockTest, TestGetOpen) {
		EXPECT_EQ(stock.GetOpen(), 0);
		stock.SetOpen(10101);
		EXPECT_EQ(stock.GetOpen(), 10101);
	}

	TEST_F(CWorldStockTest, TestGetHigh) {
		EXPECT_EQ(stock.GetHigh(), 0);
		stock.SetHigh(19980101);
		EXPECT_EQ(stock.GetHigh(), 19980101);
	}

	TEST_F(CWorldStockTest, TestGetLow) {
		EXPECT_EQ(stock.GetLow(), 0);
		stock.SetLow(19980101);
		EXPECT_EQ(stock.GetLow(), 19980101);
	}

	TEST_F(CWorldStockTest, TestGetNew) {
		EXPECT_EQ(stock.GetNew(), 0);
		stock.SetNew(10101010);
		EXPECT_EQ(stock.GetNew(), 10101010);
	}

	TEST_F(CWorldStockTest, TestGetUpDown) {
		EXPECT_EQ(stock.GetUpDown(), 0);
		stock.SetUpDown(10101010);
		EXPECT_EQ(stock.GetUpDown(), 10101010);
	}

	TEST_F(CWorldStockTest, TestGetAmount) {
		EXPECT_EQ(stock.GetAmount(), 0);
		stock.SetAmount(1010101010101010);
		EXPECT_EQ(stock.GetAmount(), 1010101010101010);
	}

	TEST_F(CWorldStockTest, TestGetVolume) {
		EXPECT_EQ(stock.GetVolume(), 0);
		stock.SetVolume(10101010);
		EXPECT_EQ(stock.GetVolume(), 10101010);
	}

	TEST_F(CWorldStockTest, TestGetTotalValue) {
		EXPECT_EQ(stock.GetTotalValue(), 0);
		stock.SetTotalValue(10101010);
		EXPECT_EQ(stock.GetTotalValue(), 10101010);
	}

	TEST_F(CWorldStockTest, TestGetCurrentValue) {
		EXPECT_EQ(stock.GetCurrentValue(), 0);
		stock.SetCurrentValue(10101010);
		EXPECT_EQ(stock.GetCurrentValue(), 10101010);
	}

	TEST_F(CWorldStockTest, TestGetChangeHandRate) {
		EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 0.0);
		stock.SetChangeHandRate(1.1);
		EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 1.1);
	}

	TEST_F(CWorldStockTest, TestGetUpDownRate) {
		EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 0.0);
		stock.SetUpDownRate(1.1);
		EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 1.1);
	}

	TEST_F(CWorldStockTest, TestIsTodayNewStock) {
		EXPECT_FALSE(stock.IsTodayNewStock());
		stock.SetTodayNewStock(true);
		EXPECT_TRUE(stock.IsTodayNewStock());
		stock.SetTodayNewStock(false);
		EXPECT_FALSE(stock.IsTodayNewStock());
	}

	TEST_F(CWorldStockTest, TestIsUpdateStockProfileDB) {
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
		EXPECT_FALSE(stock.IsUpdateProfileDB());
		EXPECT_FALSE(stock.IsUpdateProfileDBAndClearFlag());
		stock.SetUpdateProfileDB(true);
		EXPECT_TRUE(stock.IsUpdateProfileDBAndClearFlag());
		EXPECT_FALSE(stock.IsUpdateProfileDB());
	}

	TEST_F(CWorldStockTest, TestIsUpdateCompanyNewsDB) {
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

	TEST_F(CWorldStockTest, TestIsUpdateCompanyNewsDBAndClearFlag) {
		EXPECT_FALSE(stock.IsUpdateCompanyNewsDB());
		EXPECT_FALSE(stock.IsUpdateCompanyNewsDBAndClearFlag());
		stock.SetUpdateCompanyNewsDB(true);
		EXPECT_TRUE(stock.IsUpdateCompanyNewsDBAndClearFlag());
		EXPECT_FALSE(stock.IsUpdateCompanyNewsDB());
	}

	TEST_F(CWorldStockTest, TestIsActive) {
		EXPECT_FALSE(stock.IsActive());
		stock.SetActive(true);
		EXPECT_TRUE(stock.IsActive());
		stock.SetActive(false);
		EXPECT_FALSE(stock.IsActive());
	}

	TEST_F(CWorldStockTest, TestGetDayLineEndDate) {
		EXPECT_EQ(stock.GetDayLineEndDate(), 19800101);
		stock.SetDayLineEndDate(19980101);
		EXPECT_EQ(stock.GetDayLineEndDate(), 19980101);
	}

	TEST_F(CWorldStockTest, TestGetDayLineStartDate) {
		EXPECT_EQ(stock.GetDayLineStartDate(), 29900101);
		stock.SetDayLineStartDate(19980101);
		EXPECT_EQ(stock.GetDayLineStartDate(), 19980101);
	}

	TEST_F(CWorldStockTest, TestGetIPOStatus) {
		EXPECT_TRUE(stock.IsNotChecked());
		stock.SetIPOStatus(255);
		EXPECT_EQ(stock.GetIPOStatus(), 255);
	}

	TEST_F(CWorldStockTest, TestIsNullStock) {
		stock.SetIPOStatus(_STOCK_NULL_);
		EXPECT_TRUE(stock.IsNullStock());
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_FALSE(stock.IsNullStock());
	}

	TEST_F(CWorldStockTest, TestIsIPOed) {
		stock.SetIPOStatus(_STOCK_IPOED_);
		EXPECT_TRUE(stock.IsIPOed());
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_FALSE(stock.IsIPOed());
	}

	TEST_F(CWorldStockTest, TestIsNotChecked) {
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_TRUE(stock.IsNotChecked());
		stock.SetIPOStatus(_STOCK_DELISTED_);
		EXPECT_FALSE(stock.IsNotChecked());
	}

	TEST_F(CWorldStockTest, TestIsDelisted) {
		stock.SetIPOStatus(_STOCK_DELISTED_);
		EXPECT_TRUE(stock.IsDelisted());
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_FALSE(stock.IsDelisted());
	}

	TEST_F(CWorldStockTest, TestIsNotYetList) {
		stock.SetIPOStatus(_STOCK_NOT_YET_LIST_);
		EXPECT_TRUE(stock.IsNotYetList());
		stock.SetIPOStatus(_STOCK_NOT_CHECKED_);
		EXPECT_FALSE(stock.IsNotYetList());
	}

	TEST_F(CWorldStockTest, TestIsUpdateDayLine) {
		EXPECT_TRUE(stock.IsUpdateDayLine());
		stock.SetUpdateDayLine(false);
		EXPECT_FALSE(stock.IsUpdateDayLine());
		stock.SetUpdateDayLine(true);
		EXPECT_TRUE(stock.IsUpdateDayLine());
	}

	TEST_F(CWorldStockTest, TestIsDayLineNeededSaving) {
		// �����������Ǿ߱�ͬ�����Ƶģ�����û�н��в���
		EXPECT_FALSE(stock.IsUpdateDayLineDB());
		stock.SetUpdateDayLineDB(true);
		EXPECT_TRUE(stock.IsUpdateDayLineDB());
		stock.SetUpdateDayLineDB(false);
		EXPECT_FALSE(stock.IsUpdateDayLineDB());
		stock.SetUpdateDayLineDB(true);
		EXPECT_TRUE(stock.IsUpdateDayLineDBAndClearFlag());
		EXPECT_FALSE(stock.IsUpdateDayLineDB());
	}

	TEST_F(CWorldStockTest, TestGetProfileUpdateDate) {
		EXPECT_EQ(stock.GetProfileUpdateDate(), 19800101);
		stock.SetProfileUpdateDate(10101016);
		EXPECT_EQ(stock.GetProfileUpdateDate(), 10101016);
	}

	TEST_F(CWorldStockTest, TestGetCompanyNewsUpdateDate) {
		EXPECT_EQ(stock.GetCompanyNewsUpdateDate(), 19800101);
		stock.SetCompanyNewsUpdateDate(10101017);
		EXPECT_EQ(stock.GetCompanyNewsUpdateDate(), 10101017);
	}

	TEST_F(CWorldStockTest, TestGetBasicFinancialUpdateDate) {
		EXPECT_EQ(stock.GetBasicFinancialUpdateDate(), 19800101);
		stock.SetBasicFinancialUpdateDate(10101018);
		EXPECT_EQ(stock.GetBasicFinancialUpdateDate(), 10101018);
	}

	TEST_F(CWorldStockTest, TestGetLastRTDataUpdateDate) {
		EXPECT_EQ(stock.GetLastRTDataUpdateDate(), 19800101);
		stock.SetLastRTDataUpdateDate(10101011);
		EXPECT_EQ(stock.GetLastRTDataUpdateDate(), 10101011);
	}

	TEST_F(CWorldStockTest, TestPeerUpdateDate) {
		EXPECT_EQ(stock.GetPeerUpdateDate(), 19800101);
		stock.SetPeerUpdateDate(10101012);
		EXPECT_EQ(stock.GetPeerUpdateDate(), 10101012);
	}

	TEST_F(CWorldStockTest, TestInsiderTransactionUpdateDate) {
		EXPECT_EQ(stock.GetInsiderTransactionUpdateDate(), 19800101);
		stock.SetInsiderTransactionUpdateDate(10101013);
		EXPECT_EQ(stock.GetInsiderTransactionUpdateDate(), 10101013);
	}

	TEST_F(CWorldStockTest, TestGetInsiderSentimentUpdateDate) {
		EXPECT_EQ(stock.GetInsiderSentimentUpdateDate(), 19800101);
		stock.SetInsiderSentimentUpdateDate(10101014);
		EXPECT_EQ(stock.GetInsiderSentimentUpdateDate(), 10101014);
	}

	TEST_F(CWorldStockTest, TestLastEPSSurpriseUpdateDate) {
		EXPECT_EQ(stock.GetLastEPSSurpriseUpdateDate(), 19800101);
		stock.SetLastEPSSurpriseUpdateDate(10101015);
		EXPECT_EQ(stock.GetLastEPSSurpriseUpdateDate(), 10101015);
	}

	TEST_F(CWorldStockTest, TestIsUpdateCompanyProfile) {
		EXPECT_TRUE(stock.IsUpdateCompanyProfile());
		stock.SetUpdateCompanyProfile(false);
		EXPECT_FALSE(stock.IsUpdateCompanyProfile());
	}

	TEST_F(CWorldStockTest, TestIsEPSSurpriseUpdated) {
		EXPECT_FALSE(stock.IsEPSSurpriseUpdated());
		stock.SetEPSSurpriseUpdated(true);
		EXPECT_TRUE(stock.IsEPSSurpriseUpdated());
	}

	TEST_F(CWorldStockTest, TestIsEPSSurpriseNeedSave) {
		EXPECT_FALSE(stock.IsEPSSurpriseNeedSave());
		stock.SetEPSSurpriseNeedSave(true);
		EXPECT_TRUE(stock.IsEPSSurpriseNeedSave());
		EXPECT_TRUE(stock.IsEPSSurpriseNeedSaveAndClearFlag());
		EXPECT_FALSE(stock.IsEPSSurpriseNeedSave());
	}

	TEST_F(CWorldStockTest, TestIsSECFilingsNeedSave) {
		EXPECT_FALSE(stock.IsSECFilingsNeedSave());
		stock.SetSECFilingsNeedSave(true);
		EXPECT_TRUE(stock.IsSECFilingsNeedSave());
		EXPECT_TRUE(stock.IsSECFilingsNeedSaveAndClearFlag());
		EXPECT_FALSE(stock.IsSECFilingsNeedSave());
	}

	TEST_F(CWorldStockTest, TestIsUpdateInsiderTransaction) {
		EXPECT_TRUE(stock.IsUpdateInsiderTransaction());
		stock.SetUpdateInsiderTransaction(false);
		EXPECT_FALSE(stock.IsUpdateInsiderTransaction());
		stock.SetUpdateInsiderTransaction(true);
		EXPECT_TRUE(stock.IsUpdateInsiderTransaction());
	}

	TEST_F(CWorldStockTest, TestIsInsiderTransactionNeedSave) {
		EXPECT_FALSE(stock.IsUpdateInsiderTransactionDB());
		stock.SetUpdateInsiderTransactionDB(true);
		EXPECT_TRUE(stock.IsUpdateInsiderTransactionDB());
		EXPECT_TRUE(stock.IsUpdateInsiderTransactionDBAndClearFlag());
		EXPECT_FALSE(stock.IsUpdateInsiderTransactionDB());
	}

	TEST_F(CWorldStockTest, TestIsUpdateInsiderSentiment) {
		EXPECT_TRUE(stock.IsUpdateInsiderSentiment());
		stock.SetUpdateInsiderSentiment(false);
		EXPECT_FALSE(stock.IsUpdateInsiderSentiment());
		stock.SetUpdateInsiderSentiment(true);
		EXPECT_TRUE(stock.IsUpdateInsiderSentiment());
	}

	TEST_F(CWorldStockTest, TestIsInsiderSentimentNeedSave) {
		EXPECT_FALSE(stock.IsUpdateInsiderSentimentDB());
		stock.SetUpdateInsiderSentimentDB(true);
		EXPECT_TRUE(stock.IsUpdateInsiderSentimentDB());
		EXPECT_TRUE(stock.IsUpdateInsiderSentimentDBAndClearFlag());
		EXPECT_FALSE(stock.IsUpdateInsiderSentimentDB());
	}

	TEST_F(CWorldStockTest, TestGetDescription) {
		EXPECT_STREQ(stock.GetDescription(), _T(""));
		stock.SetDescription(_T("abcdef"));
		EXPECT_STREQ(stock.GetDescription(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetDisplaySymbol) {
		EXPECT_STREQ(stock.GetDisplaySymbol(), _T(""));
		stock.SetDisplaySymbol(_T("abcdef"));
		EXPECT_STREQ(stock.GetDisplaySymbol(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetType) {
		EXPECT_STREQ(stock.GetType(), _T(" "));
		stock.SetType(_T("abcdef"));
		EXPECT_STREQ(stock.GetType(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetMic) {
		EXPECT_STREQ(stock.GetMic(), _T(" "));
		stock.SetMic(_T("abcdef"));
		EXPECT_STREQ(stock.GetMic(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetFigi) {
		EXPECT_STREQ(stock.GetFigi(), _T(" "));
		stock.SetFigi(_T("abcdef"));
		EXPECT_STREQ(stock.GetFigi(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetCurrency) {
		EXPECT_STREQ(stock.GetCurrency(), _T(" "));
		stock.SetCurrency(_T("abcdef"));
		EXPECT_STREQ(stock.GetCurrency(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetAddress) {
		EXPECT_STREQ(stock.GetAddress(), _T(" "));
		stock.SetAddress(_T("abcdef"));
		EXPECT_STREQ(stock.GetAddress(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetCity) {
		EXPECT_STREQ(stock.GetCity(), _T(" "));
		stock.SetCity(_T("abcdef"));
		EXPECT_STREQ(stock.GetCity(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetCountry) {
		EXPECT_STREQ(stock.GetCountry(), _T(" "));
		stock.SetCountry(_T("abcdef"));
		EXPECT_STREQ(stock.GetCountry(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetCusip) {
		EXPECT_STREQ(stock.GetCusip(), _T(" "));
		stock.SetCusip(_T("abcdef"));
		EXPECT_STREQ(stock.GetCusip(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetSedol) {
		EXPECT_STREQ(stock.GetSedol(), _T(" "));
		stock.SetSedol(_T("abcdef"));
		EXPECT_STREQ(stock.GetSedol(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetEmployeeTotal) {
		EXPECT_EQ(stock.GetEmployeeTotal(), 0);
		stock.SetEmployeeTotal(1234567);
		EXPECT_EQ(stock.GetEmployeeTotal(), 1234567);
	}

	TEST_F(CWorldStockTest, TestGetListedExchange) {
		EXPECT_STREQ(stock.GetListedExchange(), _T(" "));
		stock.SetListedExchange(_T("abcdef"));
		EXPECT_STREQ(stock.GetListedExchange(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetGgroup) {
		EXPECT_STREQ(stock.GetGgroup(), _T(" "));
		stock.SetGgroup(_T("abcdef"));
		EXPECT_STREQ(stock.GetGgroup(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetGind) {
		EXPECT_STREQ(stock.GetGind(), _T(" "));
		stock.SetGind(_T("abcdef"));
		EXPECT_STREQ(stock.GetGind(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetGsector) {
		EXPECT_STREQ(stock.GetGsector(), _T(" "));
		stock.SetGsector(_T("abcdef"));
		EXPECT_STREQ(stock.GetGsector(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetGsubind) {
		EXPECT_STREQ(stock.GetGsubind(), _T(" "));
		stock.SetGsubind(_T("abcdef"));
		EXPECT_STREQ(stock.GetGsubind(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetIPODate) {
		EXPECT_STREQ(stock.GetIPODate(), _T(" "));
		stock.SetIPODate(_T("abcdef"));
		EXPECT_STREQ(stock.GetIPODate(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetIsin) {
		EXPECT_STREQ(stock.GetIsin(), _T(" "));
		stock.SetIsin(_T("abcdef"));
		EXPECT_STREQ(stock.GetIsin(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetMarketCapitalization) {
		EXPECT_DOUBLE_EQ(stock.GetMarketCapitalization(), 0.0);
		stock.SetMarketCapitalization(10101.010);
		EXPECT_DOUBLE_EQ(stock.GetMarketCapitalization(), 10101.010);
	}

	TEST_F(CWorldStockTest, TestGetNaics) {
		EXPECT_STREQ(stock.GetNaics(), _T(" "));
		stock.SetNaics(_T("abcdef"));
		EXPECT_STREQ(stock.GetNaics(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetNaicsNationalIndustry) {
		EXPECT_STREQ(stock.GetNaicsNationalIndustry(), _T(" "));
		stock.SetNaicsNationalIndustry(_T("abcdef"));
		EXPECT_STREQ(stock.GetNaicsNationalIndustry(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetNaicsSector) {
		EXPECT_STREQ(stock.GetNaicsSector(), _T(" "));
		stock.SetNaicsSector(_T("abcdef"));
		EXPECT_STREQ(stock.GetNaicsSector(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetNaicsSubsector) {
		EXPECT_STREQ(stock.GetNaicsSubsector(), _T(" "));
		stock.SetNaicsSubsector(_T("abcdef"));
		EXPECT_STREQ(stock.GetNaicsSubsector(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetName) {
		EXPECT_STREQ(stock.GetName(), _T(" "));
		stock.SetName(_T("abcdef"));
		EXPECT_STREQ(stock.GetName(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetPhone) {
		EXPECT_STREQ(stock.GetPhone(), _T(" "));
		stock.SetPhone(_T("abcdef"));
		EXPECT_STREQ(stock.GetPhone(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetShareOutstanding) {
		EXPECT_DOUBLE_EQ(stock.GetShareOutstanding(), 0.0);
		stock.SetShareOutstanding(10101.010);
		EXPECT_DOUBLE_EQ(stock.GetShareOutstanding(), 10101.010);
	}

	TEST_F(CWorldStockTest, TestGetState) {
		EXPECT_STREQ(stock.GetState(), _T(" "));
		stock.SetState(_T("abcdef"));
		EXPECT_STREQ(stock.GetState(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetWebURL) {
		EXPECT_STREQ(stock.GetWebURL(), _T(" "));
		stock.SetWebURL(_T("abcdef"));
		EXPECT_STREQ(stock.GetWebURL(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetLogo) {
		EXPECT_STREQ(stock.GetLogo(), _T(" "));
		stock.SetLogo(_T("abcdef"));
		EXPECT_STREQ(stock.GetLogo(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetTicker) {
		EXPECT_STREQ(stock.GetTicker(), _T(" "));
		stock.SetTicker(_T("abcdef"));
		EXPECT_STREQ(stock.GetTicker(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetFinnhubIndustry) {
		EXPECT_STREQ(stock.GetFinnhubIndustry(), _T(" "));
		stock.SetFinnhubIndustry(_T("abcdef"));
		EXPECT_STREQ(stock.GetFinnhubIndustry(), _T("abcdef"));
	}

	TEST_F(CWorldStockTest, TestGetPeer) {
		EXPECT_TRUE(stock.GetPeer().empty());
		const json jSon = json::parse(_T("[\"abcdef\",\"000001.SS\"]"));
		stock.SetPeer(jSon);
		const json jSon2 = stock.GetPeer();
		const string s1 = jSon2.dump();
		EXPECT_STREQ(s1.c_str(), _T("[\"abcdef\",\"000001.SS\"]"));
	}

	TEST_F(CWorldStockTest, TestIsUSMarket) {
		stock.SetExchangeCode(_T("SS"));
		EXPECT_FALSE(stock.IsUSMarket());
		stock.SetExchangeCode(_T("US"));
		EXPECT_TRUE(stock.IsUSMarket());
	}

	TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus1) {
		stock.SetUpdateDayLine(true);
		stock.SetActive(false);
		for (int i = 1; i < 6; i++) {
			EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "�ǻ�Ծ��Ʊ�����ղ���������\n";
			stock.SetUpdateDayLine(true);
		}
	}

	TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus2) {
		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_NULL_);
		EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, 0)) << "��Ч��Ʊ���������\n";
	}

	TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus3) {
		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_DELISTED_);
		for (int i = 0; i < 7; i++) {
			if (i == 4)
				EXPECT_TRUE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "ժ�ƹ�Ʊֻ�������ļ������\n";
			else
				EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "ժ�ƹ�Ʊֻ�������ļ������\n";
			stock.SetUpdateDayLine(true);
		}
	}

	TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus4) {
		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetActive(true);
		stock.SetDayLineEndDate(GetPrevDay(gl_pWorldMarket->GetMarketDate(), 100));
		EXPECT_TRUE(stock.CheckDayLineUpdateStatus(gl_pWorldMarket->GetMarketDate(), gl_pWorldMarket->GetMarketDate(), 0, 1));
		stock.SetDayLineEndDate(GetPrevDay(stock.GetDayLineEndDate()));
		EXPECT_FALSE(stock.CheckDayLineUpdateStatus(gl_pWorldMarket->GetMarketDate(), gl_pWorldMarket->GetMarketDate(), 0, 1)) << "����100��Ĺ�Ʊ���ٸ�������";
	}

	TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus5) {
		const long lCurrentDay = gl_pWorldMarket->GetMarketDate();
		const long lPrevDay = GetPrevDay(lCurrentDay);

		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetActive(true);
		stock.SetDayLineEndDate(lCurrentDay); // ���ս����������ѽ���
		for (int i = 1; i < 6; i++) {
			EXPECT_FALSE(stock.CheckDayLineUpdateStatus(lCurrentDay, lPrevDay, 170001, i)) << "ʱ������17ʱ�󣬼�鵱������";
			stock.SetUpdateDayLine(true); // ����״̬
		}
		stock.SetDayLineEndDate(lPrevDay); // ���ս�����������δ����
		for (int i = 1; i < 6; i++) { EXPECT_TRUE(stock.CheckDayLineUpdateStatus(lCurrentDay, lPrevDay, 170001, i)) << "ʱ������17ʱ�󣬼�鵱������"; }
	}

	TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus6) {
		const long lCurrentDay = gl_pWorldMarket->GetMarketDate();
		const long lPrevDay = GetPrevDay(lCurrentDay);

		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetActive(true);
		stock.SetDayLineEndDate(GetPrevDay(lCurrentDay)); // ��һ���������������ѽ���
		for (int i = 1; i < 6; i++) {
			EXPECT_FALSE(stock.CheckDayLineUpdateStatus(lCurrentDay, lPrevDay, 170000, i)) << "ʱ�䲻����17ʱ�������һ���������� " << i;
			stock.SetUpdateDayLine(true); // ����֮
		}
		stock.SetDayLineEndDate(GetPrevDay(lCurrentDay, 2)); // ��һ��������������δ����
		for (int i = 1; i < 6; i++) { EXPECT_TRUE(stock.CheckDayLineUpdateStatus(lCurrentDay, lPrevDay, 170000, i)) << "ʱ�䲻����17ʱ�������һ����������"; }
	}

	TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus7) {
		const long lCurrentDate = gl_pWorldMarket->GetMarketDate();
		const long lPrevMonday = GetPrevMonday(lCurrentDate);

		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetActive(true);
		stock.SetDayLineEndDate(GetPrevDay(lPrevMonday, 3)); // ��һ���������������ѽ���
		EXPECT_FALSE(stock.CheckDayLineUpdateStatus(GetPrevDay(lPrevMonday, 2), GetPrevDay(lPrevMonday, 3), 170000, 6)) << "�����������һ����������";
		stock.SetUpdateDayLine(true); // ����֮
		EXPECT_FALSE(stock.CheckDayLineUpdateStatus(GetPrevDay(lPrevMonday, 1), GetPrevDay(lPrevMonday, 3), 170000, 0)) << "���գ������һ����������";
	}

	TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus8) {
		stock.SetUpdateDayLine(true);
		stock.SetIPOStatus(_STOCK_NOT_YET_LIST_);
		for (int i = 0; i < 7; i++) {
			if (i == 4)
				EXPECT_TRUE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "ժ�ƹ�Ʊֻ�������ļ������\n";
			else
				EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "ժ�ƹ�Ʊֻ�������ļ������\n";
			stock.SetUpdateDayLine(true);
		}
	}

	TEST_F(CWorldStockTest, TestCheckEPSSurpriseStatus) {
		constexpr long lCurrentDate = 20200101;

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
		stock.SetLastEPSSurpriseUpdateDate(20191003); // ������90��
		stock.CheckEPSSurpriseStatus(lCurrentDate);
		EXPECT_TRUE(stock.IsEPSSurpriseUpdated());

		stock.SetEPSSurpriseUpdated(false);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetLastEPSSurpriseUpdateDate(20191002); // ����90�죬 ������900��
		stock.CheckEPSSurpriseStatus(lCurrentDate);
		EXPECT_FALSE(stock.IsEPSSurpriseUpdated());

		stock.SetEPSSurpriseUpdated(false);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetLastEPSSurpriseUpdateDate(20160521); // ����900��
		stock.CheckEPSSurpriseStatus(lCurrentDate);
		EXPECT_TRUE(stock.IsEPSSurpriseUpdated());
	}

	TEST_F(CWorldStockTest, TestCheckSECFilingsStatus) {
		constexpr long lCurrentDate = 20200101;

		stock.SetSECFilingsUpdated(false);
		stock.SetIPOStatus(_STOCK_NULL_);
		stock.CheckSECFilingsStatus(lCurrentDate);
		EXPECT_TRUE(stock.IsSECFilingsUpdated());

		stock.SetSECFilingsUpdated(false);
		stock.SetIPOStatus(_STOCK_DELISTED_);
		stock.CheckSECFilingsStatus(lCurrentDate);
		EXPECT_TRUE(stock.IsSECFilingsUpdated());

		stock.SetSECFilingsUpdated(false);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetSECFilingsUpdateDate(20191202); // ������30��
		stock.CheckSECFilingsStatus(lCurrentDate);
		EXPECT_TRUE(stock.IsSECFilingsUpdated());

		stock.SetSECFilingsUpdated(false);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.SetSECFilingsUpdateDate(20191201); // ����30��
		stock.CheckSECFilingsStatus(lCurrentDate);
		EXPECT_FALSE(stock.IsSECFilingsUpdated());
	}

	TEST_F(CWorldStockTest, TestSaveDayLine) {
		vector<CDayLinePtr> vDayLine;
		CSetWorldStockDayLine setDayLine;

		CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20200101); // ��������������ݿ��е��������ڣ���Ҫ��ӽ����ݿ�
		pDayLine->SetClose(10010);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210101); // �������Ϊ�����ڣ���Ҫ��ӽ����ݿ�
		pDayLine->SetClose(12345);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210107); // ������ݿ����У��������
		pDayLine->SetClose(10020);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210123); // �������Ϊ�����ڣ���Ҫ��ӽ����ݿ�
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
		EXPECT_FALSE(stock.IsUpdateDayLineDB());
		EXPECT_FALSE(stock.UpdateDayLineDB()) << "�������߱�ʶΪ��ʱ������ִ��ʵ�ʲ���";
	}

	TEST_F(CWorldStockTest, TestUpdateDayLineDB2) {
		EXPECT_FALSE(stock.IsUpdateDayLineDB());
		stock.SetUpdateDayLineDB(true); // ��Ҫ����
		EXPECT_EQ(stock.GetDayLineSize(), 0);

		EXPECT_FALSE(stock.UpdateDayLineDB()) << "��������Ϊ��ʱ������ִ��ʵ�ʲ���";
		EXPECT_FALSE(stock.IsUpdateDayLineDB()) << "���±�ʶ�ѱ�����Ϊ��";
	}

	TEST_F(CWorldStockTest, TestUpdateDayLineDB3) {
		vector<CDayLinePtr> vDayLine;
		CDayLinePtr pDayLine;
		CSetWorldStockDayLine setDayLine;

		EXPECT_FALSE(stock.IsUpdateDayLineDB());
		stock.SetUpdateDayLineDB(true); // ��Ҫ����
		EXPECT_EQ(stock.GetDayLineSize(), 0);

		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20200101); // �����Ҫ��ӽ����ݿ�
		pDayLine->SetClose(10010);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210101); // �����Ҫ��ӽ����ݿ�
		pDayLine->SetClose(12345);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210107); // ������ݿ����У��������
		pDayLine->SetClose(10020);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210123); // �����Ҫ��ӽ����ݿ�
		pDayLine->SetClose(10030);
		vDayLine.push_back(pDayLine);

		stock.SetSymbol(_T("A"));
		stock.SetDayLineStartDate(20200101); // ���߿�ʼ���ڲ����ڵ�ǰ������������
		stock.SetDayLineEndDate(20210123); // ���߽������ڲ����ڵ�ǰ������������
		stock.UpdateDayLine(vDayLine);

		EXPECT_FALSE(stock.UpdateDayLineDB()) << "����ִ��ʵ�ʲ���";
		EXPECT_FALSE(stock.IsUpdateDayLineDB()) << "���±�ʶ�ѱ�����Ϊ��";
		EXPECT_EQ(stock.GetDayLineSize(), 0) << "�������������";

		stock.SetUpdateDayLineDB(true); // ��Ҫ����
		stock.SetDayLineStartDate(20200102); // ���߿�ʼ�������ڵ�ǰ������������
		stock.SetDayLineEndDate(20210123); // ���߽������ڲ����ڵ�ǰ������������
		stock.UpdateDayLine(vDayLine);

		EXPECT_TRUE(stock.UpdateDayLineDB()) << "ִ��ʵ�ʲ���";
		EXPECT_FALSE(stock.IsUpdateDayLineDB()) << "���±�ʶ�ѱ�����Ϊ��";
		EXPECT_EQ(stock.GetDayLineSize(), 0);
		EXPECT_EQ(stock.GetDayLineStartDate(), 20200101) << "���߿�ʼ�����Ѹ���Ϊ��������";
		EXPECT_TRUE(stock.IsUpdateProfileDB());
		ASSERT_GE(gl_systemMessage.DayLineInfoSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopDayLineInfoMessage(), _T("A�������ϴ洢���"));

		// �����ӵ����ݣ��ָ�ԭ״
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

	TEST_F(CWorldStockTest, TestUpdateInsiderTransactionDB) {
		//  �������ݿ���ֻ��4�����ݣ���Ʊ���룺A�� �ڲ�������Ա��a b c��
		vector<CInsiderTransactionPtr> vInsiderTransaction;
		CSetInsiderTransaction setInsiderTransaction;

		CInsiderTransactionPtr pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = _T("B");
		pInsiderTransaction->m_strPersonName = _T("a b c");
		pInsiderTransaction->m_lTransactionDate = 20200101; // �����Ʊ���벻������Ҫ��ӽ����ݿ�
		vInsiderTransaction.push_back(pInsiderTransaction);
		pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = _T("A");
		pInsiderTransaction->m_strPersonName = _T("a b c d");
		pInsiderTransaction->m_lTransactionDate = 20210101; // ����ڲ�������Ա���Ʋ�������Ҫ��ӽ����ݿ�
		vInsiderTransaction.push_back(pInsiderTransaction);
		pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = _T("A");
		pInsiderTransaction->m_strPersonName = _T("a b c");
		pInsiderTransaction->m_lTransactionDate = 20210107;
		pInsiderTransaction->m_strTransactionCode = _T("M"); // ������ݿ����У��������
		vInsiderTransaction.push_back(pInsiderTransaction);
		pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = _T("A");
		pInsiderTransaction->m_strPersonName = _T("a b c");
		pInsiderTransaction->m_lTransactionDate = 20210124; // ������ڽ��£���Ҫ��ӽ����ݿ�
		vInsiderTransaction.push_back(pInsiderTransaction);
		pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = _T("A");
		pInsiderTransaction->m_lTransactionDate = 20210107;
		pInsiderTransaction->m_strPersonName = _T("a b c");
		pInsiderTransaction->m_strTransactionCode = _T("S"); // ����������Ͳ�������Ҫ��ӽ����ݿ�
		vInsiderTransaction.push_back(pInsiderTransaction);

		stock.SetSymbol(_T("A"));
		stock.SetInsiderTransactionUpdateDate(20210123);
		stock.UpdateInsiderTransaction(vInsiderTransaction);

		stock.UpdateInsiderTransactionDB();

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
		//  �������ݿ���ֻ��4�����ݣ���Ʊ���룺A��
		vector<CInsiderSentimentPtr> vInsiderSentiment;
		CSetInsiderSentiment setInsiderSentiment;

		CInsiderSentimentPtr pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment->m_strSymbol = _T("B"); // �����Ʊ���벻���������ܳ��ִ���������ʶ�����Ҫ��ӽ����ݿ�
		pInsiderSentiment->m_lDate = 20200101;
		vInsiderSentiment.push_back(pInsiderSentiment);
		pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment->m_strSymbol = _T("A");
		pInsiderSentiment->m_lDate = 20200101; // ������ݿ����У��������
		vInsiderSentiment.push_back(pInsiderSentiment);
		pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment->m_strSymbol = _T("A");
		pInsiderSentiment->m_lDate = 20210101; // ������ڲ�������Ҫ��ӽ����ݿ�
		vInsiderSentiment.push_back(pInsiderSentiment);

		stock.SetSymbol(_T("A"));
		stock.SetInsiderSentimentUpdateDate(20210101);
		stock.UpdateInsiderSentiment(vInsiderSentiment);

		stock.UpdateInsiderSentimentDB();

		setInsiderSentiment.m_strFilter = _T("[Symbol] = 'B'");
		setInsiderSentiment.Open();
		EXPECT_TRUE(setInsiderSentiment.IsEOF()) << "��ƱBû�д洢�������ݿ�";
		setInsiderSentiment.Close();

		setInsiderSentiment.m_strFilter = _T("[Date] = '20210101'");
		setInsiderSentiment.Open();
		setInsiderSentiment.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderSentiment.IsEOF()) << "�����ڴ洢�������ݿ�";
		setInsiderSentiment.Delete();
		setInsiderSentiment.m_pDatabase->CommitTrans();
		setInsiderSentiment.Close();
	}

	TEST_F(CWorldStockTest, TestUpdateCompanyNewsDB) {
		const auto pvCompanyNews = make_shared<vector<CCompanyNewsPtr>>();
		CSetCompanyNews setCompanyNews;

		stock.SetSymbol(_T("RIG"));

		CCompanyNewsPtr pCompanyNews = make_shared<CFinnhubCompanyNews>();
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
		EXPECT_FALSE(setCompanyNews.IsEOF()) << "�Ѿ�����������categoryΪtest������";
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
		vector<CDayLinePtr> vDayLine;
		CSetWorldStockDayLine setDayLine;

		CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210101); // �����Ҫ��ӽ����ݿ�
		pDayLine->SetClose(10010);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210102); // �����Ҫ��ӽ����ݿ�
		pDayLine->SetClose(12345);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210107); // ������ݿ����У��������
		pDayLine->SetClose(10020);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210123); // �����Ҫ��ӽ����ݿ�
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
		vector<CDayLinePtr> vDayLine;
		CSetWorldStockDayLine setDayLine;

		CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210101); // �����Ҫ��ӽ����ݿ�
		pDayLine->SetClose(10010);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210102); // �����Ҫ��ӽ����ݿ�
		pDayLine->SetClose(12345);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210107); // ������ݿ����У��������
		pDayLine->SetClose(10020);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210123); // �����Ҫ��ӽ����ݿ�
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
		vector<CEPSSurprisePtr> vEPS;
		auto pEPS = make_shared<CEPSSurprise>();

		EXPECT_TRUE(stock.UpdateEPSSurpriseDB()) << "û�����ݣ�����";
	}

	TEST_F(CWorldStockTest, TestUpdateEPSSurpriseDB2) {
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
		EXPECT_FALSE(stock.UpdateEPSSurpriseDB()) << "û�������ݣ����ؼ�";
	}

	TEST_F(CWorldStockTest, TestUpdateEPSSurpriseDB3) {
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
		EXPECT_TRUE(stock.UpdateEPSSurpriseDB()) << "�������ݴ������ݿ�(�洢��һ���µ�)";
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

	TEST_F(CWorldStockTest, TestUpdateSECFilingsDB1) {
		CSECFilingsPtr pvSECFilings = make_shared<vector<CSECFilingPtr>>();
		auto pSECFiling = make_shared<CSECFiling>();
		pSECFiling->m_strSymbol = _T("MFI"); // �Ѵ��ڴ���
		pSECFiling->m_strAccessNumber = _T("0"); // �´�ȡ�ţ���ԭ�еĶ�С
		pvSECFilings->push_back(pSECFiling);
		pSECFiling = make_shared<CSECFiling>();
		pSECFiling->m_strSymbol = _T("MFI"); // �Ѵ��ڴ���
		pSECFiling->m_strAccessNumber = _T("0000814133-03-000033"); // �Ѵ��ڴ�ȡ��
		pvSECFilings->push_back(pSECFiling);
		pSECFiling = make_shared<CSECFiling>();
		pSECFiling->m_strSymbol = _T("MFI"); // �Ѵ��ڴ���
		pSECFiling->m_strAccessNumber = _T("1000950135-08-002549"); // �´�ȡ�ţ���ԭ�еĶ���
		pvSECFilings->push_back(pSECFiling);

		stock.SetSymbol(_T("MFI"));
		stock.SetSECFilings(pvSECFilings);
		stock.SetSECFilingsNeedSave(true);
		stock.SetSECFilingsUpdated(true);

		EXPECT_TRUE(stock.UpdateSECFilingsDB());

		CSetSECFilings setSECFilings;
		setSECFilings.m_strFilter = _T("[symbol] = 'MFI'");
		setSECFilings.m_strSort = _T("[accessNumber]");
		setSECFilings.Open();
		setSECFilings.m_pDatabase->BeginTrans();
		EXPECT_TRUE(setSECFilings.m_AccessNumber.Compare(_T("0")) == 0);
		setSECFilings.Delete();
		for (int i = 0; i < 11; i++) {
			setSECFilings.MoveNext(); // ���Կ���ԭ��10���������´���Ĵ�ȡ��Ϊ"0"�����
		}
		EXPECT_FALSE(setSECFilings.IsEOF());
		EXPECT_STREQ(setSECFilings.m_AccessNumber, _T("1000950135-08-002549"));
		setSECFilings.Delete();
		setSECFilings.MoveNext();
		EXPECT_TRUE(setSECFilings.IsEOF());
		setSECFilings.m_pDatabase->CommitTrans();
		setSECFilings.Close();
	}

	TEST_F(CWorldStockTest, TestIsNeedUpdateProfile) {
		const auto pTiingoStock = make_shared<CTiingoStock>();

		stock.SetSymbol(_T("asdjfkajsfkj"));

		pTiingoStock->m_strTiingoPermaTicker = _T("askfkasjf");
		pTiingoStock->SetSymbol(_T("asdjfkajsfkj"));
		pTiingoStock->m_strName = _T("sdlfkjsdfkjg");
		pTiingoStock->m_fIsActive = true;
		pTiingoStock->m_fIsADR = true;
		pTiingoStock->m_iSicCode = 1234343445;
		pTiingoStock->m_strSicIndustry = _T("kjkjljkjgklj");
		pTiingoStock->m_strSicSector = _T("eiruir");
		pTiingoStock->m_strTiingoIndustry = _T("kdfjgkjfierj");
		pTiingoStock->m_strTiingoSector = _T("quwerpqi");
		pTiingoStock->m_strReportingCurrency = _T("asjdxcmsm");
		pTiingoStock->m_strLocation = _T("mcmvkmcvkv");
		pTiingoStock->m_strCompanyWebSite = _T(",mcvmcv");
		pTiingoStock->m_strSECFilingWebSite = _T(".,m,.cmv,.");
		pTiingoStock->SetCompanyFinancialStatementUpdateDate(457843759);
		pTiingoStock->SetDailyDataUpdateDate(234234);
		stock.UpdateStockProfile(pTiingoStock);

		EXPECT_FALSE(stock.IsNeedUpdateProfile(pTiingoStock));
	}

	TEST_F(CWorldStockTest, TestUpdateStockProfile) {
		const auto pTiingoStock = make_shared<CTiingoStock>();

		stock.SetSymbol(_T("asdjfkajsfkj"));

		pTiingoStock->m_strTiingoPermaTicker = _T("askfkasjf");
		pTiingoStock->SetSymbol(_T("asdjfkajsfkj"));
		pTiingoStock->m_strName = _T("sdlfkjsdfkjg");
		pTiingoStock->m_fIsActive = true;
		pTiingoStock->m_fIsADR = true;
		pTiingoStock->m_iSicCode = 1234343445;
		pTiingoStock->m_strSicIndustry = _T("kjkjljkjgklj");
		pTiingoStock->m_strSicSector = _T("eiruir");
		pTiingoStock->m_strTiingoIndustry = _T("kdfjgkjfierj");
		pTiingoStock->m_strTiingoSector = _T("quwerpqi");
		pTiingoStock->m_strReportingCurrency = _T("asjdxcmsm");
		pTiingoStock->m_strLocation = _T("mcmvkmcvkv");
		pTiingoStock->m_strCompanyWebSite = _T(",mcvmcv");
		pTiingoStock->m_strSECFilingWebSite = _T(".,m,.cmv,.");
		pTiingoStock->SetCompanyFinancialStatementUpdateDate(457843759);
		pTiingoStock->SetDailyDataUpdateDate(234234);

		stock.UpdateStockProfile(pTiingoStock);

		EXPECT_STREQ(stock.GetTiingoPermaTicker(), pTiingoStock->m_strTiingoPermaTicker);
		EXPECT_STREQ(stock.GetSymbol(), pTiingoStock->GetSymbol());
		EXPECT_TRUE(stock.TiingoIsActive());
		EXPECT_TRUE(stock.IsADR());
		EXPECT_EQ(stock.GetSICCode(), pTiingoStock->m_iSicCode);
		EXPECT_STREQ(stock.GetSICIndustry(), pTiingoStock->m_strSicIndustry);
		EXPECT_STREQ(stock.GetSICSector(), pTiingoStock->m_strSicSector);
		EXPECT_STREQ(stock.GetTiingoIndustry(), pTiingoStock->m_strTiingoIndustry);
		EXPECT_STREQ(stock.GetSICSector(), pTiingoStock->m_strSicSector);
		EXPECT_STREQ(stock.GetCompanyWebSite(), pTiingoStock->m_strCompanyWebSite);
		EXPECT_STREQ(stock.GetSECFilingWebSite(), pTiingoStock->m_strSECFilingWebSite);
	}

	TEST_F(CWorldStockTest, TestHaveNewDayLineData) {
		vector<CDayLinePtr> vDayLine;
		CSetWorldStockDayLine setDayLine;

		EXPECT_EQ(stock.GetDayLineSize(), 0);
		EXPECT_FALSE(stock.HaveNewDayLineData()) << "û����������";

		CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210101); // �����Ҫ��ӽ����ݿ�
		pDayLine->SetClose(10010);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210102); // �����Ҫ��ӽ����ݿ�
		pDayLine->SetClose(12345);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210107); // ������ݿ����У��������
		pDayLine->SetClose(10020);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetDate(20210123); // �����Ҫ��ӽ����ݿ�
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
		EXPECT_TRUE(stock.IsUpdatePeer());

		stock.SetUpdatePeer(false);
		stock.SetPeerUpdateDate(20200101);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.CheckPeerStatus(20200401); // 91��
		EXPECT_TRUE(stock.IsUpdatePeer()) << "��ʮһ�������";
		stock.CheckPeerStatus(20200331); // 90��
		EXPECT_FALSE(stock.IsUpdatePeer());

		stock.SetUpdatePeer(true);
		stock.SetIPOStatus(_STOCK_DELISTED_);
		stock.CheckPeerStatus(20200331); // 90��
		EXPECT_FALSE(stock.IsUpdatePeer()) << "��ʮ�����������";
		stock.CheckPeerStatus(20200401); // 91��
		EXPECT_FALSE(stock.IsUpdatePeer()) << "ժ�ƹ�Ʊ�������Peer";
	}

	TEST_F(CWorldStockTest, TestHaveInsiderTransaction) {
		const auto pInsiderTransaction = make_shared<CInsiderTransaction>();

		EXPECT_FALSE(stock.HaveInsiderTransaction());
		stock.m_vInsiderTransaction.push_back(pInsiderTransaction);
		EXPECT_TRUE(stock.HaveInsiderTransaction());
		stock.UnloadInsiderTransaction();
		EXPECT_FALSE(stock.HaveInsiderTransaction());
	}

	TEST_F(CWorldStockTest, TestCheckInsiderTransactionStatus) {
		EXPECT_TRUE(stock.IsUpdateInsiderTransaction());

		stock.SetUpdateInsiderTransaction(false);
		stock.SetInsiderTransactionUpdateDate(20200101);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.CheckInsiderTransactionStatus(20200201); // 31��
		EXPECT_TRUE(stock.IsUpdateInsiderTransaction()) << "��ʮһ�������";
		stock.CheckInsiderTransactionStatus(20200131); // 30��
		EXPECT_FALSE(stock.IsUpdateInsiderTransaction());

		stock.SetUpdateInsiderTransaction(true);
		stock.SetIPOStatus(_STOCK_DELISTED_);
		stock.CheckInsiderTransactionStatus(20200131); // 30��
		EXPECT_FALSE(stock.IsUpdateInsiderTransaction()) << "��ʮ�����������";
		stock.CheckInsiderTransactionStatus(20200201); // 31��
		EXPECT_FALSE(stock.IsUpdateInsiderTransaction()) << "ժ�ƹ�Ʊ�������InsiderTransaction";
	}

	TEST_F(CWorldStockTest, TestHaveInsiderSentiment) {
		const auto pInsiderSentiment = make_shared<CInsiderSentiment>();

		EXPECT_FALSE(stock.HaveInsiderSentiment());
		stock.m_vInsiderSentiment.push_back(pInsiderSentiment);
		EXPECT_TRUE(stock.HaveInsiderSentiment());
		stock.UnloadInsiderSentiment();
		EXPECT_FALSE(stock.HaveInsiderSentiment());
	}

	TEST_F(CWorldStockTest, TestCheckInsiderSentimentStatus) {
		EXPECT_TRUE(stock.IsUpdateInsiderSentiment());

		stock.SetUpdateInsiderSentiment(false);
		stock.SetInsiderSentimentUpdateDate(20200101);
		stock.SetIPOStatus(_STOCK_IPOED_);
		stock.CheckInsiderSentimentStatus(20200201); // 31��
		EXPECT_TRUE(stock.IsUpdateInsiderSentiment()) << "��ʮһ�������";
		stock.CheckInsiderSentimentStatus(20200131); // 30��
		EXPECT_FALSE(stock.IsUpdateInsiderSentiment());

		stock.SetUpdateInsiderSentiment(true);
		stock.SetIPOStatus(_STOCK_DELISTED_);
		stock.CheckInsiderSentimentStatus(20200131); // 30��
		EXPECT_FALSE(stock.IsUpdateInsiderSentiment()) << "��ʮ�����������";
		stock.CheckInsiderSentimentStatus(20200201); // 31��
		EXPECT_FALSE(stock.IsUpdateInsiderSentiment()) << "ժ�ƹ�Ʊ�������InsiderSentiment";
	}

	TEST_F(CWorldStockTest, TestGetFinnhubDayLineInquiryParam) {
		constexpr long lDate = 20200101;

		const time_t tt = gl_pWorldMarket->TransferToUTCTime(lDate);
		time_t ttOld = tt - static_cast<time_t>(365) * 24 * 3600;

		char buffer[30];
		sprintf_s(buffer, _T("%I64i"), tt);
		const CString strTime = buffer;
		sprintf_s(buffer, _T("%I64i"), ttOld);
		CString strTimeOld = buffer;

		stock.SetSymbol(_T("600601.SS"));
		stock.SetDayLineEndDate(20180101); // ����20190102
		CString strMiddle2 = "600601.SS&resolution=D";
		strMiddle2 += _T("&from=") + strTimeOld + _T("&to=") + strTime;
		EXPECT_STREQ(stock.GetFinnhubDayLineInquiryParam(tt), strMiddle2) << "����˻����ֻ������һ�����������";

		stock.SetSymbol(_T("600601.SS"));
		stock.SetDayLineEndDate(20190501); // ����20190102
		ttOld = gl_pWorldMarket->TransferToUTCTime(20190501);
		sprintf_s(buffer, _T("%I64i"), ttOld);
		strTimeOld = buffer;
		CString strParam = "600601.SS&resolution=D";
		strParam += _T("&from=") + strTimeOld + _T("&to=") + strTime;
		EXPECT_STREQ(stock.GetFinnhubDayLineInquiryParam(tt), strMiddle2) << "���һ�������";
	}

	TEST_F(CWorldStockTest, TestGetTiingoDayLineInquiryParam) {
		stock.SetSymbol(_T("600601.SS"));
		const CString strParam = "600601.SS/prices?&startDate=2018-1-1&endDate=2020-1-1";

		EXPECT_STREQ(stock.GetTiingoDayLineInquiryParam(20180101, 20200101), strParam);
	}

	TEST_F(CWorldStockTest, TestGetTiingoDayLineInquiryParam2) {
		stock.SetSymbol(_T("600601.SS"));
		const CString strParam = "600601.SS/prices?&startDate=1980-1-1&endDate=2020-1-1";

		EXPECT_STREQ(stock.GetTiingoDayLineInquiryParam(19800101, 20200101), strParam);
	}

	TEST_F(CWorldStockTest, TestSave) {
		CWorldStock stock2;
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
		EXPECT_STREQ(stock.GetPeer().dump().c_str(), stock2.GetPeer().dump().c_str());
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
	}

	TEST_F(CWorldStockTest, TestCheckCompanyNewsUpdated) {
		stock.SetCompanyNewsUpdateDate(20220101);
		EXPECT_FALSE(stock.CheckCompanyNewsUpdateStatus(20220102));
		stock.SetUpdateCompanyNews(true);
		EXPECT_FALSE(stock.CheckCompanyNewsUpdateStatus(20220107)) << "ֻ������";
		stock.SetUpdateCompanyNews(true);
		EXPECT_TRUE(stock.CheckCompanyNewsUpdateStatus(20220108)) << "ÿ�ܼ��һ�ι�˾����";
		stock.SetUpdateCompanyNews(true);
		EXPECT_TRUE(stock.CheckCompanyNewsUpdateStatus(20220110));
	}
}
