#include "pch.h"
#include "ContainerCryptoDayLine.h"

#include "InfoReport.h"
#include"SetCryptoDayLine.h"

CContainerCryptoDayLine::CContainerCryptoDayLine() {}

bool CContainerCryptoDayLine::SaveDB(const CString& strCryptoSymbol) {
	try {
		CSetCryptoDayLine setCryptoDayLineBasic;
		UpdateBasicDB(&setCryptoDayLineBasic, strCryptoSymbol);
	}
	catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}

	return true;
}

bool CContainerCryptoDayLine::LoadDB(const CString& strCryptoSymbol) {
	CSetCryptoDayLine setDayLineBasic;

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
