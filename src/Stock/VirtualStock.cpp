#include"pch.h"

#include "VirtualStock.h"

#include "jsonParse.h"

CVirtualStock::CVirtualStock() {
	CVirtualStock::ResetAllUpdateDate();
}

void CVirtualStock::ResetAllUpdateDate() {
	m_jsonUpdateDate[_T("DayLineStartDate")] = 29900101;
	m_jsonUpdateDate[_T("DayLineEndDate")] = 19800101;
}

void CVirtualStock::LoadUpdateDate(const string& strUpdateDate) {
	if (strUpdateDate.length() < 10) {
		ResetAllUpdateDate();
	}
	else {
		try {
			CreateJsonWithNlohmann(m_jsonUpdateDate, strUpdateDate);
		} catch (json::exception&) {
			CreateJsonWithNlohmann(m_jsonUpdateDate, _T("{}"));
			ResetAllUpdateDate();
		}
	}
}

void CVirtualStock::LoadSymbol(CVirtualSetStockSymbol& setStockSymbol) {
	m_strDescription = setStockSymbol.m_Description;
	CString str;
	str = setStockSymbol.m_DisplaySymbol; //Note 需要如此初始化，不能将声明和赋值放在一起
	m_strDisplaySymbol = str;
	m_strExchangeCode = setStockSymbol.m_Exchange;
	m_strSymbol = setStockSymbol.m_Symbol;
	m_lIPOStatus = setStockSymbol.m_IPOStatus;
	LoadUpdateDate(setStockSymbol.m_UpdateDate.GetString());
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
		l = m_jsonUpdateDate[_T("DayLineStartDate")];
	} catch (json::exception&) {
		m_jsonUpdateDate[_T("DayLineStartDate")] = 29900101;
		l = 29901010;
	}
	return l;
}

long CVirtualStock::GetDayLineEndDate() {
	long l;
	try {
		l = m_jsonUpdateDate[_T("DayLineEndDate")];
		if (l < 19800101) {
			m_jsonUpdateDate[_T("DayLineEndDate")] = 19800101;
		}
	} catch (json::exception&) {
		m_jsonUpdateDate[_T("DayLineEndDate")] = 19800101;
		l = 19800101;
	}

	return l;
}
