#include"pch.h"

#include "ChinaStock.h"
#include "ContainerChinaStockWeekLine.h"

namespace {
	CChinaStock s_stockContainerChinaWeekLine;
}

CContainerChinaStockWeekLine::CContainerChinaStockWeekLine() {
	m_ratio = s_stockContainerChinaWeekLine.GetRatio();
}

void CContainerChinaStockWeekLine::StoreVectorData(const vector<CWeekLine>& vWeekLine) {
	for (const auto& weekLine : vWeekLine) {
		Add(weekLine);
	}
	SetDataLoaded(true);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 更新日线容器。
//
/////////////////////////////////////////////////////////////////////////////////////
void CContainerChinaStockWeekLine::UpdateData(const vector<CWeekLine>& vTempWeekLine) {
	Unload(); // 清除已载入的周线数据（如果有的话）
	// 将日线数据以时间为正序存入
	for (const auto& pWeekLine : vTempWeekLine) {
		Add(pWeekLine);
	}
	SetDataLoaded(true);
}

void CContainerChinaStockWeekLine::UpdateData(const CVirtualHistoryCandle* pHistoryCandle) {
	for (size_t index = 0; index < m_vHistoryData.size(); ++index) {
		auto pWeekLine = GetData(index);
		auto str = pWeekLine->GetStockSymbol();
		if (str == pHistoryCandle->GetStockSymbol()) {
			pWeekLine->UpdateWeekLine(pHistoryCandle);
			break;
		}
	}
}
