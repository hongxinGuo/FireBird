#include "pch.h"

#include"containerFinnhubForexSymbol.h"
#include "dataBaseConnector.h"
#include"FinnhubForex.h"
#include "InfoReport.h"

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

	auto result = db(select(all_of(t)).from(t).unconditionally().order_by(t.ID.asc()));
	Reset();
	size_t rows = result.size();
	Reserve(rows + 10);
	for (const auto& row : result) {
		const std::string symbol = row.Symbol;
		if (!IsSymbol(symbol)) {
			const auto pSymbol = make_shared<CFinnhubForex>();
			pSymbol->SetSymbol(row.Symbol);
			pSymbol->SetDescription(row.Description);
			pSymbol->SetExchangeCode(row.Exchange);
			pSymbol->SetDisplaySymbol(row.DisplaySymbol);
			pSymbol->SetIPOStatus(row.IPOStatus);
			pSymbol->LoadUpdateDate(row.UpdateDate);
			pSymbol->SetCheckingDayLineStatus();
			Add(pSymbol);
		}
		else {
			db(sqlpp::remove_from(t).where(t.ID == row.ID)); // 如果数据库中存在重复的股票代码，则删除重复的记录。
		}
	}
	tx.commit();
	Sort();
	m_lastTotalSymbol = m_vStock.size();

	return true;
}

void CContainerFinnhubForexSymbol::UpdateProfileDB() {
	if (IsUpdateProfileDB()) {
		try {
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
							t.Exchange = pStock->GetExchangeCode(),
							t.DisplaySymbol = pStock->GetDisplaySymbol(),
							t.IPOStatus = pStock->GetIPOStatus(),
							t.UpdateDate = pStock->GetJsonUpdateDate().dump()
						));
						pStock->SetTodayNewStock(false);
					}
					else {//更新现有股票代码
						db(sqlpp::update(t).set(
							t.Symbol = pStock->GetSymbol(),
							t.Description = pStock->GetDescription(),
							t.Exchange = pStock->GetExchangeCode(),
							t.DisplaySymbol = pStock->GetDisplaySymbol(),
							t.IPOStatus = pStock->GetIPOStatus(),
							t.UpdateDate = pStock->GetJsonUpdateDate().dump()
						).where(t.Symbol == pStock->GetSymbol()));
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
