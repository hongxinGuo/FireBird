////////////////////////////////////////////////////////////////////////////////////////////////
//
// ������ʹ��WorldMarket���ݿ��е�finnhub_stock_exchange��
// �ñ��������ԴΪ:
//
// ����ʵʱ��̬���¡�
//
///////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "DataFinnhubStockExchange.h"

CDataFinnhubStockExchange::CDataFinnhubStockExchange() {
	Reset();
}

CDataFinnhubStockExchange::~CDataFinnhubStockExchange() {
}

void CDataFinnhubStockExchange::Reset() {
	m_vFinnhubStockExchange.resize(0);
	m_mapFinnhubStockExchange.clear();
}

bool CDataFinnhubStockExchange::LoadDB(void)
{
	CSetFinnhubStockExchange setExchange;
	CFinnhubStockExchangePtr pExchange = nullptr;

	if (m_vFinnhubStockExchange.size() == 0) {
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