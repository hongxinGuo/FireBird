#include "pch.h"
#include "DataCryptoDayLine.h"

#include "InfoReport.h"
#include"SetCryptoDayLine.h"

CDataCryptoDayLine::CDataCryptoDayLine() {}

bool CDataCryptoDayLine::SaveDB(const CString& strCryptoSymbol) {
	try {
		CSetCryptoDayLine setCryptoDayLineBasic;
		UpdateBasicDB(&setCryptoDayLineBasic, strCryptoSymbol);
	}
	catch (CException* e) {
		DeleteExceptionAndReportError(e);
	}

	return true;
}

bool CDataCryptoDayLine::LoadDB(const CString& strCryptoSymbol) {
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
