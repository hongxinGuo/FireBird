#include "pch.h"

#include "DataWorldStockDayLine.h"
#include"SetWorldStockDayLine.h"

CDataWorldStockDayLine::CDataWorldStockDayLine() {
}

bool CDataWorldStockDayLine::SaveDB(const CString& strStockSymbol) {
	CSetWorldStockDayLine setWorldStockDayLineBasic;
	UpdateBasicDB(&setWorldStockDayLineBasic, strStockSymbol);

	return true;
}

bool CDataWorldStockDayLine::LoadDB(const CString& strStockSymbol) {
	CSetWorldStockDayLine setDayLineBasic;

	// װ��DayLine����
	setDayLineBasic.m_strFilter = _T("[Symbol] = '");
	setDayLineBasic.m_strFilter += strStockSymbol;
	setDayLineBasic.m_strFilter += _T("'");
	setDayLineBasic.m_strSort = _T("[Date]");
	setDayLineBasic.Open();
	LoadBasicDB(&setDayLineBasic);
	setDayLineBasic.Close();

	m_fDataLoaded = true;

	return true;
}