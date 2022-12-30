#pragma once

#include"WorldStock.h"

class CDataChosenStock final {
public:
	CDataChosenStock();
	virtual ~CDataChosenStock() = default;
	void Reset(void);

	CWorldStockPtr GetStock(const long lIndex) { return m_vWorldChosenStock.at(lIndex); }
	[[nodiscard]] long GetSize(void) const noexcept { return static_cast<long>(m_vWorldChosenStock.size()); }

	bool LoadDB(void);
	//bool UpdateDB(void);

protected:
	vector<CWorldStockPtr> m_vWorldChosenStock;
	map<CString, size_t> m_mapWorldChosenStock;
	long m_lChosenStockPos;
};
