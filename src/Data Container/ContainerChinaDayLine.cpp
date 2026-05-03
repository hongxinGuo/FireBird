#include"pch.h"

#include"TimeConvert.h"

#include "ContainerChinaDayLine.h"
#include"SetChinaMarketDayLineInfo.h"

#include "InfoReport.h"

namespace {
	CChinaStock s_stock;
}

CContainerChinaDayLine::CContainerChinaDayLine() {
	m_ratio = s_stock.GetRatio();
}

bool CContainerChinaDayLine::SaveDB(const string& strStockSymbol) {
	try {
		CSetChinaMarketDayLineInfo setDayLineBasic;
		UpdateBasicDB(&setDayLineBasic, strStockSymbol);
	} catch (CException& e) {
		ReportInformation(e);
	}

	return true;
}

bool CContainerChinaDayLine::LoadDB(const string& strStockSymbol) {
	CSetChinaMarketDayLineInfo setDayLineBasicInfo;

	ASSERT(!m_fBasicDataLoaded);

	// 装入DayLine数据
	setDayLineBasicInfo.m_strFilter = "[Symbol] = '";
	setDayLineBasicInfo.m_strFilter += strStockSymbol.c_str();
	setDayLineBasicInfo.m_strFilter += "'";
	setDayLineBasicInfo.m_strSort = "[Date]";
	setDayLineBasicInfo.Open();
	LoadBasicDB(&setDayLineBasicInfo);
	setDayLineBasicInfo.Close();

	CalculateRS0();
	m_fDataLoaded = true;
	ASSERT(!m_fBasicDataLoaded);

	return true;
}

bool CContainerChinaDayLine::BuildWeekLine(vector<CWeekLine>& vWeekLine) {
	ASSERT(IsDataLoaded());
	ASSERT(Size() > 0);
	long lCurrentDayLinePos = 0;
	CWeekLine pWeekLine;

	vWeekLine.clear();
	do {
		pWeekLine = CreateNewWeekLine(lCurrentDayLinePos);
		vWeekLine.push_back(pWeekLine);
	} while (lCurrentDayLinePos < Size());

	return true;
}

CWeekLine CContainerChinaDayLine::CreateNewWeekLine(long& lCurrentDayLinePos) {
	ASSERT(Size() > 0);
	ASSERT(lCurrentDayLinePos < Size());
	auto data = GetData(lCurrentDayLinePos);
	const long lNextMonday = GetNextMonday(GetData(lCurrentDayLinePos)->GetDate());
	const long lNewestDay = GetData(Size() - 1)->GetDate();
	CWeekLine weekLine;
	if (lNextMonday < lNewestDay) {
		// 中间数据
		while (GetData(lCurrentDayLinePos)->GetDate() < lNextMonday) {
			weekLine.UpdateWeekLine(GetData(lCurrentDayLinePos++));
		}
	}
	else {
		// 最后一组数据
		while (lCurrentDayLinePos <= (Size() - 1)) {
			weekLine.UpdateWeekLine(GetData(lCurrentDayLinePos++));
		}
	}

	if (weekLine.GetLastClose() > 0) {
		weekLine.SetUpDownRate(weekLine.GetUpDown() * 100 * 1000 / weekLine.GetLastClose());
	}
	else {
		weekLine.SetUpDownRate(weekLine.GetUpDown() * 100 * 1000 / weekLine.GetOpen());
	}

	return weekLine;
}
