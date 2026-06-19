#include"pch.h"

#include"GeneralCheck.h"

#include"FinnhubStock.h"
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
		EXPECT_FALSE(gl_pWorldMarket->IsTimeToResetSystem(GetPrevTime(gl_pWorldMarket->GetResetTime(), 0h, 2min, 1s)));
		EXPECT_TRUE(gl_pWorldMarket->IsTimeToResetSystem(GetPrevTime(gl_pWorldMarket->GetResetTime(), 0h, 2min, 0s)));
		EXPECT_TRUE(gl_pWorldMarket->IsTimeToResetSystem(GetPrevTime(gl_pWorldMarket->GetResetTime(), 0h, 1min, 59s)));
		EXPECT_TRUE(gl_pWorldMarket->IsTimeToResetSystem(GetNextTime(gl_pWorldMarket->GetResetTime(), 0h, 4min, 59s)));
		EXPECT_TRUE(gl_pWorldMarket->IsTimeToResetSystem(GetNextTime(gl_pWorldMarket->GetResetTime(), 0h, 5min, 0s)));
		EXPECT_FALSE(gl_pWorldMarket->IsTimeToResetSystem(GetNextTime(gl_pWorldMarket->GetResetTime(), 0h, 5min, 1s)));
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
		for (size_t i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
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
		EXPECT_FALSE(gl_dataContainerFinnhubForexExchange.Delete("Test")) << "此符号在符号集中不存在";
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
		EXPECT_FALSE(gl_dataContainerFinnhubCryptoExchange.Delete("Test")) << "此符号在符号集中不存在";
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
		CCountry country;

		EXPECT_FALSE(gl_dataContainerFinnhubCountry.IsCountry("ABC"));
		EXPECT_TRUE(gl_dataContainerFinnhubCountry.IsCountry("American Samoa"));

		country.m_strCountry = "ABC";
		EXPECT_FALSE(gl_dataContainerFinnhubCountry.IsCountry(country));
		country.m_strCountry = "American Samoa";
		EXPECT_TRUE(gl_dataContainerFinnhubCountry.IsCountry(country));
	}

	TEST_F(CWorldMarketTest, TestAddCountry) {
		CCountry country;
		const auto lTotalCountry = gl_dataContainerFinnhubCountry.GetTotalCountry();
		country.m_strCountry = "SZ";

		EXPECT_FALSE(gl_dataContainerFinnhubCountry.IsCountry(country));
		gl_dataContainerFinnhubCountry.Add(country);
		EXPECT_TRUE(gl_dataContainerFinnhubCountry.IsCountry(country));
		EXPECT_EQ(gl_dataContainerFinnhubCountry.GetTotalCountry(), lTotalCountry + 1);
		gl_dataContainerFinnhubCountry.Delete(country);
		EXPECT_FALSE(gl_dataContainerFinnhubCountry.IsCountry(country));
		EXPECT_EQ(gl_dataContainerFinnhubCountry.GetTotalCountry(), lTotalCountry);
	}

	TEST_F(CWorldMarketTest, TestUpdateCountryDB) {
		const size_t lTotal = gl_dataContainerFinnhubCountry.GetTotalCountry();

		CCountry country;
		country.m_strCode2 = "AB";
		country.m_strCountry = "NoName";
		EXPECT_FALSE(gl_dataContainerFinnhubCountry.IsCountry(country));
		gl_dataContainerFinnhubCountry.Add(country);
		EXPECT_EQ(gl_dataContainerFinnhubCountry.GetTotalCountry(), lTotal + 1);
		gl_dataContainerFinnhubCountry.UpdateDB(); // 此测试函数执行完后，新增了一个Country没有删除（数据库中的删除了）。

		using namespace StockMarket;
		const auto& t = FinnhubCountryList{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		db(remove_from(t).where(t.Code2 == std::string("AB")));
		tx.commit();
	}

	TEST_F(CWorldMarketTest, TestUpdateStockProfileDB) {
		auto pStock = make_shared<CFinnhubStock>();
		pStock->SetSymbol("SS.SS.US");
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsSymbol(pStock)); // 确保是一个新股票代码
		pStock->SetNewStock(true);
		pStock->SetCurrency("No Currency");
		pStock->SetUpdateProfileDB(true);
		gl_dataContainerFinnhubStock.Add(pStock);
		pStock = gl_dataContainerFinnhubStock.GetItem("000001.SS");
		EXPECT_TRUE(pStock != nullptr);
		EXPECT_EQ(pStock->GetCurrency(), "CNY");
		pStock->SetCurrency("Currency"); // 更新这个条目
		pStock->SetUpdateProfileDB(true);

		try {
			gl_dataContainerFinnhubStock.UpdateProfileDB();
		} catch (std::exception& e) {
			EXPECT_TRUE(false) << e.what();
		} catch (CException& e) {
			EXPECT_TRUE(false);
		} catch (...) {
			EXPECT_TRUE(false) << "未知exception";
			ASSERT_FALSE(true);
		}

		using namespace StockMarket;
		const auto& t = FinnhubStockProfile{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);

		auto result = db(select(all_of(t)).from(t).where(t.Symbol == std::string("000001.SS")));
		size_t rows = result.size();
		EXPECT_EQ(rows, 1);
		auto& row = result.front();
		string str = row.Currency;
		EXPECT_STREQ(str.c_str(), "Currency") << "此条目已更新";

		db(update(t).set(t.Currency = std::string("CNY")).where(t.Symbol == std::string("000001.SS")));

		auto result2 = db(select(all_of(t)).from(t).where(t.Symbol == std::string("SS.SS.US")));
		size_t rows2 = result2.size();
		EXPECT_EQ(rows2, 1);
		auto& row2 = result2.front();
		string str2 = row2.Currency;
		EXPECT_STREQ(str2.c_str(), "No Currency") << "此条目已更新";

		db(sqlpp::remove_from(t).where(t.Symbol == std::string("SS.SS.US")));
		tx.commit();

		// 恢复原状
		pStock = gl_dataContainerFinnhubStock.GetItem("SS.SS.US");
		EXPECT_TRUE(pStock != nullptr);
		gl_dataContainerFinnhubStock.Delete(pStock);
		pStock = gl_dataContainerFinnhubStock.GetItem("000001.SS");
		EXPECT_TRUE(pStock != nullptr);
		pStock->SetCurrency("CNY");
		while (gl_systemMessage.InnerSystemInfoSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
		EXPECT_EQ(gl_dataContainerFinnhubStock.Size(), 4847);
	}

	TEST_F(CWorldMarketTest, TestUpdateDayLineDB) {
		EXPECT_TRUE(gl_pWorldMarket->UpdateFinnhubStockDayLineDB());

		EXPECT_FALSE(gl_dataContainerFinnhubStock.GetItem(0)->IsUpdateDayLineDB()) << "此标识被重置";
		gl_dataContainerFinnhubStock.GetItem(0)->SetUpdateDayLineDB(true);

		EXPECT_TRUE(gl_pWorldMarket->UpdateFinnhubStockDayLineDB());
		for (size_t i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			EXPECT_FALSE(gl_dataContainerFinnhubStock.GetItem(i)->IsUpdateDayLineDB()) << "此标识被重置";
		}
	}

	TEST_F(CWorldMarketTest, TestUpdateForexSymbolDB) {
		auto pForexSymbol = make_shared<CFinnhubForex>();
		pForexSymbol->SetSymbol("Test"); // 新符号
		pForexSymbol->SetExchange("Test");
		pForexSymbol->SetUpdateProfileDB(true);
		pForexSymbol->SetNewStock(true);
		EXPECT_FALSE(gl_dataFinnhubForexSymbol.IsSymbol(pForexSymbol));
		gl_dataFinnhubForexSymbol.Add(pForexSymbol);
		pForexSymbol = gl_dataFinnhubForexSymbol.GetItem("OANDA:GBP_ZAR"); // 第二个现存的符号
		auto exchange = pForexSymbol->GetExchange();
		pForexSymbol->SetExchange("CN");
		pForexSymbol->SetUpdateProfileDB(true);

		gl_dataFinnhubForexSymbol.UpdateProfileDB();

		using namespace StockMarket;
		const auto& t = FinnhubForexSymbol{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);

		auto result2 = db(select(all_of(t)).from(t).where(t.Symbol == std::string("OANDA:GBP_ZAR")));
		size_t rows2 = result2.size();
		EXPECT_EQ(rows2, 1);
		auto& row2 = result2.front();
		EXPECT_EQ(row2.Exchange.value(), "CN");
		db(update(t).set(t.Exchange = exchange).where(t.Symbol == std::string("OANDA:GBP_ZAR")));

		auto result = db(select(all_of(t)).from(t).where(t.Symbol == std::string("Test")));
		size_t rows = result.size();
		EXPECT_EQ(rows, 1) << "存入了新符号";
		db(sqlpp::remove_from(t).where(t.Symbol == std::string("Test")));
		tx.commit();

		// 恢复原状
		pForexSymbol = gl_dataFinnhubForexSymbol.GetItem("Test");
		EXPECT_TRUE(pForexSymbol != nullptr);
		gl_dataFinnhubForexSymbol.Delete(pForexSymbol);
	}

	TEST_F(CWorldMarketTest, TestUpdateFinnhubCryptoSymbolDB) {
		auto pCryptoSymbol = make_shared<CFinnhubCrypto>();
		pCryptoSymbol->SetSymbol("Test"); // 新符号
		pCryptoSymbol->SetExchange("Test");
		pCryptoSymbol->SetUpdateProfileDB(true);
		pCryptoSymbol->SetNewStock(true);
		EXPECT_FALSE(gl_dataFinnhubCryptoSymbol.IsSymbol(pCryptoSymbol));
		gl_dataFinnhubCryptoSymbol.Add(pCryptoSymbol);
		pCryptoSymbol = gl_dataFinnhubCryptoSymbol.GetItem("BINANCE:USDTUAH"); // 第二个现存的符号
		auto exchange = pCryptoSymbol->GetExchange();
		pCryptoSymbol->SetExchange("CN");
		pCryptoSymbol->SetUpdateProfileDB(true);

		gl_dataFinnhubCryptoSymbol.UpdateProfileDB();

		using namespace StockMarket;
		const auto& t = FinnhubCryptoSymbol{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);

		auto result2 = db(select(all_of(t)).from(t).where(t.Symbol == std::string("BINANCE:USDTUAH")));
		size_t rows2 = result2.size();
		EXPECT_EQ(rows2, 1);
		auto& row2 = result2.front();
		EXPECT_EQ(row2.Exchange.value(), "CN");
		db(update(t).set(t.Exchange = exchange).where(t.Symbol == std::string("BINANCE:USDTUAH")));

		auto result = db(select(all_of(t)).from(t).where(t.Symbol == std::string("Test")));
		size_t rows = result.size();
		EXPECT_EQ(rows, 1) << "存入了新符号";
		db(sqlpp::remove_from(t).where(t.Symbol == std::string("Test")));
		tx.commit();

		// 恢复原状
		pCryptoSymbol = gl_dataFinnhubCryptoSymbol.GetItem("Test");
		EXPECT_TRUE(pCryptoSymbol != nullptr);
		gl_dataFinnhubCryptoSymbol.Delete(pCryptoSymbol);
	}

	TEST_F(CWorldMarketTest, TestUpdateTiingoStockDB) {
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsUpdateProfileDB());
		EXPECT_TRUE(gl_systemConfiguration.IsPaidTypeTiingoAccount()) << "函数UpdateProfile只运行在付费账户状态下";

		auto pTiingoStock = make_shared<CTiingoStock>(); // 这个是数据库中已存在的证券
		pTiingoStock->SetActive(true);
		pTiingoStock->SetIsADR(false);
		pTiingoStock->SetSicCode(1002);
		pTiingoStock->SetCompanyFinancialStatementUpdateDate(toLocalDays(20210101));
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
		pTiingoStock->SetCompanyFinancialStatementUpdateDate(toLocalDays(20210101));
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

		EXPECT_TRUE(gl_dataContainerTiingoStock.IsUpdateProfileDB()) << "更新了两个股票";

		gl_dataContainerTiingoStock.UpdateProfileDB(); // 更新代码集

		// 恢复原状
		using namespace StockMarket;
		const auto& t = TiingoStockProfile{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		auto result = db(select(all_of(t)).from(t).order_by(t.Symbol.asc()).where(t.SICSector == std::string("Test")));
		int rows = result.size();
		EXPECT_EQ(rows, 2);
		EXPECT_EQ(result.front().Symbol, "A");
		result.pop_front();
		EXPECT_EQ(result.front().Symbol, "ABCDEF");

		db(update(t).set(t.SICSector = std::string("")).where(t.Symbol == std::string("A")));
		db(remove_from(t).where(t.Symbol == std::string("ABCDEF")));
		tx.commit();

		gl_dataContainerTiingoStock.Delete(pTiingoStock);
	}

	TEST_F(CWorldMarketTest, TestUpdateForexExchangeDB) {
		const string strSymbol = "Test";

		EXPECT_FALSE(gl_dataContainerFinnhubForexExchange.IsNeedUpdate());
		EXPECT_FALSE(gl_dataContainerFinnhubForexExchange.UpdateDB()) << "没有新Forex Exchange";

		EXPECT_FALSE(gl_dataContainerFinnhubForexExchange.IsExchange(strSymbol)); // 确保是一个新股票代码
		gl_dataContainerFinnhubForexExchange.Add(strSymbol);
		EXPECT_TRUE(gl_dataContainerFinnhubForexExchange.IsNeedUpdate());
		EXPECT_TRUE(gl_dataContainerFinnhubForexExchange.UpdateDB());

		using namespace StockMarket;
		const auto& t = FinnhubForexExchange{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);

		auto result = db(select(all_of(t)).from(t).where(t.code == "Test"));
		auto rows = result.size();
		EXPECT_EQ(rows, 1);
		db(sqlpp::remove_from(t).where(t.code == "Test"));
		tx.commit();

		// 恢复原状
		gl_dataContainerFinnhubForexExchange.Delete(strSymbol);
		gl_dataContainerFinnhubForexExchange.SetLastSize(gl_dataContainerFinnhubForexExchange.Size());
		EXPECT_FALSE(gl_dataContainerFinnhubForexExchange.IsNeedUpdate());
	}

	TEST_F(CWorldMarketTest, TestUpdateCryptoExchangeDB) {
		const string sSymbol = "Test";

		EXPECT_FALSE(gl_dataContainerFinnhubCryptoExchange.IsNeedUpdate());
		EXPECT_FALSE(gl_dataContainerFinnhubCryptoExchange.UpdateDB()) << "没有新Crypto Exchange";

		EXPECT_FALSE(gl_dataContainerFinnhubCryptoExchange.IsExchange(sSymbol)); // 确保是一个新Crypto代码
		gl_dataContainerFinnhubCryptoExchange.Add(sSymbol);
		EXPECT_TRUE(gl_dataContainerFinnhubCryptoExchange.IsNeedUpdate());
		EXPECT_TRUE(gl_dataContainerFinnhubCryptoExchange.UpdateDB());

		using namespace StockMarket;
		const auto& t = FinnhubCryptoExchange{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);

		auto result = db(select(all_of(t)).from(t).where(t.code == "Test"));
		auto rows = result.size();
		EXPECT_EQ(rows, 1);
		db(sqlpp::remove_from(t).where(t.code == "Test"));
		tx.commit();

		// 恢复原状
		gl_dataContainerFinnhubCryptoExchange.Delete(sSymbol);
		gl_dataContainerFinnhubCryptoExchange.SetLastSize(gl_dataContainerFinnhubCryptoExchange.Size());
		EXPECT_FALSE(gl_dataContainerFinnhubCryptoExchange.IsNeedUpdate());
	}

	TEST_F(CWorldMarketTest, TestUpdateEconomicCalendarDB) {
		CEconomicCalendar economicCalendar;
		vector<CEconomicCalendar> vEconomicCalendar;

		economicCalendar.m_strCountry = "USA";
		economicCalendar.m_strTime = "20200101";
		economicCalendar.m_strEvent = "abc";
		economicCalendar.m_dActual = 1.0;
		economicCalendar.m_dEstimate = 2.0;
		economicCalendar.m_dPrev = 3.0;
		economicCalendar.m_strImpact = "s";
		economicCalendar.m_strUnit = "USD";

		vEconomicCalendar.push_back(economicCalendar);

		EXPECT_TRUE(gl_dataContainerFinnhubEconomicCalendar.Update(vEconomicCalendar));
		EXPECT_TRUE(gl_dataContainerFinnhubEconomicCalendar.UpdateDB());

		// 测试并恢复原状
		using namespace StockMarket;
		const auto& t = FinnhubEconomicCalendar{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);

		auto result = db(select(all_of(t)).from(t).where(t.Country == std::string("USA") && t.Time == std::string("20200101") && t.Event == std::string("abc")));
		auto rows = result.size();
		auto& row = result.front();
		EXPECT_FALSE(rows == 0);

		EXPECT_EQ(row.Country, "USA");
		EXPECT_EQ(row.Time, "20200101");
		EXPECT_EQ(row.Event, "abc");
		EXPECT_DOUBLE_EQ(row.Actual, 1.0);
		EXPECT_DOUBLE_EQ(row.Estimate, 2.0);
		EXPECT_DOUBLE_EQ(row.Prev, 3.0);
		EXPECT_EQ(row.Impact, "s");
		EXPECT_EQ(row.Unit, "USD");

		db(remove_from(t).unconditionally());
		tx.commit();
	}

	TEST_F(CWorldMarketTest, TestRebuildEPSSurprise) {
		CFinnhubStockPtr pStock;
		for (size_t i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(i);
			pStock->SetLastEPSSurpriseUpdateDate(toLocalDays(20200101));
			pStock->m_fUpdateEPSSurprise = false;
		}
		gl_pFinnhubDataSource->SetUpdateEPSSurprise(false);

		gl_pWorldMarket->RebuildEPSSurprise();

		for (size_t i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(i);
			EXPECT_EQ(pStock->GetLastEPSSurpriseUpdateDate(), toLocalDays(19800101));
			EXPECT_TRUE(pStock->m_fUpdateEPSSurprise);
		}
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateEPSSurprise());
	}

	TEST_F(CWorldMarketTest, TestRebuildPeer) {
		CFinnhubStockPtr pStock;
		for (size_t i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(i);
			pStock->SetPeerUpdateDate(toLocalDays(20200101));
			pStock->SetUpdatePeer(false);
			pStock->SetUpdateProfileDB(false);
		}
		gl_pFinnhubDataSource->SetUpdatePeer(false);

		gl_pWorldMarket->RebuildPeer();

		for (size_t i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(i);
			EXPECT_EQ(pStock->GetPeerUpdateDate(), toLocalDays(19800101));
			EXPECT_TRUE(pStock->IsUpdatePeer());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
		}
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdatePeer());

		for (size_t i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(i);
			pStock->SetUpdatePeer(true);
			pStock->SetUpdateProfileDB(false);
		}
	}

	TEST_F(CWorldMarketTest, TestRebuildStockDayLine) {
		CFinnhubStockPtr pStock;
		for (size_t i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(i);
			pStock->SetDayLineStartDate(toLocalDays(20200101));
			pStock->SetDayLineEndDate(toLocalDays(20200101));
			pStock->SetUpdateDayLine(false);
			pStock->SetUpdateProfileDB(false);
		}
		gl_pFinnhubDataSource->SetUpdateStockProfile(false);

		gl_pWorldMarket->RebuildStockDayLineDB();

		for (size_t i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(i);
			EXPECT_EQ(pStock->GetDayLineStartDate(), toLocalDays(29900101));
			EXPECT_EQ(pStock->GetDayLineEndDate(), toLocalDays(19800101));
			EXPECT_TRUE(pStock->IsUpdateDayLine());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
		}
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateStockProfile());

		for (size_t i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
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

		gl_pWorldMarket->TEST_SetMarketTime(toLocalTime(10010));
		gl_pWorldMarket->TaskCreateTask();

		EXPECT_FALSE(gl_pWorldMarket->IsMarketTaskEmpty());
		auto pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), toLocalTime(1));
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__);
		EXPECT_EQ(pTask->GetTime(), toLocalTime(10010));
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_TIINGO_INQUIRE_DAYlINE__);
		EXPECT_EQ(pTask->GetTime(), toLocalTime(10030));
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_UPDATE_DB__);
		EXPECT_EQ(pTask->GetTime(), toLocalTime(10040));
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_TIINGO_INQUIRE_IEX_TOP_OF_BOOK__);
		EXPECT_EQ(pTask->GetTime(), toLocalTime(10100));
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__);
		EXPECT_EQ(pTask->GetTime(), toLocalTime(10200));
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_CALCULATE_NASDAQ100_200MA_UPDOWN_RATE);
		EXPECT_EQ(pTask->GetTime(), toLocalTime(10300));
		gl_pWorldMarket->DiscardCurrentMarketTask();

#ifndef _DEBUG
		//Note 为了方便调试，测试版不再添加以下任务。发行版依然添加。
		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_TIINGO_PROCESS_DAYLINE__);
		EXPECT_EQ(pTask->GetTime(), toLocalTime(10500));
		gl_pWorldMarket->DiscardCurrentMarketTask();
#endif

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_RESET__);
		EXPECT_EQ(pTask->GetTime(), gl_pWorldMarket->GetResetTime());
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_CREATE_TASK__);
		EXPECT_EQ(pTask->GetTime(), toLocalTime(240000));
		gl_pWorldMarket->DiscardCurrentMarketTask();

		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());
	}

	TEST_F(CWorldMarketTest, TestTaskCreateTask2) {
		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());

		gl_pWorldMarket->TEST_SetMarketTime(gl_pWorldMarket->GetResetTime() + 1min + 10s);
		gl_pWorldMarket->TaskCreateTask();

		EXPECT_FALSE(gl_pWorldMarket->IsMarketTaskEmpty());
		auto pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), toLocalTime(1));
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__);
		EXPECT_EQ(pTask->GetTime(), gl_pWorldMarket->GetResetTime() + 1min + 10s);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_TIINGO_INQUIRE_DAYlINE__);
		EXPECT_EQ(pTask->GetTime(), gl_pWorldMarket->GetResetTime() + 1min + 30s);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_UPDATE_DB__);
		EXPECT_EQ(pTask->GetTime(), gl_pWorldMarket->GetResetTime() + 1min + 40s);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_TIINGO_INQUIRE_IEX_TOP_OF_BOOK__);
		EXPECT_EQ(pTask->GetTime(), gl_pWorldMarket->GetResetTime() + 2min);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__);
		EXPECT_EQ(pTask->GetTime(), gl_pWorldMarket->GetResetTime() + 3min + 00s);
		gl_pWorldMarket->DiscardCurrentMarketTask();

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_CALCULATE_NASDAQ100_200MA_UPDOWN_RATE);
		EXPECT_EQ(pTask->GetTime(), gl_pWorldMarket->GetResetTime() + 4min + 00s);
		gl_pWorldMarket->DiscardCurrentMarketTask();

#ifndef _DEBUG
		//Note 为了方便调试，测试版不再添加以下任务。发行版依然添加。
		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_TIINGO_PROCESS_DAYLINE__);
		EXPECT_EQ(pTask->GetTime(), gl_pWorldMarket->GetResetTime() + 6min + 00s);
		gl_pWorldMarket->DiscardCurrentMarketTask();
#endif

		pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_CREATE_TASK__);
		EXPECT_EQ(pTask->GetTime(), toLocalTime(240000));
		gl_pWorldMarket->DiscardCurrentMarketTask();

		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());
	}

	TEST_F(CWorldMarketTest, TestTaskProcessWebSocketData1) {
		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());

		gl_pWorldMarket->TEST_SetMarketTime(GetPrevTime(gl_pWorldMarket->GetResetTime(), 0h, 2min, 1s));
		gl_pWorldMarket->TaskProcessWebSocketData();

		EXPECT_FALSE(gl_pWorldMarket->IsMarketTaskEmpty());
		const auto pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__);
		EXPECT_EQ(pTask->GetTime(), GetNextTime(gl_pWorldMarket->GetResetTime(), 0h, 5min, 1s));
		gl_pWorldMarket->DiscardCurrentMarketTask();

		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());
	}

	TEST_F(CWorldMarketTest, TestTaskProcessWebSocketData2) {
		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());

		gl_pWorldMarket->TEST_SetMarketTime(GetPrevTime(gl_pWorldMarket->GetResetTime(), 0h, 2min, 1s));
		gl_pWorldMarket->TaskProcessWebSocketData();

		EXPECT_FALSE(gl_pWorldMarket->IsMarketTaskEmpty());
		const auto pTask = gl_pWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__);
		EXPECT_EQ(pTask->GetTime(), GetNextTime(gl_pWorldMarket->GetResetTime(), 0h, 5min, 1s));
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
