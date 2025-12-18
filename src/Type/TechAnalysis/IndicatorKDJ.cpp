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
	for (int i = 0; i < m_Period; i++) {
		m_vKDJ[i].m_RSV = 50;
		m_vKDJ[i].m_K = 50;
		m_vKDJ[i].m_D = 50;
		m_vKDJ[i].m_J = 50;
	}
	for (int index = m_Period - 1; index < m_pvCandle->Size(); index++) {
		auto data = m_pvCandle->GetData(index);
		long lHigh = 0;
		long lLow = data->GetLow();
		for (size_t i = index - m_Period + 1; i <= index; i++) {
			auto data2 = m_pvCandle->GetData(i);
			lHigh = max(lHigh, data2->GetHigh());
			lLow = min(lLow, data2->GetLow());
		}
		if (lHigh == 0 || lHigh == lLow) m_vKDJ[index].m_RSV = 50.0;
		else m_vKDJ[index].m_RSV = (static_cast<double>(data->GetClose() - lLow)) * 100 / (lHigh - lLow);
		m_vKDJ[index].m_K = (m_vKDJ[index - 1].m_K * 2 + m_vKDJ[index].m_RSV) / 3;
		m_vKDJ[index].m_D = (m_vKDJ[index - 1].m_D * 2 + m_vKDJ[index].m_K) / 3;
		m_vKDJ[index].m_J = m_vKDJ[index].m_K * 3 - m_vKDJ[index].m_D * 2;
	}
}

void CIndicatorKDJ::ToShow(CDC* pDC, CRect rectDrawArea, int iStepWidth) {
	constexpr COLORREF crPurple(RGB(128, 0, 128)), crWhite(RGB(255, 255, 255)),
	                   crRed(RGB(255, 0, 0)), crYellow(RGB(255, 255, 0));
	CPen penPurple(PS_SOLID, 1, crPurple), penWhite1(PS_SOLID, 1, crWhite), penRed1(PS_SOLID, 1, crRed);
	CPen penYellow(PS_SOLID, 1, crYellow);

	CPen penWhiteDash(PS_DOT, 1, crWhite);
	CPen* pOldPen = pDC->SelectObject(&penWhiteDash);
	int y20 = rectDrawArea.top + rectDrawArea.Height() * 8 / 10;
	int y80 = rectDrawArea.top + rectDrawArea.Height() * 2 / 10;
	pDC->MoveTo(rectDrawArea.left, y20);
	pDC->LineTo(rectDrawArea.right, y20);
	pDC->MoveTo(rectDrawArea.left, y80);
	pDC->LineTo(rectDrawArea.right, y80);
	//pDC->SelectObject(pOldPen);

	pDC->SelectObject(&penYellow);
	auto it = m_vKDJ.end();
	int i = 1;
	--it;
	int y = rectDrawArea.bottom - (it--)->m_K * rectDrawArea.Height() / 100;
	pDC->MoveTo(rectDrawArea.right - 1, y);
	for (; it != m_vKDJ.begin(); --it, i++) {
		y = rectDrawArea.bottom - it->m_K * rectDrawArea.Height() / 100;
		pDC->LineTo(rectDrawArea.right - 1 - iStepWidth * i, y);
		if (i >= m_vKDJ.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}

	pDC->SelectObject(&penWhite1);
	it = m_vKDJ.end();
	i = 1;
	--it;
	y = rectDrawArea.bottom - (it--)->m_D * rectDrawArea.Height() / 100;
	pDC->MoveTo(rectDrawArea.right - 1, y);
	for (; it != m_vKDJ.begin(); --it, i++) {
		y = rectDrawArea.bottom - it->m_D * rectDrawArea.Height() / 100;
		pDC->LineTo(rectDrawArea.right - 1 - iStepWidth * i, y);
		if (i >= m_vKDJ.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}

	pDC->SelectObject(&penYellow);
	it = m_vKDJ.end();
	i = 1;
	--it;
	y = rectDrawArea.bottom - (it--)->m_J * rectDrawArea.Height() / 100;
	pDC->MoveTo(rectDrawArea.right - 1, y);
	for (; it != m_vKDJ.begin(); --it, i++) {
		y = rectDrawArea.bottom - it->m_J * rectDrawArea.Height() / 100;
		pDC->LineTo(rectDrawArea.right - 1 - iStepWidth * i, y);
		if (i >= m_vKDJ.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}

	pDC->SelectObject(pOldPen);
}
