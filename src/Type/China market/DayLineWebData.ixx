module;

export module FireBirdLib.DayLineWebData;

import FireBirdLib.HistoryCandle.DayLine;

import FireBirdLib.WebData;

import std;
using std::vector;
using std::string;
using std::int64_t;
using std::shared_ptr;

export {
	class CDayLineWebData final {
	public:
		CDayLineWebData();
		~CDayLineWebData() = default;
		void Reset();

		void ClearDayLine() noexcept { m_vTempDayLine.clear(); }
		std::vector<CDayLine>& GetProcessedDayLine() { return m_vTempDayLine; }
		void AppendDayLine(const CDayLine& data) { m_vTempDayLine.push_back(data); }

		void SetStockCode(const std::string& strSymbol) noexcept { m_strStockCode = strSymbol; }
		std::string GetStockCode() { return m_strStockCode; }

		bool TransferWebDataToBuffer(shared_ptr<CWebData> pWebData);
		string GetBuffer() { return m_sBuffer; }

		int64_t GetCurrentPos() const noexcept { return m_lCurrentPos; }
		void SetCurrentPos(const int64_t lValue) noexcept { m_lCurrentPos = lValue; }

	private:
		string m_strStockCode{};
		string m_sBuffer{}; // 日线读取缓冲区
		vector<CDayLine> m_vTempDayLine{}; // 日线数据缓冲区
		int64_t m_lCurrentPos{ 0 };
	};

	using CDayLineWebDataPtr = shared_ptr<CDayLineWebData>;
}
