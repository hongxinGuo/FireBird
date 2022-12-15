#pragma once

#include"SetEPSSurprise.h"

#include<memory>

class CEPSSurprise final : public CObject {
public:
	CEPSSurprise();

	void Append(CSetEPSSurprise& setEPSSurprise);
	void Load(CSetEPSSurprise& setEPSSurprise);

public:
	CString m_strSymbol;
	long m_lDate;
	double m_dActual;
	double m_dEstimate;
};

typedef shared_ptr<CEPSSurprise> CEPSSurprisePtr;
typedef shared_ptr<vector<CEPSSurprisePtr>> CEPSSurpriseVectorPtr;
