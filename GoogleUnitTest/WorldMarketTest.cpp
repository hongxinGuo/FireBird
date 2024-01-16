#include"pch.h"

#include"GeneralCheck.h"

#include"WorldStock.h"

#include"ProductFinnhubCompanyInsiderTransaction.h"
#include"ProductFinnhubCompanyInsiderSentiment.h"

#include"ProductTiingoStockSymbol.h"

#include"SetFinnhubForexExchange.h"
#include"SetFinnhubCryptoExchange.h"
#include"SetFinnhubCryptoSymbol.h"
#include"SetFinnhubForexSymbol.h"

#include"FinnhubDataSource.h"
#include"FinnhubCryptoSymbol.h"
#include "InfoReport.h"
#include "WorldMarket.h"

using namespace testing;

namespace FireBirdTest {
	class CWorldMarketTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			EXPECT_THAT(gl_systemMessage.InformationSize(), 0);
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			gl_pFinnhubDataSource->SetInquiring(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
	};

	TEST_F(CWorldMarketTest, TestIsResetSystemTime) {
		EXPECT_FALSE(gl_pWorldMarket->IsTimeToResetSystem(165759));
		EXPECT_TRUE(gl_pWorldMarket->IsTimeToResetSystem(165800));
		EXPECT_TRUE(gl_pWorldMarket->IsTimeToResetSystem(165801));
		EXPECT_TRUE(gl_pWorldMarket->IsTimeToResetSystem(170459));
		EXPECT_TRUE(gl_pWorldMarket->IsTimeToResetSystem(170500));
		EXPECT_FALSE(gl_pWorldMarket->IsTimeToResetSystem(170501));
	}

	TEST_F(CWorldMarketTest, TestTransferMarketTime) {
		tm tm2_;
		time_t tt;

		gl_pWorldMarket->CalculateTime();
		time(&tt);
		gmtime_s(&tm2_, &tt);
		const tm tm_ = gl_pWorldMarket->GetMarketTime(GetUTCTime());
		EXPECT_TRUE((tm_.tm_hour == (tm2_.tm_hour - 4) || (tm_.tm_hour == tm2_.tm_hour + 20))) << "WorldMarket默认为西四区(美东标准时间)";
	}

	TEST_F(CWorldMarketTest, TestGetTotalStock) {
		EXPECT_EQ(gl_dataContainerFinnhubStock.Size(), 4847) << "默认状态下数据库总数为4847(全部上海股票和小部分美国股票)";
	}

	TEST_F(CWorldMarketTest, TestIsStock) {
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsSymbol(_T("000000.SS")));
		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsSymbol(_T("000001.SS")));
		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsSymbol(_T("600601.SS")));
		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsSymbol(_T("A")));
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsSymbol(_T("000001.SZ"))) << "目前测试数据库中只有上海和美国股票集";

		const auto pStock = make_shared<CWorldStock>();
		pStock->SetSymbol(_T("000000.SS"));
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsSymbol(pStock));
		pStock->SetSymbol(_T("000001.SS"));
		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsSymbol(pStock));
		pStock->SetSymbol(_T("600601.SS"));
		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsSymbol(pStock));
		pStock->SetSymbol(_T("A"));
		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsSymbol(pStock));
		pStock->SetSymbol(_T("000001.SZ"));
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsSymbol(pStock));
	}

	TEST_F(CWorldMarketTest, TestIsTiingoStock) {
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsStock(_T("000000.SS")));
		EXPECT_TRUE(gl_dataContainerTiingoStock.IsStock(_T("AA")));
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsStock(_T("600601.SS")));
		EXPECT_TRUE(gl_dataContainerTiingoStock.IsStock(_T("A")));
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsStock(_T("000001.SZ"))) << "目前测试数据库中只有上海和美国股票集";

		const auto pStock = make_shared<CWorldStock>();
		pStock->SetSymbol(_T("000000.SS"));
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsStock(pStock));
		pStock->SetSymbol(_T("AA"));
		EXPECT_TRUE(gl_dataContainerTiingoStock.IsStock(pStock));
		pStock->SetSymbol(_T("600601.SS"));
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsStock(pStock));
		pStock->SetSymbol(_T("A"));
		EXPECT_TRUE(gl_dataContainerTiingoStock.IsStock(pStock));
		pStock->SetSymbol(_T("000001.SZ"));
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsStock(pStock));

		const auto pTiingoStock = make_shared<CTiingoStock>();
		pTiingoStock->m_strTicker = _T("000000.SS");
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsStock(pTiingoStock));
		pTiingoStock->m_strTicker = _T("AA");
		EXPECT_TRUE(gl_dataContainerTiingoStock.IsStock(pTiingoStock));
		pTiingoStock->m_strTicker = _T("600601.SS");
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsStock(pTiingoStock));
		pTiingoStock->m_strTicker = _T("A");
		EXPECT_TRUE(gl_dataContainerTiingoStock.IsStock(pTiingoStock));
		pTiingoStock->m_strTicker = _T("000001.SZ");
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsStock(pTiingoStock));
	}

	TEST_F(CWorldMarketTest, TestGetTiingoStock) {
		CTiingoStockPtr pStock = gl_dataContainerTiingoStock.GetStock(0); // A
		EXPECT_STREQ(pStock->m_strTicker, _T("A")) << "第一个股票代码为A";
		pStock = gl_dataContainerTiingoStock.GetStock(_T("A"));
		EXPECT_FALSE(pStock == nullptr);
		EXPECT_STREQ(pStock->m_strName, _T("Agilent Technologies Inc"));
	}

	TEST_F(CWorldMarketTest, TestIsUpdateProfileDB) {
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetUpdateProfileDB(false);
		}
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());

		gl_dataContainerFinnhubStock.GetStock(0)->SetUpdateProfileDB(true);
		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());

		gl_dataContainerFinnhubStock.GetStock(0)->SetUpdateProfileDB(false);
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
	}

	TEST_F(CWorldMarketTest, TestAddStock) {
		const auto pStock = make_shared<CWorldStock>();
		pStock->SetSymbol(_T("000001.SZ"));

		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsSymbol(pStock));
		gl_dataContainerFinnhubStock.Add(pStock);
		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsSymbol(pStock));
		EXPECT_EQ(gl_dataContainerFinnhubStock.Size(), 4848);
		gl_dataContainerFinnhubStock.Delete(pStock);
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsSymbol(pStock));
		EXPECT_EQ(gl_dataContainerFinnhubStock.Size(), 4847);
	}

	TEST_F(CWorldMarketTest, TestDeleteStock) {
		// do nothing. 已经在TestAddStock中测试了DeleteStock函数
		CWorldStockPtr pStock = nullptr;

		gl_dataContainerFinnhubStock.Delete(pStock);

		pStock = make_shared<CWorldStock>();
		pStock->SetSymbol(_T("000001.SZ"));
		gl_dataContainerFinnhubStock.Delete(pStock); // "此股票代码不存在于代码集中";
	}

	TEST_F(CWorldMarketTest, TestGetStock) {
		CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(0); // 000001.SS
		EXPECT_STREQ(pStock->GetSymbol(), _T("000001.SS")) << "第一个股票代码为000001.SS";
		pStock = gl_dataContainerFinnhubStock.GetStock(_T("000001.SS"));
		EXPECT_FALSE(pStock == nullptr);
		EXPECT_STREQ(pStock->GetDescription(), _T("SSE Composite Index"));
	}

	TEST_F(CWorldMarketTest, TestAddTiingoStock) {
		const auto pStock = make_shared<CTiingoStock>();
		const auto lTotalStock = gl_dataContainerTiingoStock.GetTotalStock();
		pStock->m_strTicker = _T("ABCDEF");

		EXPECT_FALSE(gl_dataContainerTiingoStock.IsStock(pStock));
		gl_dataContainerTiingoStock.Add(pStock);
		EXPECT_TRUE(gl_dataContainerTiingoStock.IsStock(pStock));
		EXPECT_EQ(gl_dataContainerTiingoStock.GetTotalStock(), lTotalStock + 1);
		gl_dataContainerTiingoStock.Delete(pStock);
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsStock(pStock));
		EXPECT_EQ(gl_dataContainerTiingoStock.GetTotalStock(), lTotalStock);
	}

	TEST_F(CWorldMarketTest, TestDeleteTiingoStock) {
		// do nothing. 已经在TestAddStock中测试了DeleteStock函数
		CTiingoStockPtr pStock = nullptr;

		EXPECT_FALSE(gl_dataContainerTiingoStock.Delete(pStock)) << "空指针";

		pStock = make_shared<CTiingoStock>();
		pStock->m_strTicker = _T("ABCDEF");
		EXPECT_FALSE(gl_dataContainerTiingoStock.Delete(pStock)) << "此股票代码不存在于代码集中";
	}

	TEST_F(CWorldMarketTest, TestIsForexExchange) {
		EXPECT_FALSE(gl_dataContainerFinnhubForexExchange.IsExchange(_T("ABC")));
		EXPECT_TRUE(gl_dataContainerFinnhubForexExchange.IsExchange(_T("forex.com")));
		EXPECT_TRUE(gl_dataContainerFinnhubForexExchange.IsExchange(_T("ic markets")));
	}

	TEST_F(CWorldMarketTest, TestAddForexExchange) {
		const auto lTotalForexExchange = gl_dataContainerFinnhubForexExchange.Size();
		const CString strForexExchange = _T("000001.SZ");

		EXPECT_FALSE(gl_dataContainerFinnhubForexExchange.IsExchange(strForexExchange));
		gl_dataContainerFinnhubForexExchange.Add(strForexExchange);
		EXPECT_TRUE(gl_dataContainerFinnhubForexExchange.IsExchange(strForexExchange));
		EXPECT_EQ(gl_dataContainerFinnhubForexExchange.Size(), lTotalForexExchange + 1);
		gl_dataContainerFinnhubForexExchange.Delete(strForexExchange);
		EXPECT_FALSE(gl_dataContainerFinnhubForexExchange.IsExchange(strForexExchange));
		EXPECT_EQ(gl_dataContainerFinnhubForexExchange.Size(), lTotalForexExchange);
	}

	TEST_F(CWorldMarketTest, TestDeleteForexExchange) {
		// do nothing. 已经在TestAddForexExchange中测试了DeleteForexExchange函数
		EXPECT_FALSE(gl_dataContainerFinnhubForexExchange.Delete(_T("US.US.US"))) << "此符号在符号集中不存在";
	}

	TEST_F(CWorldMarketTest, TestIsForexSymbol) {
		EXPECT_FALSE(gl_dataFinnhubForexSymbol.IsSymbol(_T("ABC")));
		EXPECT_TRUE(gl_dataFinnhubForexSymbol.IsSymbol(_T("OANDA:XAU_SGD")));
		EXPECT_TRUE(gl_dataFinnhubForexSymbol.IsSymbol(_T("FXCM:EUR/CHF")));

		const auto pForexSymbol = make_shared<CFinnhubForexSymbol>();
		pForexSymbol->SetSymbol(_T("ABC"));
		EXPECT_FALSE(gl_dataFinnhubForexSymbol.IsSymbol(pForexSymbol));
		pForexSymbol->SetSymbol(_T("OANDA:XAU_SGD"));
		EXPECT_TRUE(gl_dataFinnhubForexSymbol.IsSymbol(pForexSymbol));
		pForexSymbol->SetSymbol(_T("FXCM:EUR/CHF"));
		EXPECT_TRUE(gl_dataFinnhubForexSymbol.IsSymbol(pForexSymbol));
	}

	TEST_F(CWorldMarketTest, TestAddForexSymbol) {
		const auto pForexSymbol = make_shared<CFinnhubForexSymbol>();
		const auto lTotalForexSymbol = gl_dataFinnhubForexSymbol.Size();
		pForexSymbol->SetSymbol(_T("000001.SZ"));

		EXPECT_FALSE(gl_dataFinnhubForexSymbol.IsSymbol(pForexSymbol));
		gl_dataFinnhubForexSymbol.Add(pForexSymbol);
		EXPECT_TRUE(gl_dataFinnhubForexSymbol.IsSymbol(pForexSymbol));
		EXPECT_EQ(gl_dataFinnhubForexSymbol.Size(), lTotalForexSymbol + 1);
		gl_dataFinnhubForexSymbol.Delete(pForexSymbol);
		EXPECT_FALSE(gl_dataFinnhubForexSymbol.IsSymbol(pForexSymbol));
		EXPECT_EQ(gl_dataFinnhubForexSymbol.Size(), lTotalForexSymbol);
	}

	TEST_F(CWorldMarketTest, TestDeleteForexSymbol) {
		// do nothing. 已经在TestAddForexSymbol中测试了DeleteForexSymbol函数
		CForexSymbolPtr pForexSymbol = nullptr;

		gl_dataFinnhubForexSymbol.Delete(pForexSymbol); // "空指针";

		pForexSymbol = make_shared<CFinnhubForexSymbol>();
		pForexSymbol->SetSymbol(_T("000001.SZ"));
		gl_dataFinnhubForexSymbol.Delete(pForexSymbol); // "此符号在符号集中不存在";
	}

	TEST_F(CWorldMarketTest, TestIsCryptoExchange) {
		EXPECT_FALSE(gl_dataContainerFinnhubCryptoExchange.IsExchange(_T("ABC")));
		EXPECT_TRUE(gl_dataContainerFinnhubCryptoExchange.IsExchange(_T("BITFINEX")));
		EXPECT_TRUE(gl_dataContainerFinnhubCryptoExchange.IsExchange(_T("HUOBI")));
	}

	TEST_F(CWorldMarketTest, TestAddCryptoExchange) {
		const auto lTotalCryptoExchange = gl_dataContainerFinnhubCryptoExchange.Size();
		const string strCryptoExchange = _T("000001.SZ");

		EXPECT_FALSE(gl_dataContainerFinnhubCryptoExchange.IsExchange(strCryptoExchange));
		gl_dataContainerFinnhubCryptoExchange.Add(strCryptoExchange);
		EXPECT_TRUE(gl_dataContainerFinnhubCryptoExchange.IsExchange(strCryptoExchange));
		EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), lTotalCryptoExchange + 1);
		gl_dataContainerFinnhubCryptoExchange.Delete(strCryptoExchange);
		EXPECT_FALSE(gl_dataContainerFinnhubCryptoExchange.IsExchange(strCryptoExchange));
		EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), lTotalCryptoExchange);
	}

	TEST_F(CWorldMarketTest, TestDeleteCryptoExchange) {
		// do nothing. 已经在TestAddCryptoExchange中测试了DeleteCryptoExchange函数
		EXPECT_FALSE(gl_dataContainerFinnhubCryptoExchange.Delete(_T("US.US.US"))) << "此符号在符号集中不存在";
	}

	TEST_F(CWorldMarketTest, TestIsCryptoSymbol) {
		EXPECT_FALSE(gl_dataFinnhubCryptoSymbol.IsSymbol(_T("ABC")));
		EXPECT_TRUE(gl_dataFinnhubCryptoSymbol.IsSymbol(_T("POLONIEX:BTC_DOT")));
		EXPECT_TRUE(gl_dataFinnhubCryptoSymbol.IsSymbol(_T("BINANCE:USDTUAH")));
		EXPECT_TRUE(gl_dataFinnhubCryptoSymbol.IsSymbol(_T("COINBASE:TRIBE-USD")));

		const auto pCryptoSymbol = make_shared<CFinnhubCryptoSymbol>();
		pCryptoSymbol->SetSymbol(_T("ABC"));
		EXPECT_FALSE(gl_dataFinnhubCryptoSymbol.IsSymbol(pCryptoSymbol));
		pCryptoSymbol->SetSymbol(_T("BINANCE:USDTUAH"));
		EXPECT_TRUE(gl_dataFinnhubCryptoSymbol.IsSymbol(pCryptoSymbol));
		pCryptoSymbol->SetSymbol(_T("COINBASE:TRIBE-USD"));
		EXPECT_TRUE(gl_dataFinnhubCryptoSymbol.IsSymbol(pCryptoSymbol));
	}

	TEST_F(CWorldMarketTest, TestAddCryptoSymbol) {
		const auto pCryptoSymbol = make_shared<CFinnhubCryptoSymbol>();
		const auto lTotalCryptoSymbol = gl_dataFinnhubCryptoSymbol.Size();
		pCryptoSymbol->SetSymbol(_T("000001.SZ"));

		EXPECT_FALSE(gl_dataFinnhubCryptoSymbol.IsSymbol(pCryptoSymbol));
		gl_dataFinnhubCryptoSymbol.Add(pCryptoSymbol);
		EXPECT_TRUE(gl_dataFinnhubCryptoSymbol.IsSymbol(pCryptoSymbol));
		EXPECT_EQ(gl_dataFinnhubCryptoSymbol.Size(), lTotalCryptoSymbol + 1);

		gl_dataFinnhubCryptoSymbol.Delete(pCryptoSymbol);
		EXPECT_FALSE(gl_dataFinnhubCryptoSymbol.IsSymbol(pCryptoSymbol));
		EXPECT_EQ(gl_dataFinnhubCryptoSymbol.Size(), lTotalCryptoSymbol);
	}

	TEST_F(CWorldMarketTest, TestDeleteCryptoSymbol) {
		// do nothing. 已经在TestAddCryptoSymbol中测试了DeleteCryptoSymbol函数
		CFinnhubCryptoSymbolPtr pCryptoSymbol = nullptr;

		gl_dataFinnhubCryptoSymbol.Delete(pCryptoSymbol); // "空指针";

		pCryptoSymbol = make_shared<CFinnhubCryptoSymbol>();
		pCryptoSymbol->SetSymbol(_T("000001.SZ"));
		gl_dataFinnhubCryptoSymbol.Delete(pCryptoSymbol); // "此符号在符号集中不存在";
	}

	TEST_F(CWorldMarketTest, TestIsCountry) {
		const auto pCountry = make_shared<CCountry>();

		EXPECT_FALSE(gl_dataContainerFinnhubCountry.IsCountry(_T("ABC")));
		EXPECT_TRUE(gl_dataContainerFinnhubCountry.IsCountry(_T("American Samoa")));

		pCountry->m_strCountry = _T("ABC");
		EXPECT_FALSE(gl_dataContainerFinnhubCountry.IsCountry(pCountry));
		pCountry->m_strCountry = _T("American Samoa");
		EXPECT_TRUE(gl_dataContainerFinnhubCountry.IsCountry(pCountry));
	}

	TEST_F(CWorldMarketTest, TestAddCountry) {
		const auto pCountry = make_shared<CCountry>();
		const auto lTotalCountry = gl_dataContainerFinnhubCountry.GetTotalCountry();
		pCountry->m_strCountry = _T("SZ");

		EXPECT_FALSE(gl_dataContainerFinnhubCountry.IsCountry(pCountry));
		gl_dataContainerFinnhubCountry.Add(pCountry);
		EXPECT_TRUE(gl_dataContainerFinnhubCountry.IsCountry(pCountry));
		EXPECT_EQ(gl_dataContainerFinnhubCountry.GetTotalCountry(), lTotalCountry + 1);
		gl_dataContainerFinnhubCountry.Delete(pCountry);
		EXPECT_FALSE(gl_dataContainerFinnhubCountry.IsCountry(pCountry));
		EXPECT_EQ(gl_dataContainerFinnhubCountry.GetTotalCountry(), lTotalCountry);
	}

	TEST_F(CWorldMarketTest, TestLoadExchangeCode) {
		// todo 暂缓
	}

	TEST_F(CWorldMarketTest, TestUpdateCountryDB) {
		const size_t lTotal = gl_dataContainerFinnhubCountry.GetTotalCountry();

		const auto pCountry = make_shared<CCountry>();
		pCountry->m_strCode2 = _T("AB");
		pCountry->m_strCountry = _T("NoName");
		EXPECT_FALSE(gl_dataContainerFinnhubCountry.IsCountry(pCountry));
		gl_dataContainerFinnhubCountry.Add(pCountry);
		EXPECT_EQ(gl_dataContainerFinnhubCountry.GetTotalCountry(), lTotal + 1);
		gl_dataContainerFinnhubCountry.UpdateDB(); // 此测试函数执行完后，新增了一个Country没有删除（数据库中的删除了）。

		CSetCountry setCountry;
		setCountry.m_strFilter = _T("[Country] = 'NoName'");
		setCountry.Open();
		EXPECT_FALSE(setCountry.IsEOF());
		setCountry.m_pDatabase->BeginTrans();
		while (!setCountry.IsEOF()) {
			setCountry.Delete();
			setCountry.MoveNext();
		}
		setCountry.m_pDatabase->CommitTrans();
		setCountry.Close();
	}

	TEST_F(CWorldMarketTest, TestUpdateStockProfileDB) {
		auto pStock = make_shared<CWorldStock>();
		pStock->SetSymbol(_T("SS.SS.US"));
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsSymbol(pStock)); // 确保是一个新股票代码
		pStock->SetTodayNewStock(true);
		pStock->SetUpdateProfileDB(true);
		gl_dataContainerFinnhubStock.Add(pStock);
		pStock = gl_dataContainerFinnhubStock.GetStock(_T("000001.SS"));
		EXPECT_TRUE(pStock != nullptr);
		EXPECT_STREQ(pStock->GetCurrency(), _T(""));
		pStock->SetUpdateProfileDB(true);
		pStock->SetCurrency(_T("No Currency")); // 更新这个条目

		try {
			gl_dataContainerFinnhubStock.UpdateProfileDB();
		}
		catch (std::exception& e) {
			EXPECT_TRUE(false) << e.what();
		}
		catch (CException* e) {
			ReportInformationAndDeleteException(e);
		}
		catch (...) {
			EXPECT_TRUE(false) << "未知exception";
			ASSERT_FALSE(true);
		}

		CSetWorldStock setWorldStock;
		setWorldStock.m_strFilter = _T("[Symbol] = '000001.SS'");
		setWorldStock.Open();
		EXPECT_FALSE(setWorldStock.IsEOF());
		EXPECT_STREQ(setWorldStock.m_Currency, _T("No Currency")) << "此条目已更新";
		setWorldStock.m_pDatabase->BeginTrans();
		setWorldStock.Edit();
		setWorldStock.m_Currency = _T("");
		setWorldStock.Update();
		setWorldStock.m_pDatabase->CommitTrans();
		setWorldStock.Close();

		CSetWorldStock setWorldStock2;
		setWorldStock2.m_strFilter = _T("[Symbol] = 'SS.SS.US'");
		setWorldStock2.Open();
		EXPECT_FALSE(setWorldStock2.IsEOF());
		setWorldStock2.m_pDatabase->BeginTrans();
		while (!setWorldStock2.IsEOF()) {
			setWorldStock2.Delete();
			setWorldStock2.MoveNext();
		}
		setWorldStock2.m_pDatabase->CommitTrans();
		setWorldStock2.Close();

		// 恢复原状
		pStock = gl_dataContainerFinnhubStock.GetStock(_T("SS.SS.US"));
		EXPECT_TRUE(pStock != nullptr);
		gl_dataContainerFinnhubStock.Delete(pStock);
		pStock = gl_dataContainerFinnhubStock.GetStock(_T("000001.SS"));
		EXPECT_TRUE(pStock != nullptr);
		pStock->SetCurrency(_T(""));
		while (gl_systemMessage.InnerSystemInfoSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
		EXPECT_EQ(gl_dataContainerFinnhubStock.Size(), 4847);
	}

	TEST_F(CWorldMarketTest, TestUpdateDayLineDB) {
		EXPECT_TRUE(gl_pWorldMarket->UpdateStockDayLineDB());

		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetStock(0)->IsDayLineNeedSaving()) << "此标识被重置";
		gl_dataContainerFinnhubStock.GetStock(0)->SetDayLineNeedSaving(true);

		EXPECT_TRUE(gl_pWorldMarket->UpdateStockDayLineDB());
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			EXPECT_FALSE(gl_dataContainerFinnhubStock.GetStock(i)->IsDayLineNeedSaving()) << "此标识被重置";
		}
	}

	TEST_F(CWorldMarketTest, TestUpdateForexSymbolDB) {
		auto pForexSymbol = make_shared<CFinnhubForexSymbol>();
		pForexSymbol->SetSymbol(_T("SS.SS.US")); // 新符号
		EXPECT_FALSE(gl_dataFinnhubForexSymbol.IsSymbol(pForexSymbol));
		gl_dataFinnhubForexSymbol.Add(pForexSymbol);
		pForexSymbol = gl_dataFinnhubForexSymbol.GetSymbol(_T("OANDA:GBP_ZAR")); // 第二个现存的符号
		EXPECT_EQ(pForexSymbol->GetIPOStatus(), _STOCK_IPOED_);
		pForexSymbol->SetUpdateProfileDB(true);
		pForexSymbol->SetIPOStatus(_STOCK_DELISTED_);
		gl_dataFinnhubForexSymbol.UpdateDB();

		CSetFinnhubForexSymbol setCryptoSymbol;
		setCryptoSymbol.m_strFilter = _T("[Symbol] = 'OANDA:GBP_ZAR'");
		setCryptoSymbol.Open();
		EXPECT_EQ(setCryptoSymbol.m_IPOStatus, _STOCK_DELISTED_);
		setCryptoSymbol.m_pDatabase->BeginTrans();
		setCryptoSymbol.Edit();
		setCryptoSymbol.m_IPOStatus = _STOCK_IPOED_;
		setCryptoSymbol.Update();
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();

		setCryptoSymbol.m_strFilter = _T("[Symbol] = 'SS.SS.US'");
		setCryptoSymbol.Open();
		EXPECT_FALSE(setCryptoSymbol.IsEOF()) << "存入了新符号";
		setCryptoSymbol.m_pDatabase->BeginTrans();
		while (!setCryptoSymbol.IsEOF()) {
			setCryptoSymbol.Delete();
			setCryptoSymbol.MoveNext();
		}
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();

		pForexSymbol = gl_dataFinnhubForexSymbol.GetSymbol(_T("SS.SS.US"));
		EXPECT_TRUE(pForexSymbol != nullptr);
		gl_dataFinnhubForexSymbol.Delete(pForexSymbol); // 恢复原状
	}

	TEST_F(CWorldMarketTest, TestUpdateFinnhubCryptoSymbolDB) {
		auto pCryptoSymbol = make_shared<CFinnhubCryptoSymbol>();
		pCryptoSymbol->SetSymbol(_T("SS.SS.US")); // 新符号
		EXPECT_FALSE(gl_dataFinnhubCryptoSymbol.IsSymbol(pCryptoSymbol));
		gl_dataFinnhubCryptoSymbol.Add(pCryptoSymbol);
		pCryptoSymbol = gl_dataFinnhubCryptoSymbol.GetSymbol(_T("BINANCE:USDTUAH")); // 第二个现存的符号
		EXPECT_EQ(pCryptoSymbol->GetIPOStatus(), _STOCK_IPOED_);
		pCryptoSymbol->SetUpdateProfileDB(true);
		pCryptoSymbol->SetIPOStatus(_STOCK_DELISTED_);
		gl_dataFinnhubCryptoSymbol.UpdateDB();

		CSetFinnhubCryptoSymbol setCryptoSymbol;
		setCryptoSymbol.m_strFilter = _T("[Symbol] = 'BINANCE:USDTUAH'");
		setCryptoSymbol.Open();
		EXPECT_EQ(setCryptoSymbol.m_IPOStatus, _STOCK_DELISTED_) << "状态已被修改为摘牌";
		setCryptoSymbol.m_pDatabase->BeginTrans();
		setCryptoSymbol.Edit();
		setCryptoSymbol.m_IPOStatus = _STOCK_IPOED_;
		setCryptoSymbol.Update();
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();

		setCryptoSymbol.m_strFilter = _T("[Symbol] = 'SS.SS.US'");
		setCryptoSymbol.Open();
		EXPECT_FALSE(setCryptoSymbol.IsEOF()) << "存入了新符号";
		setCryptoSymbol.m_pDatabase->BeginTrans();
		while (!setCryptoSymbol.IsEOF()) {
			setCryptoSymbol.Delete(); // 删除此新代码
			setCryptoSymbol.MoveNext();
		}
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();

		pCryptoSymbol = gl_dataFinnhubCryptoSymbol.GetSymbol(_T("SS.SS.US"));
		EXPECT_TRUE(pCryptoSymbol != nullptr);
		gl_dataFinnhubCryptoSymbol.Delete(pCryptoSymbol); // 恢复原状
	}

	TEST_F(CWorldMarketTest, TestUpdateTiingoStockDB) {
		CSetTiingoStock setTiingoStock;

		const auto pTiingoStock = make_shared<CTiingoStock>();
		pTiingoStock->m_fIsActive = true;
		pTiingoStock->m_fIsADR = false;
		pTiingoStock->m_iSICCode = 1002;
		pTiingoStock->m_lDailyDataUpdateDate = 20200101;
		pTiingoStock->m_lStatementUpdateDate = 20210101;
		pTiingoStock->m_strCompanyWebSite = _T("www.abc.com");
		pTiingoStock->m_strLocation = _T("Irvine CA USA");
		pTiingoStock->m_strName = _T("ABCDEF"); // 新代码
		pTiingoStock->m_strReportingCurrency = _T("US Doller");
		pTiingoStock->m_strSECFilingWebSite = _T("abc");
		pTiingoStock->m_strSICIndustry = _T("Computer Science");
		pTiingoStock->m_strSICSector = _T("Communication");
		pTiingoStock->m_strTicker = _T("ABCDEF"); // 新代码
		pTiingoStock->m_strTiingoIndustry = _T("Computer");
		pTiingoStock->m_strTiingoPermaTicker = _T("abcdefg");
		pTiingoStock->m_strTiingoSector = _T("gfedcba");

		gl_dataContainerTiingoStock.Add(pTiingoStock);

		gl_dataContainerTiingoStock.UpdateDB(); // 更新代码集

		// 恢复原状
		setTiingoStock.m_strFilter = _T("[Ticker] = 'ABCDEF'");
		setTiingoStock.Open();
		EXPECT_FALSE(setTiingoStock.IsEOF()) << "存入了ABCDEF股票代码";
		setTiingoStock.m_pDatabase->BeginTrans();
		while (!setTiingoStock.IsEOF()) {
			setTiingoStock.Delete();
			setTiingoStock.MoveNext();
		}
		setTiingoStock.m_pDatabase->CommitTrans();
		setTiingoStock.Close();

		gl_dataContainerTiingoStock.Delete(pTiingoStock);
	}

	TEST_F(CWorldMarketTest, TestUpdateForexExchangeDB) {
		const CString strSymbol = _T("US.US.US");

		EXPECT_FALSE(gl_dataContainerFinnhubForexExchange.UpdateDB()) << "没有新Forex Exchange";

		EXPECT_FALSE(gl_dataContainerFinnhubForexExchange.IsExchange(strSymbol)); // 确保是一个新股票代码
		gl_dataContainerFinnhubForexExchange.Add(strSymbol);
		EXPECT_TRUE(gl_dataContainerFinnhubForexExchange.UpdateDB());

		CSetFinnhubForexExchange setForexExchange;
		setForexExchange.m_strFilter = _T("[Code] = 'US.US.US'");
		setForexExchange.Open();
		EXPECT_FALSE(setForexExchange.IsEOF());
		setForexExchange.m_pDatabase->BeginTrans();
		while (!setForexExchange.IsEOF()) {
			setForexExchange.Delete();
			setForexExchange.MoveNext();
		}
		setForexExchange.m_pDatabase->CommitTrans();
		setForexExchange.Close();

		gl_dataContainerFinnhubForexExchange.Delete(strSymbol); // 恢复原状
	}

	TEST_F(CWorldMarketTest, TestUpdateCryptoExchangeDB) {
		const string sSymbol = _T("US.US.US");

		EXPECT_FALSE(gl_dataContainerFinnhubCryptoExchange.UpdateDB()) << "没有新Crypto Exchange";

		EXPECT_FALSE(gl_dataContainerFinnhubCryptoExchange.IsExchange(sSymbol)); // 确保是一个新Crypto代码
		gl_dataContainerFinnhubCryptoExchange.Add(sSymbol);
		EXPECT_TRUE(gl_dataContainerFinnhubCryptoExchange.UpdateDB());

		CSetFinnhubCryptoExchange setCryptoExchange;
		setCryptoExchange.m_strFilter = _T("[Code] = 'US.US.US'");
		setCryptoExchange.Open();
		EXPECT_FALSE(setCryptoExchange.IsEOF());
		setCryptoExchange.m_pDatabase->BeginTrans();
		while (!setCryptoExchange.IsEOF()) {
			setCryptoExchange.Delete(); // 删除新添加的这个代码
			setCryptoExchange.MoveNext();
		}
		setCryptoExchange.m_pDatabase->CommitTrans();
		setCryptoExchange.Close();

		gl_dataContainerFinnhubCryptoExchange.Delete(sSymbol); // 恢复原状
	}

	TEST_F(CWorldMarketTest, TaskUpdateInsiderTransactionDB) {
		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetStock(_T("A"))->HaveInsiderTransaction());
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 0);

		vector<CInsiderTransactionPtr> vInsiderTransaction;
		CSetInsiderTransaction setInsiderTransaction;

		CInsiderTransactionPtr pInsiderTransaction = make_shared<CInsiderTransaction>();
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

		const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(_T("A"));
		EXPECT_FALSE(pStock->HaveInsiderTransaction()) << "此时尚未存入数据";

		pStock->SetSaveInsiderTransaction(true);
		pStock->SetInsiderTransactionUpdateDate(20210123);
		pStock->UpdateInsiderTransaction(vInsiderTransaction);

		EXPECT_TRUE(gl_pWorldMarket->UpdateInsiderTransactionDB());

		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 1);
		gl_systemMessage.PopDayLineInfoMessage();
		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetStock(_T("A"))->IsSaveInsiderTransaction());
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(_T("A"))->HaveInsiderTransaction()) << "存储后并没有删除数据";
		gl_dataContainerFinnhubStock.GetStock(_T("A"))->UnloadInsiderTransaction();

		// 验证并恢复原状
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

	TEST_F(CWorldMarketTest, TaskUpdateInsiderSentimentDB) {
		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetStock(_T("A"))->HaveInsiderSentiment());
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 0);

		vector<CInsiderSentimentPtr> vInsiderSentiment;
		CSetInsiderSentiment setInsiderSentiment;

		CInsiderSentimentPtr pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment->m_strSymbol = _T("B");// 这个股票代码不符，需要添加进数据库
		pInsiderSentiment->m_lDate = 20200101;
		vInsiderSentiment.push_back(pInsiderSentiment);
		pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment->m_strSymbol = _T("A");
		pInsiderSentiment->m_lDate = 20200101;// 这个数据库中有，无需添加
		vInsiderSentiment.push_back(pInsiderSentiment);
		pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment->m_strSymbol = _T("A");
		pInsiderSentiment->m_lDate = 20210101; // 这个日期不符，需要添加进数据库
		vInsiderSentiment.push_back(pInsiderSentiment);

		const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(_T("A"));
		EXPECT_FALSE(pStock->HaveInsiderSentiment()) << "此时尚未存入数据";

		pStock->SetSaveInsiderSentiment(true);
		pStock->SetInsiderSentimentUpdateDate(20210101);
		pStock->UpdateInsiderSentiment(vInsiderSentiment);

		EXPECT_TRUE(gl_pWorldMarket->UpdateInsiderSentimentDB());

		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 0);
		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetStock(_T("A"))->IsSaveInsiderSentiment());
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(_T("A"))->HaveInsiderSentiment()) << "存储后并没有删除数据";

		// 验证并恢复原状
		setInsiderSentiment.m_strFilter = _T("[Symbol] = 'B'");
		setInsiderSentiment.Open();
		EXPECT_TRUE(setInsiderSentiment.IsEOF());
		setInsiderSentiment.Close();

		setInsiderSentiment.m_strFilter = _T("[Date] = '20210101'");
		setInsiderSentiment.Open();
		setInsiderSentiment.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderSentiment.IsEOF());
		setInsiderSentiment.Delete();
		setInsiderSentiment.m_pDatabase->CommitTrans();
		setInsiderSentiment.Close();
	}

	TEST_F(CWorldMarketTest, TestUpdateEconomicCalendarDB) {
		CSetEconomicCalendar setEconomicCalendar;
		const auto pEconomicCalendar = make_shared<CEconomicCalendar>();
		vector<CEconomicCalendarPtr> vEconomicCalendar;

		pEconomicCalendar->m_strCountry = _T("USA");
		pEconomicCalendar->m_strTime = _T("20200101");
		pEconomicCalendar->m_strEvent = _T("abc");
		pEconomicCalendar->m_dActual = 1.0;
		pEconomicCalendar->m_dEstimate = 2.0;
		pEconomicCalendar->m_dPrev = 3.0;
		pEconomicCalendar->m_strImpact = _T("s");
		pEconomicCalendar->m_strUnit = _T("USD");

		vEconomicCalendar.push_back(pEconomicCalendar);

		EXPECT_TRUE(gl_dataContainerFinnhubEconomicCalendar.Update(vEconomicCalendar));
		EXPECT_TRUE(gl_dataContainerFinnhubEconomicCalendar.UpdateDB());

		// 测试并恢复原状
		setEconomicCalendar.Open();
		EXPECT_FALSE(setEconomicCalendar.IsEOF());
		EXPECT_STREQ(setEconomicCalendar.m_Country, _T("USA"));
		EXPECT_STREQ(setEconomicCalendar.m_Time, _T("20200101"));
		EXPECT_STREQ(setEconomicCalendar.m_Event, _T("abc"));
		EXPECT_DOUBLE_EQ(atof(setEconomicCalendar.m_Actual), 1.0);
		EXPECT_DOUBLE_EQ(atof(setEconomicCalendar.m_Estimate), 2.0);
		EXPECT_DOUBLE_EQ(atof(setEconomicCalendar.m_Prev), 3.0);
		EXPECT_STREQ(setEconomicCalendar.m_Impact, _T("s"));
		EXPECT_STREQ(setEconomicCalendar.m_Unit, _T("USD"));
		setEconomicCalendar.m_pDatabase->BeginTrans();
		while (!setEconomicCalendar.IsEOF()) {
			setEconomicCalendar.Delete();
			setEconomicCalendar.MoveNext();
		}
		setEconomicCalendar.m_pDatabase->CommitTrans();
		setEconomicCalendar.Close();
	}

	TEST_F(CWorldMarketTest, TestRebuildEPSSurprise) {
		CWorldStockPtr pStock;
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetLastEPSSurpriseUpdateDate(20200101);
			pStock->m_fEPSSurpriseUpdated = true;
		}
		gl_pFinnhubDataSource->SetUpdateEPSSurprise(false);

		gl_pWorldMarket->RebuildEPSSurprise();

		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			EXPECT_EQ(pStock->GetLastEPSSurpriseUpdateDate(), 19800101);
			EXPECT_FALSE(pStock->m_fEPSSurpriseUpdated);
		}
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateEPSSurprise());
	}

	TEST_F(CWorldMarketTest, TestRebuildPeer) {
		CWorldStockPtr pStock;
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetPeerUpdateDate(20200101);
			pStock->SetUpdatePeer(false);
			pStock->SetUpdateProfileDB(false);
		}
		gl_pFinnhubDataSource->SetUpdatePeer(false);

		gl_pWorldMarket->RebuildPeer();

		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			EXPECT_EQ(pStock->GetPeerUpdateDate(), 19800101);
			EXPECT_TRUE(pStock->IsUpdatePeer());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
		}
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdatePeer());

		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetUpdatePeer(true);
			pStock->SetUpdateProfileDB(false);
		}
	}

	TEST_F(CWorldMarketTest, TestRebuildStockDayLine) {
		CWorldStockPtr pStock;
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetIPOStatus(_STOCK_IPOED_);
			pStock->SetDayLineStartDate(20200101);
			pStock->SetDayLineEndDate(20200101);
			pStock->SetDayLineNeedUpdate(false);
			pStock->SetUpdateProfileDB(false);
		}
		gl_pFinnhubDataSource->SetUpdateStockProfile(false);

		gl_pWorldMarket->RebuildStockDayLineDB();

		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			EXPECT_EQ(pStock->GetDayLineStartDate(), 29900101);
			EXPECT_EQ(pStock->GetDayLineEndDate(), 19800101);
			EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
		}
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateStockProfile());

		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetUpdateProfileDB(false);
		}
	}

	TEST_F(CWorldMarketTest, TestUpdateTiingoIndustry) {
		EXPECT_THAT(gl_pWorldMarket->UpdateTiingoIndustry(), IsFalse());
	}

	TEST_F(CWorldMarketTest, TestUpdateSICIndustry) {
		EXPECT_THAT(gl_pWorldMarket->UpdateSICIndustry(), IsFalse());
	}

	TEST_F(CWorldMarketTest, TestUpdateNaicsIndustry) {
		EXPECT_THAT(gl_pWorldMarket->UpdateNaicsIndustry(), IsFalse());
	}

	TEST_F(CWorldMarketTest, TestTaskResetMarket1) {
		EXPECT_TRUE(gl_pWorldMarket->IsResetMarket());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());

		gl_pWorldMarket->TaskResetMarket(1000);

		EXPECT_TRUE(gl_pWorldMarket->IsResetMarket());
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pWorldMarket->IsMarketTaskEmpty());
		const auto pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1000);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		// 恢复原状
		gl_pWorldMarket->SetResetMarket(true);
		gl_pWorldMarket->SetSystemReady(true);
	}

	TEST_F(CWorldMarketTest, TestTaskResetMarket2) {
		EXPECT_TRUE(gl_pWorldMarket->IsResetMarket());
		gl_pWorldMarket->SetResetMarket(false);
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		gl_pWorldMarket->SetSystemReady(false);
		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());

		gl_pWorldMarket->TaskResetMarket(1000);

		EXPECT_TRUE(gl_pWorldMarket->IsResetMarket());
		EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pWorldMarket->IsMarketTaskEmpty());
		const auto pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1000);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		// 恢复原状
		gl_pWorldMarket->SetResetMarket(true);
		gl_pWorldMarket->SetSystemReady(true);
	}

	TEST_F(CWorldMarketTest, TestTaskCreateTask1) {
		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());

		gl_pWorldMarket->TaskCreateTask(10010);

		EXPECT_FALSE(gl_pWorldMarket->IsMarketTaskEmpty());
		auto pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__);
		EXPECT_EQ(pTask->GetTime(), 10010);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_UPDATE_STOCK_PROFILE_DB__);
		EXPECT_EQ(pTask->GetTime(), 10040);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_START_ALL_WEB_SOCKET__);
		EXPECT_EQ(pTask->GetTime(), 10100);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_STOP_ALL_WEB_SOCKET__);
		EXPECT_EQ(pTask->GetTime(), 10130);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_RESET__);
		EXPECT_EQ(pTask->GetTime(), 170000);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), CREATE_TASK__);
		EXPECT_EQ(pTask->GetTime(), 240000);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());
	}

	TEST_F(CWorldMarketTest, TestTaskCreateTask2) {
		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());

		gl_pWorldMarket->TaskCreateTask(180110);

		EXPECT_FALSE(gl_pWorldMarket->IsMarketTaskEmpty());
		auto pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__);
		EXPECT_EQ(pTask->GetTime(), 180110);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_UPDATE_STOCK_PROFILE_DB__);
		EXPECT_EQ(pTask->GetTime(), 180140);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_START_ALL_WEB_SOCKET__);
		EXPECT_EQ(pTask->GetTime(), 180200);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_STOP_ALL_WEB_SOCKET__);
		EXPECT_EQ(pTask->GetTime(), 180230);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), CREATE_TASK__);
		EXPECT_EQ(pTask->GetTime(), 240000);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());
	}

	TEST_F(CWorldMarketTest, TestTaskProcessWebSocketData1) {
		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());

		gl_pWorldMarket->TaskProcessWebSocketData(165758);

		EXPECT_FALSE(gl_pWorldMarket->IsMarketTaskEmpty());
		const auto pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__);
		EXPECT_EQ(pTask->GetTime(), 165759);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());
	}

	TEST_F(CWorldMarketTest, TestTaskProcessWebSocketData2) {
		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());

		gl_pWorldMarket->TaskProcessWebSocketData(165759);

		EXPECT_FALSE(gl_pWorldMarket->IsMarketTaskEmpty());
		const auto pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__);
		EXPECT_EQ(pTask->GetTime(), 170501);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());
	}

	TEST_F(CWorldMarketTest, TestGetFinnhubWebSocketSymbolVector) {
		const vectorString vString = gl_pWorldMarket->GetFinnhubWebSocketSymbolVector();

		EXPECT_STREQ(vString.at(0).c_str(), _T("A"));
		EXPECT_STREQ(vString.at(1).c_str(), _T("AA"));
		EXPECT_STREQ(vString.at(2).c_str(), _T("AAL"));
		EXPECT_STREQ(vString.at(3).c_str(), _T("AAPL"));

		EXPECT_STREQ(vString.at(4).c_str(), _T("FXCM:USD/JPY"));
		EXPECT_STREQ(vString.at(5).c_str(), _T("IC MARKETS:1"));
		EXPECT_STREQ(vString.at(6).c_str(), _T("OANDA:AUD_SGD"));

		EXPECT_STREQ(vString.at(7).c_str(), _T("BINANCE:IDEXBUSD"));
		EXPECT_STREQ(vString.at(8).c_str(), _T("BINANCE:MDAETH"));
		EXPECT_STREQ(vString.at(9).c_str(), _T("BINANCE:OCEANBTC"));
	}
}
