#include "pch.h"

#include "ContainerTiingoStockDayLine.h"

#include "InfoReport.h"
#include"SetTiingoStockDayLine.h"

CContainerTiingoStockDayLine::CContainerTiingoStockDayLine() {}

bool CContainerTiingoStockDayLine::SaveDB(const CString& strStockSymbol) {
	try {
		CSetTiingoStockDayLine setTiingoStockDayLineBasic;
		UpdateBasicDB(&setTiingoStockDayLineBasic, strStockSymbol);
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}

	return true;
}

bool CContainerTiingoStockDayLine::LoadDB(const CString& strStockSymbol) {
	try {
		CSetTiingoStockDayLine setDayLineBasic;

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
