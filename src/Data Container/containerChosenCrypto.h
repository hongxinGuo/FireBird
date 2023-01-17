#pragma once

#include"ContainerVirtualStock.h"

class CContainerChosenCrypto : public CContainerVirtualStock {
public:
	CContainerChosenCrypto();
	~CContainerChosenCrypto() override = default;
	void Reset(void) override;

	bool LoadDB(void);
	//bool UpdateDB(void);

protected:
	long m_lChosenCryptoPos;
};
