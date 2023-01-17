#pragma once

#include"ContainerVirtualStock.h"

class CContainerFinnhubCryptoSymbol : public CContainerVirtualStock {
public:
	CContainerFinnhubCryptoSymbol();
	~CContainerFinnhubCryptoSymbol() override = default;
	void Reset(void) override;

	bool LoadDB(void);
	bool UpdateDB(void);

protected:
	long m_lLastTotalSymbol;
};
