#include"pch.h"

#include "VirtualStock.h"

CVirtualStock::CVirtualStock() {
	CVirtualStock::Reset();
}

void CVirtualStock::Reset(void) {
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
	m_fClearDayLineVector = false;
	m_fActive = false;
	m_lDayLineStartDate = 29900101;
	m_lDayLineEndDate = 19000101;
	m_lIPOStatus = _STOCK_NOT_CHECKED_; // Ĭ��״̬Ϊ��Ч��Ʊ���롣

	m_fDayLineNeedUpdate = true; // Ĭ��״̬��������Ҫ����
	m_fDayLineNeedSaving = false;
}

void CVirtualStock::LoadSymbol(CVirtualSetStockSymbol& setStockSymbol) {
	m_strDescription = setStockSymbol.m_Description;
	m_strDisplaySymbol = setStockSymbol.m_DisplaySymbol;
	m_strExchangeCode = setStockSymbol.m_Exchange;
	m_strSymbol = setStockSymbol.m_Symbol;
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

void CVirtualStock::UpdateRealTimeData(CVirtualStockPtr pStock) {
	m_TransactionTime = pStock->m_TransactionTime;
	m_lLastClose = pStock->m_lLastClose;
	m_lOpen = pStock->m_lOpen;
	m_lHigh = pStock->m_lHigh;
	m_lLow = pStock->m_lLow;
	m_lNew = pStock->m_lNew;
}