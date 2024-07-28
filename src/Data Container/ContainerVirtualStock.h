#pragma once

#include"VirtualStock.h"
#include "VirtualContainer.h"

using vectorString = vector<string>;

class CContainerVirtualStock : public CVirtualContainer {
public:
	CContainerVirtualStock();
	~CContainerVirtualStock() override = default ;

	void Reset() override;
	CString GetItemSymbol(long lIndex) override;
	vectorString GetSymbols();

	bool IsEmpty() override { return m_vStock.empty(); }
	size_t Size() override { return m_vStock.size(); }

	bool IsSymbol(const CString& strSymbol) const { return m_mapSymbol.contains(strSymbol); }
	bool IsSymbol(const CVirtualStockPtr& p) const { return IsSymbol(p->GetSymbol()); }
	bool IsUpdateProfileDB() noexcept;
	bool IsDayLineNeedUpdate() noexcept;
	bool IsDayLineNeedSaving() noexcept;

	CVirtualStockPtr Get(const size_t lIndex) { return m_vStock.at(lIndex); }
	CVirtualStockPtr Get(const CString& strSymbol) { return m_vStock.at(m_mapSymbol.at(strSymbol)); }

	size_t GetOffset(const CString& strSymbol) const { return m_mapSymbol.at(strSymbol); }
	size_t GetOffset(const CChinaStockPtr& pStock) const { return GetOffset(pStock->GetSymbol()); }

	void Add(const CVirtualStockPtr& pStock);
	void Delete(const CVirtualStockPtr& pStock);
	void UpdateSymbolMap();
	void Sort();

protected:
	vector<CVirtualStockPtr> m_vStock; //此容器中真实存储的，为CVirtualStock类的各继承类，使用时需要使用dynamic_pointer_cast<>转换成对应的继承类。
	map<CString, size_t> m_mapSymbol; // m_vStock在容器中的具体偏移量。
};
