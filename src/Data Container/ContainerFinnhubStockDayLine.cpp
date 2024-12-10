#include "pch.h"
module;
#include"SetFinnhubStockDayLine.h"
module FireBird.Container.FinnhubStockDayLine;

import FireBird.Accessory.InfoReport;

CContainerFinnhubStockDayLine::CContainerFinnhubStockDayLine() {}

bool CContainerFinnhubStockDayLine::SaveDB(const CString& strStockSymbol) {
	try {
		CSetFinnhubStockDayLine setFinnhubStockDayLineBasic;
		UpdateBasicDB(&setFinnhubStockDayLineBasic, strStockSymbol);
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}

	return true;
}

bool CContainerFinnhubStockDayLine::LoadDB(const CString& strStockSymbol) {
	try {
		CSetFinnhubStockDayLine setDayLineBasic;

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
