#include"pch.h"
#include<atlconv.h>

#include "VirtualStock.h"

#include "CharSetTransfer.h"
#include "jsonParse.h"

CVirtualStockPtr gl_pCurrentStock{ nullptr };

CVirtualStock::CVirtualStock() {
	CVirtualStock::ResetAllUpdateDate();
}

void CVirtualStock::ResetAllUpdateDate() {
	m_jsonUpdateDate["DayLineStartDate"] = 29900101;
	m_jsonUpdateDate["DayLineEndDate"] = 19800101;
}

void CVirtualStock::LoadUpdateDate(const string& strUpdateDate) {
	if (strUpdateDate.length() < 10) {
		ResetAllUpdateDate();
	}
	else {
		try {
			CreateJsonWithNlohmann(m_jsonUpdateDate, strUpdateDate);
		} catch (json::exception&) {
			CreateJsonWithNlohmann(m_jsonUpdateDate, "{}");
			ResetAllUpdateDate();
		}
	}
}

void CVirtualStock::LoadSymbol(CVirtualSetStockSymbol& setStockSymbol) {
	USES_CONVERSION;
	m_strDescription = T2Utf8(setStockSymbol.m_Description);
	m_strDisplaySymbol = T2Utf8(setStockSymbol.m_DisplaySymbol);
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
	setStockSymbol.m_DisplaySymbol = m_strDisplaySymbol.c_str();
	setStockSymbol.m_Exchange = m_strExchangeCode.c_str();
	setStockSymbol.m_Symbol = m_strSymbol.c_str();
	setStockSymbol.m_IPOStatus = m_lIPOStatus;
	const string sUpdateDate = m_jsonUpdateDate.dump();
	setStockSymbol.m_UpdateDate = sUpdateDate.c_str();
	ASSERT(sUpdateDate.size() < 10000);
}

long CVirtualStock::GetDayLineStartDate() {
	long l;
	try {
		l = m_jsonUpdateDate["DayLineStartDate"];
	} catch (json::exception&) {
		m_jsonUpdateDate["DayLineStartDate"] = 29900101;
		l = 29901010;
	}
	return l;
}

long CVirtualStock::GetDayLineEndDate() {
	long l;
	try {
		l = m_jsonUpdateDate["DayLineEndDate"];
		if (l < 19800101) {
			m_jsonUpdateDate["DayLineEndDate"] = 19800101;
		}
	} catch (json::exception&) {
		m_jsonUpdateDate["DayLineEndDate"] = 19800101;
		l = 19800101;
	}

	return l;
}

bool CVirtualStock::IsSameStock(const CVirtualStockPtr& pStock) const {
	if (pStock == nullptr) return false;
	if (m_strSymbol == pStock->GetSymbol()) {
		return true;
	}
	return false;
}
