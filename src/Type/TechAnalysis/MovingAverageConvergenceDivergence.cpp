#include"pch.h"

#include "MovingAverageConvergenceDivergence.h"

double MACDResult::Max() {
	if (macd >= hist) {
		if (macd >= signal) return macd;
		else return signal;
	}
	else {
		if (signal < hist) return hist;
		else return signal;
	}
}

double MACDResult::Min() {
	if (macd <= hist) {
		if (macd <= signal) return macd;
		else return signal;
	}
	else {
		if (signal > hist) return hist;
		else return signal;
	}
}

void CIndicatorMACD::reset() {
	fastEMA_.reset();
	slowEMA_.reset();
	signalEMA_.reset();
	currentHigh_ = std::numeric_limits<double>::quiet_NaN();
	currentLow_ = std::numeric_limits<double>::quiet_NaN();
	currentDif_ = std::numeric_limits<double>::quiet_NaN();
}

// Batch compute: returns vector of Result aligned with input prices; entries before availability are NaN.
void CIndicatorMACD::Calculate(int fastPeriod, int slowPeriod, int signalPeriod) {
	auto candleSize = m_pvCandle->Size();
	if (m_pvCandle->Empty()) return;

	auto sma = [](CVirtualDataHistoryCandle* p, std::size_t start, std::size_t len) {
		double s = 0.0;
		for (std::size_t i = start; i < start + len; ++i) s += static_cast<double>(p->GetData(i)->GetClose()) / p->GetRatio();
		return s / static_cast<double>(len);
	};

	std::vector<double> fastEMA(candleSize, std::numeric_limits<double>::quiet_NaN());
	std::vector<double> slowEMA(candleSize, std::numeric_limits<double>::quiet_NaN());
	std::vector<double> macdLine(candleSize, std::numeric_limits<double>::quiet_NaN());

	double alphaFast = 2.0 / (fastPeriod + 1.0);
	double alphaSlow = 2.0 / (slowPeriod + 1.0);

	if (candleSize >= static_cast<std::size_t>(fastPeriod)) {
		fastEMA[fastPeriod - 1] = sma(m_pvCandle, 0, fastPeriod);
		for (std::size_t i = fastPeriod; i < candleSize; ++i)
			fastEMA[i] = alphaFast * static_cast<double>(m_pvCandle->GetData(i)->GetClose()) / m_pvCandle->GetRatio() + (1.0 - alphaFast) * fastEMA[i - 1];
	}
	else {
		// initialize first point
		fastEMA[0] = m_pvCandle->GetData(0)->GetClose();
		for (std::size_t i = 1; i < candleSize; ++i)
			fastEMA[i] = alphaFast * static_cast<double>(m_pvCandle->GetData(i)->GetClose()) / m_pvCandle->GetRatio() + (1.0 - alphaFast) * fastEMA[i - 1];
	}

	if (candleSize >= static_cast<std::size_t>(slowPeriod)) {
		slowEMA[slowPeriod - 1] = sma(m_pvCandle, 0, slowPeriod);
		for (std::size_t i = slowPeriod; i < candleSize; ++i)
			slowEMA[i] = alphaSlow * static_cast<double>(m_pvCandle->GetData(i)->GetClose()) / m_pvCandle->GetRatio() + (1.0 - alphaSlow) * slowEMA[i - 1];
	}
	else {
		slowEMA[0] = m_pvCandle->GetData(0)->GetClose();
		for (std::size_t i = 1; i < candleSize; ++i)
			slowEMA[i] = alphaSlow * static_cast<double>(m_pvCandle->GetData(i)->GetClose()) / m_pvCandle->GetRatio() + (1.0 - alphaSlow) * slowEMA[i - 1];
	}

	for (std::size_t i = 0; i < candleSize; ++i) {
		if (!std::isnan(fastEMA[i]) && !std::isnan(slowEMA[i]))
			macdLine[i] = fastEMA[i] - slowEMA[i];
	}

	// compute signal EMA on macdLine
	std::vector<double> signalEMA(candleSize, std::numeric_limits<double>::quiet_NaN());
	double alphaSignal = 2.0 / (signalPeriod + 1.0);
	// find first valid macd index
	std::size_t firstMacdIdx = candleSize;
	for (std::size_t i = 0; i < candleSize; ++i)
		if (!std::isnan(macdLine[i])) {
			firstMacdIdx = i;
			break;
		}
	if (firstMacdIdx == candleSize) return; // no macd values

	if (candleSize - firstMacdIdx >= static_cast<std::size_t>(signalPeriod)) {
		// SMA over first signalPeriod macd values starting at firstMacdIdx
		double s = 0.0;
		for (std::size_t i = 0; i < static_cast<std::size_t>(signalPeriod); ++i) s += macdLine[firstMacdIdx + i];
		signalEMA[firstMacdIdx + signalPeriod - 1] = s / static_cast<double>(signalPeriod);
		for (std::size_t i = firstMacdIdx + signalPeriod; i < candleSize; ++i)
			signalEMA[i] = alphaSignal * macdLine[i] + (1.0 - alphaSignal) * signalEMA[i - 1];
	}
	else {
		// fallback: initialize first known macd value then compute
		signalEMA[firstMacdIdx] = macdLine[firstMacdIdx];
		for (std::size_t i = firstMacdIdx + 1; i < candleSize; ++i)
			if (!std::isnan(macdLine[i]))
				signalEMA[i] = alphaSignal * macdLine[i] + (1.0 - alphaSignal) * signalEMA[i - 1];
	}

	m_vMACD.reserve(candleSize);
	for (std::size_t i = 0; i < candleSize; ++i) {
		MACDResult macd;
		macd.macd = macdLine[i];
		macd.signal = signalEMA[i];
		macd.hist = std::numeric_limits<double>::quiet_NaN();
		if (!std::isnan(macd.macd) && !std::isnan(macd.signal)) macd.hist = macd.macd - macd.signal;
		m_vMACD.push_back(macd);
		macd.Reset();
	}
}

void CIndicatorMACD::ToShow(CDC* pDC, CRect rectDrawArea, int iStepWidth) {
	constexpr COLORREF crGreen(RGB(0, 255, 0)), crWhite(RGB(255, 255, 255)),
	                   crRed(RGB(255, 0, 0)), crYellow(RGB(255, 255, 0));
	CPen penGreen(PS_SOLID, 1, crGreen), penWhite1(PS_SOLID, 1, crWhite), penRed1(PS_SOLID, 1, crRed);
	CPen penYellow(PS_SOLID, 1, crYellow);

	CalculateCurrentHighLow(rectDrawArea, iStepWidth);

	CPen penWhiteDash(PS_DOT, 1, crWhite);
	CPen* pOldPen = pDC->SelectObject(&penWhiteDash);
	int y0 = rectDrawArea.bottom - ((0 - currentLow_) / currentDif_) * rectDrawArea.Height();
	pDC->MoveTo(rectDrawArea.left, y0);
	pDC->LineTo(rectDrawArea.right, y0);
	//pDC->SelectObject(pOldPen);

	long offset = iStepWidth / 2;

	pDC->SelectObject(&penWhite1);
	auto it = m_vMACD.end();
	int i = 1;
	--it;
	long y = rectDrawArea.bottom - (((it--)->macd - currentLow_) / currentDif_) * rectDrawArea.Height();
	pDC->MoveTo(rectDrawArea.right - offset, y);
	for (; it != m_vMACD.begin(); --it, i++) {
		y = rectDrawArea.bottom - (((it)->macd - currentLow_) / currentDif_) * rectDrawArea.Height();
		pDC->LineTo(rectDrawArea.right - offset - iStepWidth * i, y);
		if (i >= m_vMACD.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}

	pDC->SelectObject(&penYellow);
	it = m_vMACD.end();
	i = 1;
	--it;
	y = rectDrawArea.bottom - (((it--)->signal - currentLow_) / currentDif_) * rectDrawArea.Height();
	pDC->MoveTo(rectDrawArea.right - offset, y);
	for (; it != m_vMACD.begin(); --it, i++) {
		y = rectDrawArea.bottom - (((it)->signal - currentLow_) / currentDif_) * rectDrawArea.Height();
		pDC->LineTo(rectDrawArea.right - offset - iStepWidth * i, y);
		if (i >= m_vMACD.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}

	it = m_vMACD.end();
	i = 1;
	--it;
	long y01 = rectDrawArea.bottom - ((0 - currentLow_) / currentDif_) * rectDrawArea.Height();
	for (; it != m_vMACD.begin(); --it, i++) {
		pDC->MoveTo(rectDrawArea.right - offset - iStepWidth * i, y01);
		y = rectDrawArea.bottom - (((it)->hist - currentLow_) / currentDif_) * rectDrawArea.Height();
		if (y < y01) pDC->SelectObject(&penRed1);
		else pDC->SelectObject(&penGreen);
		pDC->LineTo(rectDrawArea.right - offset - iStepWidth * i, y);
		if (i >= m_vMACD.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}

	pDC->SelectObject(pOldPen);
}

void CIndicatorMACD::CalculateCurrentHighLow(CRect rectDrawArea, int iStepWidth) {
	auto it = m_vMACD.end();
	int i = 1;
	--it;
	currentHigh_ = it->Max();
	currentLow_ = it->Min();
	--it;
	for (; it != m_vMACD.begin(); --it, i++) {
		currentHigh_ = max(currentHigh_, it->Max());
		currentLow_ = min(currentLow_, it->Min());
		if (i >= m_vMACD.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 到窗口左边框为止
	}
	currentDif_ = currentHigh_ - currentLow_;
}
