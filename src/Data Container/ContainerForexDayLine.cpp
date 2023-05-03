#include "pch.h"
#include "ContainerForexDayLine.h"

#include "InfoReport.h"
#include"SetForexDayLine.h"

CContainerForexDayLine::CContainerForexDayLine() {}

bool CContainerForexDayLine::SaveDB(const CString& strCryptoSymbol) {
	try {
		CSetForexDayLine setForexDayLineBasic;
		UpdateBasicDB(&setForexDayLineBasic, strCryptoSymbol);
	}
	catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}

	return true;
}

bool CContainerForexDayLine::LoadDB(const CString& strCryptoSymbol) {
	CSetForexDayLine setDayLineBasic;

	// 装入DayLine数据
	setDayLineBasic.m_strFilter = _T("[Symbol] = '");
	setDayLineBasic.m_strFilter += strCryptoSymbol;
	setDayLineBasic.m_strFilter += _T("'");
	setDayLineBasic.m_strSort = _T("[Date]");
	setDayLineBasic.Open();
	LoadBasicDB(&setDayLineBasic);
	setDayLineBasic.Close();

	m_fDataLoaded = true;

	return true;
}
