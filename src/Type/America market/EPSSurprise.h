#pragma once

#include"SetEPSSurprise.h"

#include<memory>

class CEPSSurprise final {
public:
	CEPSSurprise();
	CEPSSurprise(const CEPSSurprise&) = default;
	CEPSSurprise& operator=(const CEPSSurprise&) = default;
	CEPSSurprise(CEPSSurprise&&) noexcept = default;
	CEPSSurprise& operator=(CEPSSurprise&&) noexcept = default;
	~CEPSSurprise() = default;

	void Append(CSetEPSSurprise& setEPSSurprise) const;
	void Load(const CSetEPSSurprise& setEPSSurprise);

public:
	string m_strSymbol;
	long m_lDate;
	double m_dActual;
	double m_dEstimate;
};

using CEPSSurprisePtr = shared_ptr<CEPSSurprise>;
using CEPSSurprisesPtr = shared_ptr<vector<CEPSSurprise>>;
