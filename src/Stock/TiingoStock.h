#pragma once

#include "ContainerTiingoStockDayLine.h"
#include"SetTiingoStock.h"
#include "SetTiingoStockDayLine.h"

#include "TiingoCompanyFinancialState.h"
#include "TiingoIEXTopOFBook.h"

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

	void ResetAllUpdateDate() override;
	int GetRatio() const override { return 1000; };

	void Load(CSetTiingoStock& setTiingoStock);
	void Append(CSetTiingoStock& setTiingoStock);
	void Save(CSetTiingoStock& setTiingoStock);
	void Update(CSetTiingoStock& setTiingoStock);

	bool IsUpdateFinancialState() const noexcept { return m_fUpdateFinancialState; }
	void SetUpdateFinancialState(bool fFlag) noexcept { m_fUpdateFinancialState = fFlag; }

	bool IsUpdateFinancialStateDB() const noexcept { return m_fUpdateFinancialStateDB; }
	void SetUpdateFinancialStateDB(bool fFlag) noexcept { m_fUpdateFinancialStateDB = fFlag; }

	void UpdateRTData(const CTiingoIEXTopOfBookPtr& pIEXTopOfBook);
	void UpdateFinancialState(const CTiingoCompanyFinancialStatesPtr& pv) noexcept { m_pvFinancialState = pv; }
	void UpdateDayLine(const vector<CDayLinePtr>& vDayLine) { m_dataDayLine.UpdateData(vDayLine); }
	void UpdateFinancialStateDB();
	bool UpdateDayLineDB();
	void SaveCurrentDataToDayLineDB(CSetTiingoStockDayLine& setDayLine, long lTradeDay) const; // 将当前数据存入日线数据库

	void UpdateProfile(const CTiingoStockPtr& pStock);

	void UpdateDayLineStartEndDate();
	long GetDayLineSize() const noexcept { return m_dataDayLine.Size(); }
	CDayLinePtr GetDayLine(const long lIndex) const { return dynamic_pointer_cast<CDayLine>(m_dataDayLine.GetData(lIndex)); }
	void UnloadDayLine() { m_dataDayLine.Unload(); }
	void SaveDayLine() { m_dataDayLine.SaveDB(m_strSymbol); }

	bool HaveNewDayLineData();

	void CheckUpdateStatus(long lTodayDate);
	void CheckFinancialStateUpdateStatus(long lTodayDate);
	void CheckIPOStatus(long lCurrentDate);
	bool CheckDayLineUpdateStatus(long lTodayDate, long lLastTradeDate, long lTime, long lDayOfWeek);

	long GetStatementLastUpdatedDate() { return m_jsonUpdateDate["StatementLastUpdated"]; }
	void SetStatementLastUpdatedDate(long lLDate) { m_jsonUpdateDate["StatementLastUpdated"] = lLDate; }
	long GetDayLineUpdateDate() { return m_jsonUpdateDate["DayLine"]; }
	void SetDayLineUpdateDate(long lLDate) { m_jsonUpdateDate["DayLine"] = lLDate; }

	long GetCompanyFinancialStatementUpdateDate() { return m_jsonUpdateDate["CompanyFinancialStatement"]; }
	void SetCompanyFinancialStatementUpdateDate(long lDate) { m_jsonUpdateDate["CompanyFinancialStatement"] = lDate; }

	long GetDayLineProcessDate();
	void SetDayLineProcessDate(long lDate) { m_jsonUpdateDate["DayLineProcessDate"] = lDate; }
	void Set52WeekLow() { m_jsonUpdateDate["52WeekLow"] = m_v52WeekLow; }
	void Get52WeekLow();
	void Set52WeekHigh() { m_jsonUpdateDate["52WeekHigh"] = m_v52WeekHigh; }
	void Get52WeekHigh();

	bool Have52WeekLowDate(long lDate) { return std::ranges::find(m_v52WeekLow, lDate) != m_v52WeekLow.end(); }
	bool Have52WeekHighDate(long lDate) { return std::ranges::find(m_v52WeekHigh, lDate) != m_v52WeekHigh.end(); }
	void Add52WeekLow(long lDate) { m_v52WeekLow.push_back(lDate); }
	void Delete52WeekLow(long lDate);
	void Add52WeekHigh(long lDate) { m_v52WeekHigh.push_back(lDate); }
	void Delete52WeekHigh(long lDate);

	void ProcessDayLine();
	int IsLowOrHigh(size_t index, long lClose) const;

	// 测试用函数
	void Clear52WeekLow() { m_v52WeekLow.clear(); }
	void Clear52WeekHigh() { m_v52WeekHigh.clear(); }

public:
	CString m_strTiingoPermaTicker{ _T("") }; // Tiingo永久代码标识
	//CString m_strTicker; // 这个使用VirtualStock中的m_strSymbol来代替。
	CString m_strName{ _T("") };
	bool m_fIsActive{ false }; //
	bool m_fIsADR{ false };
	CString m_strTiingoSector{ _T("") };
	CString m_strTiingoIndustry{ _T("") };
	INT32 m_iSicCode{ 0 };
	CString m_strSicSector{ _T("") };
	CString m_strSicIndustry{ _T("") };
	CString m_strReportingCurrency{ _T("") };
	CString m_strLocation{ _T("") };
	CString m_strCompanyWebSite{ _T("") };
	CString m_strSECFilingWebSite{ _T("") };
	CString m_strDataProviderPermaTicker{ _T("") };

protected:
	CTiingoCompanyFinancialStatesPtr m_pvFinancialState{ nullptr };
	CContainerTiingoStockDayLine m_dataDayLine;

	vector<long> m_v52WeekLow; // 年度最低价的日期
	vector<long> m_v52WeekHigh; // 年度最高价的日期

	// 无需存储数据区
	bool m_fUpdateFinancialState{ true };
	bool m_fUpdateDailyData{ true };

	// 更新数据库
	bool m_fUpdateFinancialStateDB{ false };
	bool m_fUpdateDailyDataDB{ false };
};

using CTiingoStockPtr = shared_ptr<CTiingoStock>;
using CTiingoStocksPtr = shared_ptr<vector<CTiingoStockPtr>>;
