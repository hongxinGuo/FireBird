#include"pch.h"

#include"TimeConvert.h"

#include"DayLine.h"
#include "DataChinaDayLine.h"
#include"SetDayLineBasicInfo.h"
#include"SetDayLineExtendInfo.h"

#include<memory>
using std::make_shared;

CDataChinaDayLine::CDataChinaDayLine() {}

bool CDataChinaDayLine::SaveDB(const CString& strStockSymbol) {
	CSetDayLineBasicInfo setDayLineBasic;
	UpdateBasicDB(&setDayLineBasic, strStockSymbol);

	return true;
}

bool CDataChinaDayLine::LoadDB(const CString& strStockSymbol) {
	CSetDayLineBasicInfo setDayLineBasicInfo;
	CSetDayLineExtendInfo setDayLineExtendInfo;

	ASSERT(!m_fLoadDataFirst);

	// װ��DayLine����
	setDayLineBasicInfo.m_strFilter = _T("[Symbol] = '");
	setDayLineBasicInfo.m_strFilter += strStockSymbol;
	setDayLineBasicInfo.m_strFilter += _T("'");
	setDayLineBasicInfo.m_strSort = _T("[Date]");
	setDayLineBasicInfo.Open();
	LoadBasicDB(&setDayLineBasicInfo);
	setDayLineBasicInfo.Close();

	// װ��DayLineExtendInfo����
	setDayLineExtendInfo.m_strFilter = _T("[Symbol] = '");
	setDayLineExtendInfo.m_strFilter += strStockSymbol;
	setDayLineExtendInfo.m_strFilter += _T("'");
	setDayLineExtendInfo.m_strSort = _T("[Date]");
	setDayLineExtendInfo.Open();
	LoadExtendDB(&setDayLineExtendInfo);
	setDayLineExtendInfo.Close();

	m_fDataLoaded = true;
	ASSERT(!m_fLoadDataFirst);

	return true;
}

bool CDataChinaDayLine::BuildWeekLine(vector<CWeekLinePtr>& vWeekLine) {
	ASSERT(IsDataLoaded());
	ASSERT(Size() > 0);
	long lCurrentDayLinePos = 0;
	CWeekLinePtr pWeekLine = nullptr;

	vWeekLine.clear();
	do {
		pWeekLine = CreateNewWeekLine(lCurrentDayLinePos);
		vWeekLine.push_back(pWeekLine);
	}
	while (lCurrentDayLinePos < Size());

	return true;
}

CWeekLinePtr CDataChinaDayLine::CreateNewWeekLine(long& lCurrentDayLinePos) {
	ASSERT(Size() > 0);
	ASSERT(lCurrentDayLinePos < Size());

	long lNextMonday = GetNextMonday(GetData(lCurrentDayLinePos)->GetMarketDate());
	long lNewestDay = GetData(Size() - 1)->GetMarketDate();
	auto pWeekLine = make_shared<CWeekLine>();
	if (lNextMonday < lNewestDay) {
		// �м�����
		while (GetData(lCurrentDayLinePos)->GetMarketDate() < lNextMonday) { pWeekLine->UpdateWeekLine(dynamic_pointer_cast<CDayLine>(GetData(lCurrentDayLinePos++))); }
	}
	else {
		// ���һ������
		while (lCurrentDayLinePos <= (Size() - 1)) { pWeekLine->UpdateWeekLine(dynamic_pointer_cast<CDayLine>(GetData(lCurrentDayLinePos++))); }
	}

	if (pWeekLine->GetLastClose() > 0) { pWeekLine->SetUpDownRate(pWeekLine->GetUpDown() * 100 * 1000 / pWeekLine->GetLastClose()); }
	else { pWeekLine->SetUpDownRate(pWeekLine->GetUpDown() * 100 * 1000 / pWeekLine->GetOpen()); }

	return pWeekLine;
}