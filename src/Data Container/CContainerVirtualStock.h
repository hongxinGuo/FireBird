#pragma once

#include"VirtualStock.h"

class CContainerVirtualStock {
public:
	CContainerVirtualStock();
	~CContainerVirtualStock();

	bool Add(CVirtualStockPtr pStock);
	bool Delete(CVirtualStockPtr pStock);
	void Sort();

protected:
	vector<CChinaStockPtr> m_vStock; // 本系统允许的所有股票池（无论代码是否存在）
	long m_lLoadedStock; // 本次装载的股票总数
};
