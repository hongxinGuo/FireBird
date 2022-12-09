#pragma once

#include"WorldStock.h"

#include<vector>
#include<map>

class CDataChosenStock final : public CObject {
public:
	CDataChosenStock();
	~CDataChosenStock() override = default;
	void Reset(void);

	CWorldStockPtr GetStock(long lIndex) { return m_vWorldChosenStock.at(lIndex); }
	size_t GetSize(void) const noexcept { return m_vWorldChosenStock.size(); }

	bool LoadDB(void);
	//bool UpdateDB(void);

protected:
	vector<CWorldStockPtr> m_vWorldChosenStock;
	map<CString, long> m_mapWorldChosenStock;
	long m_lChosenStockPos;
};
