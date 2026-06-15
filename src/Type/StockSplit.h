#pragma once

class CStockSplit {
public:
	CStockSplit() = default;
	CStockSplit(const CStockSplit&) = default;
	CStockSplit& operator=(const CStockSplit&) = default;
	CStockSplit(CStockSplit&&) = default;
	CStockSplit& operator=(CStockSplit&&) = default;
	~CStockSplit() = default;

	// Getter and Setter for m_date
	string GetSymbol() const { return m_sSymbol; }
	void SetSymbol(const string& symbol) { m_sSymbol = symbol; }
	chrono::local_days GetDate() const { return m_date; }
	void SetDate(chrono::local_days date) { m_date = date; }
	// Getter and Setter for m_ratio
	double GetRatio() const { return m_dRatio; }
	void SetRatio(double ratio) { m_dRatio = ratio; }

protected:
	string m_sSymbol{ "" };
	chrono::local_days m_date{ chrono::local_days{ chrono::year_month_day{ chrono::year{ 1970 }, chrono::month{ 1 }, chrono::day{ 1 } } } };

	double m_dRatio{ 1.0 }; // Split ratio
};

using CStockSplitPtr = std::shared_ptr<CStockSplit>;
using CStockSplitsPtr = std::shared_ptr<std::vector<CStockSplit>>;
