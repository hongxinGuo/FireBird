#include"pch.h"

#include"globedef.h"
#include"VirtualDataHistoryCandleExtend.h"

CVirtualDataHistoryCandleExtend::CVirtualDataHistoryCandleExtend() : CObject() {
	Reset();
}

CVirtualDataHistoryCandleExtend::~CVirtualDataHistoryCandleExtend() {
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	��������ʷ���ݴ������ݿ⣮Ĭ�����ݿ�Ϊ�ա�
//  �˺����������̵߳��ã���Ҫע������ͬ�����⡣
//  �����ھ�������ʷ����ʱ��������ֻ�Ǹ��¡�
//
//  ������������ݱ��ɵ�һ���������룬
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CVirtualDataHistoryCandleExtend::UpdateBasicDB(CVirtualSetHistoryCandleBasic* psetHistoryCandleBasic, CString strStockSymbol) {
	size_t lSize = 0;
	vector<CVirtualHistoryCandleExtendPtr> vHistoryCandle;
	CVirtualHistoryCandleExtendPtr pHistoryCandle = nullptr;
	long lCurrentPos = 0, lSizeOfOldDayLine = 0;
	bool fNeedUpdate = false;

	ASSERT(GetDataSize() > 0);

	lSize = GetDataSize();
	if (strStockSymbol.GetLength() > 0) {
		psetHistoryCandleBasic->m_strFilter = _T("[Symbol] = '");
		psetHistoryCandleBasic->m_strFilter += strStockSymbol + _T("'");
		psetHistoryCandleBasic->m_strSort = _T("[Date]");

		psetHistoryCandleBasic->Open();
		while (!psetHistoryCandleBasic->IsEOF()) {
			pHistoryCandle = make_shared<CVirtualHistoryCandleExtend>();
			pHistoryCandle->LoadBasicData(psetHistoryCandleBasic);
			vHistoryCandle.push_back(pHistoryCandle);
			lSizeOfOldDayLine++;
			psetHistoryCandleBasic->MoveNext();
		}
		psetHistoryCandleBasic->Close();
	}
	lCurrentPos = 0;
	psetHistoryCandleBasic->m_strFilter = _T("[ID] = 1");
	psetHistoryCandleBasic->Open();
	psetHistoryCandleBasic->m_pDatabase->BeginTrans();
	if (lSizeOfOldDayLine > 0) { // �о�����
		for (int i = 0; i < lSize; i++) { // ����������洢�ģ���Ҫ��ͷ����ʼ�洢
			pHistoryCandle = GetData(i);
			if (pHistoryCandle->GetMarketDate() < vHistoryCandle.at(0)->GetMarketDate()) { // �и���������ݣ�
				pHistoryCandle->AppendBasicData(psetHistoryCandleBasic);
			}
			else {
				while ((lCurrentPos < lSizeOfOldDayLine) && (vHistoryCandle.at(lCurrentPos)->GetMarketDate() < pHistoryCandle->GetMarketDate())) lCurrentPos++;
				if (lCurrentPos < lSizeOfOldDayLine) {
					if (vHistoryCandle.at(lCurrentPos)->GetMarketDate() > pHistoryCandle->GetMarketDate()) {
						pHistoryCandle->AppendBasicData(psetHistoryCandleBasic);
						fNeedUpdate = true;
					}
				}
				else {
					pHistoryCandle->AppendBasicData(psetHistoryCandleBasic);
					fNeedUpdate = true;
				}
			}
		}
	}
	else { // û�о�����
		for (int i = 0; i < lSize; i++) { // ����������洢�ģ���Ҫ��ͷ����ʼ�洢
			pHistoryCandle = GetData(i);
			pHistoryCandle->AppendBasicData(psetHistoryCandleBasic);
		}
	}
	psetHistoryCandleBasic->m_pDatabase->CommitTrans();
	psetHistoryCandleBasic->Close();

	return fNeedUpdate;
}

bool CVirtualDataHistoryCandleExtend::SaveExtendDB(CVirtualSetHistoryCandleExtend* psetHistoryCandleExtend) {
	ASSERT(m_vHistoryData.size() > 0);

	psetHistoryCandleExtend->m_strFilter = _T("[ID] = 1");
	psetHistoryCandleExtend->Open();
	psetHistoryCandleExtend->m_pDatabase->BeginTrans();
	for (auto pData : m_vHistoryData) {
		pData->AppendExtendData(psetHistoryCandleExtend);
	}
	psetHistoryCandleExtend->m_pDatabase->CommitTrans();
	psetHistoryCandleExtend->Close();

	return true;
}

bool CVirtualDataHistoryCandleExtend::LoadBasicDB(CVirtualSetHistoryCandleBasic* psetHistoryCandleBasic) {
	CVirtualHistoryCandleExtendPtr pHistoryCandle = nullptr;

	if (gl_fNormalMode) ASSERT(!m_fLoadDataFirst);
	ASSERT(psetHistoryCandleBasic->IsOpen());

	// װ��DayLine����
	Unload();
	while (!psetHistoryCandleBasic->IsEOF()) {
		pHistoryCandle = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryCandle->LoadBasicData(psetHistoryCandleBasic);
		StoreData(pHistoryCandle);
		psetHistoryCandleBasic->MoveNext();
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
bool CVirtualDataHistoryCandleExtend::LoadExtendDB(CVirtualSetHistoryCandleExtend* psetHistoryCandleExtend) {
	CVirtualHistoryCandleExtendPtr pHistoryCandle = nullptr;
	int iPosition = 0;

	if (gl_fNormalMode) ASSERT(m_fLoadDataFirst);
	ASSERT(psetHistoryCandleExtend->IsOpen());

	while (!psetHistoryCandleExtend->IsEOF()) {
		pHistoryCandle = GetData(iPosition);
		while ((pHistoryCandle->GetMarketDate() < psetHistoryCandleExtend->m_Date)
			&& (GetDataSize() > (iPosition + 1))) {
			iPosition++;
			pHistoryCandle = GetData(iPosition);
		}
		if (pHistoryCandle->GetMarketDate() == psetHistoryCandleExtend->m_Date) {
			pHistoryCandle->LoadExtendData(psetHistoryCandleExtend);
		}
		if (GetDataSize() <= (iPosition + 1)) break;
		psetHistoryCandleExtend->MoveNext();
	}
	m_fLoadDataFirst = false;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// ��������������
//
/////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataHistoryCandleExtend::UpdateData(vector<CVirtualHistoryCandleExtendPtr>& vTempData, bool fRevertSave) {
	CVirtualHistoryCandleExtendPtr pData = nullptr;
	Unload(); // �������������ݣ�����еĻ���
	if (fRevertSave) {
		for (int i = vTempData.size() - 1; i >= 0; i--) {
			pData = vTempData.at(i);
			if (pData->IsActive())	StoreData(pData);
		}
	}
	else {
		for (int i = 0; i < vTempData.size(); i++) {
			pData = vTempData.at(i);
			if (pData->IsActive())	StoreData(pData);
		}
	}
	SetDataLoaded(true);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// ��������������
//
/////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataHistoryCandleExtend::UpdateData(vector<CDayLinePtr>& vTempDayLine, bool fRevertSave) {
	CDayLinePtr pDayLine = nullptr;
	Unload(); // �����������������ݣ�����еĻ���
	// ������������ʱ��Ϊ�������
	if (fRevertSave) {
		for (int i = vTempDayLine.size() - 1; i >= 0; i--) {
			pDayLine = vTempDayLine.at(i);
			if (pDayLine->IsActive()) {
				// ��������ٽ��ף�ͣ�ƻ����к���ֵģ��Ĺ�Ʊ����
				StoreData(pDayLine);
			}
		}
	}
	else {
		for (int i = 0; i < vTempDayLine.size(); i++) {
			pDayLine = vTempDayLine.at(i);
			if (pDayLine->IsActive()) {
				// ��������ٽ��ף�ͣ�ƻ����к���ֵģ��Ĺ�Ʊ����
				StoreData(pDayLine);
			}
		}
	}
	SetDataLoaded(true);
}

void CVirtualDataHistoryCandleExtend::ShowData(CDC* pDC, CRect rectClient) {
	constexpr COLORREF crBlue(RGB(0, 0, 255)), crGreen(RGB(0, 255, 0)), crWhite(RGB(255, 255, 255)), crRed(RGB(255, 0, 0));
	CPen penGreen1(PS_SOLID, 1, crGreen), penWhite1(PS_SOLID, 1, crWhite), penRed1(PS_SOLID, 1, crRed);
	long lHigh = 0;
	long lDate{ 0 };
	vector<CVirtualHistoryCandleExtendPtr>::iterator it = m_vHistoryData.end();
	it--;
	int i = 0, y = 0;
	long lLow = (*it)->GetLow();
	for (; it != m_vHistoryData.begin(); it--) {
		if (lHigh < (*it)->GetHigh()) lHigh = (*it)->GetHigh();
		if ((*it)->GetLow() > 0) {
			if (lLow > (*it)->GetLow()) lLow = (*it)->GetLow();
		}
		if (3 * i > m_vHistoryData.size()) break;
		if (rectClient.right <= 3 * i) break; // ����
		else i++;
	}

	it = m_vHistoryData.end();
	it--;
	i = 0;
	long x = 0;
	pDC->SelectObject(&penRed1);
	for (; it != m_vHistoryData.begin(); it--) {
		x = rectClient.right - 2 - i * 3;
		y = (0.5 - static_cast<double>((*it)->GetHigh() - lLow) / (2 * (lHigh - lLow))) * rectClient.Height();
		pDC->MoveTo(x, y);
		if ((*it)->GetHigh() == (*it)->GetLow()) {
			y = y - 1;
		}
		else {
			y = (0.5 - static_cast<double>((*it)->GetLow() - lLow) / (2 * (lHigh - lLow))) * rectClient.Height();
		}
		pDC->LineTo(x, y);
		lDate = (*it)->GetMarketDate();
		i++;
		if (3 * i > m_vHistoryData.size()) break;
		if (rectClient.right <= 3 * i) break; // ����������߿�Ϊֹ
	}
}

bool CVirtualDataHistoryCandleExtend::GetStartEndDate(long& lStartDate, long& lEndDate) {
	if (m_vHistoryData.size() == 0) return false;

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
	double dTempRS = 0;
	const INT64 lTotalNumber = m_vHistoryData.size();
	for (INT64 i = lNumber; i < lTotalNumber; i++) {
		dTempRS = 0;
		for (INT64 j = i - lNumber; j < i; j++) {
			dTempRS += m_vHistoryData.at(j)->GetRSLogarithm();
		}
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
	double dTempRS = 0;
	const INT64 lTotalNumber = m_vHistoryData.size();
	for (INT64 i = lNumber; i < lTotalNumber; i++) {
		dTempRS = 0;
		for (INT64 j = i - lNumber; j < i; j++) {
			dTempRS += m_vHistoryData.at(j)->GetRS();
		}
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
	double dTempRS = 0;
	const INT64 lTotalNumber = m_vHistoryData.size();
	for (INT64 i = lNumber; i < lTotalNumber; i++) {
		dTempRS = 0;
		for (INT64 j = i - lNumber; j < i; j++) {
			dTempRS += m_vHistoryData.at(j)->GetRSIndex();
		}
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

void CVirtualDataHistoryCandleExtend::GetRS1(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->GetRSIndex();
	}
}

void CVirtualDataHistoryCandleExtend::GetRSIndex1(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->GetRSIndex();
	}
}

void CVirtualDataHistoryCandleExtend::GetRSLogarithm1(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->GetRSLogarithm();
	}
}

void CVirtualDataHistoryCandleExtend::GetRS3(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get3RS();
	}
}

void CVirtualDataHistoryCandleExtend::GetRS5(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get5RS();
	}
}

void CVirtualDataHistoryCandleExtend::GetRS10(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get10RS();
	}
}

void CVirtualDataHistoryCandleExtend::GetRS30(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get30RS();
	}
}

void CVirtualDataHistoryCandleExtend::GetRS60(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get60RS();
	}
}

void CVirtualDataHistoryCandleExtend::GetRS120(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get120RS();
	}
}