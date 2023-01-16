#pragma once

#include"ContainerVirtualStock.h"

class CDataChosenForex : public CContainerVirtualStock {
public:
	CDataChosenForex();
	~CDataChosenForex() override = default;
	void Reset(void);

	bool LoadDB(void);
	//bool UpdateDB(void);

protected:
	long m_lChosenForexPos;
};
