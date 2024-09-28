#pragma once

#include"SetTiingoStock.h"

#include<memory>
#include<vector>

#include "TiingoFinancialState.h"

class CTiingoStock : public CVirtualStock {
public:
	CTiingoStock();
	// �������ƺ͸�ֵ��
	CTiingoStock(const CTiingoStock&) = delete;
	CTiingoStock& operator=(const CTiingoStock&) = delete;
	CTiingoStock(const CTiingoStock&&) noexcept = delete;
	CTiingoStock& operator=(const CTiingoStock&&) noexcept = delete;
	~CTiingoStock() override = default;
	int GetRatio() const override { return 3; };

	void Reset() override;

	void Load(const CSetTiingoStock& setTiingoStock);
	void Append(CSetTiingoStock& setTiingoStock);
	void Save(CSetTiingoStock& setTiingoStock);

	bool IsFinancialStateNeedUpdate() const noexcept { return m_fUpdateFinancialState; }
	void SetFinancialStateNeedUpdate(bool fFlag) noexcept { m_fUpdateFinancialState = fFlag; }
	bool IsSaveFinancialState() const noexcept { return m_fSaveFinancialState; }
	void SetSaveFinancialState(bool fFlag) noexcept { m_fSaveFinancialState = fFlag; }

	void UpdateFinancialState(const CTiingoFinancialStatesPtr& pv) noexcept { m_pvFinancialState = pv; }
	bool UpdateFinancialStateDB() const;

public:
	CString m_strTiingoPermaTicker; // Tiingo���ô����ʶ
	//CString m_strTicker; // ���ʹ��VirtualStock�е�m_strSymbol�����档
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

protected:
	CTiingoFinancialStatesPtr m_pvFinancialState{ nullptr };

	// ����洢������
	bool m_fUpdateFinancialState{ true };
	bool m_fSaveFinancialState{ false };
};

using CTiingoStockPtr = shared_ptr<CTiingoStock>;
using CTiingoStocksPtr = shared_ptr<vector<CTiingoStockPtr>>;
