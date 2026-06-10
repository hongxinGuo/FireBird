#include"pch.h"
#include"SECFiling.h"

CSECFiling::CSECFiling() {
	Reset();
}

void CSECFiling::Reset() {
	m_strSymbol = " ";
	m_strAccessNumber = " ";
	m_strFilingURL = " ";
	m_strReportURL = " ";
	m_strForm = " ";
	m_iAcceptedDate = 0;
	m_iFiledDate = 0;
	m_iCIK = 0;
}
