#include "pch.h"
#include "ContainerForexDayLine.h"

#include "InfoReport.h"
#include"SetForexDayLine.h"

CContainerForexDayLine::CContainerForexDayLine() {}

bool CContainerForexDayLine::SaveDB(const string& strCryptoSymbol) {
	try {
		CSetForexDayLine setForexDayLineBasic;
		UpdateBasicDB(&setForexDayLineBasic, strCryptoSymbol);
	} catch (CException& e) {
		ReportInformation(e);
	}

	return true;
}

bool CContainerForexDayLine::LoadDB(const string& strCryptoSymbol) {
	CSetForexDayLine setDayLineBasic;

	// װ��DayLine����
	setDayLineBasic.m_strFilter = _T("[Symbol] = '");
	setDayLineBasic.m_strFilter += strCryptoSymbol.c_str();
	setDayLineBasic.m_strFilter += _T("'");
	setDayLineBasic.m_strSort = _T("[Date]");
	setDayLineBasic.Open();
	LoadBasicDB(&setDayLineBasic);
	setDayLineBasic.Close();

	m_fDataLoaded = true;

	return true;
}
