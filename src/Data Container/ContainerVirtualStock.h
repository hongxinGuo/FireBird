#pragma once

#include"VirtualStock.h"

#include<vector>
#include<map>
using std::vector;
using std::map;

class CContainerVirtualStock {
public:
	CContainerVirtualStock();
	virtual ~CContainerVirtualStock() = default;

	virtual void Reset();

	bool IsSymbol(const CString& strSymbol) const { return m_mapSymbol.contains(strSymbol); }
	bool IsUpdateProfileDB() noexcept;
	bool IsDayLineNeedUpdate() noexcept;
	bool IsDayLineNeedSaving() noexcept;

	CVirtualStockPtr Get(const size_t lIndex) { return m_vStock.at(lIndex); }
	CVirtualStockPtr Get(const CString& strSymbol) { return m_vStock.at(m_mapSymbol.at(strSymbol)); }

	size_t GetOffset(const CString& strSymbol) const { return m_mapSymbol.at(strSymbol); }

	void Add(const CVirtualStockPtr& pStock);
	void Delete(const CVirtualStockPtr& pStock);
	void UpdateSymbolMap();
	void Sort();
	size_t Size() const { return m_vStock.size(); }

protected:
	vector<CVirtualStockPtr> m_vStock; //此容器中真实存储的，为CVirtualStock类的各继承类，使用时需要使用dynamic_pointer_cast<>转换成对应的继承类。
	map<CString, size_t> m_mapSymbol; // m_vStock在容器中的具体偏移量。
};
