#include "pch.h"

#include "DataWorldStockDayLine.h"

#include "InfoReport.h"
#include"SetWorldStockDayLine.h"

CDataWorldStockDayLine::CDataWorldStockDayLine() {}

bool CDataWorldStockDayLine::SaveDB(const CString& strStockSymbol) {
	try {
		CSetWorldStockDayLine setWorldStockDayLineBasic;
		UpdateBasicDB(&setWorldStockDayLineBasic, strStockSymbol);
	}
	catch (CException* e) {
		DeleteExceptionAndReportError(e);
	}

	return true;
}

bool CDataWorldStockDayLine::LoadDB(const CString& strStockSymbol) {
	try {
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
	}
	catch (CException* e) {
		DeleteExceptionAndReportError(e);
	}
	return true;
}
