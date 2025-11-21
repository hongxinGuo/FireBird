#include"pch.h"

#include "IndicatorKDJ.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// n日RSV=（Cn－Ln）÷（Hn－Ln）×100
//
// 当日K值=2/3×前一日K值＋1/3×当日RSV
// 当日D值=2/3×前一日D值＋1/3×当日K值
// J值=3*当日K值-2*当日D值
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CIndicatorKDJ::Calculate() {
	ASSERT(m_pvCandle != nullptr);

	m_vKDJ.resize(m_pvCandle->Size());
	long lHigh = 0;
	long lLow = m_pvCandle->GetData(0)->GetLow();
	for (size_t i = 0; i < m_Period; i++) {
		m_vKDJ[i].m_RSV = 50;
		m_vKDJ[i].m_K = 50;
		m_vKDJ[i].m_D = 50;
		m_vKDJ[i].m_J = 50;
	}
	for (size_t index = m_Period - 1; index < m_pvCandle->Size(); index++) {
		auto data = m_pvCandle->GetData(index);
		lHigh = 0;
		lLow = data->GetLow();
		for (size_t i = index - m_Period + 1; i <= index; i++) {
			auto data2 = m_pvCandle->GetData(i);
			if (lHigh < data2->GetHigh()) lHigh = data2->GetHigh();
			if (lLow > data2->GetLow()) lLow = data2->GetLow();
		}
		m_vKDJ[index].m_RSV = (static_cast<double>(data->GetClose() - lLow)) * 100 / (lHigh - lLow);
		m_vKDJ[index].m_K = (m_vKDJ[index - 1].m_K * 2 + m_vKDJ[index].m_RSV) / 3;
		m_vKDJ[index].m_D = (m_vKDJ[index - 1].m_D * 2 + m_vKDJ[index].m_K) / 3;
		m_vKDJ[index].m_J = m_vKDJ[index].m_K * 3 - m_vKDJ[index].m_D * 2;
	}
}

void CIndicatorKDJ::ToShow(CDC* pDC, CRect rectDraw) {
	constexpr COLORREF crPurple(RGB(128, 0, 128)), crWhite(RGB(255, 255, 255)),
	                   crRed(RGB(255, 0, 0)), crBlue(RGB(0, 0, 255)), crYellow(RGB(255, 255, 0));
	CPen penPurple(PS_SOLID, 1, crPurple), penWhite1(PS_SOLID, 1, crWhite), penRed1(PS_SOLID, 1, crRed);
	CPen penYellow(PS_SOLID, 1, crYellow);

	auto pOLdPen = pDC->SelectObject(&penYellow);
	auto it = m_vKDJ.end();
	int i = 1;
	--it;
	int y = rectDraw.bottom - (it--)->m_K * rectDraw.Height() / 100;
	pDC->MoveTo(rectDraw.right - 1, y);
	for (; it != m_vKDJ.begin(); --it, i++) {
		y = rectDraw.bottom - it->m_K * rectDraw.Height() / 100;
		pDC->LineTo(rectDraw.right - 1 - 3 * i, y);
		if (3 * i > m_vKDJ.size()) break;
		if (rectDraw.right <= 3 * i) break; // 画到窗口左边框为止
	}

	pDC->SelectObject(&penWhite1);
	it = m_vKDJ.end();
	i = 1;
	--it;
	y = rectDraw.bottom - (it--)->m_D * rectDraw.Height() / 100;
	pDC->MoveTo(rectDraw.right - 1, y);
	for (; it != m_vKDJ.begin(); --it, i++) {
		y = rectDraw.bottom - it->m_D * rectDraw.Height() / 100;
		pDC->LineTo(rectDraw.right - 1 - 3 * i, y);
		if (3 * i > m_vKDJ.size()) break;
		if (rectDraw.right <= 3 * i) break; // 画到窗口左边框为止
	}

	pDC->SelectObject(&penYellow);
	it = m_vKDJ.end();
	i = 1;
	--it;
	y = rectDraw.bottom - (it--)->m_J * rectDraw.Height() / 100;
	pDC->MoveTo(rectDraw.right - 1, y);
	for (; it != m_vKDJ.begin(); --it, i++) {
		y = rectDraw.bottom - it->m_J * rectDraw.Height() / 100;
		pDC->LineTo(rectDraw.right - 1 - 3 * i, y);
		if (3 * i > m_vKDJ.size()) break;
		if (rectDraw.right <= 3 * i) break; // 画到窗口左边框为止
	}

	pDC->SelectObject(pOLdPen);
}
