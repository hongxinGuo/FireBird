#pragma once

#include"WorldStock.h"
#include"ContainerVirtualStock.h"

class CContainerChosenStock : public CContainerVirtualStock {
public:
	CContainerChosenStock();
	~CContainerChosenStock() override = default;
	void Reset(void);

	bool LoadDB(void);
	//bool UpdateDB(void);

protected:
	long m_lChosenStockPos;
};