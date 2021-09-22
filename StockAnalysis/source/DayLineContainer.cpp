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

	// װ��DayLine����
	setDayLineBasicInfo.m_strFilter = _T("[Symbol] = '");
	setDayLineBasicInfo.m_strFilter += strStockSymbol;
	setDayLineBasicInfo.m_strFilter += _T("'");
	setDayLineBasicInfo.m_strSort = _T("[Date]");
	setDayLineBasicInfo.Open();
	LoadBasicData(&setDayLineBasicInfo);
	setDayLineBasicInfo.Close();

	// װ��DayLineInfo����
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
			pWeekLine->UpdateWeekLine(dynamic_pointer_cast<CDayLine>(GetData(lCurrentDayLinePos++)));
		}
	}
	else { // ���һ������
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