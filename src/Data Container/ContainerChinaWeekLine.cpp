#include"pch.h"

#include "ContainerChinaWeekLine.h"

#include"SetWeekLineInfo.h"
#include"SetCurrentWeekLine.h"

#include "InfoReport.h"

namespace {
	CChinaStock s_stock;
}

CContainerChinaWeekLine::CContainerChinaWeekLine() {
	m_ratio = s_stock.GetRatio();
}

bool CContainerChinaWeekLine::SaveDB(const string& strStockSymbol) {
	try {
		CSetWeekLineInfo setWeekLineBasic;
		UpdateBasicDB(&setWeekLineBasic, strStockSymbol);
	} catch (CException& e) {
		ReportInformation(e);
	}

	return true;
}

void CContainerChinaWeekLine::SaveCurrentWeekLine() const {
	CSetCurrentWeekLine setCurrentWeekLineInfo;
	CWeekLinePtr pWeekLine = nullptr;

	ASSERT(!m_vHistoryData.empty());

	setCurrentWeekLineInfo.m_strFilter = "[ID] = 1";
	setCurrentWeekLineInfo.Open();
	setCurrentWeekLineInfo.m_pDatabase->BeginTrans();
	for (const auto& data : m_vHistoryData) {
		data.AppendBasicData(&setCurrentWeekLineInfo);
	}
	setCurrentWeekLineInfo.m_pDatabase->CommitTrans();
	setCurrentWeekLineInfo.Close();
	TRACE(_T("存储了%d个当前周周线数据\n"), m_vHistoryData.size());
}

bool CContainerChinaWeekLine::LoadDB(const string& strStockCode) {
	CSetWeekLineInfo setWeekLineBasicInfo;

	// 装入WeekLine数据
	setWeekLineBasicInfo.m_strFilter = "[Symbol] = '";
	setWeekLineBasicInfo.m_strFilter += strStockCode.c_str();
	setWeekLineBasicInfo.m_strFilter += "'";
	setWeekLineBasicInfo.m_strSort = "[Date]";
	setWeekLineBasicInfo.Open();
	LoadBasicDB(&setWeekLineBasicInfo);
	setWeekLineBasicInfo.Close();

	CalculateRSIndex0();
	m_fDataLoaded = true;

	return true;
}

bool CContainerChinaWeekLine::LoadCurrentWeekLine() {
	CSetCurrentWeekLine setCurrentWeekLineInfo;

	setCurrentWeekLineInfo.Open();
	setCurrentWeekLineInfo.m_pDatabase->BeginTrans();
	while (!setCurrentWeekLineInfo.IsEOF()) {
		CWeekLine weekLine;
		weekLine.LoadBasicData(&setCurrentWeekLineInfo);
		Add(weekLine);
		setCurrentWeekLineInfo.MoveNext();
	}
	setCurrentWeekLineInfo.m_pDatabase->CommitTrans();
	setCurrentWeekLineInfo.Close();

	return true;
}

void CContainerChinaWeekLine::StoreVectorData(const vector<CWeekLine>& vWeekLine) {
	for (const auto& weekLine : vWeekLine) {
		Add(weekLine);
	}
	SetDataLoaded(true);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 更新日线容器。
//
/////////////////////////////////////////////////////////////////////////////////////
void CContainerChinaWeekLine::UpdateData(const vector<CWeekLine>& vTempWeekLine) {
	Unload(); // 清除已载入的周线数据（如果有的话）
	// 将日线数据以时间为正序存入
	for (const auto& pWeekLine : vTempWeekLine) {
		Add(pWeekLine);
	}
	SetDataLoaded(true);
}

void CContainerChinaWeekLine::UpdateData(const CVirtualHistoryCandle* pHistoryCandle) {
	for (size_t index = 0; index < m_vHistoryData.size(); ++index) {
		auto pWeekLine = GetData(index);
		auto str = pWeekLine->GetStockSymbol();
		if (str == pHistoryCandle->GetStockSymbol()) {
			pWeekLine->UpdateWeekLine(pHistoryCandle);
			break;
		}
	}
}
