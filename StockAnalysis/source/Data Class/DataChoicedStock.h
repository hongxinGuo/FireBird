#pragma once

#include"WorldStock.h"

using namespace std;
#include<vector>
#include<map>

class CDataChoicedStock : CObject {
public:
	CDataChoicedStock();
	~CDataChoicedStock();
	void Reset(void);

	CWorldStockPtr GetChoicedStock(long lIndex) { return m_vWorldChoicedStock.at(lIndex); }
	size_t GetSize(void) { return m_vWorldChoicedStock.size(); }

	bool LoadDB(void);
	//bool UpdateDB(void);

protected:
	vector<CWorldStockPtr> m_vWorldChoicedStock;
	map<CString, long> m_mapWorldChoicedStock;
	long m_lChoicedStockPos;
};
