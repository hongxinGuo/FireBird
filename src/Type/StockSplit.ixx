module;

export module FireBirdLib.StockSplit;

import std;

using std::chrono::year_month_day;
using std::chrono::year;
using std::chrono::month;
using std::chrono::day;
using std::chrono::local_days;
using std::string;
using std::shared_ptr;
using std::vector;

export {
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
		local_days GetDate() const { return m_date; }
		void SetDate(local_days date) { m_date = date; }
		// Getter and Setter for m_ratio
		double GetRatio() const { return m_dRatio; }
		void SetRatio(double ratio) { m_dRatio = ratio; }

	protected:
		string m_sSymbol{ "" };
		local_days m_date{ local_days{ year_month_day{ year{ 1970 }, month{ 1 }, day{ 1 } } } };

		double m_dRatio{ 1.0 }; // Split ratio
	};

	using CStockSplitPtr = std::shared_ptr<CStockSplit>;
	using CStockSplitsPtr = shared_ptr<vector<shared_ptr<CStockSplit>>>;
}