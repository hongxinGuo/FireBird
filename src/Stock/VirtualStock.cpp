#include"pch.h"

#include "VirtualStock.h"

#include "jsonParse.h"

CVirtualStock::CVirtualStock() {
}

void CVirtualStock::ResetAllUpdateDate() {
	m_jsonUpdateDate[_T("DayLineStartDate")] = m_lDayLineStartDate;
	m_jsonUpdateDate[_T("DayLineEndDate")] = m_lDayLineEndDate;
}

void CVirtualStock::LoadSymbol(CVirtualSetStockSymbol& setStockSymbol) {
	m_strDescription = setStockSymbol.m_Description;
	m_strDisplaySymbol = setStockSymbol.m_DisplaySymbol;
	m_strExchangeCode = setStockSymbol.m_Exchange;
	m_strSymbol = setStockSymbol.m_Symbol;
	m_lDayLineStartDate = setStockSymbol.m_DayLineStartDate;
	m_lDayLineEndDate = setStockSymbol.m_DayLineEndDate;
	m_lIPOStatus = setStockSymbol.m_IPOStatus;
	if (setStockSymbol.m_UpdateDate.GetLength() < 10) {
		ResetAllUpdateDate();
	}
	else {
		CreateJsonWithNlohmann(m_jsonUpdateDate, setStockSymbol.m_UpdateDate);
	}
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
	setStockSymbol.m_Description = m_strDescription;
	setStockSymbol.m_DisplaySymbol = m_strDisplaySymbol;
	setStockSymbol.m_Exchange = m_strExchangeCode;
	setStockSymbol.m_Symbol = m_strSymbol;
	setStockSymbol.m_DayLineStartDate = m_lDayLineStartDate;
	setStockSymbol.m_DayLineEndDate = m_lDayLineEndDate;
	setStockSymbol.m_IPOStatus = m_lIPOStatus;
	const string sUpdateDate = m_jsonUpdateDate.dump();
	setStockSymbol.m_UpdateDate = sUpdateDate.c_str();
	ASSERT(sUpdateDate.size() < 10000);
}

long CVirtualStock::GetDayLineStartDate() {
	long l;
	try {
		l = m_jsonUpdateDate[_T("DayLineStartDate")];
	} catch (json::exception&) {
		m_jsonUpdateDate[_T("DayLineStartDate")] = m_lDayLineStartDate;
		l = m_lDayLineStartDate;
	}
	return l;
}

long CVirtualStock::GetDayLineEndDate() {
	long l;
	try {
		l = m_jsonUpdateDate[_T("DayLineEndDate")];
	} catch (json::exception&) {
		m_jsonUpdateDate[_T("DayLineEndDate")] = m_lDayLineEndDate;
		l = m_lDayLineEndDate;
	}
	return l;
}
