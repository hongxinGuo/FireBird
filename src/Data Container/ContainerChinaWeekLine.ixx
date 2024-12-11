#include"pch.h"
export module FireBird.Container.ChinaStockWeekLine;

export import FireBird.Container.VirtualHistoryCandleExtend;
export import FireBird.HistoryCandle.WeekLine;

export {
	class CContainerChinaWeekLine : public CVirtualContainerHistoryCandleExtend {
	public:
		CContainerChinaWeekLine();
		~CContainerChinaWeekLine() override = default;

		bool LoadDB(const CString& strStockSymbol) final;
		CContainerChinaWeekLine(const CContainerChinaWeekLine& other) = delete;
		CContainerChinaWeekLine(CContainerChinaWeekLine&& other) noexcept = delete;
		CContainerChinaWeekLine& operator=(const CContainerChinaWeekLine& other) = delete;
		CContainerChinaWeekLine& operator=(CContainerChinaWeekLine&& other) noexcept = delete;
		bool SaveDB(const CString& strStockSymbol) final;

		void SaveCurrentWeekLine() const;
		bool LoadCurrentWeekLine();

		void UpdateData(const vector<CWeekLinePtr>& vTempWeekLine);
		void UpdateData(const CVirtualHistoryCandleExtendPtr& pHistoryCandleExtend) const;

		void StoreVectorData(const vector<CWeekLinePtr>& vWeekLine);
	};

	using CContainerChinaWeekLinePtr = shared_ptr<CContainerChinaWeekLine>;
}
