#pragma once

class CStockSymbol {
public:
	CStockSymbol() = default;
	CStockSymbol(const CStockSymbol&) = default;
	CStockSymbol& operator=(const CStockSymbol&) = default;
	CStockSymbol(CStockSymbol&&) noexcept = default;
	CStockSymbol& operator=(CStockSymbol&&) noexcept = default;
	~CStockSymbol() = default;

	string GetCode() const { return m_Symbol; }
	void SetCode(string& val) { m_Symbol = val; }
	string GetName() const { return m_DisplaySymbol; }
	void SetName(string& val) { m_DisplaySymbol = val; }
	string GetDescription() const { return m_Description; }
	void SetDescription(string& val) { m_Description = val; }
	string GetExchangeCode() const { return m_Exchange; }
	void SetExchangeCode(string& val) { m_Exchange = val; }
	long GetIPOStatus() const { return m_IPOStatus; }
	void SetIPOStatus(long val) { m_IPOStatus = val; }
	string GetUpdateDate() const { return m_UpdateDate; }
	void SetUpdateDate(string& val) { m_UpdateDate = val; }

	string m_Description;
	string m_Exchange;
	string m_Symbol;
	string m_DisplaySymbol; //当用于中国股票时，存储的股票中文名称。
	long m_IPOStatus;
	string m_UpdateDate;
};

using CStockSymbolPtr = shared_ptr<CStockSymbol>;
