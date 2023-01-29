#include"pch.h"

#include"VirtualDataHistoryCandleExtend.h"
#include"DayLine.h"

#include<memory>
using std::make_shared;

CVirtualDataHistoryCandleExtend::CVirtualDataHistoryCandleExtend() {
	Reset();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	��������ʷ���ݴ������ݿ⣮Ĭ�����ݿ�Ϊ�ա�
//  �˺����������̵߳��ã���Ҫע������ͬ�����⡣
//  �����ھ�������ʷ����ʱ��������ֻ�Ǹ��¡�
//
//  ������������ݱ��ɵ�һ���������룬
//  �Զ�ɾ���������е��ظ����ݡ�
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CVirtualDataHistoryCandleExtend::UpdateBasicDB(CVirtualSetHistoryCandleBasic* pSetHistoryCandleBasic, const CString& strStockSymbol) {
	vector<CVirtualHistoryCandleExtendPtr> vOldHistoryCandle;
	CVirtualHistoryCandleExtendPtr pHistoryCandle = nullptr;
	long lSizeOfOldDayLine = 0;
	bool fNeedUpdate = false;

	ASSERT(Size() > 0);

	const size_t lSize = Size();
	if (strStockSymbol.GetLength() > 0) {
		long lLastDate = 0;
		pSetHistoryCandleBasic->m_strFilter = _T("[Symbol] = '");
		pSetHistoryCandleBasic->m_strFilter += strStockSymbol + _T("'");
		pSetHistoryCandleBasic->m_strSort = _T("[Date]");

		pSetHistoryCandleBasic->Open();
		pSetHistoryCandleBasic->m_pDatabase->BeginTrans();
		while (!pSetHistoryCandleBasic->IsEOF()) {
			if (pSetHistoryCandleBasic->m_Date > lLastDate) {
				lLastDate = pSetHistoryCandleBasic->m_Date;
				pHistoryCandle = make_shared<CVirtualHistoryCandleExtend>();
				pHistoryCandle->LoadBasicData(pSetHistoryCandleBasic);

				vOldHistoryCandle.push_back(pHistoryCandle);
				lSizeOfOldDayLine++;
			}
			else {
				pSetHistoryCandleBasic->Delete(); //ɾ�������ظ�������
			}
			pSetHistoryCandleBasic->MoveNext();
		}
		pSetHistoryCandleBasic->m_pDatabase->CommitTrans();
		pSetHistoryCandleBasic->Close();
	}
	pSetHistoryCandleBasic->m_strFilter = _T("[ID] = 1");
	pSetHistoryCandleBasic->Open();
	pSetHistoryCandleBasic->m_pDatabase->BeginTrans();
	if (lSizeOfOldDayLine > 0) {
		long lCurrentPos = 0;
		// �о�����
		for (int i = 0; i < lSize; i++) {
			// ����������洢�ģ���Ҫ��ͷ����ʼ�洢
			pHistoryCandle = GetData(i);
			if (pHistoryCandle->GetMarketDate() < vOldHistoryCandle.at(0)->GetMarketDate()) {	// �и���������ݣ�
				pHistoryCandle->AppendBasicData(pSetHistoryCandleBasic);
			}
			else {
				while ((lCurrentPos < lSizeOfOldDayLine) && (vOldHistoryCandle.at(lCurrentPos)->GetMarketDate() < pHistoryCandle->GetMarketDate())) lCurrentPos++;
				if (lCurrentPos < lSizeOfOldDayLine) {
					if (vOldHistoryCandle.at(lCurrentPos)->GetMarketDate() > pHistoryCandle->GetMarketDate()) { // ǰ���ݼ�������©������
						pHistoryCandle->AppendBasicData(pSetHistoryCandleBasic);
						fNeedUpdate = true;
					}
				}
				else {
					pHistoryCandle->AppendBasicData(pSetHistoryCandleBasic);
					fNeedUpdate = true;
				}
			}
		}
	}
	else {
		// û�о�����
		for (int i = 0; i < lSize; i++) {
			// ����������洢�ģ���Ҫ��ͷ����ʼ�洢
			pHistoryCandle = GetData(i);
			pHistoryCandle->AppendBasicData(pSetHistoryCandleBasic);
		}
	}
	pSetHistoryCandleBasic->m_pDatabase->CommitTrans();
	pSetHistoryCandleBasic->Close();

	return fNeedUpdate;
}

bool CVirtualDataHistoryCandleExtend::SaveExtendDB(CVirtualSetHistoryCandleExtend* pSetHistoryCandleExtend) {
	ASSERT(!m_vHistoryData.empty());

	pSetHistoryCandleExtend->m_strFilter = _T("[ID] = 1");
	pSetHistoryCandleExtend->Open();
	pSetHistoryCandleExtend->m_pDatabase->BeginTrans();
	for (const auto& pData : m_vHistoryData) { pData->AppendExtendData(pSetHistoryCandleExtend); }
	pSetHistoryCandleExtend->m_pDatabase->CommitTrans();
	pSetHistoryCandleExtend->Close();

	return true;
}

bool CVirtualDataHistoryCandleExtend::LoadBasicDB(CVirtualSetHistoryCandleBasic* pSetHistoryCandleBasic) {
	if (gl_systemStatus.IsWorkingMode())
		ASSERT(!m_fLoadDataFirst);
	ASSERT(pSetHistoryCandleBasic->IsOpen());

	// װ��DayLine����
	Unload();
	while (!pSetHistoryCandleBasic->IsEOF()) {
		const auto pHistoryCandle = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryCandle->LoadBasicData(pSetHistoryCandleBasic);
		StoreData(pHistoryCandle);
		pSetHistoryCandleBasic->MoveNext();
	}
	m_fLoadDataFirst = true;
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//
// װ��ExtendData�������װ��BasicData��֮��
//
//
////////////////////////////////////////////////////////////////////////////
bool CVirtualDataHistoryCandleExtend::LoadExtendDB(CVirtualSetHistoryCandleExtend* pSetHistoryCandleExtend) {
	int iPosition = 0;

	if (gl_systemStatus.IsWorkingMode())
		ASSERT(m_fLoadDataFirst);
	ASSERT(pSetHistoryCandleExtend->IsOpen());

	while (!pSetHistoryCandleExtend->IsEOF()) {
		CVirtualHistoryCandleExtendPtr pHistoryCandle = GetData(iPosition);
		while ((pHistoryCandle->GetMarketDate() < pSetHistoryCandleExtend->m_Date)
			&& (Size() > (iPosition + 1))) {
			iPosition++;
			pHistoryCandle = GetData(iPosition);
		}
		if (pHistoryCandle->GetMarketDate() == pSetHistoryCandleExtend->m_Date) { pHistoryCandle->LoadExtendData(pSetHistoryCandleExtend); }
		if (Size() <= (iPosition + 1)) break;
		pSetHistoryCandleExtend->MoveNext();
	}
	m_fLoadDataFirst = false;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// ��������������
//
/////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataHistoryCandleExtend::UpdateData(vector<CVirtualHistoryCandleExtendPtr>& vTempData) {
	Unload(); // �������������ݣ�����еĻ���
	for (const auto& p : vTempData) {
		if (p->IsActive()) StoreData(p);
	}
	SetDataLoaded(true);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// ��������������
//
/////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataHistoryCandleExtend::UpdateData(vector<CDayLinePtr>& vTempDayLine) {
	Unload(); // �����������������ݣ�����еĻ���
	// ������������ʱ��Ϊ�������
	for (const auto& pDayLine : vTempDayLine) {
		if (pDayLine->IsActive()) {
			// ��������ٽ��ף�ͣ�ƻ����к���ֵģ��Ĺ�Ʊ����
			StoreData(pDayLine);
		}
	}
	SetDataLoaded(true);
}

void CVirtualDataHistoryCandleExtend::ShowData(CDC* pDC, CRect rectClient) {
	constexpr COLORREF crBlue(RGB(0, 0, 255)), crGreen(RGB(0, 255, 0)), crWhite(RGB(255, 255, 255)),
	                   crRed(RGB(255, 0, 0));
	CPen penGreen1(PS_SOLID, 1, crGreen), penWhite1(PS_SOLID, 1, crWhite), penRed1(PS_SOLID, 1, crRed);
	long lHigh = 0;
	auto it = m_vHistoryData.end();
	--it;
	int i = 0;
	long lLow = (*it)->GetLow();
	for (; it != m_vHistoryData.begin(); --it) {
		if (lHigh < (*it)->GetHigh()) lHigh = (*it)->GetHigh();
		if ((*it)->GetLow() > 0) { if (lLow > (*it)->GetLow()) lLow = (*it)->GetLow(); }
		if (3 * i > m_vHistoryData.size()) break;
		if (rectClient.right <= 3 * i) break; // ����
		i++;
	}

	it = m_vHistoryData.end();
	--it;
	i = 0;
	pDC->SelectObject(&penRed1);
	for (; it != m_vHistoryData.begin(); --it) {
		const long x = rectClient.right - 2 - i * 3;
		int y = (0.5 - static_cast<double>((*it)->GetHigh() - lLow) / (2 * (lHigh - lLow))) * rectClient.Height();
		pDC->MoveTo(x, y);
		if ((*it)->GetHigh() == (*it)->GetLow()) { y = y - 1; }
		else { y = (0.5 - static_cast<double>((*it)->GetLow() - lLow) / (2 * (lHigh - lLow))) * rectClient.Height(); }
		pDC->LineTo(x, y);
		long lDate = (*it)->GetMarketDate();
		i++;
		if (3 * i > m_vHistoryData.size()) break;
		if (rectClient.right <= 3 * i) break; // ����������߿�Ϊֹ
	}
}

bool CVirtualDataHistoryCandleExtend::GetStartEndDate(long& lStartDate, long& lEndDate) {
	if (m_vHistoryData.empty()) return false;

	lStartDate = m_vHistoryData.at(0)->GetMarketDate();
	lEndDate = m_vHistoryData.at(m_vHistoryData.size() - 1)->GetMarketDate();
	return true;
}

void CVirtualDataHistoryCandleExtend::Reset(void) {
	m_vHistoryData.clear();

	m_fDataLoaded = false;
	m_fDatabaseTodayUpdated = false;

	m_fLoadDataFirst = false;
}

bool CVirtualDataHistoryCandleExtend::CalculateRS0(void) {
	CalculateRS1(3);
	CalculateRS1(5);
	CalculateRS1(10);
	CalculateRS1(30);
	CalculateRS1(60);
	CalculateRS1(120);
	return true;
}

bool CVirtualDataHistoryCandleExtend::CalculateRSLogarithm0(void) {
	CalculateRSLogarithm1(3);
	CalculateRSLogarithm1(5);
	CalculateRSLogarithm1(10);
	CalculateRSLogarithm1(30);
	CalculateRSLogarithm1(60);
	CalculateRSLogarithm1(120);
	return true;
}

bool CVirtualDataHistoryCandleExtend::CalculateRSIndex0(void) {
	CalculateRSIndex1(3);
	CalculateRSIndex1(5);
	CalculateRSIndex1(10);
	CalculateRSIndex1(30);
	CalculateRSIndex1(60);
	CalculateRSIndex1(120);
	return true;
}

bool CVirtualDataHistoryCandleExtend::CalculateRSLogarithm1(INT64 lNumber) {
	const INT64 lTotalNumber = m_vHistoryData.size();
	for (INT64 i = lNumber; i < lTotalNumber; i++) {
		double dTempRS = 0;
		for (INT64 j = i - lNumber; j < i; j++) { dTempRS += m_vHistoryData.at(j)->GetRSLogarithm(); }
		switch (lNumber) {
		case 3:
			m_vHistoryData.at(i)->m_d3RS = dTempRS / lNumber;
			break;
		case 5:
			m_vHistoryData.at(i)->m_d5RS = dTempRS / lNumber;
			break;
		case 10:
			m_vHistoryData.at(i)->m_d10RS = dTempRS / lNumber;
			break;
		case 30:
			m_vHistoryData.at(i)->m_d30RS = dTempRS / lNumber;
			break;
		case 60:
			m_vHistoryData.at(i)->m_d60RS = dTempRS / lNumber;
			break;
		case 120:
			m_vHistoryData.at(i)->m_d120RS = dTempRS / lNumber;
			break;
		default:
			ASSERT(0);
		}
	}
	return true;
}

bool CVirtualDataHistoryCandleExtend::CalculateRS1(INT64 lNumber) {
	const INT64 lTotalNumber = m_vHistoryData.size();
	for (INT64 i = lNumber; i < lTotalNumber; i++) {
		double dTempRS = 0;
		for (INT64 j = i - lNumber; j < i; j++) { dTempRS += m_vHistoryData.at(j)->GetRS(); }
		switch (lNumber) {
		case 3:
			m_vHistoryData.at(i)->m_d3RS = dTempRS / lNumber;
			break;
		case 5:
			m_vHistoryData.at(i)->m_d5RS = dTempRS / lNumber;
			break;
		case 10:
			m_vHistoryData.at(i)->m_d10RS = dTempRS / lNumber;
			break;
		case 30:
			m_vHistoryData.at(i)->m_d30RS = dTempRS / lNumber;
			break;
		case 60:
			m_vHistoryData.at(i)->m_d60RS = dTempRS / lNumber;
			break;
		case 120:
			m_vHistoryData.at(i)->m_d120RS = dTempRS / lNumber;
			break;
		default:
			ASSERT(0);
		}
	}
	return true;
}

bool CVirtualDataHistoryCandleExtend::CalculateRSIndex1(INT64 lNumber) {
	const INT64 lTotalNumber = m_vHistoryData.size();
	for (INT64 i = lNumber; i < lTotalNumber; i++) {
		double dTempRS = 0;
		for (INT64 j = i - lNumber; j < i; j++) { dTempRS += m_vHistoryData.at(j)->GetRSIndex(); }
		switch (lNumber) {
		case 3:
			m_vHistoryData.at(i)->m_d3RS = dTempRS / lNumber;
			break;
		case 5:
			m_vHistoryData.at(i)->m_d5RS = dTempRS / lNumber;
			break;
		case 10:
			m_vHistoryData.at(i)->m_d10RS = dTempRS / lNumber;
			break;
		case 30:
			m_vHistoryData.at(i)->m_d30RS = dTempRS / lNumber;
			break;
		case 60:
			m_vHistoryData.at(i)->m_d60RS = dTempRS / lNumber;
			break;
		case 120:
			m_vHistoryData.at(i)->m_d120RS = dTempRS / lNumber;
			break;
		default:
			ASSERT(0);
		}
	}
	return true;
}

void CVirtualDataHistoryCandleExtend::GetRS1(vector<double>& vRS) { for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->GetRSIndex(); } }

void CVirtualDataHistoryCandleExtend::GetRSIndex1(vector<double>& vRS) { for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->GetRSIndex(); } }

void CVirtualDataHistoryCandleExtend::GetRSLogarithm1(vector<double>& vRS) { for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->GetRSLogarithm(); } }

void CVirtualDataHistoryCandleExtend::GetRS3(vector<double>& vRS) { for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->Get3RS(); } }

void CVirtualDataHistoryCandleExtend::GetRS5(vector<double>& vRS) { for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->Get5RS(); } }

void CVirtualDataHistoryCandleExtend::GetRS10(vector<double>& vRS) { for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->Get10RS(); } }

void CVirtualDataHistoryCandleExtend::GetRS30(vector<double>& vRS) { for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->Get30RS(); } }

void CVirtualDataHistoryCandleExtend::GetRS60(vector<double>& vRS) { for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->Get60RS(); } }

void CVirtualDataHistoryCandleExtend::GetRS120(vector<double>& vRS) { for (int i = 0; i < m_vHistoryData.size(); i++) { vRS[i] = m_vHistoryData.at(i)->Get120RS(); } }
