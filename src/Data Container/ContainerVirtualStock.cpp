﻿#include "pch.h"

#include "ContainerVirtualStock.h"

using namespace std;

CContainerVirtualStock::CContainerVirtualStock() {
}

void CContainerVirtualStock::Reset() {
	m_vStock.resize(0);
	m_mapSymbol.clear();
}

bool CContainerVirtualStock::IsUpdateProfileDB(void) {
	return ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return pStock->IsUpdateProfileDB(); });
}

bool CContainerVirtualStock::Add(CVirtualStockPtr pStock) {
	if (pStock == nullptr) return false;
	if (IsInSymbolMap(pStock->GetSymbol())) return false;

	m_mapSymbol[pStock->GetSymbol()] = m_vStock.size(); // 使用下标生成新的映射
	m_vStock.push_back(pStock);

	return true;
}

bool CContainerVirtualStock::Delete(CVirtualStockPtr pStock) {
	if (pStock == nullptr) return false;
	if (!IsInSymbolMap(pStock->GetSymbol())) return false;

	m_vStock.erase(m_vStock.begin() + m_mapSymbol.at(pStock->GetSymbol()));
	m_mapSymbol.erase(pStock->GetSymbol());

	UpdateSymbolMap();

	return true;
}

bool CContainerVirtualStock::UpdateSymbolMap() {
	m_mapSymbol.clear();
	int j = 0;
	for (const auto& pStock : m_vStock) {
		m_mapSymbol[pStock->GetSymbol()] = j++;
	}
	return false;
}

void CContainerVirtualStock::Sort() {
	ranges::sort(m_vStock, [](CVirtualStockPtr& p1, CVirtualStockPtr p2) { return (p1->GetSymbol().Compare(p2->GetSymbol()) < 0); });
	UpdateSymbolMap();
}