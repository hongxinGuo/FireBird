module;

export module ContainerFinnhubStockDayLine;

import VirtualDataHistoryCandle;
import DayLine;

import std;
using std::string;
using std::shared_ptr;


export {
	class CContainerFinnhubStockDayLine final : public CVirtualDataHistoryCandle {
	public:
		CContainerFinnhubStockDayLine();
		CContainerFinnhubStockDayLine(const CContainerFinnhubStockDayLine& other) = delete;
		CContainerFinnhubStockDayLine(CContainerFinnhubStockDayLine&& other) noexcept = delete;
		CContainerFinnhubStockDayLine& operator=(const CContainerFinnhubStockDayLine& other) = delete;
		CContainerFinnhubStockDayLine& operator=(CContainerFinnhubStockDayLine&& other) noexcept = delete;
		~CContainerFinnhubStockDayLine() override = default;

		void SaveDB(const string& strStockSymbol) override;
		void LoadDB(const string& strStockSymbol) override;

		CDayLine* GetData(size_t lIndex);
		CDayLine* GetDayLine(std::chrono::local_days lDate);

		// 特有函数
	};

	using CContainerFinnhubStockDayLinePtr = shared_ptr<CContainerFinnhubStockDayLine>;
}