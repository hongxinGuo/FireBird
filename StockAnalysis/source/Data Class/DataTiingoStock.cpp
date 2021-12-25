#include "pch.h"
#include "DataTiingoStock.h"

CDataTiingoStock::CDataTiingoStock() {
	Reset();
}

CDataTiingoStock::~CDataTiingoStock() {
}

void CDataTiingoStock::Reset(void) {
	m_vTiingoStock.resize(0);
	m_mapTiingoStock.clear();
	m_lLastTotalTiingoStock = 0;
}

void CDataTiingoStock::Add(CTiingoStockPtr pTiingoStock) {
	m_mapTiingoStock[pTiingoStock->m_strTicker] = m_vTiingoStock.size();
	m_vTiingoStock.push_back(pTiingoStock);
}

bool CDataTiingoStock::Delete(CTiingoStockPtr pTiingoStock) {
	if (pTiingoStock == nullptr) return false;
	if (!IsStock(pTiingoStock)) return false;

	m_mapTiingoStock.erase(pTiingoStock->m_strTicker);
	auto it = find(m_vTiingoStock.begin(), m_vTiingoStock.end(), pTiingoStock);
	m_vTiingoStock.erase(it);

	return true;
}

bool CDataTiingoStock::UpdateDB(void) {
	CTiingoStockPtr pTiingoStock = nullptr;
	CSetTiingoStock setTiingoStock;

	if (m_lLastTotalTiingoStock < m_vTiingoStock.size()) {
		setTiingoStock.Open();
		setTiingoStock.m_pDatabase->BeginTrans();
		for (long l = m_lLastTotalTiingoStock; l < m_vTiingoStock.size(); l++) {
			pTiingoStock = m_vTiingoStock.at(l);
			pTiingoStock->Append(setTiingoStock);
		}
		setTiingoStock.m_pDatabase->CommitTrans();
		setTiingoStock.Close();
		m_lLastTotalTiingoStock = m_vTiingoStock.size();
	}

	return true;
}

bool CDataTiingoStock::LoadDB(void) {
	CSetTiingoStock setTiingoStock;
	CTiingoStockPtr pTiingoStock = nullptr;
	CString strSymbol = _T("");

	setTiingoStock.Open();
	while (!setTiingoStock.IsEOF()) {
		pTiingoStock = make_shared<CTiingoStock>();
		pTiingoStock->Load(setTiingoStock);
		m_mapTiingoStock[pTiingoStock->m_strTicker] = m_vTiingoStock.size();
		m_vTiingoStock.push_back(pTiingoStock);
		setTiingoStock.MoveNext();
	}
	setTiingoStock.Close();
	m_lLastTotalTiingoStock = m_vTiingoStock.size();

	return true;
}