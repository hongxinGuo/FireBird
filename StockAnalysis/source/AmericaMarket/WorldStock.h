#pragma once
#include"globeSymbolDef.h"

#include"VirtualStock.h"

#include"DayLine.h"
#include"EPSSurprise.h"
#include"SetWorldStock.h"

using namespace std;
#include<memory>
#include<atomic>
#include<vector>

class CWorldStock : public CVirtualStock {
public:
  CWorldStock();
  ~CWorldStock();
  virtual void Reset(void);
public:
  void Load(CSetWorldStock& setWorldStock);
  bool CheckDayLineUpdateStatus(long lTodayDate, long lLastTradeDate, long lTime, long lDayOfWeek);
  void Save(CSetWorldStock& setWorldStock);
  void Update(CSetWorldStock& setWorldStock);
  void Append(CSetWorldStock& setWorldStock);
  void SaveDayLine(void);
  virtual bool UpdateEPSSurpriseDB(void);

  void UpdateDayLine(vector<CDayLinePtr>& vDayLine);
  void UpdateEPSSurprise(vector<CEPSSurprisePtr>& vEPSSurprise);

  void UpdateDayLineStartEndDate(void);
  long GetDayLineSize(void) noexcept { return m_vDayLine.size(); }
  CDayLinePtr GetDayLine(long lIndex) { return m_vDayLine.at(lIndex); }
  void UnloadDayLine(void) { m_vDayLine.resize(0); }

  bool HaveNewDayLineData(void);

  bool IsEPSSurpriseNeedUpdate(void) noexcept { return m_fEPSSurpriseNeedUpdate; }
  void SetEPSSurpriseNeedUpdate(bool fFlag) noexcept { m_fEPSSurpriseNeedUpdate = fFlag; }
  bool CheckEPSSurpriseStatus(long lCurrentDate);
  bool IsEPSSurpriseNeedSave(void) noexcept { return m_fEPSSurpriseNeedSave; }
  void SetEPSSurpriseNeedSave(bool fFlag) noexcept { m_fEPSSurpriseNeedSave = fFlag; }
  bool IsEPSSurpriseNeedSaveAndClearFlag(void);

  bool IsPeerUpdated(void) noexcept { return m_fFinnhubPeerUpdated; }
  void SetPeerUpdated(bool fFlag) noexcept { m_fFinnhubPeerUpdated = fFlag; }
  bool CheckPeerStatus(long lCurrentDate);

  CString GetDescription(void) { return m_strDescription; }
  void SetDescription(CString strDescription) { m_strDescription = strDescription; }
  CString GetDisplaySymbol(void) { return m_strDisplaySymbol; }
  void SetDisplaySymbol(CString strDisplaySymbol) { m_strDisplaySymbol = strDisplaySymbol; }
  CString GetExchangeCode(void) { return m_strExchangeCode; }
  void SetExchangeCode(CString strExchangeCode) { m_strExchangeCode = strExchangeCode; }
  CString GetType(void) { return m_strType; }
  void SetType(CString strType) { m_strType = strType; }
  CString GetMic(void) { return m_strMic; }
  void SetMic(CString strMic) { m_strMic = strMic; }
  CString GetFigi(void) { return m_strFigi; }
  void SetFigi(CString strFigi) { m_strFigi = strFigi; }
  CString GetCurrency(void) { return m_strCurrency; }
  void SetCurrency(CString strCurrency) { m_strCurrency = strCurrency; }

  CString GetAddress(void) { return m_strAddress; }
  void SetAddress(CString strAddress) { m_strAddress = strAddress; }
  CString GetCity(void) { return m_strCity; }
  void SetCity(CString strCity) { m_strCity = strCity; }
  CString GetCountry(void) { return m_strCountry; }
  void SetCountry(CString strCountry) { m_strCountry = strCountry; }
  CString GetCusip(void) { return m_strCusip; }
  void SetCusip(CString strCusip) { m_strCusip = strCusip; }
  CString GetSedol(void) { return m_strSedol; }
  void SetSedol(CString strSedol) { m_strSedol = strSedol; }
  long GetEmployeeTotal(void) noexcept { return m_lEmployeeTotal; }
  void SetEmployeeTotal(long lEmployeeTotal) noexcept { m_lEmployeeTotal = lEmployeeTotal; }
  CString GetListedExchange(void) { return m_strListedExchange; }
  void SetListedExchange(CString strListedExchange) { m_strListedExchange = strListedExchange; }
  CString GetGgroup(void) { return m_strGgroup; }
  void SetGgroup(CString strGgroup) { m_strGgroup = strGgroup; }
  CString GetGind(void) { return m_strGind; }
  void SetGind(CString strGind) { m_strGind = strGind; }
  CString GetGsector(void) { return m_strGsector; }
  void SetGsector(CString strGsector) { m_strGsector = strGsector; }
  CString GetGsubind(void) { return m_strGsubind; }
  void SetGsubind(CString strGsubind) { m_strGsubind = strGsubind; }
  CString GetIPODate(void) { return m_strIPODate; }
  void SetIPODate(CString strIPODate) { m_strIPODate = strIPODate; }
  CString GetIsin(void) { return m_strIsin; }
  void SetIsin(CString strIsin) { m_strIsin = strIsin; }
  double GetMarketCapitalization(void) { return m_dMarketCapitalization; }
  void SetMarketCapitalization(double dMarketCapitalization) { m_dMarketCapitalization = dMarketCapitalization; }
  CString GetNaics(void) { return m_strNaics; }
  void SetNaics(CString strNaics) { m_strNaics = strNaics; }
  CString GetNaicsNationalIndustry(void) { return m_strNaicsNationalIndustry; }
  void SetNaicsNationalIndustry(CString strNaicsNationalIndustry) { m_strNaicsNationalIndustry = strNaicsNationalIndustry; }
  CString GetNaicsSector(void) { return m_strNaicsSector; }
  void SetNaicsSector(CString strNaicsSector) { m_strNaicsSector = strNaicsSector; }
  CString GetNaicsSubsector(void) { return m_strNaicsSubsector; }
  void SetNaicsSubsector(CString strNaicsSubsector) { m_strNaicsSubsector = strNaicsSubsector; }
  CString GetName(void) { return m_strName; }
  void SetName(CString strName) { m_strName = strName; }
  CString GetPhone(void) { return m_strPhone; }
  void SetPhone(CString strPhone) { m_strPhone = strPhone; }
  double GetShareOutstanding(void) noexcept { return m_dShareOutstanding; }
  void SetShareOutstanding(double dShareOutstanding) noexcept { m_dShareOutstanding = dShareOutstanding; }
  CString GetState(void) { return m_strState; }
  void SetState(CString strState) { m_strState = strState; }
  CString GetTicker(void) { return m_strTicker; }
  void SetTicker(CString strTicker) { m_strTicker = strTicker; }
  CString GetWebURL(void) { return m_strWebURL; }
  void SetWebURL(CString strWebURL) { m_strWebURL = strWebURL; }
  CString GetLogo(void) { return m_strLogo; }
  void SetLogo(CString strLogo) { m_strLogo = strLogo; }
  CString GetFinnhubIndustry(void) { return m_strFinnhubIndustry; }
  void SetFinnhubIndustry(CString strFinnhubIndustry) { m_strFinnhubIndustry = strFinnhubIndustry; }
  CString GetPeer(void) { return m_strPeer; }
  void SetPeer(CString strPeer) { m_strPeer = strPeer; }
  long GetProfileUpdateDate(void) noexcept { return m_lProfileUpdateDate; }
  void SetProfileUpdateDate(long lProfileUpdateDate) noexcept { m_lProfileUpdateDate = lProfileUpdateDate; }
  long GetLastRTDataUpdateDate(void) noexcept { return m_lLastRTDataUpdateDate; }
  void SetLastRTDataUpdateDate(long lDate) noexcept { m_lLastRTDataUpdateDate = lDate; }
  long GetPeerUpdateDate(void) noexcept { return m_lPeerUpdateDate; }
  void SetPeerUpdateDate(long lDate) noexcept { m_lPeerUpdateDate = lDate; }
  long GetLastEPSSurpriseUpdateDate(void) noexcept { return m_lLastEPSSurpriseUpdateDate; }
  void SetLastEPSSurpriseUpdateDate(long lDate) noexcept { m_lLastEPSSurpriseUpdateDate = lDate; }

  CString GetFinnhubDayLineInquiryString(time_t tCurrentTime);
  CString GetTiingoDayLineInquiryString(long lCurrentDate);

  bool IsUSMarket(void);

public:
  // Tiingo Symbol信息
  CString m_strTiingoPermaTicker; // Tiingo永久代码标识
  bool m_fIsActive; //
  bool m_fIsADR;
  INT32 m_iSICCode;
  CString m_strSICIndustry;
  CString m_strSICSector;
  CString m_strTiingoIndustry;
  CString m_strTiingoSector;
  CString m_strCompanyWebSite;
  CString m_strSECFilingWebSite;
  long m_lStatementUpdateDate;
  long m_lDailyDataUpdateDate;

  // 系统生成信息
  vector<CDayLinePtr> m_vDayLine;
  vector<CEPSSurprisePtr> m_vEPSSurprise;
  bool m_fEPSSurpriseNeedUpdate;
  atomic_bool m_fEPSSurpriseNeedSave;
  // 无需存储数据区
  bool m_fInquiryStockProfile; // 要求更新公司简介

protected:
  // Finnhub symbol信息
  CString m_strDescription;
  CString m_strDisplaySymbol;
  CString m_strExchangeCode; // 股票所属交易所代码，美国为“US”
  CString m_strType;
  CString m_strMic;
  CString m_strFigi;
  CString m_strCurrency;

  // Finnhub profile信息
  CString m_strAddress;
  CString m_strCity;
  CString m_strCountry;
  CString m_strCusip;
  CString m_strSedol;
  long m_lEmployeeTotal;
  CString m_strListedExchange;
  CString m_strGgroup;
  CString m_strGind;
  CString m_strGsector;
  CString m_strGsubind;
  CString m_strIPODate;
  CString m_strIsin;
  double m_dMarketCapitalization;
  CString m_strNaics;
  CString m_strNaicsNationalIndustry;
  CString m_strNaicsSector;
  CString m_strNaicsSubsector;
  CString m_strName;
  CString m_strPhone;
  double m_dShareOutstanding;
  CString m_strState;
  CString m_strTicker;
  CString m_strWebURL;
  CString m_strLogo;
  CString m_strFinnhubIndustry;
  CString m_strPeer;
  long m_lProfileUpdateDate; // 最新简介更新日期
  long m_lLastRTDataUpdateDate; // 最新实时数据更新日期
  long m_lPeerUpdateDate; // 最新实时数据更新日期
  long m_lLastEPSSurpriseUpdateDate; // 最新EPS Surprise更新日期

  // 无需存储数据区
  bool m_fFinnhubPeerUpdated; // 同业公司数据已更新
};

typedef shared_ptr<CWorldStock> CWorldStockPtr;