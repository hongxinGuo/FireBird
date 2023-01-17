#pragma once

#include"ContainerVirtualStock.h"

class CContainerFinnhubForexSymbol : public CContainerVirtualStock {
public:
	CContainerFinnhubForexSymbol();
	~CContainerFinnhubForexSymbol() override = default;
	void Reset(void) override;

	bool LoadDB(void);
	bool UpdateDB(void);

protected:
	long m_lLastTotalSymbol;
};
