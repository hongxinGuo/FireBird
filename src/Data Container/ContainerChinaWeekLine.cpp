#include"pch.h"

#include "ContainerChinaWeekLine.h"

#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"
#include"SetCurrentWeekLine.h"

#include<memory>
using std::make_shared;

CContainerChinaWeekLine::CContainerChinaWeekLine() {}

bool CContainerChinaWeekLine::SaveDB(const CString& strStockSymbol) {
	CSetWeekLineBasicInfo setWeekLineBasic;
	CSetWeekLineExtendInfo setWeekLineExtend;
	UpdateBasicDB(&setWeekLineBasic, strStockSymbol);
	SaveExtendDB(&setWeekLineExtend);

	return true;
}

bool CContainerChinaWeekLine::SaveCurrentWeekLine(void) {
	CSetCurrentWeekLine setCurrentWeekLineInfo;
	CWeekLinePtr pWeekLine = nullptr;

	ASSERT(!m_vHistoryData.empty());

	setCurrentWeekLineInfo.m_strFilter = _T("[ID] = 1");
	setCurrentWeekLineInfo.Open();
	setCurrentWeekLineInfo.m_pDatabase->BeginTrans();
	for (const auto& pData : m_vHistoryData) { pData->Append(&setCurrentWeekLineInfo); }
	setCurrentWeekLineInfo.m_pDatabase->CommitTrans();
	setCurrentWeekLineInfo.Close();
	TRACE("�洢��%d����ǰ����������\n", m_vHistoryData.size());

	return true;
}

bool CContainerChinaWeekLine::LoadDB(const CString& strStockCode) {
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

bool CContainerChinaWeekLine::LoadCurrentWeekLine(void) {
	CSetCurrentWeekLine setCurrentWeekLineInfo;

	setCurrentWeekLineInfo.Open();
	setCurrentWeekLineInfo.m_pDatabase->BeginTrans();
	while (!setCurrentWeekLineInfo.IsEOF()) {
		auto pWeekLine = make_shared<CWeekLine>();
		pWeekLine->Load(&setCurrentWeekLineInfo);
		StoreData(pWeekLine);
		setCurrentWeekLineInfo.MoveNext();
	}
	setCurrentWeekLineInfo.m_pDatabase->CommitTrans();
	setCurrentWeekLineInfo.Close();

	return true;
}

bool CContainerChinaWeekLine::StoreVectorData(vector<CWeekLinePtr>& vWeekLine) {
	for (const auto& pWeekLine : vWeekLine) { StoreData(pWeekLine); }
	SetDataLoaded(true);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// ��������������
//
/////////////////////////////////////////////////////////////////////////////////////
void CContainerChinaWeekLine::UpdateData(vector<CWeekLinePtr>& vTempWeekLine) {
	Unload(); // �����������������ݣ�����еĻ���
	// ������������ʱ��Ϊ�������
	for (const auto& pWeekLine : vTempWeekLine) { StoreData(pWeekLine); }
	SetDataLoaded(true);
}

bool CContainerChinaWeekLine::UpdateData(CVirtualHistoryCandleExtendPtr pHistoryCandleExtend) {
	for (const auto& pData : m_vHistoryData) {
		if (strcmp(pData->GetStockSymbol(), pHistoryCandleExtend->GetStockSymbol()) == 0) {
			//
			static_pointer_cast<CWeekLine>(pData)->UpdateWeekLine(pHistoryCandleExtend);
			break;
		}
	}
	return true;
}
