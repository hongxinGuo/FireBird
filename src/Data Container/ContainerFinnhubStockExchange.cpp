////////////////////////////////////////////////////////////////////////////////////////////////
//
// ������ʹ��WorldMarket���ݿ��е�finnhub_stock_exchange����
// �ñ���������ԴΪ:
//
// ����ʵʱ��̬���¡�
//
///////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "ContainerFinnhubStockExchange.h"

#include<memory>
using std::make_shared;

CContainerFinnhubStockExchange::CContainerFinnhubStockExchange() {
	Reset();
}

void CContainerFinnhubStockExchange::Reset() {
	m_vFinnhubStockExchange.resize(0);
	m_mapFinnhubStockExchange.clear();
}

bool CContainerFinnhubStockExchange::LoadDB() {
	CFinnhubStockExchangePtr pExchange = nullptr;

	if (m_vFinnhubStockExchange.size() == 0) {
		CSetFinnhubStockExchange setExchange;
		setExchange.m_strSort = _T("[Code]");
		setExchange.Open();
		while (!setExchange.IsEOF()) {
			pExchange = make_shared<CFinnhubStockExchange>();
			pExchange->Load(setExchange);
			m_vFinnhubStockExchange.push_back(pExchange);
			m_mapFinnhubStockExchange[pExchange->m_strCode] = m_vFinnhubStockExchange.size();
			setExchange.MoveNext();
		}
		setExchange.Close();
	}

	return true;
}