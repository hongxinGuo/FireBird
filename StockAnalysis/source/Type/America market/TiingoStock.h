#pragma once

#include"SetWorldStock.h"
#include"SetTiingoStock.h"

using namespace std;
#include<memory>
#include<atomic>
#include<vector>

class CTiingoStock : public CObject {
public:
	CTiingoStock();
	// �������ƺ͸�ֵ��
	CTiingoStock(const CTiingoStock&) = delete;
	CTiingoStock& operator=(const CTiingoStock&) = delete;
	CTiingoStock(const CTiingoStock&&) noexcept = delete;
	CTiingoStock& operator=(const CTiingoStock&&) noexcept = delete;
	virtual ~CTiingoStock() {}

	void Reset(void);

	void Load(CSetTiingoStock& setTiingoStock);
	void Append(CSetTiingoStock& setTiingoStock);
	void Save(CSetTiingoStock& setTiingoStock);

public:
	CString m_strTiingoPermaTicker; // Tiingo���ô����ʶ
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

protected:
	// ����洢������
};

typedef shared_ptr<CTiingoStock> CTiingoStockPtr;
typedef shared_ptr<vector<CTiingoStockPtr>> CTiingoStockVectorPtr;
