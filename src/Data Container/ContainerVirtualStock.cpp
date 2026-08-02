module;

module FireBirdLib.Container.Stock;

import FireBirdLib.Stock;

import std;
using std::vector;
using std::string;
using std::shared_ptr;

CContainerVirtualStock::CContainerVirtualStock() {
	CContainerVirtualStock::Reset();
}

void CContainerVirtualStock::Reset() {
	CVirtualContainer::Reset();

	m_vStock.clear();
	m_mapSymbol.clear();
}

string CContainerVirtualStock::GetItemSymbol(size_t lIndex) {
	return m_vStock.at(lIndex)->GetSymbol();
}

vector<string> CContainerVirtualStock::GetSymbols() {
	vector<string> vSymbol;
	for (size_t l = 0; l < Size(); l++) {
		vSymbol.push_back(GetItemSymbol(l));
	}

	return vSymbol;
}

bool CContainerVirtualStock::IsSymbol(const shared_ptr<CVirtualStock>& p) const {
	return IsSymbol(p->GetSymbol());
}
bool CContainerVirtualStock::IsUpdateProfileDB() noexcept {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return pStock->IsUpdateProfileDB(); });
}

bool CContainerVirtualStock::IsUpdateDayLine() noexcept {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return pStock->IsUpdateDayLine(); });
}

size_t CContainerVirtualStock::GetDayLineNeedUpdateNumber() const {
	size_t size = 0;
	for (const auto& pSymbol : m_vStock) {
		if (pSymbol->IsUpdateDayLine()) size++;
	}
	return size;
}

bool CContainerVirtualStock::IsUpdateDayLineDB() noexcept {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return pStock->IsUpdateDayLineDB(); });
}

shared_ptr<CVirtualStock> CContainerVirtualStock::Get(size_t lIndex) {
	return m_vStock.at(lIndex);
}

shared_ptr<CVirtualStock> CContainerVirtualStock::Get(const string& strSymbol) {
	return m_vStock.at(m_mapSymbol.at(strSymbol));
}

string CContainerVirtualStock::GetItemExchangeCode(const size_t lIndex) const {
	return m_vStock.at(lIndex)->GetExchange();
}

size_t CContainerVirtualStock::GetOffset(const shared_ptr<CVirtualStock>& pStock) const {
	return GetOffset(pStock->GetSymbol());
}

void CContainerVirtualStock::Add(const CVirtualStockPtr& pStock) {
	if (pStock == nullptr) return;
	if (IsSymbol(pStock->GetSymbol())) {
		return;
	}

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

void CContainerVirtualStock::Delete(const string& strSymbol) {
	if (!IsSymbol(strSymbol)) return;

	auto pStock = Get(strSymbol);
	Delete(pStock);
}

void CContainerVirtualStock::UpdateSymbolMap() {
	m_mapSymbol.clear();
	int j = 0;
	for (const auto& pStock : m_vStock) {
		m_mapSymbol[pStock->GetSymbol()] = j++;
	}
}

void CContainerVirtualStock::Sort() {
	std::ranges::sort(m_vStock, [](const CVirtualStockPtr& p1, const CVirtualStockPtr& p2) {
		auto str = p1->GetSymbol();
		return (str.compare(p2->GetSymbol()) < 0);
	});
	UpdateSymbolMap();
}
