#include"pch.h"

#include"EPSSurprise.h"

CEPSSurprise::CEPSSurprise() {
	m_strSymbol = _T(" ");
	m_lDate = 0;
	m_dActual = 0.0;
	m_dEstimate = 0.0;
}

void CEPSSurprise::Append(CSetEPSSurprise& setEPSSurprise) const {
	setEPSSurprise.AddNew();
	setEPSSurprise.m_Symbol = m_strSymbol;
	setEPSSurprise.m_Date = m_lDate;
	setEPSSurprise.m_Actual = m_dActual;
	setEPSSurprise.m_Estimate = m_dEstimate;
	setEPSSurprise.Update();
}

void CEPSSurprise::Load(const CSetEPSSurprise& setEPSSurprise) {
	m_strSymbol = setEPSSurprise.m_Symbol;
	m_lDate = setEPSSurprise.m_Date;
	m_dActual = setEPSSurprise.m_Actual;
	m_dEstimate = setEPSSurprise.m_Estimate;
}
