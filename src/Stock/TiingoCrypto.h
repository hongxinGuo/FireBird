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

	void Load(CSetTiingoCrypto& setTiingoCrypto);
	void Append(CSetTiingoCrypto& setTiingoCrypto);
	void Save(CSetTiingoCrypto& setTiingoCrypto);
	void Update(CSetTiingoCrypto& setTiingoCrypto);

	//string m_strTicker; //��VirtualStock�е�m_strSymbol����
	string m_strName{ _T("") };
	//string m_strDescription; // �ѷ�������CVirtualStock�е�m_strDescription����
	string m_strBaseCurrency{ _T("") };
	string m_strQuoteCurrency{ _T("") };
};

using CTiingoCryptoPtr = shared_ptr<CTiingoCrypto>;
using CTiingoCryptosPtr = shared_ptr<vector<CTiingoCryptoPtr>>;
