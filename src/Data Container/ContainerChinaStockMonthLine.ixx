module;

export module ContainerChinaStockMonthLine;

import VirtualDataHistoryCandle;

import MonthLine;

import std;
using std::chrono::local_days;
using std::shared_ptr;

export {
	class CContainerChinaStockMonthLine : public CVirtualDataHistoryCandle {
	public:
		CContainerChinaStockMonthLine();
		~CContainerChinaStockMonthLine() override = default;
		CContainerChinaStockMonthLine(const CContainerChinaStockMonthLine& other) = delete;
		CContainerChinaStockMonthLine(CContainerChinaStockMonthLine&& other) noexcept = delete;
		CContainerChinaStockMonthLine& operator=(const CContainerChinaStockMonthLine& other) = delete;
		CContainerChinaStockMonthLine& operator=(CContainerChinaStockMonthLine&& other) noexcept = delete;

		CMonthLine* GetData(size_t lIndex);
		CMonthLine* GetDayLine(local_days lDate);

		bool Add(CMonthLine& data);
	};

	using CContainerChinaStockMonthLinePtr = shared_ptr<CContainerChinaStockMonthLine>;
}