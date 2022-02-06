#include"pch.h"
#include"globedef.h"

#include "DataChinaWeekLine.h"
#include"WeekLine.h"

#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"
#include"SetCurrentWeekLine.h"

CDataChinaWeekLine::CDataChinaWeekLine() {
}

CDataChinaWeekLine::~CDataChinaWeekLine() {
}

bool CDataChinaWeekLine::SaveDB(CString strStockSymbol) {
	CSetWeekLineBasicInfo setWeekLineBasic;
	CSetWeekLineExtendInfo setWeekLineExtend;
	UpdateBasicDB(&setWeekLineBasic, strStockSymbol);
	SaveExtendDB(&setWeekLineExtend);

	return true;
}

bool CDataChinaWeekLine::SaveCurrentWeekLine(void) {
	CSetCurrentWeekLine setCurrentWeekLineInfo;
	CWeekLinePtr pWeekLine = nullptr;

	ASSERT(m_vHistoryData.size() > 0);

	setCurrentWeekLineInfo.m_strFilter = _T("[ID] = 1");
	setCurrentWeekLineInfo.Open();
	setCurrentWeekLineInfo.m_pDatabase->BeginTrans();
	for (auto pData : m_vHistoryData) {
		pData->Append(&setCurrentWeekLineInfo);
	}
	setCurrentWeekLineInfo.m_pDatabase->CommitTrans();
	setCurrentWeekLineInfo.Close();
	TRACE("�洢��%d����ǰ����������\n", m_vHistoryData.size());

	return true;
}

bool CDataChinaWeekLine::LoadDB(CString strStockCode) {
	CSetWeekLineBasicInfo setWeekLineBasicInfo;
	CSetWeekLineExtendInfo setWeekLineExtendInfo;

	ASSERT(!m_fLoadDataFirst);

	// װ��WeekLine����
	setWeekLineBasicInfo.m_strFilter = _T("[Symbol] = '");
	setWeekLineBasicInfo.m_strFilter += strStockCode;
	setWeekLineBasicInfo.m_strFilter += _T("'");
	setWeekLineBasicInfo.m_strSort = _T("[Date]");
	setWeekLineBasicInfo.Open();
	LoadBasicDB(&setWeekLineBasicInfo);
	setWeekLineBasicInfo.Close();

	// װ��WeekLineInfo����
	setWeekLineExtendInfo.m_strFilter = _T("[Symbol] = '");
	setWeekLineExtendInfo.m_strFilter += strStockCode;
	setWeekLineExtendInfo.m_strFilter += _T("'");
	setWeekLineExtendInfo.m_strSort = _T("[Date]");
	setWeekLineExtendInfo.Open();
	LoadExtendDB(&setWeekLineExtendInfo);
	setWeekLineExtendInfo.Close();

	m_fDataLoaded = true;

	ASSERT(!m_fLoadDataFirst);

	return true;
}

bool CDataChinaWeekLine::LoadCurrentWeekLine(void) {
	CSetCurrentWeekLine setCurrentWeekLineInfo;

	setCurrentWeekLineInfo.Open();
	setCurrentWeekLineInfo.m_pDatabase->BeginTrans();
	while (!setCurrentWeekLineInfo.IsEOF()) {
		CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
		pWeekLine->Load(&setCurrentWeekLineInfo);
		StoreData(pWeekLine);
		setCurrentWeekLineInfo.MoveNext();
	}
	setCurrentWeekLineInfo.m_pDatabase->CommitTrans();
	setCurrentWeekLineInfo.Close();

	return true;
}

bool CDataChinaWeekLine::StoreVectorData(vector<CWeekLinePtr>& vWeekLine) {
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
void CDataChinaWeekLine::UpdateData(vector<CWeekLinePtr>& vTempWeekLine) {
	Unload(); // �����������������ݣ�����еĻ���
	// ������������ʱ��Ϊ�������
	for (auto pWeekLine : vTempWeekLine) {
		StoreData(pWeekLine);
	}
	SetDataLoaded(true);
}

bool CDataChinaWeekLine::UpdateData(CVirtualHistoryCandleExtendPtr pHistoryCandleExtend) {
	for (auto pData : m_vHistoryData) {
		if (strcmp(pData->GetStockSymbol(), pHistoryCandleExtend->GetStockSymbol()) == 0) { //
			static_pointer_cast<CWeekLine>(pData)->UpdateWeekLine(pHistoryCandleExtend);
			break;
		}
	}
	return true;
}