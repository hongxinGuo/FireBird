#include"pch.h"

#include "VirtualStock.h"
#include "StockSplit.h"

#include "jsonParse.h"
#include "nlohmannJsonGetValue.h"

#include"TimeConvert.h"

using namespace std;

shared_ptr<CVirtualStock> gl_pCurrentStock{ nullptr };

CVirtualStock::CVirtualStock() {
	CVirtualStock::ResetAllUpdateDate();
}

void CVirtualStock::ResetAllUpdateDate() {
	m_dayLineStartDate = local_days(2990y / 01 / 01);
	m_dayLineEndDate = local_days(1980y / 01 / 01);
	m_pvStockSplit = make_shared<vector<shared_ptr<CStockSplit>>>();
	m_pvStockSplit->reserve(100);
	UpdateJsonUpdateDate();
}

void CVirtualStock::UpdateJsonUpdateDate() {
	m_jsonUpdateDate["DayLineStartDate"] = toFormattedDate(m_dayLineStartDate);
	m_jsonUpdateDate["DayLineEndDate"] = toFormattedDate(m_dayLineEndDate);
	nlohmannJson jsStockSplit = nlohmannJson::array();
	for (auto pStockSplit : *m_pvStockSplit) {
		nlohmannJson js;
		js["date"] = toFormattedDate(pStockSplit->GetDate());
		js["ratio"] = pStockSplit->GetRatio();
		jsStockSplit.push_back(js);
	}
	m_jsonUpdateDate["StockSplit"] = jsStockSplit;
}

void CVirtualStock::UpdateAllUpdateDate() {
	m_dayLineStartDate = toLocalDays(m_jsonUpdateDate["DayLineStartDate"]);
	m_dayLineEndDate = toLocalDays(m_jsonUpdateDate["DayLineEndDate"]);

	m_pvStockSplit->clear();
	auto js = m_jsonUpdateDate["StockSplit"];
	for (auto it = js.begin(); it != js.end(); ++it) {
		CStockSplitPtr pStockSplit = make_shared<CStockSplit>();
		pStockSplit->SetDate(XferToLocalDays(jsonGetString(it, "date")));
		pStockSplit->SetRatio(jsonGetDouble(it, "ratio"));
		m_pvStockSplit->push_back(pStockSplit);
	}
}

void CVirtualStock::LoadUpdateDate(const string& strUpdateDate) {
	try {
		CreateJsonWithNlohmann(m_jsonUpdateDate, strUpdateDate);
		UpdateAllUpdateDate();
	} catch (nlohmannJson::exception&) {
		string s = "{}";
		CreateJsonWithNlohmann(m_jsonUpdateDate, s);
		ResetAllUpdateDate();
	}
}

void CVirtualStock::AddStockSplit(const shared_ptr<CStockSplit>& StockSplit) const noexcept {
	for (auto& p : *m_pvStockSplit) {
		if (p->GetDate() == StockSplit->GetDate()) return; // 已经有了，不添加了。
	}
	m_pvStockSplit->push_back(StockSplit);
	//按日期顺序添加拆股信息
	ranges::sort(*m_pvStockSplit, [](CStockSplitPtr a, CStockSplitPtr b) {
		return a->GetDate() < b->GetDate();
	});
}

void CVirtualStock::AddStockSplits(const shared_ptr<vector<shared_ptr<CStockSplit>>>& pvStockSplit) noexcept {
	m_pvStockSplit = pvStockSplit;
}

bool CVirtualStock::IsSameStock(const shared_ptr<CVirtualStock>& pStock) const {
	if (pStock == nullptr) return false;
	if (m_strSymbol == pStock->GetSymbol()) {
		return true;
	}
	return false;
}
