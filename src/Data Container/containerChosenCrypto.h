#pragma once

#include"ContainerVirtualStock.h"

class CContianerChosenCrypto : public CContainerVirtualStock {
public:
	CContianerChosenCrypto();
	~CContianerChosenCrypto() override = default;
	void Reset(void);

	bool LoadDB(void);
	//bool UpdateDB(void);

protected:
	long m_lChosenCryptoPos;
};
