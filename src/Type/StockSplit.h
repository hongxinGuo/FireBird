#pragma once

#include <chrono>

class CStockSplit {
public:
	CStockSplit() = default;
	~CStockSplit() = default;

	// Getter and Setter for m_date
	long GetDate() const { return m_lDate; }
	void SetDate(long date) { m_lDate = date; }
	// Getter and Setter for m_ratio
	double GetRatio() const { return m_dRatio; }
	void SetRatio(double ratio) { m_dRatio = ratio; }

protected:
	long m_lDate{ 0 };
	double m_dRatio{ 1.0 }; // Split ratio
};

using CStockSplitPtr = std::shared_ptr<CStockSplit>;
