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
bool CVirtualDataHistoryCandle::UpdateBasicDB(CVirtualSetHistoryCandle* pSetHistoryCandle, const string& strStockSymbol) {
	vector<CVirtualHistoryCandle> vOldHistoryCandle;
	vOldHistoryCandle.reserve(3000);
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
			CVirtualHistoryCandle historyCandle;
			historyCandle.SetRatio(m_ratio);
			long lLastDate = 0;
			pSetHistoryCandle->m_pDatabase->BeginTrans();
			while (!pSetHistoryCandle->IsEOF()) {
				if (pSetHistoryCandle->m_Date > lLastDate) {
					lLastDate = pSetHistoryCandle->m_Date;
					historyCandle.Reset();
					historyCandle.LoadBasicData(pSetHistoryCandle);
					vOldHistoryCandle.push_back(historyCandle);
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
				CVirtualHistoryCandle* pCandle = GetData(i);
				if (pCandle->GetDate() < vOldHistoryCandle.at(0).GetDate()) {	// 有更早的新数据？
					pCandle->AppendBasicData(pSetHistoryCandle);
				}
				else {
					while ((lCurrentPos < lSizeOfOldDayLine) && (vOldHistoryCandle.at(lCurrentPos).GetDate() < pCandle->GetDate())) lCurrentPos++;
					if (lCurrentPos < lSizeOfOldDayLine) {
						if (vOldHistoryCandle.at(lCurrentPos).GetDate() > pCandle->GetDate()) { // 前数据集中有遗漏的日期
							pCandle->AppendBasicData(pSetHistoryCandle);
							fNeedUpdate = true;
						}
					}
					else {
						pCandle->AppendBasicData(pSetHistoryCandle);
						fNeedUpdate = true;
					}
				}
			}
		}
		else {// 没有旧数据
			for (size_t i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
				GetData(i)->AppendBasicData(pSetHistoryCandle);
			}
		}
		pSetHistoryCandle->m_pDatabase->CommitTrans();
		pSetHistoryCandle->Close();
	}

	return fNeedUpdate;
}

bool CVirtualDataHistoryCandle::LoadBasicDB(CVirtualSetHistoryCandle* pSetHistoryCandle) {
	if (gl_systemConfiguration.IsWorkingMode())
		ASSERT(pSetHistoryCandle->IsOpen());

	Unload(); // 卸载之前的日线
	Reserve(3000);
	// 装入DayLine数据
	while (!pSetHistoryCandle->IsEOF()) {
		CVirtualHistoryCandle historyCandle;
		historyCandle.Reset();
		historyCandle.SetRatio(m_ratio);
		historyCandle.LoadBasicData(pSetHistoryCandle);
		Add(historyCandle);
		pSetHistoryCandle->MoveNext();
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 更新日线容器。
//
/////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataHistoryCandle::UpdateData(const vector<CVirtualHistoryCandle>& vTempData) {
	Unload(); // 清除已载入的数据（如果有的话）
	Reserve(vTempData.size());
	for (auto& p : vTempData) {
		if (p.IsActive()) Add(p);
	}
	SetDataLoaded(true);
}

void CVirtualDataHistoryCandle::UpdateData(const vector<CDayLine>& vTempData) {
	Unload(); // 清除已载入的数据（如果有的话）
	Reserve(vTempData.size());
	for (const auto& p : vTempData) {
		if (p.IsActive()) Add(p);
	}
	SetDataLoaded(true);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 更新日线容器。
//
/////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataHistoryCandle::UpdateData(const CDayLinesPtr& pvTempDayLine) {
	Unload(); // 清除已载入的日线数据（如果有的话）
	Reserve(pvTempDayLine->size());
	// 将日线数据以时间为正序存入
	for (auto& dayLine : *pvTempDayLine) {
		if (dayLine.IsActive()) {	// 清除掉不再交易（停牌或退市后出现的）的股票日线
			Add(dayLine);
		}
	}
	SetDataLoaded(true);
}

bool CVirtualDataHistoryCandle::GetStartEndDate(long& lStartDate, long& lEndDate) const {
	if (m_vHistoryData.empty()) return false;

	lStartDate = m_vHistoryData.at(0).GetDate();
	lEndDate = m_vHistoryData.at(m_vHistoryData.size() - 1).GetDate();
	ASSERT(lStartDate <= lEndDate);
	return true;
}

bool CVirtualDataHistoryCandle::HaveDayLine(long lDate) {
	auto it = ranges::find_if(m_vHistoryData.begin(), m_vHistoryData.end(),
	                          [lDate](const CVirtualHistoryCandle& p) { return p.GetDate() == lDate; });
	if (it == m_vHistoryData.end()) {
		return false;
	}
	return true;
}

CVirtualHistoryCandle* CVirtualDataHistoryCandle::GetCandle(long lDate) {
	auto it = std::ranges::find_if(m_vHistoryData.begin(), m_vHistoryData.end(),
	                               [lDate](const CVirtualHistoryCandle& p) { return p.GetDate() == lDate; });
	if (it != m_vHistoryData.end()) {
		return &*it;
	}
	return nullptr;
}

void CVirtualDataHistoryCandle::Reset() {
	m_vHistoryData.clear();

	m_fDataLoaded = false;
	m_fDatabaseTodayUpdated = false;
}

void CVirtualDataHistoryCandle::CalculateMA(size_t length) {
	if (m_vHistoryData.size() < length) return;

	long lSumMA = 0;
	for (size_t i = 0; i < length - 1; i++) {
		lSumMA += m_vHistoryData.at(i).GetClose();
	}
	for (size_t i = length - 1; i < m_vHistoryData.size(); i++) {
		lSumMA += m_vHistoryData.at(i).GetClose();
		const long lMA = lSumMA / length;
		m_vHistoryData.at(i).SetAverage(length, lMA);
		lSumMA -= m_vHistoryData.at(i - (length - 1)).GetClose();
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
	const size_t lTotalNumber = m_vHistoryData.size();
	for (size_t i = lNumber; i < lTotalNumber; i++) {
		double dTempRS = 0;
		for (INT64 j = i - lNumber; j < i; j++) { dTempRS += m_vHistoryData.at(j).GetRSLogarithm(); }
		switch (lNumber) {
		case 3:
			m_vHistoryData.at(i).Set3RS(dTempRS / lNumber);
			break;
		case 5:
			m_vHistoryData.at(i).Set5RS(dTempRS / lNumber);
			break;
		case 10:
			m_vHistoryData.at(i).Set10RS(dTempRS / lNumber);
			break;
		case 30:
			m_vHistoryData.at(i).Set30RS(dTempRS / lNumber);
			break;
		case 60:
			m_vHistoryData.at(i).Set60RS(dTempRS / lNumber);
			break;
		case 120:
			m_vHistoryData.at(i).Set120RS(dTempRS / lNumber);
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
		for (INT64 j = i - lNumber; j < i; j++) { dTempRS += m_vHistoryData.at(j).GetRS(); }
		switch (lNumber) {
		case 3:
			m_vHistoryData.at(i).Set3RS(dTempRS / lNumber);
			break;
		case 5:
			m_vHistoryData.at(i).Set5RS(dTempRS / lNumber);
			break;
		case 10:
			m_vHistoryData.at(i).Set10RS(dTempRS / lNumber);
			break;
		case 30:
			m_vHistoryData.at(i).Set30RS(dTempRS / lNumber);
			break;
		case 60:
			m_vHistoryData.at(i).Set60RS(dTempRS / lNumber);
			break;
		case 120:
			m_vHistoryData.at(i).Set120RS(dTempRS / lNumber);
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
			dTempRS += m_vHistoryData.at(j).GetRSIndex();
		}
		switch (lNumber) {
		case 3:
			m_vHistoryData.at(i).Set3RS(dTempRS / lNumber);
			break;
		case 5:
			m_vHistoryData.at(i).Set5RS(dTempRS / lNumber);
			break;
		case 10:
			m_vHistoryData.at(i).Set10RS(dTempRS / lNumber);
			break;
		case 30:
			m_vHistoryData.at(i).Set30RS(dTempRS / lNumber);
			break;
		case 60:
			m_vHistoryData.at(i).Set60RS(dTempRS / lNumber);
			break;
		case 120:
			m_vHistoryData.at(i).Set120RS(dTempRS / lNumber);
			break;
		default:
			ASSERT(0);
		}
	}
	return true;
}

void CVirtualDataHistoryCandle::GetRS1(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i).GetRSIndex();
	}
}

void CVirtualDataHistoryCandle::GetRSIndex1(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i).GetRSIndex();
	}
}

void CVirtualDataHistoryCandle::GetRSLogarithm1(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i).GetRSLogarithm();
	}
}

void CVirtualDataHistoryCandle::GetRS3(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i).Get3RS();
	}
}

void CVirtualDataHistoryCandle::GetRS5(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i).Get5RS();
	}
}

void CVirtualDataHistoryCandle::GetRS10(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i).Get10RS();
	}
}

void CVirtualDataHistoryCandle::GetRS30(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i).Get30RS();
	}
}

void CVirtualDataHistoryCandle::GetRS60(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i).Get60RS();
	}
}

void CVirtualDataHistoryCandle::GetRS120(vector<double>& vRS) const {
	for (size_t i = 0; i < m_vHistoryData.size(); i++) {
		vRS[i] = m_vHistoryData.at(i).Get120RS();
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
		const auto& value = *it;
		const long x = rectClient.right - offset - i * iStepWidth;
		int y1 = rectClient.top + (1 - static_cast<double>(value.GetClose() - lLow) / (lHigh - lLow)) * rectClient.Height();
		int yHigh = rectClient.top + (1 - static_cast<double>(value.GetHigh() - lLow) / (lHigh - lLow)) * rectClient.Height();
		int yLow = rectClient.top + (1 - static_cast<double>(value.GetLow() - lLow) / (lHigh - lLow)) * rectClient.Height();
		int y2 = rectClient.top + (1 - static_cast<double>(value.GetOpen() - lLow) / (lHigh - lLow)) * rectClient.Height();
		int y3 = rectClient.top + (1 - static_cast<double>(value.GetClose() - lLow) / (lHigh - lLow)) * rectClient.Height();
		pDC->MoveTo(x, yHigh);

		if (value.GetClose() == value.GetOpen()) {
			pDC->SelectObject(penRed1);
			pDC->LineTo(x, y3);
			pDC->SelectObject(penRedN);
			pDC->LineTo(x, y3 + 1);
			pDC->SelectObject(penRed1);
			pDC->LineTo(x, yLow);
		}
		else if (value.GetClose() > value.GetOpen()) {
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
		const auto& value = *it;
		int y = 50.0 + (value.Get3RS() - 50.0) * dZoomInRatio;
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
		const auto& value = *it;
		int y = 50.0 + (value.Get5RS() - 50.0) * dZoomInRatio;
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
		const auto& value = *it;
		int y = 50.0 + (value.Get10RS() - 50.0) * dZoomInRatio;
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
		const auto& value = *it;
		int y = 50.0 + (value.Get30RS() - 50.0) * dZoomInRatio;
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
		const auto& value = *it;
		int y = 50.0 + (value.Get60RS() - 50.0) * dZoomInRatio;
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
		const auto& value = *it;
		int y = 50.0 + (value.Get120RS() - 50.0) * dZoomInRatio;
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
		const auto& value = *it;
		int y = 50.0 + (value.GetRSIndex() - 50.0);
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
		const auto& value = *it;
		int y = 50.0 + (value.GetRSLogarithm() - 50.0);
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
		const auto& value = *it;
		int y = 50.0 + (value.GetRS() - 50.0);
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
	auto value = *it;

	long lLow = value.GetLow();
	for (; it != m_vHistoryData.begin(); --it) {
		value = *it;
		if (lHigh < value.GetHigh()) lHigh = value.GetHigh();
		if (value.GetLow() > 0) {
			if (lLow > value.GetLow()) lLow = value.GetLow();
		}
		if (i > m_vHistoryData.size()) break;
		if (i >= iCandleNumber) break;
		i++;
	}
	return { lHigh, lLow };
}
