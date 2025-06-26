#include"pch.h"

#include"InsiderSentiment.h"

CInsiderSentiment::CInsiderSentiment() {
	m_strSymbol = _T(" ");
	m_lDate = 19800101;
	m_lChange = 0;
	m_mspr = 0.0;
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
	m_strSymbol = setInsiderSentiment.m_Symbol;
	m_lDate = setInsiderSentiment.m_Date;
	m_lChange = setInsiderSentiment.m_Change;
	m_mspr = setInsiderSentiment.m_mspr;
}
