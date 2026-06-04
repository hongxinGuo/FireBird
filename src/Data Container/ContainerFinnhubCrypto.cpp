#include "pch.h"

#include "ContainerFinnhubCrypto.h"

#include "dataBaseConnector.h"
#include"FinnhubCrypto.h"
#include "InfoReport.h"

CContainerFinnhubCrypto::CContainerFinnhubCrypto() {
	CContainerFinnhubCrypto::Reset();
}

void CContainerFinnhubCrypto::Reset() {
	CContainerVirtualStock::Reset();

	m_llLastTotalSymbol = 0;
}

bool CContainerFinnhubCrypto::LoadProfileDB() {
	using namespace StockMarket;
	const auto& t = FinnhubCryptoSymbol{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t).unconditionally().order_by(t.ID.asc()));
	Reset();
	size_t rows = result.size();
	Reserve(rows + 10);
	for (const auto& row : result) {
		const std::string symbol = row.Symbol;
		if (!IsSymbol(symbol)) {
			const auto pSymbol = make_shared<CFinnhubCrypto>();
			pSymbol->SetSymbol(row.Symbol);
			pSymbol->SetDescription(row.Description);
			pSymbol->SetExchange(row.Exchange);
			pSymbol->SetDisplaySymbol(row.DisplaySymbol);
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
	m_llLastTotalSymbol = m_vStock.size();

	return true;
}

void CContainerFinnhubCrypto::UpdateProfileDB() {
	if (IsUpdateProfileDB()) {
		try {
			using namespace StockMarket;
			const auto& t = FinnhubCryptoSymbol{};
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
		} catch (CException& e) {
			ReportInformation(e);
		}
	}
}
