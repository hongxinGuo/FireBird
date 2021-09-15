#include"pch.h"
#include"globedef.h"

#include "DayLineContainer.h"

CDayLineContainer::CDayLineContainer() {
}

CDayLineContainer::~CDayLineContainer() {
}

bool CDayLineContainer::SaveData(CString strStockSymbol) {
	SaveDayLineBasicInfo(strStockSymbol);

	return true;
}

bool CDayLineContainer::LoadData(CString strStockSymbol) {
	LoadDayLine(strStockSymbol);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将日线历史数据存入数据库．默认数据库为空。
//  此函数被工作线程调用，需要注意数据同步问题。
// 当存在旧日线历史数据时，本函数只是更新。
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CDayLineContainer::SaveDayLineBasicInfo(CString strStockSymbol) {
	CSetDayLineBasicInfo setDayLineBasicInfo, setDayLineBasicInfo2;
	size_t lSize = 0;
	vector<CDayLinePtr> vDayLine;
	CDayLinePtr pDayLine = nullptr;
	long lCurrentPos = 0, lSizeOfOldDayLine = 0;
	bool fNeedUpdate = false;

	ASSERT(GetDataSize() > 0);

	lSize = GetDataSize();
	setDayLineBasicInfo.m_strFilter = _T("[Symbol] = '");
	setDayLineBasicInfo.m_strFilter += strStockSymbol + _T("'");
	setDayLineBasicInfo.m_strSort = _T("[Date]");

	setDayLineBasicInfo.Open();
	while (!setDayLineBasicInfo.IsEOF()) {
		pDayLine = make_shared<CDayLine>();
		pDayLine->LoadHistoryCandle(&setDayLineBasicInfo);
		vDayLine.push_back(pDayLine);
		lSizeOfOldDayLine++;
		setDayLineBasicInfo.MoveNext();
	}
	setDayLineBasicInfo.Close();

	lCurrentPos = 0;
	setDayLineBasicInfo2.m_strFilter = _T("[ID] = 1");
	setDayLineBasicInfo2.Open();
	setDayLineBasicInfo2.m_pDatabase->BeginTrans();
	for (int i = 0; i < lSize; i++) { // 数据是正序存储的，需要从头部开始存储
		pDayLine = GetData(i);
		while ((lCurrentPos < lSizeOfOldDayLine) && (vDayLine.at(lCurrentPos)->GetFormatedMarketDate() < pDayLine->GetFormatedMarketDate())) lCurrentPos++;
		if (lCurrentPos < lSizeOfOldDayLine) {
			if (vDayLine.at(lCurrentPos)->GetFormatedMarketDate() > pDayLine->GetFormatedMarketDate()) {
				pDayLine->AppendHistoryCandle(&setDayLineBasicInfo2);
				fNeedUpdate = true;
			}
		}
		else {
			pDayLine->AppendHistoryCandle(&setDayLineBasicInfo2);
			fNeedUpdate = true;
		}
	}
	setDayLineBasicInfo2.m_pDatabase->CommitTrans();
	setDayLineBasicInfo2.Close();

	return fNeedUpdate;
}

bool CDayLineContainer::LoadDayLine(CString strStockCode) {
	CSetDayLineBasicInfo setDayLineBasicInfo;
	CSetDayLineExtendInfo setDayLineExtendInfo;

	ASSERT(!m_fLoadDataFirst);

	// 装入DayLine数据
	setDayLineBasicInfo.m_strFilter = _T("[Symbol] = '");
	setDayLineBasicInfo.m_strFilter += strStockCode;
	setDayLineBasicInfo.m_strFilter += _T("'");
	setDayLineBasicInfo.m_strSort = _T("[Date]");
	setDayLineBasicInfo.Open();
	LoadDayLineBasicInfo(&setDayLineBasicInfo);
	setDayLineBasicInfo.Close();

	// 装入DayLineInfo数据
	setDayLineExtendInfo.m_strFilter = _T("[Symbol] = '");
	setDayLineExtendInfo.m_strFilter += strStockCode;
	setDayLineExtendInfo.m_strFilter += _T("'");
	setDayLineExtendInfo.m_strSort = _T("[Date]");
	setDayLineExtendInfo.Open();
	LoadDayLineExtendInfo(&setDayLineExtendInfo);
	setDayLineExtendInfo.Close();

	m_fDataLoaded = true;
	ASSERT(!m_fLoadDataFirst);
	return true;
}

bool CDayLineContainer::LoadDayLineBasicInfo(not_null<CSetDayLineBasicInfo*> psetDayLineBasicInfo) {
	CDayLinePtr pDayLine;

	if (gl_fNormalMode) ASSERT(!m_fLoadDataFirst);
	// 装入DayLine数据
	Unload();
	while (!psetDayLineBasicInfo->IsEOF()) {
		pDayLine = make_shared<CDayLine>();
		pDayLine->LoadHistoryCandle(psetDayLineBasicInfo);
		StoreData(pDayLine);
		psetDayLineBasicInfo->MoveNext();
	}
	m_fLoadDataFirst = true;
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//
// 装载DayLineInfo表必须在装载DayLine表之后。
//
//
////////////////////////////////////////////////////////////////////////////
bool CDayLineContainer::LoadDayLineExtendInfo(not_null<CSetDayLineExtendInfo*> psetDayLineExtendInfo) {
	CDayLinePtr pDayLine;
	int iPosition = 0;

	if (gl_fNormalMode) ASSERT(m_fLoadDataFirst);

	while (!psetDayLineExtendInfo->IsEOF()) {
		pDayLine = GetData(iPosition);
		while ((pDayLine->GetFormatedMarketDate() < psetDayLineExtendInfo->m_Date)
			&& (GetDataSize() > (iPosition + 1))) {
			iPosition++;
			pDayLine = GetData(iPosition);
		}
		if (pDayLine->GetFormatedMarketDate() == psetDayLineExtendInfo->m_Date) {
			pDayLine->LoadChinaMarketExtendData(psetDayLineExtendInfo);
		}
		if (GetDataSize() <= (iPosition + 1)) break;
		psetDayLineExtendInfo->MoveNext();
	}
	m_fLoadDataFirst = false;
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
			pWeekLine->UpdateWeekLine(GetData(lCurrentDayLinePos++));
		}
	}
	else { // 最后一组数据
		while (lCurrentDayLinePos <= (GetDataSize() - 1)) {
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