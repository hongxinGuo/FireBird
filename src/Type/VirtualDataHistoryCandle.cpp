module;
#include <afx.h>
#include <absl/log/absl_check.h>
module FireBirdLib.Container.HistoryCandle;

import FireBirdLib.HistoryCandle;
import FireBirdLib.HistoryCandle.DayLine;

import FireBirdLib.Accessory.TimeConvert;

import std;
using namespace std;
using std::chrono::local_days;

CVirtualDataHistoryCandle::CVirtualDataHistoryCandle() {
	Reset();
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 更新日线容器。
//
/////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataHistoryCandle::UpdateData(const vector<CVirtualHistoryCandle>& vTempData) {
	Reserve(vTempData.size());
	for (auto& p : vTempData) {
		if (p.IsActive()) Add(p);
	}
	SetDataLoaded(true);
}

void CVirtualDataHistoryCandle::UpdateData(const vector<CDayLine>& vTempData) {
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
	Reserve(pvTempDayLine->size());
	// 将日线数据以时间为正序存入
	for (auto& dayLine : *pvTempDayLine) {
		if (dayLine.IsActive()) {	// 清除掉不再交易（停牌或退市后出现的）的股票日线
			Add(dayLine);
		}
	}
	SetDataLoaded(true);
}

bool CVirtualDataHistoryCandle::GetStartEndDate(local_days& lStartDate, local_days& lEndDate) const {
	if (m_vHistoryData.empty()) return false;

	lStartDate = m_vHistoryData.at(0).GetDate();
	lEndDate = m_vHistoryData.at(m_vHistoryData.size() - 1).GetDate();
	ABSL_CHECK(lStartDate <= lEndDate);
	return true;
}

void CVirtualDataHistoryCandle::Add(const CDayLine& data) {
	Add(static_cast<CVirtualHistoryCandle>(data));
}

bool CVirtualDataHistoryCandle::HaveDayLine(local_days date) {
	return std::ranges::any_of(m_vHistoryData.begin(), m_vHistoryData.end(),
	                           [date](const CVirtualHistoryCandle& p) { return p.GetDate() == date; });
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 历史数据是按照日期顺序存储的，所以可以使用二分法来查找元素，比顺序查找要快得多。
///
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
CVirtualHistoryCandle* CVirtualDataHistoryCandle::GetCandle(local_days date) {
	auto it = std::ranges::lower_bound(m_vHistoryData, date,
	                                   std::ranges::less{},
	                                   [](const CVirtualHistoryCandle& c) { return c.GetDate(); });
	if (it != m_vHistoryData.end() && it->GetDate() == date) {
		return &*it;
	}
	return nullptr;
}

CVirtualHistoryCandle* CVirtualDataHistoryCandle::GetCandle2(local_days date) {
	auto& localHistoryData = m_vHistoryData;
	auto it = std::ranges::find_if(localHistoryData.begin(), localHistoryData.end(),
	                               [date](const CVirtualHistoryCandle& p) { return p.GetDate() == date; });
	if (it != localHistoryData.end()) {
		return &*it;
	}
	return nullptr;
}

void CVirtualDataHistoryCandle::Reset() {
	m_vHistoryData.clear();
	m_vHistoryData.shrink_to_fit();
	m_fDataLoaded = false;
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

void CVirtualDataHistoryCandle::CreateWeekLine(CVirtualDataHistoryCandle& dataDayLine) {
	ABSL_CHECK(dataDayLine.IsDataLoaded());
	size_t index = 0;
	CVirtualHistoryCandle weekLine;
	size_t dayLineSize = dataDayLine.Size();
	long lastClose = 0;
	while (index < dayLineSize) {
		weekLine.Reset();
		auto pDayLine = dataDayLine.GetData(index);
		auto lCurrentEndDate = GetNextMonday(pDayLine->GetDate());
		weekLine.SetDate(pDayLine->GetDate());
		weekLine.SetLastClose(lastClose);
		weekLine.SetOpen(pDayLine->GetOpen());
		weekLine.SetLow(pDayLine->GetLow());
		do {
			if (pDayLine->GetHigh() > weekLine.GetHigh()) weekLine.SetHigh(pDayLine->GetHigh());
			if (pDayLine->GetLow() < weekLine.GetLow()) weekLine.SetLow(pDayLine->GetLow());
			weekLine.SetVolume(weekLine.GetVolume() + pDayLine->GetVolume());
			weekLine.SetAmount(weekLine.GetAmount() + pDayLine->GetAmount());
			weekLine.SetClose(pDayLine->GetClose());
			index++;
			if (index < dayLineSize) pDayLine = dataDayLine.GetData(index);
			else break;
			if (pDayLine->GetDate() >= lCurrentEndDate) break;
		} while (true);
		lastClose = weekLine.GetClose();
		if (weekLine.GetClose() > 0) Add(weekLine); // 有数据才存储
	}

	SetDataLoaded(true);
}

void CVirtualDataHistoryCandle::CreateMonthLine(CVirtualDataHistoryCandle& dataDayLine) {
	ABSL_CHECK(dataDayLine.IsDataLoaded());
	size_t index = 0;
	CVirtualHistoryCandle monthLine;
	size_t monthLineSize = dataDayLine.Size();
	while (index < monthLineSize) {
		monthLine.Reset();
		auto pDayLine = dataDayLine.GetData(index++);
		local_days lCurrentEndDate = GetNextMonth(pDayLine->GetDate());
		monthLine.SetDate(pDayLine->GetDate());
		monthLine.SetOpen(pDayLine->GetOpen());
		monthLine.SetLow(pDayLine->GetLow());
		do {
			if (pDayLine->GetHigh() > monthLine.GetHigh()) monthLine.SetHigh(pDayLine->GetHigh());
			if (pDayLine->GetLow() < monthLine.GetLow()) monthLine.SetLow(pDayLine->GetLow());
			monthLine.SetVolume(monthLine.GetVolume() + pDayLine->GetVolume());
			monthLine.SetAmount(monthLine.GetAmount() + pDayLine->GetAmount());
			monthLine.SetClose(pDayLine->GetClose());
			if (index < monthLineSize) pDayLine = dataDayLine.GetData(index);
			else break;
			if (pDayLine->GetDate() < lCurrentEndDate) index++;
			else break;
		} while (true);

		if (monthLine.GetClose() > 0) Add(monthLine); // 有数据才存储
	}

	SetDataLoaded(true);
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
	size_t i = 0;
	auto value = *it;

	long lLow = value.GetLow();
	for (; it != m_vHistoryData.begin(); --it) {
		value = *it;
		lHigh = std::max<long long>(lHigh, value.GetHigh());
		if (value.GetLow() > 0) {
			lLow = std::min<long long>(lLow, value.GetLow());
		}
		if (i > m_vHistoryData.size()) break;
		if (i >= iCandleNumber) break;
		i++;
	}
	return { lHigh, lLow };
}
