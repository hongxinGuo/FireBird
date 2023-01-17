#pragma once

#include"ContainerVirtualStock.h"

class CContainerChosenForex : public CContainerVirtualStock {
public:
	CContainerChosenForex();
	~CContainerChosenForex() override = default;
	void Reset(void) override;

	bool LoadDB(void);
	//bool UpdateDB(void);

protected:
	long m_lChosenForexPos;
};
