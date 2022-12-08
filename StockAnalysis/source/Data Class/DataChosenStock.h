#pragma once

#include"WorldStock.h"

using namespace std;
#include<vector>
#include<map>

class CDataChosenStock : public CObject {
public:
	CDataChosenStock();
	~CDataChosenStock();
	void Reset(void);

	CWorldStockPtr GetStock(long lIndex) { return m_vWorldChosenStock.at(lIndex); }
	size_t GetSize(void) { return m_vWorldChosenStock.size(); }

	bool LoadDB(void);
	//bool UpdateDB(void);

protected:
	vector<CWorldStockPtr> m_vWorldChosenStock;
	map<CString, long> m_mapWorldChosenStock;
	long m_lChosenStockPos;
};
