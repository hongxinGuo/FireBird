#include "pch.h"
#include "DataForexDayLine.h"
#include"SetForexDayLine.h"

CDataForexDayLine::CDataForexDayLine() {
}

CDataForexDayLine::~CDataForexDayLine() {
}

bool CDataForexDayLine::SaveDB(CString strCryptoSymbol) {
	CSetForexDayLine setForexDayLineBasic;
	UpdateBasicDB(&setForexDayLineBasic, strCryptoSymbol);

	return true;
}

bool CDataForexDayLine::LoadDB(CString strCryptoSymbol) {
	CSetForexDayLine setDayLineBasic;

	// װ��DayLine����
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