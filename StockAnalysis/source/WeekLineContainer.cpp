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
	TRACE("�洢��%d����ǰ����������\n", m_vHistoryData.size());

	return true;
}

bool CWeekLineContainer::LoadData(CString strStockCode) {
	CSetWeekLineBasicInfo setWeekLineBasicInfo;
	CSetWeekLineExtendInfo setWeekLineExtendInfo;

	ASSERT(!m_fLoadDataFirst);

	// װ��WeekLine����
	setWeekLineBasicInfo.m_strFilter = _T("[Symbol] = '");
	setWeekLineBasicInfo.m_strFilter += strStockCode;
	setWeekLineBasicInfo.m_strFilter += _T("'");
	setWeekLineBasicInfo.m_strSort = _T("[Date]");
	setWeekLineBasicInfo.Open();
	LoadBasicData(&setWeekLineBasicInfo);
	setWeekLineBasicInfo.Close();

	// װ��WeekLineInfo����
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
// ��������������
//
/////////////////////////////////////////////////////////////////////////////////////
void CWeekLineContainer::UpdateData(vector<CWeekLinePtr>& vTempWeekLine) {
	Unload(); // �����������������ݣ�����еĻ���
	// ������������ʱ��Ϊ�������
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