module;
#include<sqlpp23/sqlpp23.h>
#include"StockMarketSQLTable.h"

module FireBirdLib.Container.Stock.FinnhubForexSymbol;
import FireBirdLib.Stock.FinnhubForex;
import FireBirdLib.Accessory.InfoReport;
import FireBirdLib.DatabaseConnector;

import std;
using std::make_shared;
using std::dynamic_pointer_cast;

CContainerFinnhubForexSymbol::CContainerFinnhubForexSymbol() {
	CContainerFinnhubForexSymbol::Reset();
}

void CContainerFinnhubForexSymbol::Reset() {
	CContainerVirtualStock::Reset();

	m_lastTotalSymbol = 0;
}

bool CContainerFinnhubForexSymbol::LoadProfileDB() {
	using namespace StockMarket;
	const auto& t = FinnhubForexSymbol{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t).order_by(t.ID.asc()));
	size_t rows = result.size();
	Reserve(rows + 10);
	for (const auto& row : result) {
		const std::string symbol = string{ row.Symbol };
		if (!IsSymbol(symbol)) {
			const auto pSymbol = make_shared<CFinnhubForex>();
			pSymbol->SetSymbol(row.Symbol);
			pSymbol->SetDescription(row.Description);
			pSymbol->SetExchange(row.Exchange);
			pSymbol->SetDisplaySymbol(row.DisplaySymbol);
			pSymbol->LoadUpdateDate(string{ row.UpdateDate });
			pSymbol->SetCheckingDayLineStatus();
			Add(pSymbol);
		}
		else {
			db(sqlpp::delete_from(t).where(t.ID == row.ID)); // 如果数据库中存在重复的股票代码，则删除重复的记录。
		}
	}
	tx.commit();
	Sort();
	m_lastTotalSymbol = m_vStock.size();

	return true;
}

void CContainerFinnhubForexSymbol::UpdateProfileDB() {
	if (IsUpdateProfileDB()) {
		using namespace StockMarket;
		const auto& t = FinnhubForexSymbol{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);

		for (size_t i = 0; i < m_vStock.size(); ++i) {
			const auto& pStock = m_vStock[i];
			if (pStock->IsUpdateProfileDB()) {
				pStock->UpdateJsonUpdateDate();
				if (pStock->IsNewStock()) {//插入新股票代码
					db(sqlpp::insert_into(t).set(
						t.Symbol = pStock->GetSymbol(),
						t.Description = pStock->GetDescription(),
						t.Exchange = pStock->GetExchange(),
						t.DisplaySymbol = pStock->GetDisplaySymbol(),
						t.UpdateDate = pStock->GetJsonUpdateDate().dump()
					));
					pStock->SetNewStock(false);
				}
				else {//更新现有股票代码
					db(sqlpp::update(t).set(
						t.Symbol = pStock->GetSymbol(),
						t.Description = pStock->GetDescription(),
						t.Exchange = pStock->GetExchange(),
						t.DisplaySymbol = pStock->GetDisplaySymbol(),
						t.UpdateDate = pStock->GetJsonUpdateDate().dump()
					).where(t.Symbol == pStock->GetSymbol()));
				}
				pStock->SetUpdateProfileDB(false);
			}
		}
		tx.commit();
	}
}

CForexSymbolPtr CContainerFinnhubForexSymbol::GetItem(size_t lIndex) {
	return dynamic_pointer_cast<CFinnhubForex>(Get(lIndex));
}
CForexSymbolPtr CContainerFinnhubForexSymbol::GetItem(const string& strStockCode) {
	return dynamic_pointer_cast<CFinnhubForex>(Get(strStockCode));
}
