////////////////////////////////////////////////////////////////////////////////////////////////
//
// ������ʹ��WorldMarket���ݿ��е�finnhub_stock_exchange��
// �ñ��������ԴΪ:
//
// ����ʵʱ��̬���¡�
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

CStockExchangePtr CContainerStockExchange::GetExchange(const CString& strExchangeSymbol) const {
	if (m_mapStockExchange.contains(strExchangeSymbol)) {
		for (auto pExchange : m_vStockExchange) {
			if (pExchange->m_strCode.Compare(strExchangeSymbol) == 0) {
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
			m_mapStockExchange[pExchange->m_strCode] = m_vStockExchange.size();
			setExchange.MoveNext();
		}
		setExchange.Close();
	}

	return true;
}
