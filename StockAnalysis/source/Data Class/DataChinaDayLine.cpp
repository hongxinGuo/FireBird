#include"pch.h"
#include"globedef.h"

#include "DataChinaDayLine.h"

CDataChinaDayLine::CDataChinaDayLine() {
}

CDataChinaDayLine::~CDataChinaDayLine() {
}

bool CDataChinaDayLine::SaveDB(CString strStockSymbol) {
	CSetDayLineBasicInfo setDayLineBasic;
	UpdateBasicDB(&setDayLineBasic, strStockSymbol);

	return true;
}

bool CDataChinaDayLine::LoadDB(CString strStockSymbol) {
	CSetDayLineBasicInfo setDayLineBasicInfo;
	CSetDayLineExtendInfo setDayLineExtendInfo;

	ASSERT(!m_fLoadDataFirst);

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
	ASSERT(!m_fLoadDataFirst);

	return true;
}

bool CDataChinaDayLine::BuildWeekLine(vector<CWeekLinePtr>& vWeekLine) {
	ASSERT(IsDataLoaded());
	ASSERT(GetDataSize() > 0);
	long lCurrentDayLinePos = 0;
	CWeekLinePtr pWeekLine = nullptr;

	vWeekLine.clear();
	do {
		pWeekLine = CreateNewWeekLine(lCurrentDayLinePos);
		vWeekLine.push_back(pWeekLine);
	} while (lCurrentDayLinePos < GetDataSize());

	return true;
}

CWeekLinePtr CDataChinaDayLine::CreateNewWeekLine(long& lCurrentDayLinePos) {
	ASSERT(GetDataSize() > 0);
	ASSERT(lCurrentDayLinePos < GetDataSize());

	long lNextMonday = GetNextMonday(GetData(lCurrentDayLinePos)->GetMarketDate());
	long lNewestDay = GetData(GetDataSize() - 1)->GetMarketDate();
	CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
	if (lNextMonday < lNewestDay) { // 中间数据
		while (GetData(lCurrentDayLinePos)->GetMarketDate() < lNextMonday) {
			pWeekLine->UpdateWeekLine(dynamic_pointer_cast<CDayLine>(GetData(lCurrentDayLinePos++)));
		}
	}
	else { // 最后一组数据
		while (lCurrentDayLinePos <= (GetDataSize() - 1)) {
			pWeekLine->UpdateWeekLine(dynamic_pointer_cast<CDayLine>(GetData(lCurrentDayLinePos++)));
		}
	}

	if (pWeekLine->GetLastClose() > 0) {
		pWeekLine->SetUpDownRate(pWeekLine->GetUpDown() * 100 * 1000 / pWeekLine->GetLastClose());
	}
	else {
		pWeekLine->SetUpDownRate(pWeekLine->GetUpDown() * 100 * 1000 / pWeekLine->GetOpen());
	}

	return pWeekLine;
}