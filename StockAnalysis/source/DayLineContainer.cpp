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
//	��������ʷ���ݴ������ݿ⣮Ĭ�����ݿ�Ϊ�ա�
//  �˺����������̵߳��ã���Ҫע������ͬ�����⡣
// �����ھ�������ʷ����ʱ��������ֻ�Ǹ��¡�
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
	for (int i = 0; i < lSize; i++) { // ����������洢�ģ���Ҫ��ͷ����ʼ�洢
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

	// װ��DayLine����
	setDayLineBasicInfo.m_strFilter = _T("[Symbol] = '");
	setDayLineBasicInfo.m_strFilter += strStockCode;
	setDayLineBasicInfo.m_strFilter += _T("'");
	setDayLineBasicInfo.m_strSort = _T("[Date]");
	setDayLineBasicInfo.Open();
	LoadDayLineBasicInfo(&setDayLineBasicInfo);
	setDayLineBasicInfo.Close();

	// װ��DayLineInfo����
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
	// װ��DayLine����
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
// װ��DayLineInfo�������װ��DayLine��֮��
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
// ��������������
//
/////////////////////////////////////////////////////////////////////////////////////
void CDayLineContainer::UpdateData(vector<CDayLinePtr>& vTempDayLine) {
	CDayLinePtr pDayLine = nullptr;
	Unload(); // �����������������ݣ�����еĻ���
	// ������������ʱ��Ϊ�������
	for (int i = 0; i < vTempDayLine.size(); i++) {
		pDayLine = vTempDayLine.at(i);
		if (pDayLine->IsActive()) {
			// ��������ٽ��ף�ͣ�ƻ����к���ֵģ��Ĺ�Ʊ����
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
	if (lNextMonday < lNewestDay) { // �м�����
		while (GetData(lCurrentDayLinePos)->GetFormatedMarketDate() < lNextMonday) {
			pWeekLine->UpdateWeekLine(GetData(lCurrentDayLinePos++));
		}
	}
	else { // ���һ������
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