#include"globedef.h"
#include"accessory.h"

#include"TiingoStockFundanmental.h"

CTiingoStockFundanmental::CTiingoStockFundanmental() : CObject() {
  Reset();
}

void CTiingoStockFundanmental::Reset(void) {
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