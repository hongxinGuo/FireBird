#include"pch.h"

#include "DataChinaStock.h"

CDataChinaStock::CDataChinaStock(CSetChinaStockSymbol* pset) {
	m_pSetChinaStockSymbol = pset;
	Reset();
}

CDataChinaStock::~CDataChinaStock() {
}

void CDataChinaStock::Reset(void) {
	m_vChinaStock.resize(0);
	m_mapChinaStock.clear();
	m_lLastTotalStock = 0;
}

bool CDataChinaStock::SortStock(void) {
	return false;
}

bool CDataChinaStock::Delete(CChinaStockPtr pStock) {
	return false;
}

void CDataChinaStock::Add(CChinaStockPtr pStock) {
}

bool CDataChinaStock::LoadDB(void) {
	return false;
}

bool CDataChinaStock::UpdateDB(void) {
	return false;
}