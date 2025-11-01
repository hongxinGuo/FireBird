////////////////////////////////////////////////////////////////////////////////////////////////
//
// 此数据使用WorldMarket数据库中的finnhub_stock_exchange表，
// 该表的数据来源为:
//
// 无需实时动态更新。
//
///////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "ContainerStockExchange.h"

CContainerStockExchange::CContainerStockExchange() {
	LoadDB();
}

void CContainerStockExchange::Reset() {
	m_vStockExchange.resize(0);
	m_mapStockExchange.clear();
}

CStockExchangePtr CContainerStockExchange::GetItem(const string& strExchangeSymbol) const {
	if (m_mapStockExchange.contains(strExchangeSymbol)) {
		for (auto pExchange : m_vStockExchange) {
			if (strExchangeSymbol.compare(pExchange->GetExchangeCode()) == 0) {
				return pExchange;
			}
		}
		return nullptr;
	}
	ASSERT(FALSE);
	return nullptr;
}

bool CContainerStockExchange::LoadDB() {
	CStockExchangePtr pExchange = nullptr;

	if (m_vStockExchange.empty()) {
		CSetStockExchange setExchange;
		setExchange.m_strSort = _T("[Code]");
		setExchange.Open();
		while (!setExchange.IsEOF()) {
			pExchange = make_shared<CStockExchange>();
			pExchange->Load(setExchange);
			m_vStockExchange.push_back(pExchange);
			m_mapStockExchange[pExchange->GetExchangeCode()] = m_vStockExchange.size();
			setExchange.MoveNext();
		}
		setExchange.Close();
	}

	return true;
}
