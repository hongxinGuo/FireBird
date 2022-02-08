#include "pch.h"

#include "DataWorldStockDayLine.h"
#include"SetWorldStockDayLine.h"

CDataWorldStockDayLine::CDataWorldStockDayLine() {
}

CDataWorldStockDayLine::~CDataWorldStockDayLine() {
}

bool CDataWorldStockDayLine::SaveDB(CString strStockSymbol) {
	CSetWorldStockDayLine setWorldStockDayLineBasic;
	UpdateBasicDB(&setWorldStockDayLineBasic, strStockSymbol);

	return true;
}

bool CDataWorldStockDayLine::LoadDB(CString strStockSymbol) {
	CSetWorldStockDayLine setDayLineBasic;

	// 装入DayLine数据
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