#include "pch.h"

#include "ContainerVirtualStock.h"

CContainerVirtualStock::CContainerVirtualStock() {
	CContainerVirtualStock::Reset();
}

void CContainerVirtualStock::Reset() {
	CVirtualContainer::Reset();

	m_vStock.resize(0);
	m_mapSymbol.clear();
}

CString CContainerVirtualStock::GetItemSymbol(size_t lIndex) {
	return m_vStock.at(lIndex)->GetSymbol();
}

vectorString CContainerVirtualStock::GetSymbols() {
	vectorString vSymbol;
	for (long l = 0; l < Size(); l++) {
		vSymbol.push_back(static_cast<LPCTSTR>(GetItemSymbol(l)));
	}

	return vSymbol;
}

bool CContainerVirtualStock::IsUpdateProfileDB() noexcept {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return pStock->IsUpdateProfileDB(); });
}

bool CContainerVirtualStock::IsUpdateDayLine() noexcept {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return pStock->IsUpdateDayLine(); });
}

size_t CContainerVirtualStock::GetDayLineNeedUpdateNumber() const {
	size_t size = 0;
	for (auto pSymbol : m_vStock) {
		if (pSymbol->IsUpdateDayLine()) size++;
	}
	return size;
}

bool CContainerVirtualStock::IsUpdateDayLineDB() noexcept {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return pStock->IsUpdateDayLineDB(); });
}

void CContainerVirtualStock::Add(const CVirtualStockPtr& pStock) {
	if (pStock == nullptr) return;
	if (IsSymbol(pStock->GetSymbol())) return;

	m_mapSymbol[pStock->GetSymbol()] = m_vStock.size(); // 使用下标生成新的映射
	m_vStock.push_back(pStock);
}

void CContainerVirtualStock::Delete(const CVirtualStockPtr& pStock) {
	if (pStock == nullptr) return;
	if (!IsSymbol(pStock->GetSymbol())) return;

	m_vStock.erase(m_vStock.begin() + m_mapSymbol.at(pStock->GetSymbol()));
	m_mapSymbol.erase(pStock->GetSymbol());

	UpdateSymbolMap();
}

void CContainerVirtualStock::UpdateSymbolMap() {
	m_mapSymbol.clear();
	int j = 0;
	for (const auto& pStock : m_vStock) {
		m_mapSymbol[pStock->GetSymbol()] = j++;
	}
}

void CContainerVirtualStock::Sort() {
	std::ranges::sort(m_vStock, [](const CVirtualStockPtr& p1, const CVirtualStockPtr& p2) { return (p1->GetSymbol().Compare(p2->GetSymbol()) < 0); });
	UpdateSymbolMap();
}
