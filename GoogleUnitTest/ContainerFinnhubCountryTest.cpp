#include "pch.h"

#include "GeneralCheck.h"
#include "ContainerFinnhubCountry.h"
#include "dataBaseConnector.h"
#include "StockMarketSQLTable.h"

#include <sqlpp11/transaction.h>

using namespace testing;

namespace FireBirdTest {
	class CContainerFinnhubCountryTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck(); // ensure DB / environment initialized like other tests
		}
	};

	TEST_F(CContainerFinnhubCountryTest, LoadDB2_InsertsAndLoads) {
		using namespace StockMarket;
		const auto& t = FinnhubCountryList{};

		// Prepare unique test rows
		const std::string codeA = "UT01";
		const std::string countryA = "UTCountry1";
		const std::string codeB = "UT02";
		const std::string countryB = "UTCountry2";

		auto db = gl_dbStockMarket.get();
		{
			auto tx = sqlpp::start_transaction(db);
			// insert two test rows
			db(insert_into(t).set(
				t.Code2 = codeA,
				t.Code3 = std::string("UT001"),
				t.CodeNo = std::string("1001"),
				t.Country = countryA,
				t.Currency = std::string("UNIT"),
				t.CurrencyCode = std::string("UTC")
			));
			db(insert_into(t).set(
				t.Code2 = codeB,
				t.Code3 = std::string("UT002"),
				t.CodeNo = std::string("1002"),
				t.Country = countryB,
				t.Currency = std::string("UNIT2"),
				t.CurrencyCode = std::string("UTC2")
			));
			tx.commit();
		}

		// Load using the container implementation under test
		CContainerFinnhubCountry container;
		EXPECT_TRUE(container.LoadDB());

		// Verify loaded data contains our inserted rows
		EXPECT_TRUE(container.IsCountry(countryA)) << "Expected country inserted to be present";
		EXPECT_TRUE(container.IsCountry(countryB)) << "Expected country inserted to be present";

		auto loadedA = container.GetCountry(countryA);
		EXPECT_EQ(loadedA.m_strCode2, codeA);
		EXPECT_EQ(loadedA.m_strCurrency, "UNIT");

		auto loadedB = container.GetCountry(countryB);
		EXPECT_EQ(loadedB.m_strCode2, codeB);
		EXPECT_EQ(loadedB.m_strCurrencyCode, "UTC2");

		// Cleanup inserted test rows
		db = GetStockMarketDB(); // Ensure we have a fresh connection for cleanup
		auto tx = sqlpp::start_transaction(db);
		db(remove_from(t).where(t.Code2 == codeA));
		db(remove_from(t).where(t.Code2 == codeB));
		tx.commit();
	}

	TEST(ContainerFinnhubCountryTest, UpdateDB2_InsertsRows) {
		// create a unique Code2 to avoid colliding with existing data
		const string uniqueSuffix = to_string(std::chrono::system_clock::now().time_since_epoch().count());
		const string code2 = "UT_" + uniqueSuffix;

		CContainerFinnhubCountry container;
		CCountry country;
		country.m_strCode2 = code2;
		country.m_strCode3 = code2 + "_3";
		country.m_strCodeNo = "999999";
		country.m_strCountry = "UnitTestCountry_" + uniqueSuffix;
		country.m_strCurrency = "UTC";
		country.m_strCurrencyCode = "UTCC";

		// Add and call UpdateDB2 (which uses sqlpp to insert new rows)
		container.Add(country);
		container.UpdateDB();

		// Verify row exists in FinnhubCountryList
		using namespace StockMarket;
		const auto& t = FinnhubCountryList{};
		auto db = GetStockMarketDB();
		auto result = db(select(all_of(t)).from(t).where(t.Code2 == code2));

		EXPECT_GT(result.size(), 0u) << "Expected the new FinnhubCountryList row to be inserted";

		// Cleanup: remove inserted test rows
		try {
			auto tx = sqlpp::start_transaction(db);
			db(remove_from(t).where(t.Code2 == code2));
			tx.commit();
		} catch (...) {
			// best-effort cleanup; do not mask original expectation failure
		}
	}
}
