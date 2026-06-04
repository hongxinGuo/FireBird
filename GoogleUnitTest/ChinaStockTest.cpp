#include"pch.h"

#include "CharSetTransfer.h"
#include"GeneralCheck.h"
#include"TimeConvert.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"
#include "dataBaseConnector.h"
#include"WebRTData.h"
#include"DayLineWebData.h"
#include "jsonParse.h"

using namespace testing;

namespace FireBirdTest {
	class AccessoryTest : public testing::Test {
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			gl_systemConfiguration.SetWorkingMode(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(AccessoryTest, TestIsChinaStock) {
		CChinaStockPtr pChinaStock = make_shared<CChinaStock>();
		CVirtualStockPtr pStock;
		CTiingoStockPtr pTiingoStock = make_shared<CTiingoStock>();
		CFinnhubStockPtr pFinnhubStock = make_shared<CFinnhubStock>();
		EXPECT_TRUE(IsChinaStock(pChinaStock));
		EXPECT_FALSE(IsChinaStock(pStock));
		EXPECT_FALSE(IsChinaStock(nullptr));
		EXPECT_FALSE(IsChinaStock(pTiingoStock));
		EXPECT_FALSE(IsChinaStock(pFinnhubStock));

		pStock = pChinaStock;
		EXPECT_TRUE(IsChinaStock(pStock));
		pStock = pTiingoStock;
		EXPECT_FALSE(IsChinaStock(pStock));
	}

	TEST_F(AccessoryTest, TestIsAStock2) {
		EXPECT_TRUE(IsShareA("600000.SS"));
		EXPECT_TRUE(IsShareA("601198.SS"));
		EXPECT_FALSE(IsShareA("602102.SS"));
		EXPECT_FALSE(IsShareA("603604.SS"));
		EXPECT_FALSE(IsShareA("604604.SS"));
		EXPECT_FALSE(IsShareA("605604.SS"));
		EXPECT_FALSE(IsShareA("606604.SS"));
		EXPECT_FALSE(IsShareA("607604.SS"));
		EXPECT_FALSE(IsShareA("608604.SS"));
		EXPECT_FALSE(IsShareA("609604.SS"));

		EXPECT_FALSE(IsShareA("600000.SZ"));
		EXPECT_FALSE(IsShareA("000001.SA"));
		EXPECT_FALSE(IsShareA("000001.AZ"));
		EXPECT_FALSE(IsShareA("200001.SZ"));
		EXPECT_TRUE(IsShareA("000001.SZ"));
		EXPECT_TRUE(IsShareA("002389.SZ"));
		EXPECT_FALSE(IsShareA("003389.SZ"));
		EXPECT_FALSE(IsShareA("004389.SZ"));
		EXPECT_FALSE(IsShareA("005389.SZ"));
		EXPECT_FALSE(IsShareA("006389.SZ"));
		EXPECT_FALSE(IsShareA("007389.SZ"));
		EXPECT_FALSE(IsShareA("008389.SZ"));
		EXPECT_FALSE(IsShareA("009389.SZ"));
		EXPECT_FALSE(IsShareA("001389.SZ"));
	}

	class CChinaStockTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();

			EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
		}

		static void TearDownTestSuite() {
			EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());

			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
			ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
			pStock = nullptr;
			gl_pChinaMarket->CalculateTime();
		}

		void TearDown() override {
			// clearUp
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
			gl_pChinaMarket->CalculateTime();
			gl_pChinaMarket->SetUpdateOptionDB(false);
			if (pStock != nullptr) {
				pStock->SetDayLineDBUpdated(false);
				if (pStock->IsUpdateDayLineDB()) pStock->SetUpdateDayLineDB(false);
				pStock = nullptr;
			}
			EXPECT_THAT(gl_dataContainerChinaStock.Size(), Eq(5701));

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CChinaStockPtr pStock;
	};

	TEST_F(CChinaStockTest, TestGetRatio) {
		const CChinaStock stock;
		EXPECT_EQ(stock.GetRatio(), 1000) << "中国市场的股票价格，放大倍数为1000";
	}

	TEST_F(CChinaStockTest, TestGetDescription) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetDescription(), "");
		stock.SetDescription("SSabcdefg");
		EXPECT_EQ(stock.GetDescription(), "SSabcdefg");
	}

	TEST_F(CChinaStockTest, TestGetExchangeCode) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetExchange(), "");
		stock.SetExchange("SS");
		EXPECT_EQ(stock.GetExchange(), "SS");
	}

	TEST_F(CChinaStockTest, TestGetSymbol) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetSymbol(), "");
		stock.SetSymbol("600000.SS");
		EXPECT_EQ(stock.GetSymbol(), "600000.SS");
	}

	TEST_F(CChinaStockTest, TestGetTransactionTime) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetTransactionTime(), 0);
		stock.SetTransactionTime(1010101010);
		EXPECT_EQ(stock.GetTransactionTime(), 1010101010);
	}

	TEST_F(CChinaStockTest, TestGetLastClose) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetLastClose(), 0);
		stock.SetLastClose(10101010);
		EXPECT_EQ(stock.GetLastClose(), 10101010);
	}

	TEST_F(CChinaStockTest, TestGetOpen) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetOpen(), 0);
		stock.SetOpen(10101);
		EXPECT_EQ(stock.GetOpen(), 10101);
	}

	TEST_F(CChinaStockTest, TestGetHigh) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetHigh(), 0);
		stock.SetHigh(19980101);
		EXPECT_EQ(stock.GetHigh(), 19980101);
	}

	TEST_F(CChinaStockTest, TestGetLow) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetLow(), 0);
		stock.SetLow(19980101);
		EXPECT_EQ(stock.GetLow(), 19980101);
	}

	TEST_F(CChinaStockTest, TestGetNew) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetNew(), 0);
		stock.SetNew(10101010);
		EXPECT_EQ(stock.GetNew(), 10101010);
	}

	TEST_F(CChinaStockTest, TestGetUpDown) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetUpDown(), 0);
		stock.SetUpDown(10101010);
		EXPECT_EQ(stock.GetUpDown(), 10101010);
	}

	TEST_F(CChinaStockTest, TestGetAmount) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetAmount(), 0);
		stock.SetAmount(1010101010101010);
		EXPECT_EQ(stock.GetAmount(), 1010101010101010);
	}

	TEST_F(CChinaStockTest, TestGetVolume) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetVolume(), 0);
		stock.SetVolume(10101010);
		EXPECT_EQ(stock.GetVolume(), 10101010);
	}

	TEST_F(CChinaStockTest, TestGetTotalValue) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetTotalValue(), 0);
		stock.SetTotalValue(10101010);
		EXPECT_EQ(stock.GetTotalValue(), 10101010);
	}

	TEST_F(CChinaStockTest, TestGetCurrentValue) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetCurrentValue(), 0);
		stock.SetCurrentValue(10101010);
		EXPECT_EQ(stock.GetCurrentValue(), 10101010);
	}

	TEST_F(CChinaStockTest, TestGetChangeHandRate) {
		CChinaStock stock;
		EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 0.0);
		stock.SetChangeHandRate(1.1);
		EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 1.1);
	}

	TEST_F(CChinaStockTest, TestGetUpDownRate) {
		CChinaStock stock;
		EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 0.0);
		stock.SetUpDownRate(1.1);
		EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 1.1);
	}

	TEST_F(CChinaStockTest, TestIsTodayNewStock) {
		CChinaStock stock;

		EXPECT_FALSE(stock.IsNewStock());
		stock.SetNewStock(true);
		EXPECT_TRUE(stock.IsNewStock());
		stock.SetNewStock(false);
		EXPECT_FALSE(stock.IsNewStock());
	}

	TEST_F(CChinaStockTest, TestIsUpdateStockProfileDB) {
		CChinaStock stock;

		EXPECT_FALSE(stock.IsUpdateProfileDB());
		stock.SetUpdateProfileDB(true);
		EXPECT_TRUE(stock.IsUpdateProfileDB());
		stock.SetUpdateProfileDB(false);
		EXPECT_FALSE(stock.IsUpdateProfileDB());
	}

	TEST_F(CChinaStockTest, TestIsActive) {
		CChinaStock stock;
		EXPECT_FALSE(stock.IsActive());
		stock.SetActive(true);
		EXPECT_TRUE(stock.IsActive());
		stock.SetActive(false);
		EXPECT_FALSE(stock.IsActive());
	}

	TEST_F(CChinaStockTest, TestGetDayLineEndDate) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetDayLineEndDate(), 19800101);
		stock.SetDayLineEndDate(19980101);
		EXPECT_EQ(stock.GetDayLineEndDate(), 19980101);
	}

	TEST_F(CChinaStockTest, TestGetDayLineStartDate) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetDayLineStartDate(), 29900101);
		stock.SetDayLineStartDate(19980101);
		EXPECT_EQ(stock.GetDayLineStartDate(), 19980101);
	}

	TEST_F(CChinaStockTest, TestIsUpdateDayLine) {
		CChinaStock stock;
		EXPECT_TRUE(stock.IsUpdateDayLine());
		stock.SetUpdateDayLine(false);
		EXPECT_FALSE(stock.IsUpdateDayLine());
		stock.SetUpdateDayLine(true);
		EXPECT_TRUE(stock.IsUpdateDayLine());
	}

	TEST_F(CChinaStockTest, TestIsUpdateDayLineDB) {
		// 此两个函数是具备同步机制的，这里没有进行测试
		CChinaStock stock;
		EXPECT_FALSE(stock.IsUpdateDayLineDB());
		stock.SetUpdateDayLineDB(true);
		EXPECT_TRUE(stock.IsUpdateDayLineDB());
		stock.SetUpdateDayLineDB(false);
		EXPECT_FALSE(stock.IsUpdateDayLineDB());
	}

	TEST_F(CChinaStockTest, TestInitialize) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetSymbol(), "");
		EXPECT_EQ(stock.GetDisplaySymbol(), "");
		EXPECT_EQ(stock.GetDayLineStartDate(), 29900101);
		EXPECT_EQ(stock.GetDayLineEndDate(), 19800101);
		EXPECT_EQ(stock.GetTransactionTime(), 0);
		EXPECT_EQ(stock.GetLastClose(), 0);
		EXPECT_EQ(stock.GetOpen(), 0);
		EXPECT_EQ(stock.GetHigh(), 0);
		EXPECT_EQ(stock.GetLow(), 0);
		EXPECT_EQ(stock.GetNew(), 0);
		EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 0);
		EXPECT_EQ(stock.GetVolume(), 0);
		EXPECT_EQ(stock.GetAmount(), 0);
		for (int i = 0; i < 5; i++) {
			EXPECT_EQ(stock.GetVBuy(i), 0);
			EXPECT_EQ(stock.GetPBuy(i), 0);
			EXPECT_EQ(stock.GetVSell(i), 0);
			EXPECT_EQ(stock.GetPSell(i), 0);
		}
		EXPECT_DOUBLE_EQ(stock.GetRS(), 0);
		EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 0);
		EXPECT_EQ(stock.GetTotalValue(), 0);
		EXPECT_EQ(stock.GetCurrentValue(), 0);
		stock.SetSymbol("abcde");
		stock.SetDisplaySymbol("dcba");
		stock.SetDayLineEndDate(20020202);
		EXPECT_EQ(stock.GetSymbol(), "abcde");
		EXPECT_EQ(stock.GetDisplaySymbol(), "dcba");
		EXPECT_EQ(stock.GetDayLineEndDate(), 20020202);
	}

	TEST_F(CChinaStockTest, TestGetDisplaySymbol) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetDisplaySymbol(), "");
		stock.SetDisplaySymbol("浦发银行");
		EXPECT_EQ(stock.GetDisplaySymbol(), "浦发银行");
	}

	TEST_F(CChinaStockTest, TestGetHighLimit) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetHighLimitFromTengxun(), 0);
		stock.SetHighLimitFromTengxun(101010);
		EXPECT_EQ(stock.GetHighLimitFromTengxun(), 101010);
	}

	TEST_F(CChinaStockTest, TestGetLowLimit) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetLowLimitFromTengxun(), 0);
		stock.SetLowLimitFromTengxun(101010);
		EXPECT_EQ(stock.GetLowLimitFromTengxun(), 101010);
	}

	TEST_F(CChinaStockTest, TestGetLastSavedVolume) {
		CChinaStock stock;
		EXPECT_EQ(stock.GetLastSavedVolume(), 0);
		stock.SetLastSavedVolume(101010101010);
		EXPECT_EQ(stock.GetLastSavedVolume(), 101010101010);
	}

	TEST_F(CChinaStockTest, TestGetPBuy) {
		CChinaStock stock;
		for (int i = 0; i < 5; i++) {
			EXPECT_EQ(stock.GetPBuy(i), 0);
			stock.SetPBuy(i, 10101010);
			EXPECT_EQ(stock.GetPBuy(i), 10101010);
		}
	}

	TEST_F(CChinaStockTest, TestGetVBuy) {
		CChinaStock stock;
		for (int i = 0; i < 5; i++) {
			EXPECT_EQ(stock.GetVBuy(i), 0);
			stock.SetVBuy(i, 10101010);
			EXPECT_EQ(stock.GetVBuy(i), 10101010);
		}
	}

	TEST_F(CChinaStockTest, TestGetPSell) {
		CChinaStock stock;
		for (int i = 0; i < 5; i++) {
			EXPECT_EQ(stock.GetPSell(i), 0);
			stock.SetPSell(i, 10101010);
			EXPECT_EQ(stock.GetPSell(i), 10101010);
		}
	}

	TEST_F(CChinaStockTest, TestGetVSell) {
		CChinaStock stock;
		for (int i = 0; i < 5; i++) {
			EXPECT_EQ(stock.GetVSell(i), 0);
			stock.SetVSell(i, 10101010);
			EXPECT_EQ(stock.GetVSell(i), 10101010);
		}
	}

	TEST_F(CChinaStockTest, TestGetRS) {
		CChinaStock stock;
		EXPECT_DOUBLE_EQ(stock.GetRS(), 0.0);
		stock.SetRS(10101010.0);
		EXPECT_DOUBLE_EQ(stock.GetRS(), 10101010.0);
	}

	TEST_F(CChinaStockTest, TestGetRSIndex) {
		CChinaStock stock;
		EXPECT_DOUBLE_EQ(stock.GetRSIndex(), 0.0);
		stock.SetRSIndex(10101010.0);
		EXPECT_DOUBLE_EQ(stock.GetRSIndex(), 10101010.0);
	}

	TEST_F(CChinaStockTest, TestIsUpdateChosenStockDB) {
		CChinaStock stock;
		EXPECT_FALSE(stock.IsUpdateChosenStockDB());
		stock.SetUpdateChosenStockDB(true);
		EXPECT_TRUE(stock.IsUpdateChosenStockDB());
		stock.SetUpdateChosenStockDB(false);
		EXPECT_FALSE(stock.IsUpdateChosenStockDB());
	}

	TEST_F(CChinaStockTest, TestIsChosen) {
		CChinaStock stock;
		EXPECT_FALSE(stock.IsChosen());
		stock.SetChosen(true);
		EXPECT_TRUE(stock.IsChosen());
		stock.SetChosen(false);
		EXPECT_FALSE(stock.IsChosen());
	}

	TEST_F(CChinaStockTest, TestIsDayLineDBUpdated) {
		CChinaStock stock;
		EXPECT_FALSE(stock.IsDayLineDBUpdated());
		stock.SetDayLineDBUpdated(true);
		EXPECT_TRUE(stock.IsDayLineDBUpdated());
		stock.SetDayLineDBUpdated(false);
		EXPECT_FALSE(stock.IsDayLineDBUpdated());
	}

	TEST_F(CChinaStockTest, TestIsDayLineLoaded) {
		CChinaStock stock;
		EXPECT_FALSE(stock.IsDayLineLoaded());
		stock.SetDayLineLoaded(true);
		EXPECT_TRUE(stock.IsDayLineLoaded());
		stock.SetDayLineLoaded(false);
		EXPECT_FALSE(stock.IsDayLineLoaded());
	}

	TEST_F(CChinaStockTest, TestHaveFirstRTData) {
		CChinaStock stock;
		EXPECT_FALSE(stock.HaveFirstRTData());
		stock.SetHavingFirstRTData(true);
		EXPECT_TRUE(stock.HaveFirstRTData());
		stock.SetHavingFirstRTData(false);
		EXPECT_TRUE(stock.HaveFirstRTData()); // 此标识设置后就永远为真了。
	}

	TEST_F(CChinaStockTest, TestIsRTDataCalculated) {
		CChinaStock stock;
		EXPECT_FALSE(stock.IsRTDataCalculated());
		stock.SetRTDataCalculated(true);
		EXPECT_TRUE(stock.IsRTDataCalculated());
		stock.SetRTDataCalculated(false);
		EXPECT_FALSE(stock.IsRTDataCalculated());
	}

	TEST_F(CChinaStockTest, TestIsStartCalculating) {
		CChinaStock stock;
		EXPECT_FALSE(stock.HaveFirstRTData());
		EXPECT_FALSE(stock.SetHavingFirstRTData(false)); // 不允许外部设置停止计算标识（内部可以）
		EXPECT_FALSE(stock.HaveFirstRTData());
		EXPECT_TRUE(stock.SetHavingFirstRTData(true));
		EXPECT_TRUE(stock.HaveFirstRTData());
		EXPECT_FALSE(stock.SetHavingFirstRTData(true)); // 不允许再次设置开始计算标识
		EXPECT_TRUE(stock.HaveFirstRTData());
	}

	TEST_F(CChinaStockTest, TestHaveNewDayLineData) {
		CChinaStock stock;
		EXPECT_FALSE(stock.HaveNewDayLineData());
		EXPECT_EQ(stock.DayLineSize(), 0);
		CDayLine dayLine;
		dayLine.SetDate(20200101);
		stock.StoreDayLine(dayLine);
		EXPECT_EQ(stock.DayLineSize(), 1);
		stock.SetDayLineEndDate(20200101);
		EXPECT_FALSE(stock.HaveNewDayLineData());
		stock.SetDayLineEndDate(20191231);
		EXPECT_TRUE(stock.HaveNewDayLineData());
		stock.UnloadDayLine();
		EXPECT_EQ(stock.DayLineSize(), 0);
	}

	TEST_F(CChinaStockTest, TestTodayDataIsActive) {
		CChinaStock stock;
		EXPECT_FALSE(stock.IsTodayDataActive());
		stock.SetActive(true);
		EXPECT_FALSE(stock.IsTodayDataActive());
		stock.SetHigh(1010);
		EXPECT_TRUE(stock.IsTodayDataActive());
		stock.SetHigh(0);
		stock.SetLow(1);
		EXPECT_TRUE(stock.IsTodayDataActive());
		stock.SetLow(0);
		stock.SetAmount(10000);
		EXPECT_TRUE(stock.IsTodayDataActive());
		stock.SetAmount(0);
		stock.SetVolume(100);
		EXPECT_TRUE(stock.IsTodayDataActive());
		stock.SetActive(false);
		EXPECT_FALSE(stock.IsTodayDataActive());
	}

	TEST_F(CChinaStockTest, TestIsSaveStock) {
		pStock = gl_dataContainerChinaStock.GetStock(1);
		CChinaStockPtr pStock2 = nullptr;
		EXPECT_FALSE(pStock->IsSameStock(pStock2));
		pStock2 = gl_dataContainerChinaStock.GetStock(2);
		EXPECT_FALSE(pStock->IsSameStock(pStock2));
		pStock2 = gl_dataContainerChinaStock.GetStock(1);
		EXPECT_TRUE(pStock->IsSameStock(pStock2));
	}

	TEST_F(CChinaStockTest, TestIsTodayDataChanged) {
		CChinaStock stock;
		EXPECT_FALSE(stock.IsTodayDataChanged());
		stock.SetHigh(10);
		EXPECT_TRUE(stock.IsTodayDataChanged());
		stock.SetHigh(0);
		stock.SetLow(10);
		EXPECT_TRUE(stock.IsTodayDataChanged());
		stock.SetLow(0);
		stock.SetVolume(10);
		EXPECT_TRUE(stock.IsTodayDataChanged());
		stock.SetVolume(0);
		stock.SetAmount(10);
		EXPECT_TRUE(stock.IsTodayDataChanged());
	}

	TEST_F(CChinaStockTest, TestCheckNeedProcessRTData) {
		CChinaStock stock;
		stock.SetSymbol("000001.SS");
		stock.CheckNeedProcessRTData();
		EXPECT_FALSE(stock.IsNeedProcessRTData());
		stock.SetSymbol("001000.SS");
		stock.CheckNeedProcessRTData();
		EXPECT_TRUE(stock.IsNeedProcessRTData());
		stock.SetSymbol("000001.SZ");
		stock.CheckNeedProcessRTData();
		EXPECT_TRUE(stock.IsNeedProcessRTData());
		stock.SetSymbol("999000.SZ");
		stock.CheckNeedProcessRTData();
		EXPECT_FALSE(stock.IsNeedProcessRTData());
	}

	TEST_F(CChinaStockTest, TestSetCheckingDayLineStatus) {
		CChinaStock stock;
		EXPECT_TRUE(stock.IsUpdateDayLine());
		stock.SetDayLineEndDate(gl_pChinaMarket->GetMarketDate());
		EXPECT_FALSE(stock.CheckDayLineStatus());
		EXPECT_FALSE(stock.IsUpdateDayLine()) << stock.GetDayLineEndDate() << gl_pChinaMarket->GetMarketDate();
		stock.SetUpdateDayLine(true);
		stock.SetDayLineEndDate(gl_pChinaMarket->GetLastTradeDate());
		EXPECT_FALSE(stock.CheckDayLineStatus());
		EXPECT_FALSE(stock.IsUpdateDayLine());
		stock.SetUpdateDayLine(true);

		stock.SetDayLineEndDate(CHINA_MARKET_BEGIN_DATE_);
		EXPECT_TRUE(stock.CheckDayLineStatus());
		EXPECT_TRUE(stock.IsUpdateDayLine());
	}

	TEST_F(CChinaStockTest, TestRTDataDeque) {
		const auto pData = make_shared<CWebRTData>();
		pData->SetSymbol("600008.SS");
		const auto pData1 = make_shared<CWebRTData>();
		pData1->SetSymbol("600009.SS");
		CChinaStock stock;
		EXPECT_EQ(stock.GetRTDataQueueSize(), 0);
		stock.PushRTData(pData);
		EXPECT_EQ(stock.GetRTDataQueueSize(), 1);
		stock.PushRTData(pData1);
		EXPECT_EQ(stock.GetRTDataQueueSize(), 2);
		EXPECT_EQ(pData->GetSymbol(), "600008.SS");
		CWebRTDataPtr pData2 = stock.PopRTData();
		EXPECT_EQ(stock.GetRTDataQueueSize(), 1);
	}

	TEST_F(CChinaStockTest, TestClearRTDataDeque) {
		CChinaStock stock;
		for (int i = 0; i < 10; i++) {
			CWebRTDataPtr pRTData = make_shared<CWebRTData>();
			pRTData->SetLastClose(i);
			stock.PushRTData(pRTData);
		}
		EXPECT_EQ(stock.GetRTDataQueueSize(), 10);
		stock.ClearRTDataDeque();
		EXPECT_EQ(stock.GetRTDataQueueSize(), 0);
	}

	TEST_F(CChinaStockTest, TestLoadDayLine) {
		CDayLine* pid;
		CChinaStock stock;

		pStock = gl_dataContainerChinaStock.GetStock("600010.SS");

		for (int i = 0; i < 8; i++) {
			CDayLine dayLine;
			dayLine.SetDate(21121201);
			dayLine.SetStockSymbol("600010.SS");
			dayLine.SetLastClose(34235345);
			dayLine.SetOpen(100000 + i);
			dayLine.SetHigh(45234543);
			dayLine.SetLow(3452341);
			dayLine.SetClose(452435);
			dayLine.SetVolume(34523454);
			dayLine.SetAmount(3245235345);
			dayLine.SetUpDown((static_cast<double>(dayLine.GetClose()) - dayLine.GetLastClose()) / dayLine.GetRatio());
			dayLine.SetUpDownRate(123.45);
			dayLine.SetTotalValue(234523452345);
			dayLine.SetCurrentValue(234145345245);
			dayLine.SetChangeHandRate(54.321);
			pStock->StoreDayLine(dayLine);
		}
		pStock->SetSymbol("600010.SS");
		pStock->SetDayLineEndDate(10190101);
		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		pStock->SaveDayLineDB();

		stock.SetSymbol("600010.SS");
		stock.LoadDayLineDB();

		for (int i = 0; i < 8; i++) {
			pid = stock.GetDayLine(i);
			auto pDayLine = pStock->GetDayLine(i);
			EXPECT_EQ(pDayLine->GetDate(), pid->GetDate());
			EXPECT_EQ(pDayLine->GetStockSymbol(), pid->GetStockSymbol());
			EXPECT_EQ(pDayLine->GetLastClose(), pid->GetLastClose());
			EXPECT_EQ(pDayLine->GetOpen(), pid->GetOpen());
			EXPECT_EQ(pDayLine->GetHigh(), pid->GetHigh());
			EXPECT_EQ(pDayLine->GetLow(), pid->GetLow());
			EXPECT_EQ(pDayLine->GetClose(), pid->GetClose());
			EXPECT_EQ(pDayLine->GetVolume(), pid->GetVolume());
			EXPECT_EQ(pDayLine->GetAmount(), pid->GetAmount());
			EXPECT_DOUBLE_EQ(pDayLine->GetUpDown(), pid->GetUpDown());
			EXPECT_DOUBLE_EQ(pDayLine->GetUpDownRate(), pid->GetUpDownRate());
			EXPECT_EQ(pDayLine->GetTotalValue(), pid->GetTotalValue());
			EXPECT_EQ(pDayLine->GetCurrentValue(), pid->GetCurrentValue());
			EXPECT_DOUBLE_EQ(pDayLine->GetChangeHandRate(), pid->GetChangeHandRate());
		}

		using namespace StockMarket;
		const auto& t = ChinaStockDayline{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);

		db(sqlpp::remove_from(t).where(t.Symbol == "600010.SS" && t.Date == 21121201));
		tx.commit();
	}

	TEST_F(CChinaStockTest, TestUpdateDayLineStartEndDate) {
		CChinaStock stock;

		pStock = gl_dataContainerChinaStock.GetStock("600004.SS");

		for (int i = 0; i < 10; i++) {
			CDayLinePtr pid = make_shared<CDayLine>();
			pid->SetDate(CHINA_MARKET_BEGIN_DATE_ + i * 100000 + 2);
			pid->SetStockSymbol("600004.SS");
			pid->SetLastClose(34235345);
			pid->SetOpen(1000000 + i);
			pid->SetHigh(45234543);
			pid->SetLow(3452345);
			pid->SetClose(452435);
			pid->SetVolume(34523454);
			pid->SetAmount(3245235345);
			pid->SetUpDown((static_cast<double>(pid->GetClose()) - pid->GetLastClose()) / pid->GetRatio());
			pid->SetUpDownRate(123.45);
			pid->SetTotalValue(234523452345);
			pid->SetCurrentValue(234145345245);
			pid->SetChangeHandRate(54.321);
			pStock->StoreDayLine(*pid);
		}
		pStock->SetSymbol("600004.SS");
		pStock->SetDayLineStartDate(19920102);
		pStock->SetDayLineEndDate(20800100);
		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		pStock->UpdateDayLineStartEndDate();
		EXPECT_EQ(pStock->GetDayLineEndDate(), CHINA_MARKET_BEGIN_DATE_ + 9 * 100000 + 2) << "日线最新日期已更新";
		EXPECT_EQ(pStock->GetDayLineStartDate(), CHINA_MARKET_BEGIN_DATE_ + 2) << "日线最初日期已更新";
		EXPECT_TRUE(gl_dataContainerChinaStock.IsDayLineDBUpdated());
		EXPECT_TRUE(pStock->IsUpdateProfileDB()) << "更新日线起止日期后，此标识也被设置";

		pStock->SetUpdateProfileDB(false);
	}

	TEST_F(CChinaStockTest, TestUpdateDayLineStartEndDate3) {
		CChinaStock stock;

		pStock = gl_dataContainerChinaStock.GetStock("600008.SS");

		for (int i = 1; i < 10; i++) {
			CDayLinePtr pid = make_shared<CDayLine>();
			pid->SetDate(CHINA_MARKET_BEGIN_DATE_ + i * 100000);
			pid->SetStockSymbol("600008.SS");
			pid->SetLastClose(34235345);
			pid->SetOpen(1000000 + i);
			pid->SetHigh(45234543);
			pid->SetLow(3452345);
			pid->SetClose(452435);
			pid->SetVolume(34523454);
			pid->SetAmount(3245235345);
			pid->SetUpDown((static_cast<double>(pid->GetClose()) - pid->GetLastClose()) / pid->GetRatio());
			pid->SetUpDownRate(123.45);
			pid->SetTotalValue(234523452345);
			pid->SetCurrentValue(234145345245);
			pid->SetChangeHandRate(54.321);
			pStock->StoreDayLine(*pid);
		}
		pStock->SetSymbol("600008.SS");
		pStock->SetDayLineStartDate(19900101);
		pStock->SetDayLineEndDate(20800102);
		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		pStock->UpdateDayLineStartEndDate();
		EXPECT_EQ(pStock->GetDayLineEndDate(), 20800102);
		EXPECT_EQ(pStock->GetDayLineStartDate(), 19900101 + 100000) << "当起始日期为19900101时，需要更新之";
		EXPECT_TRUE(gl_dataContainerChinaStock.IsDayLineDBUpdated());
		EXPECT_TRUE(pStock->IsUpdateProfileDB()) << "此标识也被设置";

		pStock->SetUpdateProfileDB(false);
	}

	TEST_F(CChinaStockTest, TestUpdateDayLineStartEndDate2) {
		CChinaStock stock;

		pStock = gl_dataContainerChinaStock.GetStock("600008.SS");

		for (int i = 1; i < 10; i++) {
			CDayLinePtr pid = make_shared<CDayLine>();
			pid->SetDate(CHINA_MARKET_BEGIN_DATE_ + i * 100000);
			pid->SetStockSymbol("600008.SS");
			pid->SetLastClose(34235345);
			pid->SetOpen(1000000 + i);
			pid->SetHigh(45234543);
			pid->SetLow(3452345);
			pid->SetClose(452435);
			pid->SetVolume(34523454);
			pid->SetAmount(3245235345);
			pid->SetUpDown((static_cast<double>(pid->GetClose()) - pid->GetLastClose()) / pid->GetRatio());
			pid->SetUpDownRate(123.45);
			pid->SetTotalValue(234523452345);
			pid->SetCurrentValue(234145345245);
			pid->SetChangeHandRate(54.321);
			pStock->StoreDayLine(*pid);
		}
		pStock->SetSymbol("600008.SS");
		pStock->SetDayLineStartDate(19900102);
		pStock->SetDayLineEndDate(20800102);
		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		pStock->UpdateDayLineStartEndDate();
		EXPECT_EQ(pStock->GetDayLineEndDate(), 20800102);
		EXPECT_EQ(pStock->GetDayLineStartDate(), 19900102);
		EXPECT_FALSE(gl_dataContainerChinaStock.IsDayLineDBUpdated());
		EXPECT_FALSE(pStock->IsUpdateProfileDB()) << "未更新日线起止日期的话，此标识也未被设置";
	}

	TEST_F(CChinaStockTest, TestReportDayLineDownLoaded) {
		CChinaStock stock;
		stock.SetSymbol("600008.SS");
		stock.ReportDayLineDownLoaded();
		//EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 1);
		//string str = gl_systemMessage.PopDayLineInfoMessage();
		//EXPECT_STREQ(str, "sh600008日线下载完成.");
	}

	TEST_F(CChinaStockTest, TestStoreDayLine) {
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();

		for (int i = 0; i < 10; i++) {
			CDayLine dayLine;
			dayLine.SetDate(19900101 + i);
			dayLine.SetClose(10);
			dayLine.SetLastClose(10);
			pvDayLine->push_back(dayLine);
		}
		EXPECT_EQ(pvDayLine->size(), 10);
		CChinaStock stock;
		EXPECT_FALSE(stock.IsDayLineLoaded());
		stock.UpdateDayLine(*pvDayLine);
		EXPECT_EQ(stock.DayLineSize(), 10);
		for (int i = 0; i < 10; i++) { EXPECT_EQ(stock.GetDayLine(i)->GetDate(), 19900101 + i); }
		EXPECT_TRUE(stock.IsDayLineLoaded());
	}

	TEST_F(CChinaStockTest, TestStoreDayLine2) {
		CDayLineWebData data;

		for (int i = 0; i < 10; i++) {
			CDayLine dayLine;
			dayLine.SetDate(19900101 + i);
			dayLine.SetClose(10);
			dayLine.SetLastClose(10);
			data.AppendDayLine(dayLine);
		}
		CChinaStock stock;
		EXPECT_FALSE(stock.IsDayLineLoaded());
		stock.UpdateDayLine(data.GetProcessedDayLine()); // 测试CDownloadedNeteaseDayLine中的GetProcessedDayLine
		EXPECT_EQ(stock.DayLineSize(), 10);
		for (int i = 0; i < 10; i++) { EXPECT_EQ(stock.GetDayLine(i)->GetDate(), 19900101 + i); }
		EXPECT_TRUE(stock.IsDayLineLoaded());
	}

	TEST_F(CChinaStockTest, TestIsAStock) {
		const auto pStock2 = make_shared<CChinaStock>();
		pStock2->SetSymbol("600000.SS");
		EXPECT_TRUE(pStock2->IsShareA());
		pStock2->SetSymbol("600000.SA");
		EXPECT_FALSE(pStock2->IsShareA());
		pStock2->SetSymbol("000001.SZ");
		EXPECT_TRUE(pStock2->IsShareA());
		pStock2->SetSymbol("10001.SZ");
		EXPECT_FALSE(pStock2->IsShareA());
	}

	TEST_F(CChinaStockTest, TestDeleteDuplicatedDayLine) {
		using namespace StockMarket;
		const auto& t = ChinaStockDayline{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		pStock = gl_dataContainerChinaStock.GetStock("000001.SZ");
		gl_pChinaMarket->TEST_SetFormattedMarketDate(202100309);
		for (int i = 0; i < 10; i++) {
			CDayLine dayLine;
			dayLine.SetDate(20210302 + i); // 数据库中已有20210301的数据，此处插入重复数据进行测试
			dayLine.SetStockSymbol("000001.SZ");
			dayLine.SetLastClose(34235345);
			dayLine.SetOpen(1000000 + i);
			dayLine.SetHigh(45234543);
			dayLine.SetLow(3452345);
			dayLine.SetClose(452435);
			dayLine.SetVolume(34523454);
			dayLine.SetAmount(3245235345);
			dayLine.SetUpDown((static_cast<double>(dayLine.GetClose()) - dayLine.GetLastClose()) / dayLine.GetRatio());
			dayLine.SetUpDownRate(123.45);
			dayLine.SetTotalValue(234523452345);
			dayLine.SetCurrentValue(234145345245);
			dayLine.SetChangeHandRate(54.321);
			pStock->StoreDayLine(dayLine);
		}
		pStock->SetSymbol("000001.SZ");
		pStock->SetDayLineEndDate(20210301);

		db(sqlpp::insert_into(t).set(
			t.Date = 20210302,
			t.Symbol = "000001.SZ",
			t.ChangeHandRate = 54.321
		));
		db(sqlpp::insert_into(t).set(
			t.Date = 20210303,
			t.Symbol = "000001.SZ",
			t.ChangeHandRate = 54.321
		));
		tx.commit();

		pStock->DeleteDuplicatedDayLine();

		auto result = db(select(all_of(t)).from(t).where(t.Symbol == "000001.SZ" && t.Date > 20210301));
		size_t rows = result.size();
		EXPECT_EQ(rows, 0) << "重复数据已被删除";
	}

	//TEST_F_TRAITS();
}
