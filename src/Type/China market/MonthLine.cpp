#include"pch.h"

#include"MonthLine.h"

namespace {
	CChinaStock s_stock;
}

CMonthLine::CMonthLine() {
	m_ratio = s_stock.GetRatio();
}
