module;

export module ContainerTiingoStockWeekLine;

import VirtualDataHistoryCandle;

import TiingoCandleLine;

import std;
using std::shared_ptr;
using std::vector;
using std::chrono::local_days;

export {
	class CContainerTiingoStockWeekLine : public CVirtualDataHistoryCandle {
	public:
		CContainerTiingoStockWeekLine();
		CContainerTiingoStockWeekLine(const CContainerTiingoStockWeekLine& other) = delete;
		CContainerTiingoStockWeekLine(CContainerTiingoStockWeekLine&& other) noexcept = delete;
		CContainerTiingoStockWeekLine& operator=(const CContainerTiingoStockWeekLine& other) = delete;
		CContainerTiingoStockWeekLine& operator=(CContainerTiingoStockWeekLine&& other) noexcept = delete;
		~CContainerTiingoStockWeekLine() override = default;

		void UpdateData(const shared_ptr<vector<CTiingoCandleLine>>& pvTempDayLine);

		CTiingoCandleLine* GetData(size_t lIndex);
		CTiingoCandleLine* GetWeekLine(local_days lDate);

		void Add(CTiingoCandleLine& pData);

		// 特有函数
	};

	using CContainerTiingoStockWeekLinePtr = shared_ptr<CContainerTiingoStockWeekLine>;
}