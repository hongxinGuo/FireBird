#include "pch.h"
#include "DataForexDayLine.h"
#include"SetForexDayLine.h"

CDataForexDayLine::CDataForexDayLine() {
}

bool CDataForexDayLine::SaveDB(const CString& strCryptoSymbol) {
	CSetForexDayLine setForexDayLineBasic;
	UpdateBasicDB(&setForexDayLineBasic, strCryptoSymbol);

	return true;
}

bool CDataForexDayLine::LoadDB(const CString& strCryptoSymbol) {
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
