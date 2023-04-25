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
};

using CTiingoStockPtr = shared_ptr<CTiingoStock>;
using CTiingoStockVectorPtr = shared_ptr<vector<CTiingoStockPtr>>;
