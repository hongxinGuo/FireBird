#include"pch.h"
#include<atlconv.h>

#include "VirtualStock.h"

#include "CharSetTransfer.h"
#include "jsonParse.h"
#include "nlohmannJsonGetValue.h"

CVirtualStockPtr gl_pCurrentStock{ nullptr };

CVirtualStock::CVirtualStock() {
	CVirtualStock::ResetAllUpdateDate();
}

void CVirtualStock::ResetAllUpdateDate() {
	m_lDayLineStartDate = 29900101;
	m_lDayLineEndDate = 19800101;
	m_vStockSplit.clear();
	UpdateJsonUpdateDate();
}

void CVirtualStock::UpdateJsonUpdateDate() {
	m_jsonUpdateDate["ShareOutstanding"] = m_dShareCount;
	m_jsonUpdateDate["DayLineStartDate"] = m_lDayLineStartDate;
	m_jsonUpdateDate["DayLineEndDate"] = m_lDayLineEndDate;
	nlohmannJson jsStockSplit = nlohmannJson::array();
	for (const auto& pStockSplit : m_vStockSplit) {
		nlohmannJson js;
		js["date"] = pStockSplit->GetDate();
		js["ratio"] = pStockSplit->GetRatio();
		jsStockSplit.push_back(js);
	}
	m_jsonUpdateDate["StockSplit"] = jsStockSplit;
}

void CVirtualStock::UpdateAllUpdateDate() {
	m_dShareCount = m_jsonUpdateDate.value("ShareOutstanding", 0.0);
	m_lDayLineStartDate = m_jsonUpdateDate["DayLineStartDate"];
	m_lDayLineEndDate = m_jsonUpdateDate["DayLineEndDate"];

	m_vStockSplit.clear();
	auto js = m_jsonUpdateDate["StockSplit"];
	for (auto it = js.begin(); it != js.end(); ++it) {
		CStockSplitPtr pStockSplit = std::make_shared<CStockSplit>();
		pStockSplit->SetDate(jsonGetLong(it, "date"));
		pStockSplit->SetRatio(jsonGetDouble(it, "ratio"));
		m_vStockSplit.push_back(pStockSplit);
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

void CVirtualStock::LoadSymbol(CVirtualSetStockSymbol& setStockSymbol) {
	m_strDescription = T2Utf8(setStockSymbol.m_Description);
	m_strDisplaySymbol = W2Utf8(setStockSymbol.m_DisplaySymbol);
	m_strExchangeCode = T2Utf8(setStockSymbol.m_Exchange);
	m_strSymbol = T2Utf8(setStockSymbol.m_Symbol);
	m_lIPOStatus = setStockSymbol.m_IPOStatus;
	LoadUpdateDate(T2Utf8(setStockSymbol.m_UpdateDate));
}

void CVirtualStock::AppendSymbol(CVirtualSetStockSymbol& setStockSymbol) {
	setStockSymbol.AddNew();
	SaveSymbol(setStockSymbol);
	setStockSymbol.Update();
}

void CVirtualStock::UpdateSymbol(CVirtualSetStockSymbol& setStockSymbol) {
	setStockSymbol.Edit();
	SaveSymbol(setStockSymbol);
	setStockSymbol.Update();
}

void CVirtualStock::SaveSymbol(CVirtualSetStockSymbol& setStockSymbol) {
	setStockSymbol.m_Description = m_strDescription.c_str();
	wstring s = Utf8ToWstring(m_strDisplaySymbol);
	setStockSymbol.m_DisplaySymbol = Utf8ToWstring(m_strDisplaySymbol).c_str(); //只用前19个字符
	setStockSymbol.m_Exchange = m_strExchangeCode.c_str();
	setStockSymbol.m_Symbol = m_strSymbol.c_str();
	setStockSymbol.m_IPOStatus = m_lIPOStatus;
	UpdateJsonUpdateDate();
	const string sUpdateDate = m_jsonUpdateDate.dump();
	setStockSymbol.m_UpdateDate = sUpdateDate.c_str();
	ASSERT(sUpdateDate.size() < 10000);
}

void CVirtualStock::AddStockSplit(const CStockSplitPtr& pStockSplit) noexcept {
	m_vStockSplit.push_back(pStockSplit);
	//按日期顺序添加拆股信息
	ranges::sort(m_vStockSplit, [](const CStockSplitPtr& a, const CStockSplitPtr& b) {
		return a->GetDate() < b->GetDate();
	});
}

bool CVirtualStock::IsSameStock(const CVirtualStockPtr& pStock) const {
	if (pStock == nullptr) return false;
	if (m_strSymbol == pStock->GetSymbol()) {
		return true;
	}
	return false;
}
