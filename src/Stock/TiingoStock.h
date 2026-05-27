#pragma once

#include "ContainerTiingoStockDayLine.h"
#include "ContainerTiingoStockWeekLine.h"
#include "ContainerTiingoStockMonthLine.h"

#include "TiingoCompanyFinancialState.h"
#include "TiingoIEXTopOFBook.h"
#include "TiingoStockDailyMeta.h"

class CTiingoStock;
using CTiingoStockPtr = shared_ptr<CTiingoStock>;

class CTiingoStock : public CVirtualStock {
public:
	CTiingoStock();
	// 不允许复制和赋值。
	CTiingoStock(const CTiingoStock&) = delete;
	CTiingoStock& operator=(const CTiingoStock&) = delete;
	CTiingoStock(const CTiingoStock&&) noexcept = delete;
	CTiingoStock& operator=(const CTiingoStock&&) noexcept = delete;
	~CTiingoStock() override = default;

	// Getter and Setter for m_strTiingoPermaTicker
	string GetTiingoPermaTicker() const { return m_strTiingoPermaTicker; }
	string GetTiingoPermaTicker2() const { return m_strTiingoPermaTicker; }
	void SetTiingoPermaTicker(const string& val) { m_strTiingoPermaTicker = val; }

	// Getter and Setter for m_strName
	string GetName() const { return m_strName; }
	void SetName(const string& val) { m_strName = val; }

	// Getter and Setter for m_fDividend
	double GetDividend() const { return m_fDividend; }
	void SetDividend(double val) { m_fDividend = val; }

	// Getter and Setter for m_fSplitFactor
	double GetSplitFactor() const { return m_fSplitFactor; }
	void SetSplitFactor(double val) { m_fSplitFactor = val; }

	// Getter and Setter for m_fIsADR
	bool IsADR() const { return m_fIsADR; }
	void SetIsADR(bool val) { m_fIsADR = val; }

	// Getter and Setter for m_strTiingoSector
	string GetTiingoSector() const { return m_strTiingoSector; }
	void SetTiingoSector(const string& val) { m_strTiingoSector = val; }

	// Getter and Setter for m_strTiingoIndustry
	string GetTiingoIndustry() const { return m_strTiingoIndustry; }
	void SetTiingoIndustry(const string& val) { m_strTiingoIndustry = val; }

	// Getter and Setter for m_iSicCode
	INT32 GetSicCode() const { return m_iSicCode; }
	void SetSicCode(INT32 val) { m_iSicCode = val; }

	// Getter and Setter for m_strSicSector
	string GetSicSector() const { return m_strSicSector; }
	void SetSicSector(const string& val) { m_strSicSector = val; }

	// Getter and Setter for m_strSicIndustry
	string GetSicIndustry() const { return m_strSicIndustry; }
	void SetSicIndustry(const string& val) { m_strSicIndustry = val; }

	// Getter and Setter for m_strReportingCurrency
	string GetReportingCurrency() const { return m_strReportingCurrency; }
	void SetReportingCurrency(const string& val) { m_strReportingCurrency = val; }

	// Getter and Setter for m_strLocation
	string GetLocation() const { return m_strLocation; }
	void SetLocation(const string& val) { m_strLocation = val; }

	// Getter and Setter for m_strCompanyWebSite
	string GetCompanyWebSite() const { return m_strCompanyWebSite; }
	void SetCompanyWebSite(const string& val) { m_strCompanyWebSite = val; }

	// Getter and Setter for m_strSECFilingWebSite
	string GetSECFilingWebSite() const { return m_strSECFilingWebSite; }
	void SetSECFilingWebSite(const string& val) { m_strSECFilingWebSite = val; }

	// Getter and Setter for m_strDataProviderPermaTicker
	string GetDataProviderPermaTicker() const { return m_strDataProviderPermaTicker; }
	void SetDataProviderPermaTicker(const string& val) { m_strDataProviderPermaTicker = val; }

	void ResetAllUpdateDate() override;
	int GetRatio() const override { return 1000000; }; // 

	bool IsUpdateStockDailyMeta() const noexcept { return m_fUpdateStockDailyMeta; }
	void SetUpdateStockDailyMeta(bool fFlag) noexcept { m_fUpdateStockDailyMeta = fFlag; }

	bool IsUpdateFinancialState() const noexcept { return m_fUpdateFinancialState; }
	void SetUpdateFinancialState(bool fFlag) noexcept { m_fUpdateFinancialState = fFlag; }

	bool IsUpdateFinancialStateDB() const noexcept { return m_fUpdateFinancialStateDB; }
	void SetUpdateFinancialStateDB(bool fFlag) noexcept { m_fUpdateFinancialStateDB = fFlag; }
	bool IsUpdate52WeekHighLowDB() const noexcept { return m_fUpdate52WeekHighLowDB; }
	void SetUpdate52WeekHighLowDB(bool fFlag) noexcept { m_fUpdate52WeekHighLowDB = fFlag; }

	bool IsDayLineLoaded() const noexcept override { return m_dataDayLine.IsDataLoaded(); }
	void SetDayLineLoaded(bool fFlag) noexcept override { m_dataDayLine.SetDataLoaded(fFlag); }
	bool IsWeekLineLoaded() const noexcept override { return m_dataWeekLine.IsDataLoaded(); }
	void SetWeekLineLoaded(bool fFlag) noexcept override { m_dataWeekLine.SetDataLoaded(fFlag); }
	bool IsMonthLineLoaded() const noexcept override { return m_dataMonthLine.IsDataLoaded(); }
	void SetMonthLineLoaded(bool fFlag) noexcept override { m_dataMonthLine.SetDataLoaded(fFlag); }

	void UpdateRTData(const CTiingoIEXTopOfBook& IEXTopOfBook);
	void UpdateFinancialState(const CTiingoCompanyFinancialStatesPtr& pv) noexcept { m_pvFinancialState = pv; }
	void UpdateDayLine(const CTiingoCandleLinesPtr& vTempDayLine);
	void UpdateFinancialStateDB();
	void ClearFinancialState() { m_pvFinancialState.reset(); }

	void UpdateProfile(const CTiingoStockPtr& pStock);
	void UpdateDailyMeta(const CTiingoStockDailyMetaPtr& pMeta);

	void UpdateDayLineStartEndDate();
	auto GetDayLineSize() const noexcept { return m_dataDayLine.Size(); }
	bool HaveDayLine(const long lDate) noexcept { return m_dataDayLine.HaveDayLine(lDate); }
	CTiingoCandleLine* GetDayLine(const size_t lIndex) { return m_dataDayLine.GetData(lIndex); }
	CTiingoCandleLine* GetDayLineAtDate(const long lDate) { return m_dataDayLine.GetDayLine(lDate); }

	// 当前被处理历史数据容器
	CVirtualDataHistoryCandle* DayLine() noexcept final { return &m_dataDayLine; }
	CVirtualDataHistoryCandle* WeekLine() noexcept final { return &m_dataWeekLine; }
	CVirtualDataHistoryCandle* MonthLine() noexcept final { return &m_dataMonthLine; }

	void UnloadDayLine() { m_dataDayLine.Unload(); }
	bool UpdateDayLineDB();
	void SaveDayLineDB() { m_dataDayLine.SaveDB(m_strSymbol); }
	bool IsDayLineDuplicated() noexcept final;
	void DeleteDuplicatedDayLine() noexcept final;
	bool LoadDayLineDB() override { return m_dataDayLine.LoadDB(m_strSymbol); }
	bool LoadWeekLineDB() override {
		CreateWeekLine();
		return true;
	}
	bool LoadMonthLineDB() override {
		CreateMonthLine();
		return true;
	}
	void CreateWeekLine();
	void CreateMonthLine();

	void CalculateDayLineMA(const int length) { m_dataDayLine.CalculateMA(length); }

	void RebuildStockSplitDB() override;

	bool HaveNewDayLineData(); //Todo: 移至VirtualStock中，合并其他股票类型的同名函数
	void CheckUpdateStatus(long lTodayDate);
	void CheckFinancialStateUpdateStatus(long lTodayDate);
	void CheckDayLineUpdateStatus(long llTodayDate);
	void CheckStockDailyMetaStatus(long lCurrentDate);

	long GetStatementLastUpdatedDate() { return m_jsonUpdateDate["StatementLastUpdated"]; }
	void SetStatementLastUpdatedDate(long lLDate) { m_jsonUpdateDate["StatementLastUpdated"] = lLDate; }
	long GetDailyUpdateDate() { return m_jsonUpdateDate["DailyUpdate"]; }
	void SetDailyUpdateDate(long lLDate) { m_jsonUpdateDate["DailyUpdate"] = lLDate; }

	long GetCompanyFinancialStatementUpdateDate() { return m_jsonUpdateDate["CompanyFinancialStatement"]; }
	void SetCompanyFinancialStatementUpdateDate(long lDate) { m_jsonUpdateDate["CompanyFinancialStatement"] = lDate; }

	long GetHistoryDayLineStartDate() { return m_jsonUpdateDate["HistoryDayLineStartDate"]; }
	void SetHistoryDayLineStartDate(long lDate) { m_jsonUpdateDate["HistoryDayLineStartDate"] = lDate; }
	long GetHistoryDayLineEndDate() { return m_jsonUpdateDate["HistoryDayLineEndDate"]; }
	void SetHistoryDayLineEndDate(long lDate) { m_jsonUpdateDate["HistoryDayLineEndDate"] = lDate; }
	long GetUpdateStockDailyMetaDate() { return m_jsonUpdateDate["UpdateStockDailyMetaDate"]; }
	void SetUpdateStockDailyMetaDate(long lDate) { m_jsonUpdateDate["UpdateStockDailyMetaDate"] = lDate; }

	long GetDayLineProcessDate();
	void SetDayLineProcessDate(long lDate) { m_jsonUpdateDate["DayLineProcessDate"] = lDate; }

	bool Have52WeekLowDate(long lDate) { return std::ranges::find(m_v52WeekLowDate, lDate) != m_v52WeekLowDate.end(); }
	bool Have52WeekHighDate(long lDate) { return std::ranges::find(m_v52WeekHighDate, lDate) != m_v52WeekHighDate.end(); }
	void Add52WeekLowDate(long lDate) { m_v52WeekLowDate.push_back(lDate); }
	void Delete52WeekLowDate(long lDate);
	void Add52WeekHighDate(long lDate) { m_v52WeekHighDate.push_back(lDate); }
	void Delete52WeekHighDate(long lDate);

	void Delete52WeekHighDB() const;
	void Delete52WeekLowDB() const;
	bool IsEnough52WeekLow();
	void Load52WeekLowDB();

	size_t Get52WeekLowSize() const { return m_v52WeekLowDate.size(); }
	auto Get52WeekLowDate(size_t index) const { return m_v52WeekLowDate.at(index); }
	size_t Get52WeekHighSize() const { return m_v52WeekHighDate.size(); }
	auto Get52WeekHighDate(size_t index) const { return m_v52WeekHighDate.at(index); }

	void ProcessDayLine(long lLastCalculatedDate); // 系统调用这个函数
	void FindHighLow3(size_t endPos);
	void FindHighLow2(size_t endPos);
	void FindAll52WeekLowDate(size_t beginPos, size_t endPos);
	void FindAll52WeekHighDate(size_t beginPos, size_t endPos);
	size_t FindCurrent52WeekLowPos(size_t beginPos, size_t endPos, double& value) const;
	size_t FindCurrent52WeekHighPos(size_t beginPos, size_t endPos, double& value) const;
	double CalculateSplitFactor(size_t beginPos, size_t endPos);
	void AdjustedStockCloseValue(double dSplitFactor, size_t calculatePos, size_t dayLineSize);
	int IsLowOrHigh(size_t index, double dClose) const;

	// 测试用函数
	void ProcessDayLine2(long lLastCalculatedDate); // 用于测试
	void ProcessDayLine3(long lLastCalculatedDate); // 用于测试
	void CalculateNewHighHigher(int period = 90);
	void CalculateNewLowLower(int period = 90);

	void Clear52WeekLow() { m_v52WeekLowDate.clear(); }
	void Clear52WeekHigh() { m_v52WeekHighDate.clear(); }

public:
	vector<long> m_v52WeekLowDate; // 年度最低价的日期
	vector<long> m_v52WeekHighDate; // 年度最高价的日期
	vector<double> m_vClose; // 收盘价

	long m_lHighHigher{ 0 };
	long m_lNoHighHigher{ 0 };
	long m_lLowLower{ 0 };
	long m_lNoLowLower{ 0 };

protected:
	string m_strTiingoPermaTicker{ "" }; // Tiingo永久代码标识
	//string m_strTicker; // 这个使用VirtualStock中的m_strSymbol来代替。
	string m_strName{ "" };
	// bool m_fIsActive{ false }; // 这个使用VirtualStock中的m_fActive来代替
	double m_fDividend{ 0 };
	double m_fSplitFactor{ 1 };
	bool m_fIsADR{ false };
	string m_strTiingoSector{ "" };
	string m_strTiingoIndustry{ "" };
	INT32 m_iSicCode{ 0 };
	string m_strSicSector{ "" };
	string m_strSicIndustry{ "" };
	string m_strReportingCurrency{ "" };
	string m_strLocation{ "" };
	string m_strCompanyWebSite{ "" };
	string m_strSECFilingWebSite{ "" };
	string m_strDataProviderPermaTicker{ "" };

	CTiingoCompanyFinancialStatesPtr m_pvFinancialState{ nullptr };

	CContainerTiingoStockDayLine m_dataDayLine; // 日线数据容器
	CContainerTiingoStockWeekLine m_dataWeekLine; // 周线数据容器 
	CContainerTiingoStockMonthLine m_dataMonthLine; // 月线数据容器

	// 无需存储数据区
	bool m_fUpdateStockDailyMeta{ true };
	bool m_fUpdateFinancialState{ true };
	bool m_fUpdateDailyData{ true };

	// 更新数据库
	bool m_fUpdateFinancialStateDB{ false };
	bool m_fUpdateDailyDataDB{ false };
	bool m_fUpdate52WeekHighLowDB{ false };
};

using CTiingoStockPtr = shared_ptr<CTiingoStock>;
using CTiingoStocksPtr = shared_ptr<vector<CTiingoStockPtr>>;

bool IsTiingoStock(const CVirtualStockPtr& pStock);
