#include"pch.h"

#include "VirtualStock.h"

#include "jsonParse.h"
#include "nlohmannJsonGetValue.h"

CVirtualStockPtr gl_pCurrentStock{ nullptr };

CVirtualStock::CVirtualStock() {
	CVirtualStock::ResetAllUpdateDate();
}

void CVirtualStock::ResetAllUpdateDate() {
	m_dayLineStartDate = chrono::local_days(2990y / 01 / 01);
	m_dayLineEndDate = chrono::local_days(1980y / 01 / 01);
	m_pvStockSplit = make_shared<vector<CStockSplit>>();
	m_pvStockSplit->reserve(100);
	UpdateJsonUpdateDate();
}

void CVirtualStock::UpdateJsonUpdateDate() {
	m_jsonUpdateDate["ShareOutstanding"] = m_dShareCount;
	m_jsonUpdateDate["DayLineStartDate"] = toFormattedDate(m_dayLineStartDate);
	m_jsonUpdateDate["DayLineEndDate"] = toFormattedDate(m_dayLineEndDate);
	nlohmannJson jsStockSplit = nlohmannJson::array();
	for (auto StockSplit : *m_pvStockSplit) {
		nlohmannJson js;
		js["date"] = toFormattedDate(StockSplit.GetDate());
		js["ratio"] = StockSplit.GetRatio();
		jsStockSplit.push_back(js);
	}
	m_jsonUpdateDate["StockSplit"] = jsStockSplit;
}

void CVirtualStock::UpdateAllUpdateDate() {
	m_dShareCount = m_jsonUpdateDate.value("ShareOutstanding", 0.0);
	m_dayLineStartDate = toLocalDays(m_jsonUpdateDate["DayLineStartDate"]);
	m_dayLineEndDate = toLocalDays(m_jsonUpdateDate["DayLineEndDate"]);

	m_pvStockSplit->clear();
	auto js = m_jsonUpdateDate["StockSplit"];
	for (auto it = js.begin(); it != js.end(); ++it) {
		CStockSplit StockSplit;
		StockSplit.SetDate(XferToLocalDays(jsonGetString(it, "date")));
		StockSplit.SetRatio(jsonGetDouble(it, "ratio"));
		m_pvStockSplit->push_back(StockSplit);
	}
}

void CVirtualStock::LoadUpdateDate(const string& strUpdateDate) {
	try {
		CreateJsonWithNlohmann(m_jsonUpdateDate, strUpdateDate);
		UpdateAllUpdateDate();
	} catch (nlohmannJson::exception&) {
		CreateJsonWithNlohmann(m_jsonUpdateDate, "{}");
		ResetAllUpdateDate();
	}
}

void CVirtualStock::AddStockSplit(const CStockSplit& StockSplit) noexcept {
	for (auto& p : *m_pvStockSplit) {
		if (p.GetDate() == StockSplit.GetDate()) return; // 已经有了，不添加了。
	}
	m_pvStockSplit->push_back(StockSplit);
	//按日期顺序添加拆股信息
	ranges::sort(*m_pvStockSplit, [](const CStockSplit& a, const CStockSplit& b) {
		return a.GetDate() < b.GetDate();
	});
}

void CVirtualStock::AddStockSplits(const CStockSplitsPtr& pvStockSplit) noexcept {
	m_pvStockSplit = pvStockSplit;
}

bool CVirtualStock::IsSameStock(const CVirtualStockPtr& pStock) const {
	if (pStock == nullptr) return false;
	if (m_strSymbol == pStock->GetSymbol()) {
		return true;
	}
	return false;
}
