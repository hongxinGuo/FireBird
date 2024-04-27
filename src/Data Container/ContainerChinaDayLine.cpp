#include"pch.h"

#include"TimeConvert.h"

#include"DayLine.h"
#include "ContainerChinaDayLine.h"
#include"SetDayLineBasicInfo.h"
#include"SetDayLineExtendInfo.h"

#include<memory>

#include "InfoReport.h"
using std::make_shared;

CContainerChinaDayLine::CContainerChinaDayLine() {}

bool CContainerChinaDayLine::SaveDB(const CString& strStockSymbol) {
	try {
		CSetDayLineBasicInfo setDayLineBasic;
		UpdateBasicDB(&setDayLineBasic, strStockSymbol);
	}
	catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}

	return true;
}

bool CContainerChinaDayLine::LoadDB(const CString& strStockSymbol) {
	CSetDayLineBasicInfo setDayLineBasicInfo;
	CSetDayLineExtendInfo setDayLineExtendInfo;

	ASSERT(!m_fBasicDataLoaded);

	// 装入DayLine数据
	setDayLineBasicInfo.m_strFilter = _T("[Symbol] = '");
	setDayLineBasicInfo.m_strFilter += strStockSymbol;
	setDayLineBasicInfo.m_strFilter += _T("'");
	setDayLineBasicInfo.m_strSort = _T("[Date]");
	setDayLineBasicInfo.Open();
	LoadBasicDB(&setDayLineBasicInfo);
	setDayLineBasicInfo.Close();

	// 装入DayLineExtendInfo数据
	setDayLineExtendInfo.m_strFilter = _T("[Symbol] = '");
	setDayLineExtendInfo.m_strFilter += strStockSymbol;
	setDayLineExtendInfo.m_strFilter += _T("'");
	setDayLineExtendInfo.m_strSort = _T("[Date]");
	setDayLineExtendInfo.Open();
	LoadExtendDB(&setDayLineExtendInfo);
	setDayLineExtendInfo.Close();

	m_fDataLoaded = true;
	ASSERT(!m_fBasicDataLoaded);

	return true;
}

bool CContainerChinaDayLine::BuildWeekLine(vector<CWeekLinePtr>& vWeekLine) const {
	ASSERT(IsDataLoaded());
	ASSERT(Size() > 0);
	long lCurrentDayLinePos = 0;
	CWeekLinePtr pWeekLine = nullptr;

	vWeekLine.clear();
	do {
		pWeekLine = CreateNewWeekLine(lCurrentDayLinePos);
		vWeekLine.push_back(pWeekLine);
	} while (lCurrentDayLinePos < Size());

	return true;
}

CWeekLinePtr CContainerChinaDayLine::CreateNewWeekLine(long& lCurrentDayLinePos) const {
	ASSERT(Size() > 0);
	ASSERT(lCurrentDayLinePos < Size());

	const long lNextMonday = GetNextMonday(GetData(lCurrentDayLinePos)->GetMarketDate());
	const long lNewestDay = GetData(Size() - 1)->GetMarketDate();
	auto pWeekLine = make_shared<CWeekLine>();
	if (lNextMonday < lNewestDay) {
		// 中间数据
		while (GetData(lCurrentDayLinePos)->GetMarketDate() < lNextMonday) { pWeekLine->UpdateWeekLine(dynamic_pointer_cast<CDayLine>(GetData(lCurrentDayLinePos++))); }
	}
	else {
		// 最后一组数据
		while (lCurrentDayLinePos <= (Size() - 1)) { pWeekLine->UpdateWeekLine(dynamic_pointer_cast<CDayLine>(GetData(lCurrentDayLinePos++))); }
	}

	if (pWeekLine->GetLastClose() > 0) { pWeekLine->SetUpDownRate(pWeekLine->GetUpDown() * 100 * 1000 / pWeekLine->GetLastClose()); }
	else { pWeekLine->SetUpDownRate(pWeekLine->GetUpDown() * 100 * 1000 / pWeekLine->GetOpen()); }

	return pWeekLine;
}
