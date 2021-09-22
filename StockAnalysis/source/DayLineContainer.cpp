#include"pch.h"
#include"globedef.h"

#include "DayLineContainer.h"

CDayLineContainer::CDayLineContainer() {
}

CDayLineContainer::~CDayLineContainer() {
}

bool CDayLineContainer::SaveData(CString strStockSymbol) {
	CSetDayLineBasicInfo setDayLineBasic;
	SaveBasicData(&setDayLineBasic, strStockSymbol);

	return true;
}

bool CDayLineContainer::LoadData(CString strStockSymbol) {
	CSetDayLineBasicInfo setDayLineBasicInfo;
	CSetDayLineExtendInfo setDayLineExtendInfo;

	ASSERT(!m_fLoadDataFirst);

	// 装入DayLine数据
	setDayLineBasicInfo.m_strFilter = _T("[Symbol] = '");
	setDayLineBasicInfo.m_strFilter += strStockSymbol;
	setDayLineBasicInfo.m_strFilter += _T("'");
	setDayLineBasicInfo.m_strSort = _T("[Date]");
	setDayLineBasicInfo.Open();
	LoadBasicData(&setDayLineBasicInfo);
	setDayLineBasicInfo.Close();

	// 装入DayLineInfo数据
	setDayLineExtendInfo.m_strFilter = _T("[Symbol] = '");
	setDayLineExtendInfo.m_strFilter += strStockSymbol;
	setDayLineExtendInfo.m_strFilter += _T("'");
	setDayLineExtendInfo.m_strSort = _T("[Date]");
	setDayLineExtendInfo.Open();
	LoadExtendData(&setDayLineExtendInfo);
	setDayLineExtendInfo.Close();

	m_fDataLoaded = true;
	ASSERT(!m_fLoadDataFirst);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 更新日线容器。
//
/////////////////////////////////////////////////////////////////////////////////////
void CDayLineContainer::UpdateData(vector<CDayLinePtr>& vTempDayLine) {
	CDayLinePtr pDayLine = nullptr;
	Unload(); // 清除已载入的日线数据（如果有的话）
	// 将日线数据以时间为正序存入
	for (int i = 0; i < vTempDayLine.size(); i++) {
		pDayLine = vTempDayLine.at(i);
		if (pDayLine->IsActive()) {
			// 清除掉不再交易（停牌或退市后出现的）的股票日线
			StoreData(pDayLine);
		}
	}
	SetDataLoaded(true);
}

bool CDayLineContainer::BuildWeekLine(vector<CWeekLinePtr>& vWeekLine) {
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

CWeekLinePtr CDayLineContainer::CreateNewWeekLine(long& lCurrentDayLinePos) {
	ASSERT(GetDataSize() > 0);
	ASSERT(lCurrentDayLinePos < GetDataSize());

	long lNextMonday = GetNextMonday(GetData(lCurrentDayLinePos)->GetFormatedMarketDate());
	long lNewestDay = GetData(GetDataSize() - 1)->GetFormatedMarketDate();
	CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
	if (lNextMonday < lNewestDay) { // 中间数据
		while (GetData(lCurrentDayLinePos)->GetFormatedMarketDate() < lNextMonday) {
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