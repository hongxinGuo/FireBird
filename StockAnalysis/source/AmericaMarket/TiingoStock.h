#pragma once
#include"globeSymbolDef.h"

#include"DayLine.h"
#include"EPSSurprise.h"
#include"SetWorldStock.h"

#include"SetTiingoStock.h"

using namespace std;
#include<memory>
#include<atomic>
#include<vector>

class CTiingoStock : public CObject {
public:
  CTiingoStock();
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
