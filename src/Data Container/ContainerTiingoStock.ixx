#include"pch.h"
module;
export module FireBird.Container.Tiingo.Stock;

import FireBird.Container.VirtualStock;
import FireBird.Stock.TiingoStock;

export {
	class CContainerTiingoStock : public CContainerVirtualStock {
	public:
		CContainerTiingoStock();
		CContainerTiingoStock(const CContainerTiingoStock& other) = delete;
		CContainerTiingoStock(CContainerTiingoStock&& other) noexcept = delete;
		CContainerTiingoStock& operator=(const CContainerTiingoStock& other) = delete;
		CContainerTiingoStock& operator=(CContainerTiingoStock&& other) noexcept = delete;
		~CContainerTiingoStock() override;
		void Reset() override;

		CTiingoStockPtr GetStock(const size_t lIndex) { return dynamic_pointer_cast<CTiingoStock>(Get(lIndex)); }
		CTiingoStockPtr GetStock(const CString& strStockCode) { return dynamic_pointer_cast<CTiingoStock>(Get(strStockCode)); }

		void UpdateProfile(const CTiingoStockPtr& pStock);

		void UpdateDB();
		bool LoadDB();

		void ResetDayLineStartEndDate();
		void BuildDayLine(long lDate);
		static void DeleteDayLine(long lDate);

		long GetTotalActiveStocks();

		void UpdateFinancialStateDB();
		void UpdateDayLineDB();
		void TaskUpdate52WeekHighLowDB();
		void TaskCalculate();
		void Delete52WeekHighData();

		void Delete52WeekLowData();

		bool IsUpdateFinancialStateDB() noexcept;
		bool IsUpdate52WeekHighLowDB() noexcept;

		void SetUpdateFinancialState(bool fFlag);

		void TaskProcessDayLine();
		void TaskProcessDayLine2();
	};
}
