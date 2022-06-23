#include"pch.h"

#include"accessory.h"

#include"TiingoStock.h"

CTiingoStock::CTiingoStock() : CObject() {
	Reset();
}

void CTiingoStock::Reset(void) {
	m_strTiingoPermaTicker = _T("");
	m_strTicker = _T("");
	m_strName = _T("");
	m_fIsActive = false;
	m_fIsADR = false;
	m_strTiingoIndustry = _T("");
	m_strTiingoSector = _T("");
	m_iSICCode = 0;
	m_strSICIndustry = _T("");
	m_strSICSector = _T("");
	m_strReportingCurrency = _T("");
	m_strLocation = _T("");
	m_strCompanyWebSite = _T("");
	m_strSECFilingWebSite = _T("");
	m_lDailyDataUpdateDate = 19800101;
	m_lStatementUpdateDate = 19800101;
}

void CTiingoStock::Load(CSetTiingoStock& setTiingoStock) {
	m_strTiingoPermaTicker = setTiingoStock.m_TiingoPermaTicker;
	m_strTicker = setTiingoStock.m_Ticker;
	m_strName = setTiingoStock.m_Name;
	m_fIsActive = setTiingoStock.m_IsActive;
	m_fIsADR = setTiingoStock.m_IsADR;
	m_iSICCode = setTiingoStock.m_SICCode;
	m_strSICIndustry = setTiingoStock.m_SICIndustry;
	m_strSICSector = setTiingoStock.m_SICSector;
	m_strTiingoIndustry = setTiingoStock.m_TiingoIndustry;
	m_strTiingoSector = setTiingoStock.m_TiingoSector;
	m_strReportingCurrency = setTiingoStock.m_ReportingCurrency;
	m_strLocation = setTiingoStock.m_Location;
	m_strCompanyWebSite = setTiingoStock.m_CompanyWebSite;
	m_strSECFilingWebSite = setTiingoStock.m_SECFilingWebSite;
	m_lStatementUpdateDate = setTiingoStock.m_StatementUpdateDate;
	m_lDailyDataUpdateDate = setTiingoStock.m_DailyDataUpdateDate;
}

void CTiingoStock::Append(CSetTiingoStock& setTiingoStock) {
	setTiingoStock.AddNew();
	Save(setTiingoStock);
	setTiingoStock.Update();
}

void CTiingoStock::Save(CSetTiingoStock& setTiingoStock) {
	if ((m_strCompanyWebSite.GetLength() > 100)
		|| (m_strName.GetLength() > 200)
		|| (m_strSECFilingWebSite.GetLength() > 150)
		|| (m_strSICIndustry.GetLength() > 100)
		|| (m_strSICSector.GetLength() > 100)
		|| (m_strTiingoIndustry.GetLength() > 100)
		|| (m_strTiingoSector.GetLength() > 100)) {
		gl_systemMessage.PushErrorMessage(_T("Tiingo stock ") + m_strName + _T(" ×Ö·û´®Ì«³¤"));
	}
	m_strName = m_strName.Left(200);
	m_strSICIndustry = m_strSICIndustry.Left(100);
	m_strSICSector = m_strSICSector.Left(100);
	m_strTiingoIndustry = m_strTiingoIndustry.Left(100);
	m_strTiingoSector = m_strTiingoSector.Left(100);
	m_strCompanyWebSite = m_strCompanyWebSite.Left(100);
	m_strSECFilingWebSite = m_strSECFilingWebSite.Left(150);

	setTiingoStock.m_TiingoPermaTicker = m_strTiingoPermaTicker;
	setTiingoStock.m_Ticker = m_strTicker;
	setTiingoStock.m_Name = m_strName;
	setTiingoStock.m_IsActive = m_fIsActive;
	setTiingoStock.m_IsADR = m_fIsADR;
	setTiingoStock.m_SICCode = m_iSICCode;
	setTiingoStock.m_SICIndustry = m_strSICIndustry;
	setTiingoStock.m_SICSector = m_strSICSector;
	setTiingoStock.m_TiingoIndustry = m_strTiingoIndustry;
	setTiingoStock.m_TiingoSector = m_strTiingoSector;
	setTiingoStock.m_ReportingCurrency = m_strReportingCurrency;
	setTiingoStock.m_Location = m_strLocation;
	setTiingoStock.m_CompanyWebSite = m_strCompanyWebSite;
	setTiingoStock.m_SECFilingWebSite = m_strSECFilingWebSite;
	setTiingoStock.m_StatementUpdateDate = m_lStatementUpdateDate;
	setTiingoStock.m_DailyDataUpdateDate = m_lDailyDataUpdateDate;
}