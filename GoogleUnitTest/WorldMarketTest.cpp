#include"pch.h"

#include"GeneralCheck.h"

#include"FinnhubStock.h"

#include"ProductFinnhubCompanyInsiderTransaction.h"
#include"ProductFinnhubCompanyInsiderSentiment.h"

#include"SetFinnhubForexExchange.h"
#include"SetFinnhubCryptoExchange.h"
#include"SetFinnhubCryptoSymbol.h"
#include"SetFinnhubForexSymbol.h"

#include"FinnhubDataSource.h"
#include"FinnhubCrypto.h"
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
		EXPECT_FALSE(gl_pWorldMarket->IsTimeToResetSystem(GetPrevTime(gl_pWorldMarket->GetResetTime(), 0, 2, 1)));
		EXPECT_TRUE(gl_pWorldMarket->IsTimeToResetSystem(GetPrevTime(gl_pWorldMarket->GetResetTime(), 0, 2, 0)));
		EXPECT_TRUE(gl_pWorldMarket->IsTimeToResetSystem(GetPrevTime(gl_pWorldMarket->GetResetTime(), 0, 1, 59)));
		EXPECT_TRUE(gl_pWorldMarket->IsTimeToResetSystem(GetNextTime(gl_pWorldMarket->GetResetTime(), 0, 4, 59)));
		EXPECT_TRUE(gl_pWorldMarket->IsTimeToResetSystem(GetNextTime(gl_pWorldMarket->GetResetTime(), 0, 5, 0)));
		EXPECT_FALSE(gl_pWorldMarket->IsTimeToResetSystem(GetNextTime(gl_pWorldMarket->GetResetTime(), 0, 5, 1)));
	}

	TEST_F(CWorldMarketTest, TestTransferMarketTime) {
		tm tm2_;
		time_t tt = GetUTCTime();

		gl_pWorldMarket->CalculateTime();
		gmtime_s(&tm2_, &tt);
		tm tm_;
		gl_pWorldMarket->GetMarketTimeStruct(&tm_, GetUTCTime());
		if (gl_pWorldMarket->GetTimeZone() == 4 * 3600) { //美东夏时制？
			EXPECT_TRUE((tm_.tm_hour == (tm2_.tm_hour - 4) || (tm_.tm_hour == tm2_.tm_hour + 20))) << "WorldMarket默认为西四区(美东夏时制标准时间)";
		}
		else {
			EXPECT_TRUE((tm_.tm_hour == (tm2_.tm_hour - 5) || (tm_.tm_hour == tm2_.tm_hour + 19))) << "WorldMarket默认为西四区(美东标准时间)";
		}
	}

	TEST_F(CWorldMarketTest, TestGetTotalStock) {
		EXPECT_EQ(gl_dataContainerFinnhubStock.Size(), 4847) << "默认状态下数据库总数为4847(全部上海股票和小部分美国股票)";
	}

	TEST_F(CWorldMarketTest, TestIsStock) {
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsSymbol("000000.SS"));
		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsSymbol("000001.SS"));
		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsSymbol("600601.SS"));
		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsSymbol("A"));
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsSymbol("000001.SZ")) << "目前测试数据库中只有上海和美国股票集";

		const auto pStock = make_shared<CFinnhubStock>();
		pStock->SetSymbol("000000.SS");
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsSymbol(pStock));
		pStock->SetSymbol("000001.SS");
		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsSymbol(pStock));
		pStock->SetSymbol("600601.SS");
		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsSymbol(pStock));
		pStock->SetSymbol("A");
		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsSymbol(pStock));
		pStock->SetSymbol("000001.SZ");
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsSymbol(pStock));
	}

	TEST_F(CWorldMarketTest, TestIsTiingoStock) {
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsSymbol("000000.SS"));
		EXPECT_TRUE(gl_dataContainerTiingoStock.IsSymbol("AA"));
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsSymbol("600601.SS"));
		EXPECT_TRUE(gl_dataContainerTiingoStock.IsSymbol("A"));
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsSymbol("000001.SZ")) << "目前测试数据库中只有上海和美国股票集";

		const auto pStock = make_shared<CFinnhubStock>();
		pStock->SetSymbol("000000.SS");
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsSymbol(pStock));
		pStock->SetSymbol("AA");
		EXPECT_TRUE(gl_dataContainerTiingoStock.IsSymbol(pStock));
		pStock->SetSymbol("600601.SS");
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsSymbol(pStock));
		pStock->SetSymbol("A");
		EXPECT_TRUE(gl_dataContainerTiingoStock.IsSymbol(pStock));
		pStock->SetSymbol("000001.SZ");
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsSymbol(pStock));

		const auto pTiingoStock = make_shared<CTiingoStock>();
		pTiingoStock->SetSymbol("000000.SS");
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsSymbol(pTiingoStock));
		pTiingoStock->SetSymbol("AA");
		EXPECT_TRUE(gl_dataContainerTiingoStock.IsSymbol(pTiingoStock));
		pTiingoStock->SetSymbol("600601.SS");
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsSymbol(pTiingoStock));
		pTiingoStock->SetSymbol("A");
		EXPECT_TRUE(gl_dataContainerTiingoStock.IsSymbol(pTiingoStock));
		pTiingoStock->SetSymbol("000001.SZ");
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsSymbol(pTiingoStock));
	}

	TEST_F(CWorldMarketTest, TestGetTiingoStock) {
		CTiingoStockPtr pStock = gl_dataContainerTiingoStock.GetStock(0); // A
		EXPECT_EQ(pStock->GetSymbol(), "A") << "第一个股票代码为A";
		pStock = gl_dataContainerTiingoStock.GetStock("A");
		EXPECT_FALSE(pStock == nullptr);
		EXPECT_EQ(pStock->GetName(), "Agilent Technologies Inc");
	}

	TEST_F(CWorldMarketTest, TestIsUpdateProfileDB) {
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(i);
			pStock->SetUpdateProfileDB(false);
		}
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());

		gl_dataContainerFinnhubStock.GetItem(0)->SetUpdateProfileDB(true);
		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());

		gl_dataContainerFinnhubStock.GetItem(0)->SetUpdateProfileDB(false);
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
	}

	TEST_F(CWorldMarketTest, TestAddStock) {
		const auto pStock = make_shared<CFinnhubStock>();
		pStock->SetSymbol("000001.SZ");

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
		CFinnhubStockPtr pStock = nullptr;

		gl_dataContainerFinnhubStock.Delete(pStock);

		pStock = make_shared<CFinnhubStock>();
		pStock->SetSymbol("000001.SZ");
		gl_dataContainerFinnhubStock.Delete(pStock); // "此股票代码不存在于代码集中";
	}

	TEST_F(CWorldMarketTest, TestGetStock) {
		CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(0); // 000001.SS
		EXPECT_EQ(pStock->GetSymbol(), "000001.SS") << "第一个股票代码为000001.SS";
		pStock = gl_dataContainerFinnhubStock.GetItem("000001.SS");
		EXPECT_FALSE(pStock == nullptr);
		EXPECT_EQ(pStock->GetDescription(), "SSE Composite Index");
	}

	TEST_F(CWorldMarketTest, TestAddTiingoStock) {
		const auto pStock = make_shared<CTiingoStock>();
		const auto lTotalStock = gl_dataContainerTiingoStock.Size();
		pStock->SetSymbol("ABCDEF");

		EXPECT_FALSE(gl_dataContainerTiingoStock.IsSymbol(pStock));
		gl_dataContainerTiingoStock.Add(pStock);
		EXPECT_TRUE(gl_dataContainerTiingoStock.IsSymbol(pStock));
		EXPECT_EQ(gl_dataContainerTiingoStock.Size(), lTotalStock + 1);
		gl_dataContainerTiingoStock.Delete(pStock);
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsSymbol(pStock));
		EXPECT_EQ(gl_dataContainerTiingoStock.Size(), lTotalStock);
	}

	TEST_F(CWorldMarketTest, TestDeleteTiingoStock) {
		// do nothing. 已经在TestAddStock中测试了DeleteStock函数
		CTiingoStockPtr pStock = nullptr;

		gl_dataContainerTiingoStock.Delete(pStock);

		pStock = make_shared<CTiingoStock>();
		pStock->SetSymbol("ABCDEF");
		gl_dataContainerTiingoStock.Delete(pStock);
	}

	TEST_F(CWorldMarketTest, TestIsForexExchange) {
		EXPECT_FALSE(gl_dataContainerFinnhubForexExchange.IsExchange("ABC"));
		EXPECT_TRUE(gl_dataContainerFinnhubForexExchange.IsExchange("forex.com"));
		EXPECT_TRUE(gl_dataContainerFinnhubForexExchange.IsExchange("ic markets"));
	}

	TEST_F(CWorldMarketTest, TestAddForexExchange) {
		const auto lTotalForexExchange = gl_dataContainerFinnhubForexExchange.Size();
		const string strForexExchange = "000001.SZ";

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
		EXPECT_FALSE(gl_dataContainerFinnhubForexExchange.Delete("US.US.US")) << "此符号在符号集中不存在";
	}

	TEST_F(CWorldMarketTest, TestIsForexSymbol) {
		EXPECT_FALSE(gl_dataFinnhubForexSymbol.IsSymbol("ABC"));
		EXPECT_TRUE(gl_dataFinnhubForexSymbol.IsSymbol("OANDA:XAU_SGD"));
		EXPECT_TRUE(gl_dataFinnhubForexSymbol.IsSymbol("FXCM:EUR/CHF"));

		const auto pForexSymbol = make_shared<CFinnhubForex>();
		pForexSymbol->SetSymbol("ABC");
		EXPECT_FALSE(gl_dataFinnhubForexSymbol.IsSymbol(pForexSymbol));
		pForexSymbol->SetSymbol("OANDA:XAU_SGD");
		EXPECT_TRUE(gl_dataFinnhubForexSymbol.IsSymbol(pForexSymbol));
		pForexSymbol->SetSymbol("FXCM:EUR/CHF");
		EXPECT_TRUE(gl_dataFinnhubForexSymbol.IsSymbol(pForexSymbol));
	}

	TEST_F(CWorldMarketTest, TestAddForexSymbol) {
		const auto pForexSymbol = make_shared<CFinnhubForex>();
		const auto lTotalForexSymbol = gl_dataFinnhubForexSymbol.Size();
		pForexSymbol->SetSymbol("000001.SZ");

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

		pForexSymbol = make_shared<CFinnhubForex>();
		pForexSymbol->SetSymbol("000001.SZ");
		gl_dataFinnhubForexSymbol.Delete(pForexSymbol); // "此符号在符号集中不存在";
	}

	TEST_F(CWorldMarketTest, TestIsCryptoExchange) {
		EXPECT_FALSE(gl_dataContainerFinnhubCryptoExchange.IsExchange("ABC"));
		EXPECT_TRUE(gl_dataContainerFinnhubCryptoExchange.IsExchange("BITFINEX"));
		EXPECT_TRUE(gl_dataContainerFinnhubCryptoExchange.IsExchange("HUOBI"));
	}

	TEST_F(CWorldMarketTest, TestAddCryptoExchange) {
		const auto lTotalCryptoExchange = gl_dataContainerFinnhubCryptoExchange.Size();
		const string strCryptoExchange = "000001.SZ";

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
		EXPECT_FALSE(gl_dataContainerFinnhubCryptoExchange.Delete("US.US.US")) << "此符号在符号集中不存在";
	}

	TEST_F(CWorldMarketTest, TestIsCryptoSymbol) {
		EXPECT_FALSE(gl_dataFinnhubCryptoSymbol.IsSymbol("ABC"));
		EXPECT_TRUE(gl_dataFinnhubCryptoSymbol.IsSymbol("POLONIEX:BTC_DOT"));
		EXPECT_TRUE(gl_dataFinnhubCryptoSymbol.IsSymbol("BINANCE:USDTUAH"));
		EXPECT_TRUE(gl_dataFinnhubCryptoSymbol.IsSymbol("COINBASE:TRIBE-USD"));

		const auto pCryptoSymbol = make_shared<CFinnhubCrypto>();
		pCryptoSymbol->SetSymbol("ABC");
		EXPECT_FALSE(gl_dataFinnhubCryptoSymbol.IsSymbol(pCryptoSymbol));
		pCryptoSymbol->SetSymbol("BINANCE:USDTUAH");
		EXPECT_TRUE(gl_dataFinnhubCryptoSymbol.IsSymbol(pCryptoSymbol));
		pCryptoSymbol->SetSymbol("COINBASE:TRIBE-USD");
		EXPECT_TRUE(gl_dataFinnhubCryptoSymbol.IsSymbol(pCryptoSymbol));
	}

	TEST_F(CWorldMarketTest, TestAddCryptoSymbol) {
		const auto pCryptoSymbol = make_shared<CFinnhubCrypto>();
		const auto lTotalCryptoSymbol = gl_dataFinnhubCryptoSymbol.Size();
		pCryptoSymbol->SetSymbol("000001.SZ");

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
		CFinnhubCryptoPtr pCryptoSymbol = nullptr;

		gl_dataFinnhubCryptoSymbol.Delete(pCryptoSymbol); // "空指针";

		pCryptoSymbol = make_shared<CFinnhubCrypto>();
		pCryptoSymbol->SetSymbol("000001.SZ");
		gl_dataFinnhubCryptoSymbol.Delete(pCryptoSymbol); // "此符号在符号集中不存在";
	}

	TEST_F(CWorldMarketTest, TestIsCountry) {
		const auto pCountry = make_shared<CCountry>();

		EXPECT_FALSE(gl_dataContainerFinnhubCountry.IsCountry("ABC"));
		EXPECT_TRUE(gl_dataContainerFinnhubCountry.IsCountry("American Samoa"));

		pCountry->m_strCountry = "ABC";
		EXPECT_FALSE(gl_dataContainerFinnhubCountry.IsCountry(pCountry));
		pCountry->m_strCountry = "American Samoa";
		EXPECT_TRUE(gl_dataContainerFinnhubCountry.IsCountry(pCountry));
	}

	TEST_F(CWorldMarketTest, TestAddCountry) {
		const auto pCountry = make_shared<CCountry>();
		const auto lTotalCountry = gl_dataContainerFinnhubCountry.GetTotalCountry();
		pCountry->m_strCountry = "SZ";

		EXPECT_FALSE(gl_dataContainerFinnhubCountry.IsCountry(pCountry));
		gl_dataContainerFinnhubCountry.Add(pCountry);
		EXPECT_TRUE(gl_dataContainerFinnhubCountry.IsCountry(pCountry));
		EXPECT_EQ(gl_dataContainerFinnhubCountry.GetTotalCountry(), lTotalCountry + 1);
		gl_dataContainerFinnhubCountry.Delete(pCountry);
		EXPECT_FALSE(gl_dataContainerFinnhubCountry.IsCountry(pCountry));
		EXPECT_EQ(gl_dataContainerFinnhubCountry.GetTotalCountry(), lTotalCountry);
	}

	TEST_F(CWorldMarketTest, TestLoadExchangeCode) {
		// not implemented
	}

	TEST_F(CWorldMarketTest, TestUpdateCountryDB) {
		const size_t lTotal = gl_dataContainerFinnhubCountry.GetTotalCountry();

		const auto pCountry = make_shared<CCountry>();
		pCountry->m_strCode2 = "AB";
		pCountry->m_strCountry = "NoName";
		EXPECT_FALSE(gl_dataContainerFinnhubCountry.IsCountry(pCountry));
		gl_dataContainerFinnhubCountry.Add(pCountry);
		EXPECT_EQ(gl_dataContainerFinnhubCountry.GetTotalCountry(), lTotal + 1);
		gl_dataContainerFinnhubCountry.UpdateDB(); // 此测试函数执行完后，新增了一个Country没有删除（数据库中的删除了）。

		CSetCountry setCountry;
		setCountry.m_strFilter = "[Country] = 'NoName'";
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
		auto pStock = make_shared<CFinnhubStock>();
		pStock->SetSymbol("SS.SS.US");
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsSymbol(pStock)); // 确保是一个新股票代码
		pStock->SetTodayNewStock(true);
		pStock->SetUpdateProfileDB(true);
		gl_dataContainerFinnhubStock.Add(pStock);
		pStock = gl_dataContainerFinnhubStock.GetItem("000001.SS");
		EXPECT_TRUE(pStock != nullptr);
		EXPECT_EQ(pStock->GetCurrency(), "");
		pStock->SetUpdateProfileDB(true);
		pStock->SetCurrency("No Currency"); // 更新这个条目

		try {
			gl_dataContainerFinnhubStock.UpdateProfileDB();
		} catch (std::exception& e) {
			EXPECT_TRUE(false) << e.what();
		}
		catch (CException&) {
		}
		catch (...) {
			EXPECT_TRUE(false) << "未知exception";
			ASSERT_FALSE(true);
		}

		CSetFinnhubStock setFinnhubStock;
		setFinnhubStock.m_strFilter = "[Symbol] = '000001.SS'";
		setFinnhubStock.Open();
		EXPECT_FALSE(setFinnhubStock.IsEOF());
		EXPECT_STREQ(setFinnhubStock.m_Currency, _T("No Currency")) << "此条目已更新";
		setFinnhubStock.m_pDatabase->BeginTrans();
		setFinnhubStock.Edit();
		setFinnhubStock.m_Currency = "";
		setFinnhubStock.Update();
		setFinnhubStock.m_pDatabase->CommitTrans();
		setFinnhubStock.Close();

		CSetFinnhubStock setFinnhubStock2;
		setFinnhubStock2.m_strFilter = "[Symbol] = 'SS.SS.US'";
		setFinnhubStock2.Open();
		EXPECT_FALSE(setFinnhubStock2.IsEOF());
		setFinnhubStock2.m_pDatabase->BeginTrans();
		while (!setFinnhubStock2.IsEOF()) {
			setFinnhubStock2.Delete();
			setFinnhubStock2.MoveNext();
		}
		setFinnhubStock2.m_pDatabase->CommitTrans();
		setFinnhubStock2.Close();

		// 恢复原状
		pStock = gl_dataContainerFinnhubStock.GetItem("SS.SS.US");
		EXPECT_TRUE(pStock != nullptr);
		gl_dataContainerFinnhubStock.Delete(pStock);
		pStock = gl_dataContainerFinnhubStock.GetItem("000001.SS");
		EXPECT_TRUE(pStock != nullptr);
		pStock->SetCurrency("");
		while (gl_systemMessage.InnerSystemInfoSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
		EXPECT_EQ(gl_dataContainerFinnhubStock.Size(), 4847);
	}

	TEST_F(CWorldMarketTest, TestUpdateDayLineDB) {
		EXPECT_TRUE(gl_pWorldMarket->UpdateFinnhubStockDayLineDB());

		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetItem(0)->IsUpdateDayLineDB()) << "此标识被重置";
		gl_dataContainerFinnhubStock.GetItem(0)->SetUpdateDayLineDB(true);

		EXPECT_TRUE(gl_pWorldMarket->UpdateFinnhubStockDayLineDB());
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			EXPECT_FALSE(gl_dataContainerFinnhubStock.GetItem(i)->IsUpdateDayLineDB()) << "此标识被重置";
		}
	}

	TEST_F(CWorldMarketTest, TestUpdateForexSymbolDB) {
		auto pForexSymbol = make_shared<CFinnhubForex>();
		pForexSymbol->SetSymbol("SS.SS.US"); // 新符号
		EXPECT_FALSE(gl_dataFinnhubForexSymbol.IsSymbol(pForexSymbol));
		gl_dataFinnhubForexSymbol.Add(pForexSymbol);
		pForexSymbol = gl_dataFinnhubForexSymbol.GetItem("OANDA:GBP_ZAR"); // 第二个现存的符号
		EXPECT_EQ(pForexSymbol->GetIPOStatus(), _STOCK_IPOED_);
		pForexSymbol->SetUpdateProfileDB(true);
		pForexSymbol->SetIPOStatus(_STOCK_DELISTED_);
		gl_dataFinnhubForexSymbol.UpdateDB();

		CSetFinnhubForexSymbol setCryptoSymbol;
		setCryptoSymbol.m_strFilter = "[Symbol] = 'OANDA:GBP_ZAR'";
		setCryptoSymbol.Open();
		EXPECT_EQ(setCryptoSymbol.m_IPOStatus, _STOCK_DELISTED_);
		setCryptoSymbol.m_pDatabase->BeginTrans();
		setCryptoSymbol.Edit();
		setCryptoSymbol.m_IPOStatus = _STOCK_IPOED_;
		setCryptoSymbol.Update();
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();

		setCryptoSymbol.m_strFilter = "[Symbol] = 'SS.SS.US'";
		setCryptoSymbol.Open();
		EXPECT_FALSE(setCryptoSymbol.IsEOF()) << "存入了新符号";
		setCryptoSymbol.m_pDatabase->BeginTrans();
		while (!setCryptoSymbol.IsEOF()) {
			setCryptoSymbol.Delete();
			setCryptoSymbol.MoveNext();
		}
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();

		// 恢复原状
		pForexSymbol->SetIPOStatus(_STOCK_IPOED_);
		pForexSymbol = gl_dataFinnhubForexSymbol.GetItem("SS.SS.US");
		EXPECT_TRUE(pForexSymbol != nullptr);
		gl_dataFinnhubForexSymbol.Delete(pForexSymbol);
	}

	TEST_F(CWorldMarketTest, TestUpdateFinnhubCryptoSymbolDB) {
		auto pCryptoSymbol = make_shared<CFinnhubCrypto>();
		pCryptoSymbol->SetSymbol("SS.SS.US"); // 新符号
		EXPECT_FALSE(gl_dataFinnhubCryptoSymbol.IsSymbol(pCryptoSymbol));
		gl_dataFinnhubCryptoSymbol.Add(pCryptoSymbol);
		pCryptoSymbol = gl_dataFinnhubCryptoSymbol.GetItem("BINANCE:USDTUAH"); // 第二个现存的符号
		EXPECT_EQ(pCryptoSymbol->GetIPOStatus(), _STOCK_IPOED_);
		pCryptoSymbol->SetUpdateProfileDB(true);
		pCryptoSymbol->SetIPOStatus(_STOCK_DELISTED_);
		gl_dataFinnhubCryptoSymbol.UpdateDB();

		CSetFinnhubCryptoSymbol setCryptoSymbol;
		setCryptoSymbol.m_strFilter = "[Symbol] = 'BINANCE:USDTUAH'";
		setCryptoSymbol.Open();
		EXPECT_EQ(setCryptoSymbol.m_IPOStatus, _STOCK_DELISTED_) << "状态已被修改为摘牌";
		setCryptoSymbol.m_pDatabase->BeginTrans();
		setCryptoSymbol.Edit();
		setCryptoSymbol.m_IPOStatus = _STOCK_IPOED_;
		setCryptoSymbol.Update();
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();

		setCryptoSymbol.m_strFilter = "[Symbol] = 'SS.SS.US'";
		setCryptoSymbol.Open();
		EXPECT_FALSE(setCryptoSymbol.IsEOF()) << "存入了新符号";
		setCryptoSymbol.m_pDatabase->BeginTrans();
		while (!setCryptoSymbol.IsEOF()) {
			setCryptoSymbol.Delete(); // 删除此新代码
			setCryptoSymbol.MoveNext();
		}
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();

		// 恢复原状
		pCryptoSymbol->SetIPOStatus(_STOCK_IPOED_);
		pCryptoSymbol = gl_dataFinnhubCryptoSymbol.GetItem("SS.SS.US");
		EXPECT_TRUE(pCryptoSymbol != nullptr);
		gl_dataFinnhubCryptoSymbol.Delete(pCryptoSymbol);
	}

	TEST_F(CWorldMarketTest, TestUpdateTiingoStockDB) {
		CSetTiingoStock setTiingoStock;
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsUpdateProfileDB());
		EXPECT_TRUE(gl_systemConfiguration.IsPaidTypeTiingoAccount()) << "函数UpdateProfile只运行在付费账户状态下";

		auto pTiingoStock = make_shared<CTiingoStock>(); // 这个是数据库中已存在的证券
		pTiingoStock->SetActive(true);
		pTiingoStock->SetIsADR(false);
		pTiingoStock->SetSicCode(1002);
		pTiingoStock->SetCompanyFinancialStatementUpdateDate(20210101);
		pTiingoStock->SetCompanyWebSite("");
		pTiingoStock->SetLocation("");
		pTiingoStock->SetName(""); //
		pTiingoStock->SetReportingCurrency("");
		pTiingoStock->SetSECFilingWebSite("");
		pTiingoStock->SetSicIndustry("");
		pTiingoStock->SetSicSector("Test"); // 用于删除
		pTiingoStock->SetSymbol("A"); // 已存在代码
		pTiingoStock->SetTiingoIndustry("");
		pTiingoStock->SetTiingoPermaTicker("");
		pTiingoStock->SetTiingoSector("");
		pTiingoStock->SetUpdateProfileDB(true);
		gl_dataContainerTiingoStock.UpdateProfile(pTiingoStock);

		pTiingoStock = make_shared<CTiingoStock>();
		pTiingoStock->SetActive(true);
		pTiingoStock->SetIsADR(false);
		pTiingoStock->SetSicCode(1002);
		pTiingoStock->SetCompanyFinancialStatementUpdateDate(20210101);
		pTiingoStock->SetCompanyWebSite("www.abc.com");
		pTiingoStock->SetLocation("Irvine CA USA");
		pTiingoStock->SetName("ABCDE"); // 新代码
		pTiingoStock->SetReportingCurrency("US Dollar");
		pTiingoStock->SetSECFilingWebSite("abc");
		pTiingoStock->SetSicIndustry("Computer Science");
		pTiingoStock->SetSicSector("Test");
		pTiingoStock->SetSymbol("ABCDEF"); // 新代码
		pTiingoStock->SetTiingoIndustry("Computer");
		pTiingoStock->SetTiingoPermaTicker("abcdefg");
		pTiingoStock->SetTiingoSector("gfedcba");
		pTiingoStock->SetUpdateProfileDB(true);
		gl_dataContainerTiingoStock.Add(pTiingoStock);

		EXPECT_TRUE(gl_dataContainerTiingoStock.IsUpdateProfileDB()) << "添加了两个股票";

		gl_dataContainerTiingoStock.UpdateDB(); // 更新代码集

		// 恢复原状
		setTiingoStock.m_strFilter = "[SICSector] = 'Test'";
		setTiingoStock.m_strSort = "[Ticker]";
		setTiingoStock.Open();
		EXPECT_FALSE(setTiingoStock.IsEOF()) << "存入了两股票代码";
		setTiingoStock.m_pDatabase->BeginTrans();
		EXPECT_STREQ(setTiingoStock.m_Ticker, _T("A")) << "已存在代码";
		EXPECT_EQ(setTiingoStock.m_SicCode, 1002);
		setTiingoStock.Edit();
		setTiingoStock.m_SicSector = "";
		setTiingoStock.m_SicCode = 0;
		setTiingoStock.Update();
		setTiingoStock.MoveNext();
		EXPECT_STREQ(setTiingoStock.m_Ticker, _T("ABCDEF")) << "新代码";
		setTiingoStock.Delete();
		setTiingoStock.MoveNext();
		EXPECT_TRUE(setTiingoStock.IsEOF());
		setTiingoStock.m_pDatabase->CommitTrans();
		setTiingoStock.Close();

		gl_dataContainerTiingoStock.Delete(pTiingoStock);
	}

	TEST_F(CWorldMarketTest, TestUpdateForexExchangeDB) {
		const string strSymbol = "US.US.US";

		EXPECT_FALSE(gl_dataContainerFinnhubForexExchange.IsNeedUpdate());
		EXPECT_FALSE(gl_dataContainerFinnhubForexExchange.UpdateDB()) << "没有新Forex Exchange";

		EXPECT_FALSE(gl_dataContainerFinnhubForexExchange.IsExchange(strSymbol)); // 确保是一个新股票代码
		gl_dataContainerFinnhubForexExchange.Add(strSymbol);
		EXPECT_TRUE(gl_dataContainerFinnhubForexExchange.IsNeedUpdate());
		EXPECT_TRUE(gl_dataContainerFinnhubForexExchange.UpdateDB());

		CSetFinnhubForexExchange setForexExchange;
		setForexExchange.m_strFilter = "[Code] = 'US.US.US'";
		setForexExchange.Open();
		EXPECT_FALSE(setForexExchange.IsEOF());
		setForexExchange.m_pDatabase->BeginTrans();
		while (!setForexExchange.IsEOF()) {
			setForexExchange.Delete();
			setForexExchange.MoveNext();
		}
		setForexExchange.m_pDatabase->CommitTrans();
		setForexExchange.Close();

		// 恢复原状
		gl_dataContainerFinnhubForexExchange.Delete(strSymbol);
		gl_dataContainerFinnhubForexExchange.SetLastSize(gl_dataContainerFinnhubForexExchange.Size());
		EXPECT_FALSE(gl_dataContainerFinnhubForexExchange.IsNeedUpdate());
	}

	TEST_F(CWorldMarketTest, TestUpdateCryptoExchangeDB) {
		const string sSymbol = "US.US.US";

		EXPECT_FALSE(gl_dataContainerFinnhubCryptoExchange.IsNeedUpdate());
		EXPECT_FALSE(gl_dataContainerFinnhubCryptoExchange.UpdateDB()) << "没有新Crypto Exchange";

		EXPECT_FALSE(gl_dataContainerFinnhubCryptoExchange.IsExchange(sSymbol)); // 确保是一个新Crypto代码
		gl_dataContainerFinnhubCryptoExchange.Add(sSymbol);
		EXPECT_TRUE(gl_dataContainerFinnhubCryptoExchange.IsNeedUpdate());
		EXPECT_TRUE(gl_dataContainerFinnhubCryptoExchange.UpdateDB());

		CSetFinnhubCryptoExchange setCryptoExchange;
		setCryptoExchange.m_strFilter = "[Code] = 'US.US.US'";
		setCryptoExchange.Open();
		EXPECT_FALSE(setCryptoExchange.IsEOF());
		setCryptoExchange.m_pDatabase->BeginTrans();
		while (!setCryptoExchange.IsEOF()) {
			setCryptoExchange.Delete(); // 删除新添加的这个代码
			setCryptoExchange.MoveNext();
		}
		setCryptoExchange.m_pDatabase->CommitTrans();
		setCryptoExchange.Close();

		// 恢复原状
		gl_dataContainerFinnhubCryptoExchange.Delete(sSymbol);
		gl_dataContainerFinnhubCryptoExchange.SetLastSize(gl_dataContainerFinnhubCryptoExchange.Size());
		EXPECT_FALSE(gl_dataContainerFinnhubCryptoExchange.IsNeedUpdate());
	}

	TEST_F(CWorldMarketTest, TaskUpdateInsiderTransactionDB) {
		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetItem("A")->HaveInsiderTransaction());
		EXPECT_EQ(gl_dataContainerFinnhubStock.GetItem("A")->GetInsiderTransactionUpdateDate(), 19800101);
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 0);

		vector<CInsiderTransactionPtr> vInsiderTransaction;
		CSetInsiderTransaction setInsiderTransaction;

		CInsiderTransactionPtr pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = "B";
		pInsiderTransaction->m_strPersonName = "a b c";
		pInsiderTransaction->m_lTransactionDate = 20200101; // 这个股票代码不符，需要添加进数据库
		vInsiderTransaction.push_back(pInsiderTransaction);
		pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = "A";
		pInsiderTransaction->m_strPersonName = "a b c d";
		pInsiderTransaction->m_lTransactionDate = 20210101; // 这个内部交易人员名称不符，需要添加进数据库
		vInsiderTransaction.push_back(pInsiderTransaction);
		pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = "A";
		pInsiderTransaction->m_strPersonName = "a b c";
		pInsiderTransaction->m_lTransactionDate = 20210107;
		pInsiderTransaction->m_strTransactionCode = "M"; // 这个数据库中有，无需添加
		vInsiderTransaction.push_back(pInsiderTransaction);
		pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = "A";
		pInsiderTransaction->m_strPersonName = "a b c";
		pInsiderTransaction->m_lTransactionDate = 20210124; // 这个日期不符，需要添加进数据库
		vInsiderTransaction.push_back(pInsiderTransaction);
		pInsiderTransaction->m_strSymbol = "A";
		pInsiderTransaction->m_strPersonName = "a b c";
		pInsiderTransaction->m_strTransactionCode = "S"; // 这个交易类型不符，需要添加进数据库
		vInsiderTransaction.push_back(pInsiderTransaction);

		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem("A");
		EXPECT_FALSE(pStock->HaveInsiderTransaction()) << "此时尚未存入数据";

		pStock->SetUpdateInsiderTransactionDB(true);
		pStock->SetInsiderTransactionUpdateDate(20210123);
		pStock->UpdateInsiderTransaction(vInsiderTransaction);

		gl_pWorldMarket->UpdateInsiderTransactionDB();

		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 0);
		//gl_systemMessage.PopDayLineInfoMessage();
		auto pStock2 = gl_dataContainerFinnhubStock.GetItem("A");
		EXPECT_FALSE(pStock2->IsUpdateInsiderTransactionDB());
		EXPECT_TRUE(pStock2->HaveInsiderTransaction()) << "存储后并没有删除数据";
		pStock2->UnloadInsiderTransaction();

		// 验证并恢复原状
		setInsiderTransaction.m_strFilter = "[Symbol] = 'B'";
		setInsiderTransaction.Open();
		setInsiderTransaction.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderTransaction.IsEOF());
		setInsiderTransaction.Delete();
		setInsiderTransaction.m_pDatabase->CommitTrans();
		setInsiderTransaction.Close();

		setInsiderTransaction.m_strFilter = "[PersonName] = 'a b c d'";
		setInsiderTransaction.Open();
		setInsiderTransaction.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderTransaction.IsEOF());
		setInsiderTransaction.Delete();
		setInsiderTransaction.m_pDatabase->CommitTrans();
		setInsiderTransaction.Close();

		setInsiderTransaction.m_strFilter = "[TransactionDate] = '20210124'";
		setInsiderTransaction.Open();
		setInsiderTransaction.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderTransaction.IsEOF());
		setInsiderTransaction.Delete();
		setInsiderTransaction.m_pDatabase->CommitTrans();
		setInsiderTransaction.Close();

		setInsiderTransaction.m_strFilter = "[TransactionCode] = 'S'";
		setInsiderTransaction.Open();
		setInsiderTransaction.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderTransaction.IsEOF());
		setInsiderTransaction.Delete();
		setInsiderTransaction.m_pDatabase->CommitTrans();
		setInsiderTransaction.Close();

		pStock->SetInsiderTransactionUpdateDate(19800101);
	}

	TEST_F(CWorldMarketTest, TaskUpdateInsiderSentimentDB) {
		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetItem("A")->HaveInsiderSentiment());
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 0);

		vector<CInsiderSentimentPtr> vInsiderSentiment;
		CSetInsiderSentiment setInsiderSentiment;

		CInsiderSentimentPtr pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment->m_strSymbol = "B";// 这个股票代码不符，需要添加进数据库
		pInsiderSentiment->m_lDate = 20200101;
		vInsiderSentiment.push_back(pInsiderSentiment);
		pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment->m_strSymbol = "A";
		pInsiderSentiment->m_lDate = 20200101;// 这个数据库中有，无需添加
		vInsiderSentiment.push_back(pInsiderSentiment);
		pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment->m_strSymbol = "A";
		pInsiderSentiment->m_lDate = 20210101; // 这个日期不符，需要添加进数据库
		vInsiderSentiment.push_back(pInsiderSentiment);

		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem("A");
		EXPECT_FALSE(pStock->HaveInsiderSentiment()) << "此时尚未存入数据";

		pStock->SetUpdateInsiderSentimentDB(true);
		pStock->SetInsiderSentimentUpdateDate(20210101);
		pStock->UpdateInsiderSentiment(vInsiderSentiment);

		EXPECT_TRUE(gl_pWorldMarket->UpdateInsiderSentimentDB());

		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 0);
		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetItem("A")->IsUpdateInsiderSentimentDB());
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetItem("A")->HaveInsiderSentiment()) << "存储后并没有删除数据";

		// 验证并恢复原状
		setInsiderSentiment.m_strFilter = "[Symbol] = 'B'";
		setInsiderSentiment.Open();
		EXPECT_TRUE(setInsiderSentiment.IsEOF());
		setInsiderSentiment.Close();

		setInsiderSentiment.m_strFilter = "[Date] = '20210101'";
		setInsiderSentiment.Open();
		setInsiderSentiment.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderSentiment.IsEOF());
		setInsiderSentiment.Delete();
		setInsiderSentiment.m_pDatabase->CommitTrans();
		setInsiderSentiment.Close();

		gl_dataContainerFinnhubStock.GetItem("A")->UnloadInsiderSentiment();
	}

	TEST_F(CWorldMarketTest, TestUpdateEconomicCalendarDB) {
		CSetEconomicCalendar setEconomicCalendar;
		const auto pEconomicCalendar = make_shared<CEconomicCalendar>();
		vector<CEconomicCalendarPtr> vEconomicCalendar;

		pEconomicCalendar->m_strCountry = "USA";
		pEconomicCalendar->m_strTime = "20200101";
		pEconomicCalendar->m_strEvent = "abc";
		pEconomicCalendar->m_dActual = 1.0;
		pEconomicCalendar->m_dEstimate = 2.0;
		pEconomicCalendar->m_dPrev = 3.0;
		pEconomicCalendar->m_strImpact = "s";
		pEconomicCalendar->m_strUnit = "USD";

		vEconomicCalendar.push_back(pEconomicCalendar);

		EXPECT_TRUE(gl_dataContainerFinnhubEconomicCalendar.Update(vEconomicCalendar));
		EXPECT_TRUE(gl_dataContainerFinnhubEconomicCalendar.UpdateDB());

		// 测试并恢复原状
		setEconomicCalendar.Open();
		EXPECT_FALSE(setEconomicCalendar.IsEOF());
		EXPECT_STREQ(setEconomicCalendar.m_Country, _T("USA"));
		EXPECT_STREQ(setEconomicCalendar.m_Time, _T("20200101"));
		EXPECT_STREQ(setEconomicCalendar.m_Event, _T("abc"));
		EXPECT_DOUBLE_EQ(_tstof(setEconomicCalendar.m_Actual), 1.0);
		EXPECT_DOUBLE_EQ(_tstof(setEconomicCalendar.m_Estimate), 2.0);
		EXPECT_DOUBLE_EQ(_tstof(setEconomicCalendar.m_Prev), 3.0);
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
		CFinnhubStockPtr pStock;
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(i);
			pStock->SetLastEPSSurpriseUpdateDate(20200101);
			pStock->m_fUpdateEPSSurprise = false;
		}
		gl_pFinnhubDataSource->SetUpdateEPSSurprise(false);

		gl_pWorldMarket->RebuildEPSSurprise();

		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(i);
			EXPECT_EQ(pStock->GetLastEPSSurpriseUpdateDate(), 19800101);
			EXPECT_TRUE(pStock->m_fUpdateEPSSurprise);
		}
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateEPSSurprise());
	}

	TEST_F(CWorldMarketTest, TestRebuildPeer) {
		CFinnhubStockPtr pStock;
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(i);
			pStock->SetPeerUpdateDate(20200101);
			pStock->SetUpdatePeer(false);
			pStock->SetUpdateProfileDB(false);
		}
		gl_pFinnhubDataSource->SetUpdatePeer(false);

		gl_pWorldMarket->RebuildPeer();

		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(i);
			EXPECT_EQ(pStock->GetPeerUpdateDate(), 19800101);
			EXPECT_TRUE(pStock->IsUpdatePeer());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
		}
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdatePeer());

		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(i);
			pStock->SetUpdatePeer(true);
			pStock->SetUpdateProfileDB(false);
		}
	}

	TEST_F(CWorldMarketTest, TestRebuildStockDayLine) {
		CFinnhubStockPtr pStock;
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(i);
			pStock->SetIPOStatus(_STOCK_IPOED_);
			pStock->SetDayLineStartDate(20200101);
			pStock->SetDayLineEndDate(20200101);
			pStock->SetUpdateDayLine(false);
			pStock->SetUpdateProfileDB(false);
		}
		gl_pFinnhubDataSource->SetUpdateStockProfile(false);

		gl_pWorldMarket->RebuildStockDayLineDB();

		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(i);
			EXPECT_EQ(pStock->GetDayLineStartDate(), 29900101);
			EXPECT_EQ(pStock->GetDayLineEndDate(), 19800101);
			EXPECT_TRUE(pStock->IsUpdateDayLine());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
		}
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateStockProfile());

		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(i);
			pStock->SetUpdateProfileDB(false);
		}
	}

	TEST_F(CWorldMarketTest, TestUpdateTiingoIndustry) {
		EXPECT_THAT(gl_pWorldMarket->UpdateTiingoIndustry(), IsFalse());
	}

	TEST_F(CWorldMarketTest, TestUpdateSicIndustry) {
		EXPECT_THAT(gl_pWorldMarket->UpdateSicIndustry(), IsFalse());
	}

	TEST_F(CWorldMarketTest, TestUpdateNaicsIndustry) {
		EXPECT_THAT(gl_pWorldMarket->UpdateNaicsIndustry(), IsFalse());
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
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_TIINGO_INQUIRE_DAYlINE__);
		EXPECT_EQ(pTask->GetTime(), 10030);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_UPDATE_DB__);
		EXPECT_EQ(pTask->GetTime(), 10040);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_TIINGO_INQUIRE_IEX_TOP_OF_BOOK__);
		EXPECT_EQ(pTask->GetTime(), 10100);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__);
		EXPECT_EQ(pTask->GetTime(), 10200);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_CALCULATE_NASDAQ100_200MA_UPDOWN_RATE);
		EXPECT_EQ(pTask->GetTime(), 10300);
		gl_pWorldMarket->DiscardCurrentMarketTask();

#ifndef _DEBUG
		//Note 为了方便调试，测试版不再添加以下任务。发行版依然添加。
		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_TIINGO_PROCESS_DAYLINE__);
		EXPECT_EQ(pTask->GetTime(), 10500);
		gl_pWorldMarket->DiscardCurrentMarketTask();
#endif

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_RESET__);
		EXPECT_EQ(pTask->GetTime(), gl_pWorldMarket->GetResetTime());
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_CREATE_TASK__);
		EXPECT_EQ(pTask->GetTime(), 240000);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());
	}

	TEST_F(CWorldMarketTest, TestTaskCreateTask2) {
		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());

		gl_pWorldMarket->TaskCreateTask(gl_pWorldMarket->GetResetTime() + 110);

		EXPECT_FALSE(gl_pWorldMarket->IsMarketTaskEmpty());
		auto pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__);
		EXPECT_EQ(pTask->GetTime(), gl_pWorldMarket->GetResetTime() + 110);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_TIINGO_INQUIRE_DAYlINE__);
		EXPECT_EQ(pTask->GetTime(), gl_pWorldMarket->GetResetTime() + 130);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_UPDATE_DB__);
		EXPECT_EQ(pTask->GetTime(), gl_pWorldMarket->GetResetTime() + 140);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_TIINGO_INQUIRE_IEX_TOP_OF_BOOK__);
		EXPECT_EQ(pTask->GetTime(), gl_pWorldMarket->GetResetTime() + 200);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__);
		EXPECT_EQ(pTask->GetTime(), gl_pWorldMarket->GetResetTime() + 300);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_CALCULATE_NASDAQ100_200MA_UPDOWN_RATE);
		EXPECT_EQ(pTask->GetTime(), gl_pWorldMarket->GetResetTime() + 400);
		gl_pWorldMarket->DiscardCurrentMarketTask();

#ifndef _DEBUG
		//Note 为了方便调试，测试版不再添加以下任务。发行版依然添加。
		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_TIINGO_PROCESS_DAYLINE__);
		EXPECT_EQ(pTask->GetTime(), gl_pWorldMarket->GetResetTime() + 600);
		gl_pWorldMarket->DiscardCurrentMarketTask();
#endif

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_CREATE_TASK__);
		EXPECT_EQ(pTask->GetTime(), 240000);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());
	}

	TEST_F(CWorldMarketTest, TestTaskProcessWebSocketData1) {
		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());

		gl_pWorldMarket->TaskProcessWebSocketData(GetPrevTime(gl_pWorldMarket->GetResetTime(), 0, 2, 1));

		EXPECT_FALSE(gl_pWorldMarket->IsMarketTaskEmpty());
		const auto pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__);
		EXPECT_EQ(pTask->GetTime(), GetNextTime(gl_pWorldMarket->GetResetTime(), 0, 5, 1));
		gl_pWorldMarket->DiscardCurrentMarketTask();

		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());
	}

	TEST_F(CWorldMarketTest, TestTaskProcessWebSocketData2) {
		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());

		gl_pWorldMarket->TaskProcessWebSocketData(GetPrevTime(gl_pWorldMarket->GetResetTime(), 0, 2, 1));

		EXPECT_FALSE(gl_pWorldMarket->IsMarketTaskEmpty());
		const auto pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__);
		EXPECT_EQ(pTask->GetTime(), GetNextTime(gl_pWorldMarket->GetResetTime(), 0, 5, 1));
		gl_pWorldMarket->DiscardCurrentMarketTask();

		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());
	}

	TEST_F(CWorldMarketTest, TestGetFinnhubWebSocketSymbols) {
		const vectorString vString = gl_pWorldMarket->GetFinnhubWebSocketSymbols();

		EXPECT_EQ(vString.at(0), "A");
		EXPECT_EQ(vString.at(1), "AA");
		EXPECT_EQ(vString.at(2), "AAL");
		EXPECT_EQ(vString.at(3), "AAPL");

		EXPECT_EQ(vString.at(4), "FXCM:USD/JPY");
		EXPECT_EQ(vString.at(5), "IC MARKETS:1");
		EXPECT_EQ(vString.at(6), "OANDA:AUD_SGD");

		EXPECT_EQ(vString.at(7), "BINANCE:IDEXBUSD");
		EXPECT_EQ(vString.at(8), "BINANCE:MDAETH");
		EXPECT_EQ(vString.at(9), "BINANCE:OCEANBTC");
	}

	TEST_F(CWorldMarketTest, TestSetTiingoStockDayLineUpdated) {
		EXPECT_EQ(gl_pWorldMarket->GetTiingoStockDayLineUpdated(), 0);
		gl_pWorldMarket->SetTiingoStockDayLineUpdated(100);
		EXPECT_EQ(gl_pWorldMarket->GetTiingoStockDayLineUpdated(), 100);
		gl_pWorldMarket->SetTiingoStockDayLineUpdated(0);
		EXPECT_EQ(gl_pWorldMarket->GetTiingoStockDayLineUpdated(), 0);
	}
}
