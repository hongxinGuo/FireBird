#include"pch.h"

#include"MonthLine.h"

namespace {
	CChinaStock s_stockMonthLine;
}

CMonthLine::CMonthLine() {
	m_ratio = s_stockMonthLine.GetRatio();
}
