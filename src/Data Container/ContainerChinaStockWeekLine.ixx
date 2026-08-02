module;

export module FireBirdLib.Container.HistoryCandle.ChinaStockWeekLine;

import FireBirdLib.Container.HistoryCandle;
import FireBirdLib.HistoryCandle;
import FireBirdLib.HistoryCandle.WeekLine;

import std;
using std::vector;
using std::shared_ptr;
using std::chrono::local_days;

export {

	class CContainerChinaStockWeekLine : public CVirtualDataHistoryCandle {
	public:
		CContainerChinaStockWeekLine();
		CContainerChinaStockWeekLine(const CContainerChinaStockWeekLine& other) = delete;
		CContainerChinaStockWeekLine(CContainerChinaStockWeekLine&& other) noexcept = delete;
		CContainerChinaStockWeekLine& operator=(const CContainerChinaStockWeekLine& other) = delete;
		CContainerChinaStockWeekLine& operator=(CContainerChinaStockWeekLine&& other) noexcept = delete;
		~CContainerChinaStockWeekLine() override = default;

		void UpdateData(const vector<CWeekLine>& vTempWeekLine);
		void UpdateData(const CVirtualHistoryCandle* pHistoryCandle);

		void StoreVectorData(const vector<CWeekLine>& vWeekLine);

		CWeekLine* GetData(size_t lIndex);
		CWeekLine* GetWeekLine(local_days lDate);
	};

	using CContainerChinaWeekLinePtr = shared_ptr<CContainerChinaStockWeekLine>;
}