#include"pch.h"

#include"TimeConvert.h"

#include "ContainerChinaDayLine.h"
#include"SetChinaMarketDayLineBasicInfo.h"
#include"SetChinaMarketDayLineExtendInfo.h"

#include "InfoReport.h"

bool CContainerChinaDayLine::SaveDB(const string& strStockSymbol) {
	try {
		CSetChinaMarketDayLneBasicInfo setDayLineBasic;
		UpdateBasicDB(&setDayLineBasic, strStockSymbol);
	} catch (CException& e) {
		ReportInformation(e);
	}

	return true;
}

bool CContainerChinaDayLine::LoadDB(const string& strStockSymbol) {
	CSetChinaMarketDayLneBasicInfo setDayLineBasicInfo;
	CSetChinaMarketDayLneExtendInfo setDayLineExtendInfo;

	ASSERT(!m_fBasicDataLoaded);

	// 装入DayLine数据
	setDayLineBasicInfo.m_strFilter = "[Symbol] = '";
	setDayLineBasicInfo.m_strFilter += strStockSymbol.c_str();
	setDayLineBasicInfo.m_strFilter += "'";
	setDayLineBasicInfo.m_strSort = "[Date]";
	setDayLineBasicInfo.Open();
	LoadBasicDB(&setDayLineBasicInfo);
	setDayLineBasicInfo.Close();

	// 装入DayLineExtendInfo数据
	setDayLineExtendInfo.m_strFilter = "[Symbol] = '";
	setDayLineExtendInfo.m_strFilter += strStockSymbol.c_str();
	setDayLineExtendInfo.m_strFilter += "'";
	setDayLineExtendInfo.m_strSort = "[Date]";
	setDayLineExtendInfo.Open();
	LoadExtendDB(&setDayLineExtendInfo);
	setDayLineExtendInfo.Close();

	CalculateRS0();
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
	auto data = GetData(lCurrentDayLinePos);
	const long lNextMonday = GetNextMonday(GetData(lCurrentDayLinePos)->GetDate());
	const long lNewestDay = GetData(Size() - 1)->GetDate();
	auto pWeekLine = make_shared<CWeekLine>();
	if (lNextMonday < lNewestDay) {
		// 中间数据
		while (GetData(lCurrentDayLinePos)->GetDate() < lNextMonday) {
			pWeekLine->UpdateWeekLine(GetData(lCurrentDayLinePos++));
		}
	}
	else {
		// 最后一组数据
		while (lCurrentDayLinePos <= (Size() - 1)) {
			pWeekLine->UpdateWeekLine(GetData(lCurrentDayLinePos++));
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
