#include "pch.h"
#include "ContainerTiingoStock.h"
#include "InfoReport.h"

#include<memory>
using std::make_shared;

CContainerTiingoStock::CContainerTiingoStock() { Reset(); }

void CContainerTiingoStock::Reset() {
	m_vTiingoStock.resize(0);
	m_mapTiingoStock.clear();
	m_lLastTotalTiingoStock = 0;
}

void CContainerTiingoStock::Add(const CTiingoStockPtr& pTiingoStock) {
	m_mapTiingoStock[pTiingoStock->m_strTicker] = m_vTiingoStock.size();
	m_vTiingoStock.push_back(pTiingoStock);
}

bool CContainerTiingoStock::Delete(const CTiingoStockPtr& pTiingoStock) {
	if (pTiingoStock == nullptr) return false;
	if (!IsStock(pTiingoStock)) return false;

	m_vTiingoStock.erase(m_vTiingoStock.begin() + m_mapTiingoStock.at(pTiingoStock->m_strTicker));
	m_mapTiingoStock.erase(pTiingoStock->m_strTicker);

	return true;
}

bool CContainerTiingoStock::UpdateDB() {
	if (m_lLastTotalTiingoStock < m_vTiingoStock.size()) {
		try {
			CSetTiingoStock setTiingoStock;
			setTiingoStock.Open();
			setTiingoStock.m_pDatabase->BeginTrans();
			for (long l = m_lLastTotalTiingoStock; l < m_vTiingoStock.size(); l++) {
				const CTiingoStockPtr pTiingoStock = m_vTiingoStock.at(l);
				pTiingoStock->Append(setTiingoStock);
			}
			setTiingoStock.m_pDatabase->CommitTrans();
			setTiingoStock.Close();
			m_lLastTotalTiingoStock = m_vTiingoStock.size();
		}
		catch (CException* e) {
			ReportInformationAndDeleteException(e);
		}
	}

	return true;
}

bool CContainerTiingoStock::LoadDB() {
	CSetTiingoStock setTiingoStock;
	CString strSymbol = _T("");

	setTiingoStock.Open();
	setTiingoStock.m_pDatabase->BeginTrans();
	while (!setTiingoStock.IsEOF()) {
		if (!IsStock(setTiingoStock.m_Ticker)) {
			const auto pTiingoStock = make_shared<CTiingoStock>();
			pTiingoStock->Load(setTiingoStock);
			Add(pTiingoStock);
		}
		else { setTiingoStock.Delete(); }
		setTiingoStock.MoveNext();
	}
	setTiingoStock.m_pDatabase->CommitTrans();
	setTiingoStock.Close();
	m_lLastTotalTiingoStock = m_vTiingoStock.size();

	return true;
}
