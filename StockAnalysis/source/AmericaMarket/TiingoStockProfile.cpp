#include"pch.h"
#include"globedef.h"
#include"accessory.h"

#include"TiingoStockProfile.h"

CTiingoStockProfile::CTiingoStockProfile() : CObject() {
  Reset();
}

void CTiingoStockProfile::Reset(void) {
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

void CTiingoStockProfile::Load(CSetTiingoStockProfile& setTiingoStockProfile) {
  m_strTiingoPermaTicker = setTiingoStockProfile.m_TiingoPermaTicker;
  m_strTicker = setTiingoStockProfile.m_Ticker;
  m_strName = setTiingoStockProfile.m_Name;
  m_fIsActive = setTiingoStockProfile.m_IsActive;
  m_fIsADR = setTiingoStockProfile.m_IsADR;
  m_iSICCode = setTiingoStockProfile.m_SICCode;
  m_strSICIndustry = setTiingoStockProfile.m_SICIndustry;
  m_strSICSector = setTiingoStockProfile.m_SICSector;
  m_strTiingoIndustry = setTiingoStockProfile.m_TiingoIndustry;
  m_strTiingoSector = setTiingoStockProfile.m_TiingoSector;
  m_strReportingCurrency = setTiingoStockProfile.m_ReportingCurrency;
  m_strLocation = setTiingoStockProfile.m_Location;
  m_strCompanyWebSite = setTiingoStockProfile.m_CompanyWebSite;
  m_strSECFilingWebSite = setTiingoStockProfile.m_SECFilingWebSite;
  m_lStatementUpdateDate = setTiingoStockProfile.m_StatementUpdateDate;
  m_lDailyDataUpdateDate = setTiingoStockProfile.m_DailyDataUpdateDate;
}

void CTiingoStockProfile::Append(CSetTiingoStockProfile& setTiingoStockProfile) {
  setTiingoStockProfile.AddNew();
  Save(setTiingoStockProfile);
  setTiingoStockProfile.Update();
}

void CTiingoStockProfile::Save(CSetTiingoStockProfile& setTiingoStockProfile) {
  setTiingoStockProfile.m_TiingoPermaTicker = m_strTiingoPermaTicker;
  setTiingoStockProfile.m_Ticker = m_strTicker;
  setTiingoStockProfile.m_Name = m_strName;
  setTiingoStockProfile.m_IsActive = m_fIsActive;
  setTiingoStockProfile.m_IsADR = m_fIsADR;
  setTiingoStockProfile.m_SICCode = m_iSICCode;
  setTiingoStockProfile.m_SICIndustry = m_strSICIndustry;
  setTiingoStockProfile.m_SICSector = m_strSICSector;
  setTiingoStockProfile.m_TiingoIndustry = m_strTiingoIndustry;
  setTiingoStockProfile.m_TiingoSector = m_strTiingoSector;
  setTiingoStockProfile.m_ReportingCurrency = m_strReportingCurrency;
  setTiingoStockProfile.m_Location = m_strLocation;
  setTiingoStockProfile.m_CompanyWebSite = m_strCompanyWebSite;
  setTiingoStockProfile.m_SECFilingWebSite = m_strSECFilingWebSite;
  setTiingoStockProfile.m_StatementUpdateDate = m_lStatementUpdateDate;
  setTiingoStockProfile.m_DailyDataUpdateDate = m_lDailyDataUpdateDate;
}