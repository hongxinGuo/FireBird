#pragma once

#include"ContainerVirtualStock.h"

class CContainerChosenStock : public CContainerVirtualStock {
public:
	CContainerChosenStock();
	~CContainerChosenStock() override = default;
	void Reset(void) override;

	bool LoadDB(void);
	//bool UpdateDB(void);

protected:
	long m_lChosenStockPos;
};
