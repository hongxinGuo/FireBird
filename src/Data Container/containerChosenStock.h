#pragma once

#include"ContainerVirtualStock.h"

class CContainerChosenStock : public CContainerVirtualStock {
public:
	CContainerChosenStock();
	~CContainerChosenStock() override = default;
	void Reset(void) override;

	bool LoadDB(void);
	//bool UpdateDB(void);

	CWorldStockPtr GetStock(const size_t lIndex) { return dynamic_pointer_cast<CWorldStock>(Get(lIndex)); };
	CWorldStockPtr GetStock(const CString& strStockCode) { return dynamic_pointer_cast<CWorldStock>(Get(strStockCode)); };

protected:
	long m_lChosenStockPos;
};
