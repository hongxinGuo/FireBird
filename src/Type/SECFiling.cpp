#include"pch.h"
#include"SECFiling.h"

CSECFiling::CSECFiling() {
	m_strSymbol = _T(" ");
	m_strAccessNumber = _T(" ");
	m_strFilingURL = _T(" ");
	m_strReportURL = _T(" ");
	m_strForm = _T(" ");
	m_iAcceptedDate = 0;
	m_iFiledDate = 0;
	m_iCIK = 0;
}

void CSECFiling::Append(CSetSECFilings& setSECFiling) const {
	setSECFiling.AddNew();
	setSECFiling.m_Symbol = m_strSymbol;
	setSECFiling.m_AccessNumber = m_strAccessNumber;
	setSECFiling.m_CIK = m_iCIK;
	setSECFiling.m_FiledDate = m_iFiledDate;
	setSECFiling.m_AcceptedDate = m_iAcceptedDate;
	setSECFiling.m_FilingURL = m_strFilingURL;
	setSECFiling.m_ReportURL = m_strReportURL;
	setSECFiling.m_Form = m_strForm;
	setSECFiling.Update();
}

void CSECFiling::Load(const CSetSECFilings& setSECFiling) {
	m_strSymbol = setSECFiling.m_Symbol;
	m_strAccessNumber = setSECFiling.m_AccessNumber;
	m_iCIK = setSECFiling.m_CIK;
	m_iFiledDate = setSECFiling.m_FiledDate;
	m_iAcceptedDate = setSECFiling.m_AcceptedDate;
	m_strFilingURL = setSECFiling.m_FilingURL;
	m_strReportURL = setSECFiling.m_ReportURL;
	m_strForm = setSECFiling.m_Form;
}
