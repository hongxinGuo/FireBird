#pragma once
#include"TiingoStock.h"

#include"ContainerVirtualStock.h"

class CContainerTiingoStock : public CContainerVirtualStock {
public:
	CContainerTiingoStock();
	CContainerTiingoStock(const CContainerTiingoStock& other) = delete;
	CContainerTiingoStock(CContainerTiingoStock&& other) noexcept = delete;
	CContainerTiingoStock& operator=(const CContainerTiingoStock& other) = delete;
	CContainerTiingoStock& operator=(CContainerTiingoStock&& other) noexcept = delete;
	~CContainerTiingoStock() override;
	void Reset() override;

	CTiingoStockPtr GetItem(const size_t lIndex) { return dynamic_pointer_cast<CTiingoStock>(Get(lIndex)); }
	CTiingoStockPtr GetStock(const size_t lIndex) { return dynamic_pointer_cast<CTiingoStock>(Get(lIndex)); }
	CTiingoStockPtr GetStock(const string& strStockCode) { return dynamic_pointer_cast<CTiingoStock>(Get(strStockCode)); }

	void UpdateProfile(const CTiingoStockPtr& pStock);

	void UpdateProfileDB();
	bool LoadProfileDB();
	bool LoadProfileDB2();

	void ResetDayLineStartEndDate();
	void BuildDayLine(long lDate);
	void LoadDayLine(long lDate);
	static void DeleteDayLine(long lDate);

	long GetTotalActiveStocks();

	void UpdateFinancialStateDB();
	void TaskUpdate52WeekHighDB();
	void TaskUpdate52WeekLowDB();
	void TaskCalculate();
	void TaskCalculate2();

	void TaskFixDayLine();

	void Delete52WeekHighData();

	void Delete52WeekLowData();

	bool IsUpdateFinancialStateDB() noexcept;
	bool IsUpdate52WeekHighLowDB() noexcept;

	void SetUpdateFinancialState(bool fFlag);

	void TaskProcessDayLine();
	void ReportHighHigherRate();
};
