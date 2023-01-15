#pragma once

#include"VirtualStock.h"

class CContainerVirtualStock {
public:
	CContainerVirtualStock();
	~CContainerVirtualStock() = default;

	bool IsInSymbolMap(const CString& strSymbol) const {
		if (m_mapSymbol.contains(strSymbol)) return true;
		return false;
	}

	CVirtualStockPtr Get(const long lIndex) { return m_vStock.at(lIndex); }
	CVirtualStockPtr Get(const CString& strSymbol) { return m_vStock.at(m_mapSymbol.at(strSymbol)); }

	size_t GetOffset(const CString& strSymbol) const { return m_mapSymbol.at(strSymbol); }

	bool Add(CVirtualStockPtr pStock);
	bool Delete(CVirtualStockPtr pStock);
	bool UpdateSymbolMap();
	void Sort();
	size_t Size() const { return m_vStock.size(); }

protected:
	vector<CVirtualStockPtr> m_vStock; // 本系统允许的所有股票池（无论代码是否存在）
	map<CString, size_t> m_mapSymbol;
};
