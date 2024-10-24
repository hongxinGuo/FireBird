#include"pch.h"

#include "VirtualStock.h"

CVirtualStock::CVirtualStock() {
	CVirtualStock::Reset();
}

void CVirtualStock::Reset() {
	m_strDescription = _T("");
	m_strExchangeCode = _T("");
	m_strSymbol = _T("");
	m_strDisplaySymbol = _T("");

	m_TransactionTime = 0;
	m_lLastClose = m_lOpen = 0;
	m_lHigh = m_lLow = m_lNew = 0;
	m_llVolume = 0;
	m_llAmount = 0;
	m_lUpDown = 0;
	m_dUpDownRate = 0;
	m_dChangeHandRate = 0;
	m_llTotalValue = m_llCurrentValue = 0;

	m_fTodayNewStock = false;
	m_fUpdateProfileDB = false;
	m_fUpdateCompanyNewsDB = false;
	m_fActive = false;
	m_lDayLineStartDate = 29900101;
	m_lDayLineEndDate = 19000101;
	m_lIPOStatus = _STOCK_NOT_CHECKED_; // 默认状态为无效股票代码。

	m_fDayLineNeedUpdate = true; // 默认状态下日线需要更新
	m_fDayLineNeedSaving = false;
}

void CVirtualStock::LoadSymbol(CVirtualSetStockSymbol& setStockSymbol) {
	m_strDescription = setStockSymbol.m_Description;
	m_strDisplaySymbol = setStockSymbol.m_DisplaySymbol;
	m_strExchangeCode = setStockSymbol.m_Exchange;
	m_strSymbol = setStockSymbol.m_Symbol;
	m_lSymbol = atol(m_strSymbol.Left(6));
	m_lDayLineStartDate = setStockSymbol.m_DayLineStartDate;
	m_lDayLineEndDate = setStockSymbol.m_DayLineEndDate;
	m_lIPOStatus = setStockSymbol.m_IPOStatus;
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
}
