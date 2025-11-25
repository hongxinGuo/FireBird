#include"pch.h"
#include"SECFiling.h"

CSECFiling::CSECFiling() {
	m_strSymbol = " ";
	m_strAccessNumber = " ";
	m_strFilingURL = " ";
	m_strReportURL = " ";
	m_strForm = " ";
	m_iAcceptedDate = 0;
	m_iFiledDate = 0;
	m_iCIK = 0;
}

void CSECFiling::Append(CSetSECFilings& setSECFiling) const {
	setSECFiling.AddNew();
	setSECFiling.m_Symbol = m_strSymbol.c_str();
	setSECFiling.m_AccessNumber = m_strAccessNumber.c_str();
	setSECFiling.m_CIK = m_iCIK;
	setSECFiling.m_FiledDate = m_iFiledDate;
	setSECFiling.m_AcceptedDate = m_iAcceptedDate;
	setSECFiling.m_FilingURL = m_strFilingURL.c_str();
	setSECFiling.m_ReportURL = m_strReportURL.c_str();
	setSECFiling.m_Form = m_strForm.c_str();
	setSECFiling.Update();
}

void CSECFiling::Load(const CSetSECFilings& setSECFiling) {
	m_strSymbol = ToUTF8(setSECFiling.m_Symbol);
	m_strAccessNumber = ToUTF8(setSECFiling.m_AccessNumber);
	m_iCIK = setSECFiling.m_CIK;
	m_iFiledDate = setSECFiling.m_FiledDate;
	m_iAcceptedDate = setSECFiling.m_AcceptedDate;
	m_strFilingURL = ToUTF8(setSECFiling.m_FilingURL);
	m_strReportURL = ToUTF8(setSECFiling.m_ReportURL);
	m_strForm = ToUTF8(setSECFiling.m_Form);
}
