#pragma once

#include"SetTiingoStock.h"

#include<memory>
#include<vector>

class CTiingoStock final {
public:
	CTiingoStock();
	// 不允许复制和赋值。
	CTiingoStock(const CTiingoStock&) = delete;
	CTiingoStock& operator=(const CTiingoStock&) = delete;
	CTiingoStock(const CTiingoStock&&) noexcept = delete;
	CTiingoStock& operator=(const CTiingoStock&&) noexcept = delete;
	~CTiingoStock() = default;

	void Reset();

	void Load(const CSetTiingoStock& setTiingoStock);
	void Append(CSetTiingoStock& setTiingoStock);
	void Save(CSetTiingoStock& setTiingoStock);

	bool IsDayLineNeedUpdate() const noexcept { return m_fUpdateDayLine; }
	void SetDayLineNeedUpdate(bool fFlag) noexcept { m_fUpdateDayLine = fFlag; }
	bool IsFinancialStateNeedUpdate() const noexcept { return m_fUpdateFinancialState; }
	void SetFinancialStateNeedUpdate(bool fFlag) noexcept { m_fUpdateFinancialState = fFlag; }

public:
	CString m_strTiingoPermaTicker; // Tiingo永久代码标识
	CString m_strTicker;
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
	long m_lStatementUpdateDate;
	long m_lDailyDataUpdateDate;

	// 无需存储数据区
	bool m_fUpdateDayLine{ true };
	bool m_fUpdateFinancialState{ true };
};

using CTiingoStockPtr = shared_ptr<CTiingoStock>;
using CTiingoStocksPtr = shared_ptr<vector<CTiingoStockPtr>>;
