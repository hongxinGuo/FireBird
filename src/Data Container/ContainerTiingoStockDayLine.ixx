module;

export module ContainerTiingoStockDayLine;

import VirtualDataHistoryCandle;

import TiingoCandleLine;

import std;
using std::string;
using std::shared_ptr;
using std::vector;
using std::chrono::local_days;

export {
	class CContainerTiingoStockDayLine : public CVirtualDataHistoryCandle {
	public:
		CContainerTiingoStockDayLine();
		CContainerTiingoStockDayLine(const CContainerTiingoStockDayLine& other) = delete;
		CContainerTiingoStockDayLine(CContainerTiingoStockDayLine&& other) noexcept = delete;
		CContainerTiingoStockDayLine& operator=(const CContainerTiingoStockDayLine& other) = delete;
		CContainerTiingoStockDayLine& operator=(CContainerTiingoStockDayLine&& other) noexcept = delete;
		~CContainerTiingoStockDayLine() override = default;

		void SaveDB(const string& strStockSymbol) override;
		void LoadDB(const string& strStockSymbol) override;

		void DeleteDuplicatedDayLine(const string& strStockSymbol) const noexcept;
		void UpdateDB(const string& strStockSymbol);

		void UpdateData(const shared_ptr<vector<CTiingoCandleLine>>& pvTempDayLine);
		void AddLastClose();
		void SplitAdjust() override;

		CTiingoCandleLine* GetData(size_t lIndex);
		CTiingoCandleLine* GetDayLine(local_days date);

		void Add(CTiingoCandleLine& data);

		// 特有函数
	};

	using CContainerTiingoStockDayLinePtr = shared_ptr<CContainerTiingoStockDayLine>;
}