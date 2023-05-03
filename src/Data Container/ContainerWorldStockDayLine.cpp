#include "pch.h"

#include "ContainerWorldStockDayLine.h"

#include "InfoReport.h"
#include"SetWorldStockDayLine.h"

CContainerWorldStockDayLine::CContainerWorldStockDayLine() {}

bool CContainerWorldStockDayLine::SaveDB(const CString& strStockSymbol) {
	try {
		CSetWorldStockDayLine setWorldStockDayLineBasic;
		UpdateBasicDB(&setWorldStockDayLineBasic, strStockSymbol);
	}
	catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}

	return true;
}

bool CContainerWorldStockDayLine::LoadDB(const CString& strStockSymbol) {
	try {
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
	}
	catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
	return true;
}
