#pragma once

#include"SetTiingoStock.h"

#include "TiingoFinancialState.h"

class CTiingoStock : public CVirtualStock {
public:
	CTiingoStock();
	// 不允许复制和赋值。
	CTiingoStock(const CTiingoStock&) = delete;
	CTiingoStock& operator=(const CTiingoStock&) = delete;
	CTiingoStock(const CTiingoStock&&) noexcept = delete;
	CTiingoStock& operator=(const CTiingoStock&&) noexcept = delete;
	~CTiingoStock() override = default;
	int GetRatio() const override { return 3; };

	void Reset() override;
	void ResetAllUpdateDate();

	void Load(CSetTiingoStock& setTiingoStock);
	void Append(CSetTiingoStock& setTiingoStock);
	void Save(CSetTiingoStock& setTiingoStock);

	bool IsFinancialStateNeedUpdate() const noexcept { return m_fUpdateFinancialState; }
	void SetFinancialStateNeedUpdate(bool fFlag) noexcept { m_fUpdateFinancialState = fFlag; }

	bool IsUpdateFinancialStateDB() const noexcept { return m_fUpdateFinancialStateDB; }
	void SetUpdateFinancialStateDB(bool fFlag) noexcept { m_fUpdateFinancialStateDB = fFlag; }

	void UpdateFinancialState(const CTiingoFinancialStatesPtr& pv) noexcept { m_pvFinancialState = pv; }
	void UpdateFinancialStateDB() const;

	long GetDailyDataUpdateDate() { return m_jsonUpdateDate["Tiingo"]["DailyData"]; }
	void SetDailyDataUpdateDate(long lDate) { m_jsonUpdateDate["Tiingo"]["DailyData"] = lDate; }
	long GetCompanyFinancialStatementUpdateDate() { return m_jsonUpdateDate["Tiingo"]["CompanyFinancialStatement"]; }
	void SetCompanyFinancialStatementUpdateDate(long lDate) { m_jsonUpdateDate["Tiingo"]["CompanyFinancialStatement"] = lDate; }
	long GetDayLineUpdateDate() { return m_jsonUpdateDate["Tiingo"]["DayLine"]; }
	void SetDayLineUpdateDate(long lDate) { m_jsonUpdateDate["Tiingo"]["DayLine"] = lDate; }
	long GetCompanyProfileUpdateDate() { return m_jsonUpdateDate["Tiingo"]["CompanyProfile"]; }
	void SetCompanyProfileUpdateDate(long lDate) { m_jsonUpdateDate["Tiingo"]["CompanyProfile"] = lDate; }

public:
	CString m_strTiingoPermaTicker; // Tiingo永久代码标识
	//CString m_strTicker; // 这个使用VirtualStock中的m_strSymbol来代替。
	CString m_strName;
	bool m_fIsActive; //
	bool m_fIsADR;
	INT32 m_iSICCode;
	CString m_strSICIndustry;
	CString m_strSICSector;
	CString m_strTiingoIndustry;
	CString m_strTiingoSector;
	CString m_strReportingCurrency;
	CString m_strLocation;
	CString m_strCompanyWebSite;
	CString m_strSECFilingWebSite;

protected:
	CTiingoFinancialStatesPtr m_pvFinancialState{ nullptr };

	// 无需存储数据区
	bool m_fUpdateFinancialState{ true };
	bool m_fUpdateDailyData{ true };

	// 更新数据库
	bool m_fUpdateFinancialStateDB{ false };
	bool m_fUpdateDailyDataDB{ false };
};

using CTiingoStockPtr = shared_ptr<CTiingoStock>;
using CTiingoStocksPtr = shared_ptr<vector<CTiingoStockPtr>>;
