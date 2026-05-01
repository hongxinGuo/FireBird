#include"pch.h"

#include"VirtualDataHistoryCandle.h"
#include"DayLine.h"

CVirtualDataHistoryCandle::CVirtualDataHistoryCandle() {
	Reset();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将日线历史数据存入数据库．默认数据库为空。
//  此函数被工作线程调用，需要注意数据同步问题。
//  当存在旧日线历史数据时，本函数只是更新。
//
//  具体操作的数据表由第一个参数传入，
//  自动删除旧数据中的重复数据。
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CVirtualDataHistoryCandle::UpdateBasicDB(CVirtualSetHistoryCandle* pSetHistoryCandle, const string& strStockSymbol) const {
	vector<CVirtualHistoryCandlePtr> vOldHistoryCandle;
	CVirtualHistoryCandlePtr pHistoryCandle = nullptr;
	long lSizeOfOldDayLine = 0;
	bool fNeedUpdate = false;

	ASSERT(Size() > 0);

	const size_t lSize = Size();
	if (!strStockSymbol.empty()) {
		pSetHistoryCandle->m_strFilter = "[Symbol] = '";
		pSetHistoryCandle->m_strFilter += strStockSymbol.c_str();
		pSetHistoryCandle->m_strFilter += "'";
		pSetHistoryCandle->m_strSort = "[Date]";

		if (pSetHistoryCandle->Open()) {
			long lLastDate = 0;
			pSetHistoryCandle->m_pDatabase->BeginTrans();
			while (!pSetHistoryCandle->IsEOF()) {
				if (pSetHistoryCandle->m_Date > lLastDate) {
					lLastDate = pSetHistoryCandle->m_Date;
					pHistoryCandle = make_shared<CVirtualHistoryCandle>();
					pHistoryCandle->LoadBasicData(pSetHistoryCandle);

					vOldHistoryCandle.push_back(pHistoryCandle);
					lSizeOfOldDayLine++;
				}
				else {
					pSetHistoryCandle->Delete(); //删除日期重复的数据
				}
				pSetHistoryCandle->MoveNext();
			}
			pSetHistoryCandle->m_pDatabase->CommitTrans();
			pSetHistoryCandle->Close();
		}
	}
	pSetHistoryCandle->m_strFilter = "[ID] = 1";
	if (pSetHistoryCandle->Open()) {
		pSetHistoryCandle->m_pDatabase->BeginTrans();
		if (lSizeOfOldDayLine > 0) {// 有旧数据
			long lCurrentPos = 0;
			for (size_t i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
				pHistoryCandle = GetData(i);
				if (pHistoryCandle->GetDate() < vOldHistoryCandle.at(0)->GetDate()) {	// 有更早的新数据？
					pHistoryCandle->AppendBasicData(pSetHistoryCandle);
				}
				else {
					while ((lCurrentPos < lSizeOfOldDayLine) && (vOldHistoryCandle.at(lCurrentPos)->GetDate() < pHistoryCandle->GetDate())) lCurrentPos++;
					if (lCurrentPos < lSizeOfOldDayLine) {
						if (vOldHistoryCandle.at(lCurrentPos)->GetDate() > pHistoryCandle->GetDate()) { // 前数据集中有遗漏的日期
							pHistoryCandle->AppendBasicData(pSetHistoryCandle);
							fNeedUpdate = true;
						}
					}
					else {
						pHistoryCandle->AppendBasicData(pSetHistoryCandle);
						fNeedUpdate = true;
					}
				}
			}
		}
		else {// 没有旧数据
			for (size_t i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
				pHistoryCandle = GetData(i);
				pHistoryCandle->AppendBasicData(pSetHistoryCandle);
			}
		}
		pSetHistoryCandle->m_pDatabase->CommitTrans();
		pSetHistoryCandle->Close();
	}

	return fNeedUpdate;
}

bool CVirtualDataHistoryCandle::LoadBasicDB(CVirtualSetHistoryCandle* pSetHistoryCandle) {
	if (gl_systemConfiguration.IsWorkingMode())
		ASSERT(!m_fBasicDataLoaded);
	ASSERT(pSetHistoryCandle->IsOpen());

	Unload(); // 卸载之前的日线
	// 装入DayLine数据
	while (!pSetHistoryCandle->IsEOF()) {
		const auto pHistoryCandle = make_shared<CVirtualHistoryCandle>();
		pHistoryCandle->LoadBasicData(pSetHistoryCandle);
		Add(pHistoryCandle);
		pSetHistoryCandle->MoveNext();
	}
	m_fBasicDataLoaded = true;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 更新日线容器。
//
/////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataHistoryCandle::UpdateData(const vector<CVirtualHistoryCandlePtr>& vTempData) {
	Unload(); // 清除已载入的数据（如果有的话）
	for (const auto& p : vTempData) {
		if (p->IsActive()) Add(p);
	}
	SetDataLoaded(true);
}
void CVirtualDataHistoryCandle::UpdateData(const vector<CDayLinePtr>& vTempData) {
	Unload(); // 清除已载入的数据（如果有的话）
	for (const auto& p : vTempData) {
		if (p->IsActive()) Add(p);
	}
	SetDataLoaded(true);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 更新日线容器。
//
/////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataHistoryCandle::UpdateData(const CDayLinesPtr& vTempDayLine) {
	Unload(); // 清除已载入的日线数据（如果有的话）
	// 将日线数据以时间为正序存入
	for (const auto& pDayLine : *vTempDayLine) {
		if (pDayLine->IsActive()) {
			// 清除掉不再交易（停牌或退市后出现的）的股票日线
			Add(pDayLine);
		}
	}
	SetDataLoaded(true);
}

bool CVirtualDataHistoryCandle::GetStartEndDate(long& lStartDate, long& lEndDate) const {
	if (m_vHistoryData.empty()) return false;

	lStartDate = m_vHistoryData.at(0)->GetDate();
	lEndDate = m_vHistoryData.at(m_vHistoryData.size() - 1)->GetDate();
	return true;
}

bool CVirtualDataHistoryCandle::HaveDayLine(long lDate) {
	auto it = ranges::find_if(m_vHistoryData.begin(), m_vHistoryData.end(),
	                          [lDate](const CVirtualHistoryCandlePtr& p) { return p->GetDate() == lDate; });
	if (it == m_vHistoryData.end()) {
		return false;
	}
	return true;
}

CVirtualHistoryCandlePtr CVirtualDataHistoryCandle::GetCandle(long lDate) {
	auto it = std::ranges::find_if(m_vHistoryData.begin(), m_vHistoryData.end(),
	                               [lDate](const CVirtualHistoryCandlePtr& p) { return (p->GetDate() == lDate); });
	if (it != m_vHistoryData.end()) {
		return *it;
	}
	else return nullptr;
}

void CVirtualDataHistoryCandle::Reset() {
	m_vHistoryData.clear();

	m_fDataLoaded = false;
	m_fDatabaseTodayUpdated = false;

	m_fBasicDataLoaded = false;
}

void CVirtualDataHistoryCandle::CalculateMA(size_t length) const {
	if (m_vHistoryData.size() < length) return;

	long lSumMA = 0;
	for (size_t i = 0; i < length - 1; i++) {
		lSumMA += m_vHistoryData.at(i)->GetClose();
	}
	for (size_t i = length - 1; i < m_vHistoryData.size(); i++) {
		lSumMA += m_vHistoryData.at(i)->GetClose();
		const long lMA = lSumMA / length;
		m_vHistoryData.at(i)->SetAverage(length, lMA);
		lSumMA -= m_vHistoryData.at(i - (length - 1))->GetClose();
	}
}

bool CVirtualDataHistoryCandle::CalculateRS0() {
	CalculateRS1(3);
	CalculateRS1(5);
	CalculateRS1(10);
	CalculateRS1(30);
	CalculateRS1(60);
	CalculateRS1(120);
	return true;
}

bool CVirtualDataHistoryCandle::CalculateRSLogarithm0() {
	CalculateRSLogarithm1(3);
	CalculateRSLogarithm1(5);
	CalculateRSLogarithm1(10);
	CalculateRSLogarithm1(30);
	CalculateRSLogarithm1(60);
	CalculateRSLogarithm1(120);
	return true;
}

bool CVirtualDataHistoryCandle::CalculateRSIndex0() {
	CalculateRSIndex1(3);
	CalculateRSIndex1(5);
	CalculateRSIndex1(10);
	CalculateRSIndex1(30);
	CalculateRSIndex1(60);
	CalculateRSIndex1(120);
	return true;
}

bool CVirtualDataHistoryCandle::CalculateRSLogarithm1(INT64 lNumber) {
	const INT64 lTotalNumber = m_vHistoryData.size();
	for (INT64 i = lNumber; i < lTotalNumber; i++) {
		double dTempRS = 0;
		for (INT64 j = i - lNumber; j < i; j++) { dTempRS += m_vHistoryData.at(j)->GetRSLogarithm(); }
		switch (lNumber) {
		case 3:
			m_vHistoryData.at(i)->Set3RS(dTempRS / lNumber);
			break;
		case 5:
			m_vHistoryData.at(i)->Set5RS(dTempRS / lNumber);
			break;
		case 10:
			m_vHistoryData.at(i)->Set10RS(dTempRS / lNumber);
			break;
		case 30:
			m_vHistoryData.at(i)->Set30RS(dTempRS / lNumber);
			break;
		case 60:
			m_vHistoryData.at(i)->Set60RS(dTempRS / lNumber);
			break;
		case 120:
			m_vHistoryData.at(i)->Set120RS(dTempRS / lNumber);
			break;
		default:
			ASSERT(0);
		}
	}
	return true;
}

bool CVirtualDataHistoryCandle::CalculateRS1(INT64 lNumber) {
	const INT64 lTotalNumber = m_vHistoryData.size();
	for (INT64 i = lNumber; i < lTotalNumber; i++) {
		double dTempRS = 0;
		for (INT64 j = i - lNumber; j < i; j++) { dTempRS += m_vHistoryData.at(j)->GetRS(); }
		switch (lNumber) {
		case 3:
			m_vHistoryData.at(i)->Set3RS(dTempRS / lNumber);
			break;
		case 5:
			m_vHistoryData.at(i)->Set5RS(dTempRS / lNumber);
			break;
		case 10:
			m_vHistoryData.at(i)->Set10RS(dTempRS / lNumber);
			break;
		case 30:
			m_vHistoryData.at(i)->Set30RS(dTempRS / lNumber);
			break;
		case 60:
			m_vHistoryData.at(i)->Set60RS(dTempRS / lNumber);
			break;
		case 120:
			m_vHistoryData.at(i)->Set120RS(dTempRS / lNumber);
			break;
		default:
			ASSERT(0);
		}
	}
	return true;
}

bool CVirtualDataHistoryCandle::CalculateRSIndex1(INT64 lNumber) {
	const INT64 lTotalNumber = m_vHistoryData.size();
	for (INT64 i = lNumber; i < lTotalNumber; i++) {
		double dTempRS = 0;
		for (INT64 j = i - lNumber; j < i; j++) {
			dTempRS += m_vHistoryData.at(j)->GetRSIndex();
		}
		switch (lNumber) {
		case 3:
			m_vHistoryData.at(i)->Set3RS(dTempRS / lNumber);
			break;
		case 5:
			m_vHistoryData.at(i)->Set5RS(dTempRS / lNumber);
			break;
		case 10:
			m_vHistoryData.at(i)->Set10RS(dTempRS / lNumber);
			break;
		case 30:
			m_vHistoryData.at(i)->Set30RS(dTempRS / lNumber);
			break;
		case 60:
			m_vHistoryData.at(i)->Set60RS(dTempRS / lNumber);
			break;
		case 120:
			m_vHistoryData.at(i)->Set120RS(dTempRS / lNumber);
			break;
		default:
			ASSERT(0);
		}
	}
	return true;
}

void CVirtualDataHistoryCandle::GetRS1(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->GetRSIndex();
	}
}

void CVirtualDataHistoryCandle::GetRSIndex1(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->GetRSIndex();
	}
}

void CVirtualDataHistoryCandle::GetRSLogarithm1(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->GetRSLogarithm();
	}
}

void CVirtualDataHistoryCandle::GetRS3(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get3RS();
	}
}

void CVirtualDataHistoryCandle::GetRS5(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get5RS();
	}
}

void CVirtualDataHistoryCandle::GetRS10(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get10RS();
	}
}

void CVirtualDataHistoryCandle::GetRS30(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get30RS();
	}
}

void CVirtualDataHistoryCandle::GetRS60(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get60RS();
	}
}

void CVirtualDataHistoryCandle::GetRS120(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i)->Get120RS();
	}
}

void CVirtualDataHistoryCandle::ToShow(CDC* pDC, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
	LOGBRUSH logBrushWhite, logBrushRed;
	logBrushWhite.lbStyle = BS_SOLID;
	logBrushWhite.lbColor = RGB(255, 255, 255);
	//logBrushWhite.lbHatch = 0;
	logBrushRed.lbStyle = BS_SOLID;
	logBrushRed.lbColor = RGB(255, 0, 0);
	logBrushRed.lbHatch = 0;
	constexpr COLORREF crWhite(RGB(255, 255, 255)), crRed(RGB(255, 0, 0));
	CPen penWhite1(PS_SOLID, 1, crWhite), penWhiteN(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT, iStepWidth, &logBrushWhite), penRed1(PS_SOLID, 1, crRed);
	CPen penRedN(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT, iStepWidth, &logBrushRed);

	auto it = m_vHistoryData.end();
	long offset = iStepWidth / 2;
	--it;
	size_t i = 0;
	auto pOldPen = pDC->SelectObject(&penWhite1);
	for (; it != m_vHistoryData.begin(); --it) {
		const long x = rectClient.right - offset - i * iStepWidth;
		int y1 = rectClient.top + (1 - static_cast<double>((*it)->GetClose() - lLow) / (lHigh - lLow)) * rectClient.Height();
		int yHigh = rectClient.top + (1 - static_cast<double>((*it)->GetHigh() - lLow) / (lHigh - lLow)) * rectClient.Height();
		int yLow = rectClient.top + (1 - static_cast<double>((*it)->GetLow() - lLow) / (lHigh - lLow)) * rectClient.Height();
		int y2 = rectClient.top + (1 - static_cast<double>((*it)->GetOpen() - lLow) / (lHigh - lLow)) * rectClient.Height();
		int y3 = rectClient.top + (1 - static_cast<double>((*it)->GetClose() - lLow) / (lHigh - lLow)) * rectClient.Height();
		pDC->MoveTo(x, yHigh);

		if ((*it)->GetClose() == (*it)->GetOpen()) {
			pDC->SelectObject(penRed1);
			pDC->LineTo(x, y3);
			pDC->SelectObject(penRedN);
			pDC->LineTo(x, y3 + 1);
			pDC->SelectObject(penRed1);
			pDC->LineTo(x, yLow);
		}
		else if ((*it)->GetClose() > (*it)->GetOpen()) {
			pDC->SelectObject(penRed1);
			pDC->LineTo(x, y3);
			pDC->SelectObject(penRedN);
			pDC->LineTo(x, y2);
			pDC->SelectObject(penRed1);
			pDC->LineTo(x, yLow);
		}
		else {
			pDC->SelectObject(penWhite1);
			pDC->LineTo(x, y2);
			pDC->SelectObject(penWhiteN);
			pDC->LineTo(x, y3);
			pDC->SelectObject(penWhite1);
			pDC->LineTo(x, yLow);
		}

		i++;
		if (i >= m_vHistoryData.size()) break;
		if (rectClient.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	pDC->SelectObject(pOldPen);
}

void CVirtualDataHistoryCandle::ShowRS3(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->Get3RS() - 50.0) * dZoomInRatio;
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandle::ShowRS5(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->Get5RS() - 50.0) * dZoomInRatio;
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandle::ShowRS10(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->Get10RS() - 50.0) * dZoomInRatio;
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandle::ShowRS30(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->Get30RS() - 50.0) * dZoomInRatio;
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandle::ShowRS60(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->Get60RS() - 50.0) * dZoomInRatio;
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandle::ShowRS120(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->Get120RS() - 50.0) * dZoomInRatio;
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandle::ShowRSIndex(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->GetRSIndex() - 50.0);
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandle::ShowRSLogarithm(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->GetRSLogarithm() - 50.0);
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandle::ShowRS1(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
	vector<double> vData;
	vData.reserve(rectDrawArea.right / iStepWidth + 1);

	auto it = m_vHistoryData.end();
	int i = 1;
	--it;
	for (; it != m_vHistoryData.begin(); --it, i++) {
		int y = 50.0 + ((*it)->GetRS() - 50.0);
		vData.push_back(y);
		if (i >= m_vHistoryData.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	ShowLine(pDC, pNewPen, rectDrawArea, iStepWidth, vData);
}

void CVirtualDataHistoryCandle::ShowLine(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, const vector<double>& vData) {
	auto it = vData.begin();
	int i = 1;

	int y = rectDrawArea.bottom - *it * rectDrawArea.Height() / 100;
	auto pLodPen = pDC->SelectObject(pNewPen);
	pDC->MoveTo(rectDrawArea.right - 1, y);
	for (; it != vData.end(); ++it, i++) {
		y = rectDrawArea.bottom - *it * rectDrawArea.Height() / 100;
		pDC->LineTo(rectDrawArea.right - 1 - iStepWidth * i, y);
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	pDC->SelectObject(pLodPen);
}

std::pair<long, long> CVirtualDataHistoryCandle::GetHighLow(int iCandleNumber) {
	long lHigh = 0;
	auto it = m_vHistoryData.end();
	--it;
	int i = 0;
	long lLow = (*it)->GetLow();
	for (; it != m_vHistoryData.begin(); --it) {
		if (lHigh < (*it)->GetHigh()) lHigh = (*it)->GetHigh();
		if ((*it)->GetLow() > 0) { if (lLow > (*it)->GetLow()) lLow = (*it)->GetLow(); }
		if (i > m_vHistoryData.size()) break;
		if (i >= iCandleNumber) break;
		i++;
	}
	return { lHigh, lLow };
}
