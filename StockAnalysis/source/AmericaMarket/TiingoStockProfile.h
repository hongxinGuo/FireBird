#pragma once
#include"globeSymbolDef.h"

#include"DayLine.h"
#include"EPSSurprise.h"
#include"SetWorldStock.h"

#include"SetTiingoStockProfile.h"

using namespace std;
#include<memory>
#include<atomic>
#include<vector>

class CTiingoStockProfile : public CObject {
public:
  CTiingoStockProfile();
  void Reset(void);

  void Load(CSetTiingoStockProfile& setTiingoStockProfile);
  void Append(CSetTiingoStockProfile& setTiingoStockProfile);
  void Save(CSetTiingoStockProfile& setTiingoStockProfile);

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

protected:
  // 无需存储数据区
};

typedef shared_ptr<CTiingoStockProfile> CTiingoStockProfilePtr;
