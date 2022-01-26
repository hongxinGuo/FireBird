#include"pch.h"

#include "DataChinaStock.h"
#include"CallableFunction.h"

CDataChinaStock::CDataChinaStock(CSetChinaStockSymbol* pset) {
	m_pSetChinaStockSymbol = pset;
	Reset();
}

CDataChinaStock::~CDataChinaStock() {
}

void CDataChinaStock::Reset(void) {
	m_vStock.resize(0);
	m_mapStock.clear();
	m_lLastTotalStock = 0;
}

bool CDataChinaStock::SortStock(void) {
	sort(m_vStock.begin(), m_vStock.end(), CompareChinaStock);
	m_mapStock.clear();
	int j = 0;
	for (auto& pStock : m_vStock) {
		m_mapStock[pStock->GetSymbol()] = j++;
	}

	return true;
}

bool CDataChinaStock::Delete(CChinaStockPtr pStock) {
	if (pStock == nullptr) return false;
	if (!IsStock(pStock->GetSymbol())) return false;

	m_vStock.erase(m_vStock.begin() + m_mapStock.at(pStock->GetSymbol()));
	m_mapStock.erase(pStock->GetSymbol());

	return true;
}

void CDataChinaStock::Add(CChinaStockPtr pStock) {
	m_mapStock[pStock->GetSymbol()] = m_vStock.size();
	m_vStock.push_back(pStock);
}

bool CDataChinaStock::LoadDB(void) {
	return false;
}

bool CDataChinaStock::UpdateDB(void) {
	return false;
}