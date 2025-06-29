#include "pch.h"

#include "ContainerFinnhubStockDayLine.h"

#include "InfoReport.h"
#include"SetFinnhubStockDayLine.h"

CContainerFinnhubStockDayLine::CContainerFinnhubStockDayLine() {}

bool CContainerFinnhubStockDayLine::SaveDB(const string& strStockSymbol) {
	try {
		CSetFinnhubStockDayLine setFinnhubStockDayLineBasic;
		UpdateBasicDB(&setFinnhubStockDayLineBasic, strStockSymbol);
	} catch (CException& e) {
		ReportInformation(e);
	}

	return true;
}

bool CContainerFinnhubStockDayLine::LoadDB(const string& strStockSymbol) {
	CSetFinnhubStockDayLine setDayLineBasic;

	// 装入DayLine数据
	setDayLineBasic.m_strFilter = _T("[Symbol] = '");
	setDayLineBasic.m_strFilter += strStockSymbol.c_str();
	setDayLineBasic.m_strFilter += _T("'");
	setDayLineBasic.m_strSort = _T("[Date]");
	setDayLineBasic.Open();
	LoadBasicDB(&setDayLineBasic);
	setDayLineBasic.Close();

	m_fDataLoaded = true;
	return true;
}
