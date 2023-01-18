#pragma once

#include"SetEPSSurprise.h"

#include<memory>

class CEPSSurprise final {
public:
	CEPSSurprise();
	~CEPSSurprise() = default;

	void Append(CSetEPSSurprise& setEPSSurprise);
	void Load(CSetEPSSurprise& setEPSSurprise);

public:
	CString m_strSymbol;
	long m_lDate;
	double m_dActual;
	double m_dEstimate;
};

using CEPSSurprisePtr = shared_ptr<CEPSSurprise>;
using CEPSSurpriseVectorPtr = shared_ptr<vector<CEPSSurprisePtr>>;
