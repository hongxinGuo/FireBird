#include"pch.h"

#include "IndicatorKDJ.h"

void CIndicatorKDJ::CalculateIndicator() {
	ASSERT(m_pvDayLine != nullptr);

	m_vKDJ.resize(m_pvDayLine->Size());
}
