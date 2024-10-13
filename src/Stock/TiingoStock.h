#pragma once

#include "ContainerTiingoStockDayLine.h"
#include"SetTiingoStock.h"

#include "TiingoCompanyFinancialState.h"

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

	void UpdateFinancialState(const CTiingoCompanyFinancialStatesPtr& pv) noexcept { m_pvFinancialState = pv; }
	void UpdateDayLine(const vector<CDayLinePtr>& vDayLine) { m_dataDayLine.UpdateData(vDayLine); }
	void UpdateFinancialStateDB() const;
	bool UpdateDayLineDB();

	void UpdateProfile(const CTiingoStockPtr& pStock);

	void UpdateDayLineStartEndDate();
	long GetDayLineSize() const noexcept { return m_dataDayLine.Size(); }
	CDayLinePtr GetDayLine(const long lIndex) const { return dynamic_pointer_cast<CDayLine>(m_dataDayLine.GetData(lIndex)); }
	void UnloadDayLine() { m_dataDayLine.Unload(); }
	void SaveDayLine() { m_dataDayLine.SaveDB(m_strSymbol); }

	bool HaveNewDayLineData();

	void CheckUpdateStatus(long lTodayDate);
	void CheckFinancialStateUpdateStatus(long lTodayDate);
	bool CheckDayLineUpdateStatus(long lTodayDate, long lLastTradeDate, long lTime, long lDayOfWeek);

	long GetStatementLastUpdatedDate() { return m_jsonUpdateDate["StatementLastUpdated"]; }
	void SetStatementLastUpdatedDate(long lLDate) { m_jsonUpdateDate["StatementLastUpdated"] = lLDate; }
	long GetDailyLastUpdatedDate() { return m_jsonUpdateDate["DailyLastUpdated"]; }
	void SetDailyLastUpdatedDate(long lLDate) { m_jsonUpdateDate["DailyLastUpdated"] = lLDate; }

	long GetDailyDataUpdateDate() { return m_jsonUpdateDate["DailyData"]; }
	void SetDailyDataUpdateDate(long lDate) { m_jsonUpdateDate["DailyData"] = lDate; }
	long GetCompanyFinancialStatementUpdateDate() { return m_jsonUpdateDate["CompanyFinancialStatement"]; }
	void SetCompanyFinancialStatementUpdateDate(long lDate) { m_jsonUpdateDate["CompanyFinancialStatement"] = lDate; }
	long GetDayLineUpdateDate() { return m_jsonUpdateDate["DayLine"]; }
	void SetDayLineUpdateDate(long lDate) { m_jsonUpdateDate["DayLine"] = lDate; }
	long GetCompanyProfileUpdateDate() { return m_jsonUpdateDate["CompanyProfile"]; }
	void SetCompanyProfileUpdateDate(long lDate) { m_jsonUpdateDate["CompanyProfile"] = lDate; }

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
	//INT64 m_llStatementLastUpdated{ 0 }; // 此项存储于json结构中
	//INT64 m_llDailyLastUpdated{ 0 }; // 此项存储于json结构中
	CString m_strDataProviderPermaTicker{ _T("") };

protected:
	CTiingoCompanyFinancialStatesPtr m_pvFinancialState{ nullptr };

	CContainerTiingoStockDayLine m_dataDayLine;

	// 无需存储数据区
	bool m_fUpdateFinancialState{ true };
	bool m_fUpdateDailyData{ true };

	// 更新数据库
	bool m_fUpdateFinancialStateDB{ false };
	bool m_fUpdateDailyDataDB{ false };
};

using CTiingoStockPtr = shared_ptr<CTiingoStock>;
using CTiingoStocksPtr = shared_ptr<vector<CTiingoStockPtr>>;
