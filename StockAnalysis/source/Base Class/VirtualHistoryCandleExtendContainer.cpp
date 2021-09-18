#include"pch.h"

#include"globedef.h"
#include"VirtualHistoryCandleExtendContainer.h"

CVirtualHistoryCandleExtendContainer::CVirtualHistoryCandleExtendContainer() : CObject() {
	Reset();
}

CVirtualHistoryCandleExtendContainer::~CVirtualHistoryCandleExtendContainer() {
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 更新日线容器。
//
/////////////////////////////////////////////////////////////////////////////////////
void CVirtualHistoryCandleExtendContainer::UpdateData(vector<CVirtualHistoryCandleExtendPtr>& vTempData) {
	CVirtualHistoryCandleExtendPtr pData = nullptr;
	Unload(); // 清除已载入的数据（如果有的话）
	// 将日线数据以时间为正序存入
	for (int i = 0; i < vTempData.size(); i++) {
		pData = vTempData.at(i);
		StoreData(pData);
	}
	SetDataLoaded(true);
}

void CVirtualHistoryCandleExtendContainer::ShowData(CDC* pDC, CRect rectClient) {
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
		if (rectClient.right <= 3 * i) break; // 画到
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
		lDate = (*it)->GetFormatedMarketDate();
		i++;
		if (3 * i > m_vHistoryData.size()) break;
		if (rectClient.right <= 3 * i) break; // 画到窗口左边框为止
	}
}

void CVirtualHistoryCandleExtendContainer::Reset(void) {
	m_vHistoryData.clear();

	m_fDataLoaded = false;
	m_fDatabaseTodayUpdated = false;

	m_fLoadDataFirst = false;
}

bool CVirtualHistoryCandleExtendContainer::CalculateRS0(void) {
	CalculateRS1(3);
	CalculateRS1(5);
	CalculateRS1(10);
	CalculateRS1(30);
	CalculateRS1(60);
	CalculateRS1(120);
	return true;
}

bool CVirtualHistoryCandleExtendContainer::CalculateRSLogarithm0(void) {
	CalculateRSLogarithm1(3);
	CalculateRSLogarithm1(5);
	CalculateRSLogarithm1(10);
	CalculateRSLogarithm1(30);
	CalculateRSLogarithm1(60);
	CalculateRSLogarithm1(120);
	return true;
}

bool CVirtualHistoryCandleExtendContainer::CalculateRSIndex0(void) {
	CalculateRSIndex1(3);
	CalculateRSIndex1(5);
	CalculateRSIndex1(10);
	CalculateRSIndex1(30);
	CalculateRSIndex1(60);
	CalculateRSIndex1(120);
	return true;
}

bool CVirtualHistoryCandleExtendContainer::CalculateRSLogarithm1(INT64 lNumber) {
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

bool CVirtualHistoryCandleExtendContainer::CalculateRS1(INT64 lNumber) {
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

bool CVirtualHistoryCandleExtendContainer::CalculateRSIndex1(INT64 lNumber) {
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

void CVirtualHistoryCandleExtendContainer::GetRS1(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->GetRSIndex();
	}
}

void CVirtualHistoryCandleExtendContainer::GetRSIndex1(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->GetRSIndex();
	}
}

void CVirtualHistoryCandleExtendContainer::GetRSLogarithm1(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->GetRSLogarithm();
	}
}

void CVirtualHistoryCandleExtendContainer::GetRS3(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get3RS();
	}
}

void CVirtualHistoryCandleExtendContainer::GetRS5(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get5RS();
	}
}

void CVirtualHistoryCandleExtendContainer::GetRS10(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get10RS();
	}
}

void CVirtualHistoryCandleExtendContainer::GetRS30(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get30RS();
	}
}

void CVirtualHistoryCandleExtendContainer::GetRS60(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get60RS();
	}
}

void CVirtualHistoryCandleExtendContainer::GetRS120(vector<double>& vRS) {
	for (int i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get120RS();
	}
}