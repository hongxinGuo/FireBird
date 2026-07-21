////////////////////////////////////////////////////////////////////////////////////////////////
//
// 此数据使用stock_market数据库中的finnhub_stock_exchange表，
// 该表的数据来源为:
//
// 无需实时动态更新。
//
///////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "ContainerStockExchange.h"

#include<sqlpp23/sqlpp23.h>

#include "dataBaseConnector.h"
#include"StockMarketSQLTable.h"

using namespace std;

CContainerStockExchange::CContainerStockExchange() {
	LoadDB(); // 生成时即装载数据库。
}

void CContainerStockExchange::Reset() {
	m_vStockExchange.empty();
	m_mapStockExchange.clear();
}

void CContainerStockExchange::Reserve(size_t size) {
	m_vStockExchange.reserve(size);
	m_vStockExchange.clear();
	m_mapStockExchange.reserve(size);
	m_mapStockExchange.clear();
}

CStockExchangePtr CContainerStockExchange::GetItem(const string& strExchangeSymbol) const {
	if (m_mapStockExchange.contains(strExchangeSymbol)) {
		for (auto pExchange : m_vStockExchange) {
			if (strExchangeSymbol == pExchange->GetExchangeCode()) {
				return pExchange;
			}
		}
		return nullptr;
	}
	ASSERT(FALSE);
	return nullptr;
}

bool CContainerStockExchange::LoadDB() {
	if (m_vStockExchange.empty()) {
		// 交易所信息永远使用工作数据库。
		InitSqlppMySQLConnectionPool("Test", "test", "stock_market_test", "localhost", 3306, 20, false);
		using namespace StockMarket;
		const auto& t = FinnhubStockExchange{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);

		auto result = db(select(all_of(t)).from(t).order_by(t.code.asc()));
		auto rows = result.size();
		Reserve(rows);

		for (const auto& row : result) {
			CStockExchangePtr pExchange;
			pExchange = make_shared<CStockExchange>();
			pExchange->SetExchangeCode(string{ row.code });
			pExchange->m_strName = string{ row.name };
			pExchange->m_strMic = string{ row.mic };
			pExchange->m_strTimeZone = string{ row.timezone };
			pExchange->m_strPreMarket = string{ row.preMarket };
			pExchange->m_strHour = string{ row.hour };
			pExchange->m_strPostMarket = string{ row.postMarket };
			pExchange->m_strCloseDate = string{ row.closeDate };
			pExchange->m_strCountry = string{ row.country };
			pExchange->m_strCountryName = string{ row.countryName };
			pExchange->m_strSource = string{ row.source };

			int openHour, openMinute, endHour, endMinute;
			sscanf_s(pExchange->m_strHour.c_str(), "%2d:%2d-%2d:%2d", &openHour, &openMinute, &endHour, &endMinute);
			pExchange->m_marketOpenTime = chrono::local_seconds(chrono::hours(openHour) + chrono::minutes(openMinute));
			pExchange->m_marketCloseTime = chrono::local_seconds(chrono::hours(endHour) + chrono::minutes(endMinute));
			m_vStockExchange.push_back(pExchange);
			m_mapStockExchange[pExchange->GetExchangeCode()] = m_vStockExchange.size();
		}
		tx.commit();
	}
	return true;
}
