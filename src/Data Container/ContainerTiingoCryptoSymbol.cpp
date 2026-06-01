#include "pch.h"
#include "ContainerTiingoCryptoSymbol.h"

#include "dataBaseConnector.h"
#include "InfoReport.h"

CContainerTiingoCryptoSymbol::CContainerTiingoCryptoSymbol() {
	CContainerTiingoCryptoSymbol::Reset();
}

void CContainerTiingoCryptoSymbol::Reset() {
	CContainerVirtualStock::Reset();
}

bool CContainerTiingoCryptoSymbol::LoadDB() {
	using namespace StockMarket;
	const auto& t = TiingoCryptoSymbol{};

	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);
	auto result = db(select(all_of(t)).from(t).unconditionally().order_by(t.ID.asc()));
	Reserve(result.size() + 2);
	for (const auto& row : result) {
		if (!IsSymbol(row.Symbol)) {
			const auto pSymbol = make_shared<CTiingoCrypto>();
			pSymbol->SetSymbol(row.Symbol);
			pSymbol->m_strName = row.Name;
			pSymbol->SetDescription(row.Description);
			pSymbol->m_strBaseCurrency = row.BaseCurrency;
			pSymbol->m_strQuoteCurrency = row.QuoteCurrency;
			Add(pSymbol);
		}
		else {
			db(remove_from(t).where(t.ID == row.ID));
		}
	}
	tx.commit();
	Sort();
	return true;
}

/*
void CContainerTiingoCryptoSymbol::UpdateDB2() {
	if (IsUpdateProfileDB()) {
		try {
			CSetTiingoCrypto setWorldCrypto;
			setWorldCrypto.m_strSort = "[Ticker]";
			setWorldCrypto.Open();
			setWorldCrypto.m_pDatabase->BeginTrans();
			while (!setWorldCrypto.IsEOF()) {	//更新原有的代码集状态
				if (IsSymbol(T2Utf8(setWorldCrypto.m_Ticker))) {
					const CTiingoCryptoPtr pCrypto = GetCrypto(T2Utf8(setWorldCrypto.m_Ticker));
					ASSERT(pCrypto != nullptr);
					if (pCrypto->IsUpdateProfileDB()) {
						pCrypto->Update(setWorldCrypto);
						pCrypto->SetUpdateProfileDB(false);
					}
				}
				else {
					setWorldCrypto.Delete();
				}
				setWorldCrypto.MoveNext();
			}
			for (size_t l = 0; l < m_vStock.size(); l++) {
				const CTiingoCryptoPtr pCrypto = GetCrypto(l);
				ASSERT(pCrypto != nullptr);
				if (pCrypto->IsUpdateProfileDB()) {
					pCrypto->Append(setWorldCrypto);
					pCrypto->SetUpdateProfileDB(false);
				}
			}
			setWorldCrypto.m_pDatabase->CommitTrans();
			setWorldCrypto.Close();
		} catch (CException& e) {
			ReportInformation(e);
		}
	}
}
*/

void CContainerTiingoCryptoSymbol::UpdateDB() {
	if (!IsUpdateProfileDB()) return;

	try {
		using namespace StockMarket;
		const auto& t = TiingoCryptoSymbol{};

		auto db = gl_dbStockMarket.get();
		auto tx = start_transaction(db);
		auto multi_insert = insert_into(t).columns(t.Symbol, t.BaseCurrency, t.QuoteCurrency, t.Name, t.Description, t.UpdateDate);

		// 1) 更新或删除数据库中已有但容器中不存在的记录
		auto rows = db(select(all_of(t)).from(t).unconditionally().order_by(t.ID.asc()));
		for (const auto& row : rows) {
			if (IsSymbol(row.Symbol)) {
				const CTiingoCryptoPtr pCrypto = GetCrypto(row.Symbol);
				ASSERT(pCrypto != nullptr);
				if (pCrypto->IsUpdateProfileDB()) {
					// 更新已有记录
					db(update(t).set(
						t.Name = pCrypto->m_strName,
						t.Description = pCrypto->GetDescription(),
						t.BaseCurrency = pCrypto->m_strBaseCurrency,
						t.QuoteCurrency = pCrypto->m_strQuoteCurrency
					).where(t.Symbol == row.Symbol));
					pCrypto->SetUpdateProfileDB(false);
				}
			}
			else {
				// 数据库中存在但容器没有，删除之
				db(remove_from(t).where(t.ID == row.ID));
			}
		}

		// 2) 插入容器中新添加或标记为需写入的记录
		int nValues = 0;
		for (size_t l = 0; l < m_vStock.size(); l++) {
			const CTiingoCryptoPtr pCrypto = GetCrypto(l);
			ASSERT(pCrypto != nullptr);
			if (pCrypto->IsUpdateProfileDB()) {
				multi_insert.values.add(
					t.Symbol = pCrypto->GetSymbol(),
					t.BaseCurrency = pCrypto->m_strBaseCurrency,
					t.QuoteCurrency = pCrypto->m_strQuoteCurrency,
					t.Name = pCrypto->m_strName,
					t.Description = pCrypto->GetDescription(),
					t.UpdateDate = pCrypto->GetJsonUpdateDate().dump()
				);
				nValues++;
				pCrypto->SetUpdateProfileDB(false);
			}
		}
		if (nValues > 0) db(multi_insert);
		tx.commit();
	} catch (CException& e) {
		ReportInformation(e);
	} catch (std::exception& e) {
		ReportErrorToSystemMessage("CContainerTiingoCryptoSymbol::UpdateDB", e);
	}
}
