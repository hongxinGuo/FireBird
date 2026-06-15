#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ContainerTiingoStock.h"
#include "dataBaseConnector.h"
#include "jsonParse.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerTiingoStockTest : public ::testing::Test {
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
		}

		void TearDown() override {
			// clearUp
			m_dataTiingoStock.Reset();
			gl_systemConfiguration.SetUpdateDB(false);
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CContainerTiingoStock m_dataTiingoStock;
	};

	TEST_F(CContainerTiingoStockTest, TestUpdateTiingoStockDB) {
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
		pTiingoStock->SetNewStock(true);
		gl_dataContainerTiingoStock.Add(pTiingoStock);

		EXPECT_TRUE(gl_dataContainerTiingoStock.IsUpdateProfileDB()) << "添加了两个股票";

		gl_dataContainerTiingoStock.UpdateProfileDB(); // 更新代码集

		// 恢复原状
		using namespace StockMarket;
		const auto& t = TiingoStockProfile{};
		auto db = GetStockMarketDB();
		auto tx = start_transaction(db);
		auto result = db(select(all_of(t)).from(t).where(t.SICSector == std::string("Test")).order_by(t.Symbol.asc()));
		size_t rows = result.size();
		EXPECT_EQ(rows, 2) << "应该有两行数据";
		auto& row = result.front();
		string str = row.Symbol;
		EXPECT_STREQ(str.c_str(), "A") << "已存在代码";
		int sicCode = row.SICCode;
		EXPECT_EQ(sicCode, 1002);
		result.pop_front();
		auto& row2 = result.front();
		str = row2.Symbol;
		EXPECT_STREQ(str.c_str(), "ABCDEF") << "新代码";
		tx.commit();

		auto tx1 = start_transaction(db);
		db(update(t).set(t.SICSector = std::string("")).where(t.Symbol == std::string("A")));
		db(remove_from(t).where(t.Symbol == std::string("ABCDEF")));
		tx1.commit();

		gl_dataContainerTiingoStock.Delete(pTiingoStock);
	}

	TEST_F(CContainerTiingoStockTest, TestSetAllFinancialStateStatus) {
		for (size_t index = 0; index < m_dataTiingoStock.Size(); index++) {
			auto pStock = m_dataTiingoStock.GetStock(index);
			EXPECT_TRUE(pStock->IsUpdateFinancialState());
		}

		m_dataTiingoStock.SetUpdateFinancialState(false);

		for (size_t index = 0; index < m_dataTiingoStock.Size(); index++) {
			auto pStock = m_dataTiingoStock.GetStock(index);
			EXPECT_FALSE(pStock->IsUpdateFinancialState());
		}

		// 恢复原状
		m_dataTiingoStock.SetUpdateFinancialState(true);
	}

	TEST_F(CContainerTiingoStockTest, TestReportHighHigherRate) {
		gl_pWorldMarket->ResetNewHighHigher();
		gl_pWorldMarket->AddNewHighHigher(10000);
		gl_pWorldMarket->AddNoNewHighHigher(20000);

		m_dataTiingoStock.ReportHighHigherRate();
		EXPECT_EQ(gl_pWorldMarket->GetNewHighHigher(), 10000);
		EXPECT_EQ(gl_pWorldMarket->GetNoNewHighHigher(), 20000);
		EXPECT_EQ(gl_systemMessage.StockMarketInformationSize(), 1);
		auto s = gl_systemMessage.PopStockMarketInformationMessage();
		EXPECT_TRUE(s.find("3月内再创新高数:10000, 3月内未再次新高数:20000, 比率:0.50") != string::npos);
		gl_pWorldMarket->ResetNewHighHigher();
	}

	TEST_F(CContainerTiingoStockTest, TestDeleteDuplicatedStockDB) {
		using namespace StockMarket;
		const auto& t = TiingoStockProfile{};
		// Ensure no leftover test symbols
		auto db = GetStockMarketDB();
		db(remove_from(t).where(t.Symbol == std::string("DUPLICATE")));

		// Insert duplicate rows for the same Symbol
		db(insert_into(t).set(t.Symbol = std::string("DUPLICATE"), t.Name = std::string("TEST_DUP1")));
		db(insert_into(t).set(t.Symbol = std::string("DUPLICATE"), t.Name = std::string("TEST_DUP2")));
		db(insert_into(t).set(t.Symbol = std::string("DUPLICATE"), t.Name = std::string("TEST_DUP3")));

		// Ensure inserts are committed and visible to other connections
		db.execute("COMMIT");

		// Verify duplicates were inserted
		db = GetStockMarketDB(); // 执行完插入后，重新获取数据库连接，以确保看到最新的数据
		auto resBefore = db(select(all_of(t)).from(t).where(t.Symbol == std::string("DUPLICATE")));
		EXPECT_TRUE(resBefore.size() >= 3);

		// Call the function under test
		m_dataTiingoStock.DeleteDuplicatedSymbolFromDB();

		// Verify only one row remains for that symbol
		db = GetStockMarketDB();
		auto resAfter = db(select(all_of(t)).from(t).where(t.Symbol == std::string("DUPLICATE")));
		EXPECT_EQ(resAfter.size(), 1);

		// Clean up test data
		db = GetStockMarketDB(); // 执行完删除重复代码任务后，重新获取数据库连接，以确保看到最新的数据
		auto tx2 = start_transaction(db);
		db(remove_from(t).where(t.Symbol == std::string("DUPLICATE")));
		tx2.commit();
	}

	TEST_F(CContainerTiingoStockTest, BuildDayLine_InsertsRow) {
		// Choose a test symbol and date
		const std::string symbol = "UTTEST";
		const chrono::local_days lDate = gl_pWorldMarket->GetMarketDate(); //使用最新的日期，防止更新其他股票

		// Ensure no pre-existing test symbol in container
		if (gl_dataContainerTiingoStock.IsSymbol(symbol)) {
			gl_dataContainerTiingoStock.Delete(symbol);
		}

		// Create a test stock and set fields (values are scaled by GetRatio())
		auto pStock = std::make_shared<CTiingoStock>();
		pStock->SetSymbol(symbol);
		pStock->SetExchange("US");
		EXPECT_FALSE(pStock->IsUpdateProfileDB());

		const int ratio = pStock->GetRatio(); // typically 1000000

		// set prices as integer internal representation
		pStock->SetLastClose(static_cast<long>(1.00 * ratio));
		pStock->SetOpen(static_cast<long>(1.10 * ratio));
		pStock->SetHigh(static_cast<long>(1.20 * ratio));
		pStock->SetLow(static_cast<long>(0.95 * ratio));
		pStock->SetNew(static_cast<long>(1.15 * ratio));

		pStock->SetVolume(1234);
		pStock->SetAmount(123400); // in cents/units expected by code
		pStock->SetDividend(0.0);
		pStock->SetSplitFactor(1.0);

		// ensure transaction time is at/after market close for the given date
		time_t tt = gl_pWorldMarket->ConvertToUTCTime(toUnsignedDate(lDate), 160000).time_since_epoch().count();
		pStock->SetTransactionTime(tt);

		// Add to global container
		gl_dataContainerTiingoStock.Add(pStock);
		EXPECT_FALSE(pStock->IsUpdateProfileDB());

		// Run the function under test
		gl_dataContainerTiingoStock.BuildDayLine(lDate);

		EXPECT_FALSE(pStock->IsUpdateProfileDB());

		EXPECT_EQ(gl_systemConfiguration.GetTiingoIEXTopOfBookUpdateDate(), lDate) << "Expected TiingoIEXTopOfBookUpdateDate to be set to the date of the built dayline";
		gl_systemConfiguration.SetTiingoIEXTopOfBookUpdateDate(toLocalDays(19800101));

		// Verify DB row inserted for our symbol and date using sqlpp11
		using namespace StockMarket;
		const auto& t = TiingoStockDayline{};
		auto db = gl_dbStockMarket.get();
		auto tx = start_transaction(db);

		auto result = db(select(all_of(t)).from(t).where((t.Date == toUnsignedDate(lDate)) and (t.Symbol == symbol)));
		ASSERT_EQ(result.size(), 1u) << "Expected exactly one inserted row for symbol";

		// inspect the row values (numeric DB values are unscaled)
		for (const auto& row : result) {
			// Close in DB should equal pStock->GetNew() / ratio
			const double expectedClose = static_cast<double>(pStock->GetNew()) / static_cast<double>(ratio);
			EXPECT_NEAR(row.Close, expectedClose, 1e-9);

			const double expectedOpen = static_cast<double>(pStock->GetOpen()) / static_cast<double>(ratio);
			EXPECT_NEAR(row.Open, expectedOpen, 1e-9);

			const double expectedHigh = static_cast<double>(pStock->GetHigh()) / static_cast<double>(ratio);
			EXPECT_NEAR(row.High, expectedHigh, 1e-9);

			const double expectedLow = static_cast<double>(pStock->GetLow()) / static_cast<double>(ratio);
			EXPECT_NEAR(row.Low, expectedLow, 1e-9);

			INT64 value = row.Volume;
			EXPECT_EQ(value, pStock->GetVolume());
			value = row.Amount;
			EXPECT_EQ(value, pStock->GetAmount());

			string str = row.Symbol;
			EXPECT_EQ(str, symbol);
			chrono::local_days date = toLocalDays(row.Date);
			EXPECT_EQ(date, lDate);
		}

		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
		gl_systemMessage.PopInnerSystemInformationMessage();
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 1);
		gl_systemMessage.PopDayLineInfoMessage();

		// cleanup: remove inserted row for this symbol/date only
		db(remove_from(t).where((t.Date == toUnsignedDate(lDate)) and (t.Symbol == symbol)));
		tx.commit();

		// remove from in-memory container
		gl_dataContainerTiingoStock.Delete(symbol);

		gl_systemConfiguration.SetUpdateDB(false);
	}

	TEST_F(CContainerTiingoStockTest, TestLoadProfileDB) {
		using namespace StockMarket;
		const auto& t = TiingoStockProfile{};
		{
			auto db = gl_dbStockMarket.get();
			auto tx = start_transaction(db);

			db(sqlpp::insert_into(t).set(t.Symbol = "A", t.Name = "Test")); // 重复代码，用于测试，此时代码总数是5702
			tx.commit();
		}
		m_dataTiingoStock.LoadProfileDB();
		EXPECT_EQ(m_dataTiingoStock.Size(), 7183); // 2024-06-01Tiingo股票总共有7183只股票。如果数据库中有重复的股票代码，则会被删除，所以最终装载的股票数应该是5701。
		{
			auto db = gl_dbStockMarket.get();
			auto tx = start_transaction(db);

			auto result = db(sqlpp::select(all_of(t)).from(t).where(t.Symbol == "A"));
			tx.commit();
			size_t rows = result.size();
			EXPECT_EQ(rows, 1) << "数据库中应该只有一条A的记录";
			auto& row = result.front();
			EXPECT_EQ(row.Symbol.value(), "A");
			EXPECT_EQ(row.ID.value(), 1);
		}
		m_dataTiingoStock.Reset();
	}

	TEST_F(CContainerTiingoStockTest, UpdateProfileDB_InsertsAndUpdates) {
		// Prepare test stocks
		const string newSymbol = "Test";
		const string existingSymbol = "A";

		// Ensure no leftover from previous runs
		{
			using namespace StockMarket;
			const auto& t = TiingoStockProfile{};
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);
			db(sqlpp::remove_from(t).where(t.Symbol == newSymbol));
			tx.commit();
		}

		// Create and add a new stock to container, mark as new and needing DB update
		auto pNewStock = make_shared<CTiingoStock>();
		pNewStock->SetSymbol(newSymbol);
		pNewStock->SetNewStock(true);
		pNewStock->SetUpdateProfileDB(true);
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsSymbol(newSymbol));
		gl_dataContainerTiingoStock.Add(pNewStock);
		EXPECT_TRUE(gl_dataContainerTiingoStock.IsSymbol(newSymbol));

		// Modify an existing stock in-memory and mark it for update
		auto pExistStock = gl_dataContainerTiingoStock.GetStock(existingSymbol);
		ASSERT_NE(pExistStock, nullptr);
		const auto originalCurrency = pExistStock->GetReportingCurrency();
		const auto originalUpdateDayLineEndDate = pExistStock->GetDayLineEndDate();
		pExistStock->SetDayLineEndDate(toLocalDays(20200220));
		pExistStock->SetReportingCurrency("CNY");
		pExistStock->SetUpdateProfileDB(true);

		// Perform the DB update
		gl_dataContainerTiingoStock.UpdateProfileDB();

		// Verify DB: existing stock updated
		{
			using namespace StockMarket;
			const auto& t = TiingoStockProfile{};
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);

			auto resultExist = db(select(all_of(t)).from(t).where(t.Symbol == existingSymbol));
			EXPECT_EQ(resultExist.size(), 1u);
			if (!resultExist.empty()) {
				auto& row = resultExist.front();
				EXPECT_EQ(row.ReportingCurrency.value(), "CNY");
				string json = row.UpdateDate.value();
				nlohmannJson js;
				CreateJsonWithNlohmann(js, json);
				long updateDate = js["DayLineEndDate"];
				EXPECT_EQ(updateDate, 20200220);
			}

			// Verify new stock inserted
			auto resultNew = db(select(all_of(t)).from(t).where(t.Symbol == newSymbol));
			EXPECT_EQ(resultNew.size(), 1u);

			// Cleanup DB: remove test insertion and restore existing stock IPO status
			pExistStock->SetDayLineEndDate(toLocalDays(19800101));
			pExistStock->UpdateJsonUpdateDate();
			string jsonUpdateDate = pExistStock->GetJsonUpdateDate().dump();
			db(update(t).set(t.ReportingCurrency = originalCurrency, t.UpdateDate = jsonUpdateDate).where(t.Symbol == existingSymbol));
			db(remove_from(t).where(t.Symbol == newSymbol));
			tx.commit();
		}

		// Cleanup in-memory container
		auto pRetrievedNew = gl_dataContainerTiingoStock.GetStock(newSymbol);
		ASSERT_NE(pRetrievedNew, nullptr);
		gl_dataContainerTiingoStock.Delete(pRetrievedNew);

		// Restore in-memory IPO status and flags
		pExistStock->SetDayLineEndDate(toLocalDays(19800101));
		pExistStock->SetUpdateProfileDB(false);

		//gl_dataContainerTiingoStock.UpdateProfileDB();
		// Final assertions: flags cleared
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsUpdateProfileDB());
	}
}
