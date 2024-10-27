#include "pch.h"

#include "ContainerFinnhubStockDayLine.h"

#include "InfoReport.h"
#include"SetWorldStockDayLine.h"

CContainerFinnhubStockDayLine::CContainerFinnhubStockDayLine() {}

bool CContainerFinnhubStockDayLine::SaveDB(const CString& strStockSymbol) {
	try {
		CSetWorldStockDayLine setWorldStockDayLineBasic;
		UpdateBasicDB(&setWorldStockDayLineBasic, strStockSymbol);
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}

	return true;
}

bool CContainerFinnhubStockDayLine::LoadDB(const CString& strStockSymbol) {
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
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
	return true;
}
