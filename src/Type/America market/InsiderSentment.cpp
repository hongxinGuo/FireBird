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
