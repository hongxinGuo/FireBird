#pragma once

#include"VirtualStock.h"

#include"SetTiingoCrypto.h"

#include<memory>
#include<vector>

class CTiingoCrypto : public CVirtualStock {
public:
	CTiingoCrypto();
	~CTiingoCrypto() override = default;
	int GetRatio() const override { return 3; };

	void Load(const CSetTiingoCrypto& setTiingoCrypto);
	void Append(CSetTiingoCrypto& setTiingoCrypto);
	void Save(CSetTiingoCrypto& setTiingoCrypto);
	void Update(CSetTiingoCrypto& setTiingoCrypto);

	//string m_strTicker; //由VirtualStock中的m_strSymbol代替
	string m_strName{ "" };
	//string m_strDescription; // 已废弃。由CVirtualStock中的m_strDescription代替
	string m_strBaseCurrency{ "" };
	string m_strQuoteCurrency{ "" };
};

using CTiingoCryptoPtr = shared_ptr<CTiingoCrypto>;
using CTiingoCryptosPtr = shared_ptr<vector<CTiingoCryptoPtr>>;
