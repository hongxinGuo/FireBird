#pragma once

#include"ContainerVirtualStock.h"

class CTiingoStock;

using std::chrono::local_days;

class CContainerTiingoStock : public CContainerVirtualStock {
public:
	CContainerTiingoStock();
	CContainerTiingoStock(const CContainerTiingoStock& other) = delete;
	CContainerTiingoStock(CContainerTiingoStock&& other) noexcept = delete;
	CContainerTiingoStock& operator=(const CContainerTiingoStock& other) = delete;
	CContainerTiingoStock& operator=(CContainerTiingoStock&& other) noexcept = delete;
	~CContainerTiingoStock() override = default;
	void Reset() override;

	shared_ptr<CTiingoStock> GetItem(size_t lIndex);
	shared_ptr<CTiingoStock> GetStock(size_t lIndex);
	shared_ptr<CTiingoStock> GetStock(const string& strStockCode);

	void UpdateProfile(const shared_ptr<CTiingoStock>& pStock);

	void UpdateProfileDB(std::stop_token st);
	bool LoadProfileDB();
	void DeleteDuplicatedSymbolFromDB();

	void ResetDayLineStartEndDate();
	void BuildDayLine(std::stop_token st, local_days date);
	void LoadDayLine(local_days date);
	void DeleteDayLine(local_days lDate);

	long GetTotalActiveStocks();

	void UpdateFinancialStateDB(std::stop_token st);
	void TaskUpdate52WeekHighDB();
	void TaskUpdate52WeekLowDB();
	void TaskCalculate();
	void TaskCalculate2(std::stop_token st);

	void Delete52WeekHighDB();

	void Delete52WeekLowDB();

	bool IsUpdateFinancialStateDB() noexcept;
	bool IsUpdate52WeekHighLowDB() noexcept;

	void SetUpdateFinancialState(bool fFlag);

	void TaskProcessTodayDayLine(std::stop_token st);
	void ReportHighHigherRate();
	void Update5YearLow90PercentStockDB();
	void Update5YearLow70PercentStockDB();
	void Update5YearLow80PercentStockDB();
	void Update5YearLowStockDB();
};

extern CContainerTiingoStock gl_dataContainerTiingoStock;
