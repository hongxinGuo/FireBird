#include"pch.h"
#include"globedef.h"

#include "WeekLineContainer.h"
#include"WeekLine.h"

#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"
#include"SetCurrentWeekLine.h"

CWeekLineContainer::CWeekLineContainer() {
}

CWeekLineContainer::~CWeekLineContainer() {
}

bool CWeekLineContainer::SaveData(CString strStockSymbol) {
	CSetWeekLineBasicInfo setWeekLineBasic;
	CSetWeekLineExtendInfo setWeekLineExtend;
	SaveBasicData(&setWeekLineBasic, strStockSymbol);
	SaveExtendData(&setWeekLineExtend);

	return true;
}

bool CWeekLineContainer::SaveCurrentWeekLine(void) {
	CSetCurrentWeekLine setCurrentWeekLineInfo;
	CWeekLinePtr pWeekLine = nullptr;

	ASSERT(m_vHistoryData.size() > 0);

	setCurrentWeekLineInfo.m_strFilter = _T("[ID] = 1");
	setCurrentWeekLineInfo.Open();
	setCurrentWeekLineInfo.m_pDatabase->BeginTrans();
	for (auto pData : m_vHistoryData) {
		pData->AppendHistoryCandle(&setCurrentWeekLineInfo);
	}
	setCurrentWeekLineInfo.m_pDatabase->CommitTrans();
	setCurrentWeekLineInfo.Close();
	TRACE("存储了%d个当前周周线数据\n", m_vHistoryData.size());

	return true;
}

bool CWeekLineContainer::LoadData(CString strStockCode) {
	CSetWeekLineBasicInfo setWeekLineBasicInfo;
	CSetWeekLineExtendInfo setWeekLineExtendInfo;

	ASSERT(!m_fLoadDataFirst);

	// 装入WeekLine数据
	setWeekLineBasicInfo.m_strFilter = _T("[Symbol] = '");
	setWeekLineBasicInfo.m_strFilter += strStockCode;
	setWeekLineBasicInfo.m_strFilter += _T("'");
	setWeekLineBasicInfo.m_strSort = _T("[Date]");
	setWeekLineBasicInfo.Open();
	LoadBasicData(&setWeekLineBasicInfo);
	setWeekLineBasicInfo.Close();

	// 装入WeekLineInfo数据
	setWeekLineExtendInfo.m_strFilter = _T("[Symbol] = '");
	setWeekLineExtendInfo.m_strFilter += strStockCode;
	setWeekLineExtendInfo.m_strFilter += _T("'");
	setWeekLineExtendInfo.m_strSort = _T("[Date]");
	setWeekLineExtendInfo.Open();
	LoadExtendData(&setWeekLineExtendInfo);
	setWeekLineExtendInfo.Close();

	m_fDataLoaded = true;

	ASSERT(!m_fLoadDataFirst);

	return true;
}

bool CWeekLineContainer::LoadCurrentWeekLine(void) {
	CSetCurrentWeekLine setCurrentWeekLineInfo;

	setCurrentWeekLineInfo.Open();
	setCurrentWeekLineInfo.m_pDatabase->BeginTrans();
	while (!setCurrentWeekLineInfo.IsEOF()) {
		CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
		pWeekLine->LoadHistoryCandle(&setCurrentWeekLineInfo);
		StoreData(pWeekLine);
		setCurrentWeekLineInfo.MoveNext();
	}
	setCurrentWeekLineInfo.m_pDatabase->CommitTrans();
	setCurrentWeekLineInfo.Close();

	return true;
}

bool CWeekLineContainer::StoreVectorData(vector<CWeekLinePtr>& vWeekLine) {
	for (auto pWeekLine : vWeekLine) {
		StoreData(pWeekLine);
	}
	SetDataLoaded(true);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 更新日线容器。
//
/////////////////////////////////////////////////////////////////////////////////////
void CWeekLineContainer::UpdateData(vector<CWeekLinePtr>& vTempWeekLine) {
	Unload(); // 清除已载入的周线数据（如果有的话）
	// 将日线数据以时间为正序存入
	for (auto pWeekLine : vTempWeekLine) {
		StoreData(pWeekLine);
	}
	SetDataLoaded(true);
}

bool CWeekLineContainer::UpdateData(CVirtualHistoryCandleExtendPtr pHistoryCandleExtend) {
	for (auto pData : m_vHistoryData) {
		if (strcmp(pData->GetStockSymbol(), pHistoryCandleExtend->GetStockSymbol()) == 0) { //
			static_pointer_cast<CWeekLine>(pData)->UpdateWeekLine(pHistoryCandleExtend);
			break;
		}
	}
	return true;
}