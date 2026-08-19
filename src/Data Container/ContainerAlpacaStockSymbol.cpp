#include "pch.h"

#include"containerAlpacaStockSymbol.h"
#include "AlpacaStock.h"
#include "InfoReport.h"

#include<sqlpp23/sqlpp23.h>
#include "dataBaseConnector.h"
#include"StockMarketSQLTable.h"

using std::make_shared;

CContainerAlpacaStockSymbol::CContainerAlpacaStockSymbol() {
	CContainerAlpacaStockSymbol::Reset();
}

void CContainerAlpacaStockSymbol::Reset() {
	CContainerVirtualStock::Reset();

	m_lastTotalSymbol = 0;
}

bool CContainerAlpacaStockSymbol::LoadProfileDB() {
	using namespace StockMarket;
	const auto& t = AlpacaStockSymbol{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t).order_by(t.ID.asc()));
	size_t rows = result.size();
	Reserve(rows + 10);
	for (const auto& row : result) {
		const std::string symbol = string{ row.Symbol };
		if (!IsSymbol(symbol)) {
			const auto pSymbol = make_shared<CAlpacaStock>();
			pSymbol->SetSymbol(row.Symbol);
			pSymbol->SetExchange(row.Exchange);
			pSymbol->SetDisplaySymbol(row.Name);
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

void CContainerAlpacaStockSymbol::UpdateProfileDB(std::stop_token st) {
	if (IsUpdateProfileDB()) {
		try {
			using namespace StockMarket;
			const auto& t = AlpacaStockSymbol{};
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);
			size_t stockSize = m_vStock.size();

			for (size_t i = 0; i < stockSize; ++i) {
				if (st.stop_requested()) break;
				const auto& pStock = GetItem(i);
				if (pStock->IsUpdateProfileDB()) {
					if (pStock->IsNewStock()) {//插入新股票代码
						db(sqlpp::insert_into(t).set(
							t.Symbol = pStock->GetSymbol(),
							t.Exchange = pStock->GetExchange(),
							t.Name = pStock->GetDisplaySymbol()
						));
						pStock->SetNewStock(false);
					}
					pStock->SetUpdateProfileDB(false);
				}
			}
			tx.commit();
		} catch (CException& e) {
			ReportInformation(e);
		}
	}
}

CAlpacaStockPtr CContainerAlpacaStockSymbol::GetItem(size_t lIndex) {
	return dynamic_pointer_cast<CAlpacaStock>(Get(lIndex));
}

CAlpacaStockPtr CContainerAlpacaStockSymbol::GetItem(const string& strStockCode) {
	return dynamic_pointer_cast<CAlpacaStock>(Get(strStockCode));
}
