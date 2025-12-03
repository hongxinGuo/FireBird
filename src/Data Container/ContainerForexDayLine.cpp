#include "pch.h"
#include "ContainerForexDayLine.h"

#include "InfoReport.h"
#include"SetForexDayLine.h"

CContainerForexDayLine::CContainerForexDayLine() {}

bool CContainerForexDayLine::SaveDB(const string& strCryptoSymbol) {
	try {
		CSetForexDayLine setForexDayLineBasic;
		UpdateBasicDB(&setForexDayLineBasic, strCryptoSymbol);
	} catch (CException& e) {
		ReportInformation(e);
	}

	return true;
}

bool CContainerForexDayLine::LoadDB(const string& strCryptoSymbol) {
	CSetForexDayLine setDayLineBasic;

	// 装入DayLine数据
	setDayLineBasic.m_strFilter = "[Symbol] = '";
	setDayLineBasic.m_strFilter += strCryptoSymbol.c_str();
	setDayLineBasic.m_strFilter += "'";
	setDayLineBasic.m_strSort = "[Date]";
	setDayLineBasic.Open();
	LoadBasicDB(&setDayLineBasic);
	setDayLineBasic.Close();

	m_fDataLoaded = true;
	int aasdfd = 0;

	return true;
}
