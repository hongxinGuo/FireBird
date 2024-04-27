#include"pch.h"

#include "ContainerChinaWeekLine.h"

#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"
#include"SetCurrentWeekLine.h"

#include<memory>

#include "InfoReport.h"
using std::make_shared;

CContainerChinaWeekLine::CContainerChinaWeekLine() {}

bool CContainerChinaWeekLine::SaveDB(const CString& strStockSymbol) {
	try {
		CSetWeekLineBasicInfo setWeekLineBasic;
		CSetWeekLineExtendInfo setWeekLineExtend;
		UpdateBasicDB(&setWeekLineBasic, strStockSymbol);
		SaveExtendDB(&setWeekLineExtend);
	}
	catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}

	return true;
}

void CContainerChinaWeekLine::SaveCurrentWeekLine() const {
	try {
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
	}
	catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

bool CContainerChinaWeekLine::LoadDB(const CString& strStockCode) {
	CSetWeekLineBasicInfo setWeekLineBasicInfo;
	CSetWeekLineExtendInfo setWeekLineExtendInfo;

	ASSERT(!m_fBasicDataLoaded);

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

	ASSERT(!m_fBasicDataLoaded);

	return true;
}

bool CContainerChinaWeekLine::LoadCurrentWeekLine() {
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

void CContainerChinaWeekLine::StoreVectorData(const vector<CWeekLinePtr>& vWeekLine) {
	for (const auto& pWeekLine : vWeekLine) { StoreData(pWeekLine); }
	SetDataLoaded(true);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// ��������������
//
/////////////////////////////////////////////////////////////////////////////////////
void CContainerChinaWeekLine::UpdateData(const vector<CWeekLinePtr>& vTempWeekLine) {
	Unload(); // �����������������ݣ�����еĻ���
	// ������������ʱ��Ϊ�������
	for (const auto& pWeekLine : vTempWeekLine) { StoreData(pWeekLine); }
	SetDataLoaded(true);
}

void CContainerChinaWeekLine::UpdateData(const CVirtualHistoryCandleExtendPtr& pHistoryCandleExtend) const {
	for (const auto& pData : m_vHistoryData) {
		if (strcmp(pData->GetStockSymbol(), pHistoryCandleExtend->GetStockSymbol()) == 0) {
			//
			static_pointer_cast<CWeekLine>(pData)->UpdateWeekLine(pHistoryCandleExtend);
			break;
		}
	}
}
