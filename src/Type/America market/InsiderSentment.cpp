#include"pch.h"

#include "CharSetTransfer.h"
#include"InsiderSentiment.h"

CInsiderSentiment::CInsiderSentiment() {
	m_strSymbol = " ";
	m_lDate = 19800101;
	m_lChange = 0;
	m_mspr = 0.0;
}

CInsiderSentiment::CInsiderSentiment(const CInsiderSentiment& other) {
	m_strSymbol = other.m_strSymbol;
	m_lDate = other.m_lDate;
	m_lChange = other.m_lChange;
	m_mspr = other.m_mspr;
}

CInsiderSentiment& CInsiderSentiment::operator=(const CInsiderSentiment& other) {
	if (this == &other) return *this;
	m_strSymbol = other.m_strSymbol;
	m_lDate = other.m_lDate;
	m_lChange = other.m_lChange;
	m_mspr = other.m_mspr;
	return *this;
}

CInsiderSentiment::CInsiderSentiment(CInsiderSentiment&& other) noexcept {
	m_strSymbol = std::move(other.m_strSymbol);
	m_lDate = other.m_lDate;
	m_lChange = other.m_lChange;
	m_mspr = other.m_mspr;
}

CInsiderSentiment& CInsiderSentiment::operator=(CInsiderSentiment&& other) noexcept {
	if (this == &other) return *this;
	m_strSymbol = std::move(other.m_strSymbol);
	m_lDate = other.m_lDate;
	m_lChange = other.m_lChange;
	m_mspr = other.m_mspr;
	return *this;
}

void CInsiderSentiment::Append(CSetInsiderSentiment& setInsiderSentiment) const {
	setInsiderSentiment.AddNew();
	Save(setInsiderSentiment);
	setInsiderSentiment.Update();
}

void CInsiderSentiment::Update(CSetInsiderSentiment& setInsiderSentiment) const {
	setInsiderSentiment.Edit();
	Save(setInsiderSentiment);
	setInsiderSentiment.Update();
}

void CInsiderSentiment::Save(CSetInsiderSentiment& setInsiderSentiment) const {
	setInsiderSentiment.m_Symbol = m_strSymbol.c_str();
	setInsiderSentiment.m_Date = m_lDate;
	setInsiderSentiment.m_Change = m_lChange;
	setInsiderSentiment.m_mspr = m_mspr;
}

void CInsiderSentiment::Load(const CSetInsiderSentiment& setInsiderSentiment) {
	m_strSymbol = T2Utf8(setInsiderSentiment.m_Symbol);
	m_lDate = setInsiderSentiment.m_Date;
	m_lChange = setInsiderSentiment.m_Change;
	m_mspr = setInsiderSentiment.m_mspr;
}
