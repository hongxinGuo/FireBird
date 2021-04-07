#pragma once
#include"globeSymbolDef.h"

#include"VirtualStock.h"

#include"DayLine.h"
#include"EPSSurprise.h"
#include"TiingoStock.h"
#include"SetWorldStock.h"

using namespace std;
#include<memory>
#include<atomic>
#include<vector>

class CWorldStock : public CVirtualStock {
public:
  CWorldStock();
  virtual ~CWorldStock();
  virtual void Reset(void) override;
  virtual int GetRatio(void) const noexcept override { return s_iRatio; }

public:
  void Load(CSetWorldStock& setWorldStock);
  bool CheckProfileUpdateStatus(long lTodayDate);
  bool CheckDayLineUpdateStatus(long lTodayDate, long lLastTradeDate, long lTime, long lDayOfWeek);
  void Save(CSetWorldStock& setWorldStock);
  void Update(CSetWorldStock& setWorldStock);
  void Append(CSetWorldStock& setWorldStock);
  void SaveDayLine(void);
  virtual bool UpdateEPSSurpriseDB(void);

  void UpdateDayLine(vector<CDayLinePtr>& vDayLine);
  void UpdateEPSSurprise(vector<CEPSSurprisePtr>& vEPSSurprise);

  void UpdateStockProfile(CTiingoStockPtr pTiingoStock);

  void UpdateDayLineStartEndDate(void);
  long GetDayLineSize(void) const noexcept { return static_cast<long>(m_vDayLine.size()); }
  CDayLinePtr GetDayLine(long lIndex) const { return m_vDayLine.at(lIndex); }
  void UnloadDayLine(void) { m_vDayLine.resize(0); }

  bool HaveNewDayLineData(void);

  bool IsProfileUpdated(void) const noexcept { return m_fProfileUpdated; }
  void SetProfileUpdated(bool fFlag) noexcept { m_fProfileUpdated = fFlag; }

  bool IsEPSSurpriseUpdated(void) const noexcept { return m_fEPSSurpriseUpdated; }
  void SetEPSSurpriseUpdated(bool fFlag) noexcept { m_fEPSSurpriseUpdated = fFlag; }
  bool CheckEPSSurpriseStatus(long lCurrentDate);
  bool IsEPSSurpriseNeedSave(void) const noexcept { return m_fEPSSurpriseNeedSave; }
  void SetEPSSurpriseNeedSave(bool fFlag) noexcept { m_fEPSSurpriseNeedSave = fFlag; }
  bool IsEPSSurpriseNeedSaveAndClearFlag(void);

  bool IsPeerUpdated(void) const noexcept { return m_fFinnhubPeerUpdated; }
  void SetPeerUpdated(bool fFlag) noexcept { m_fFinnhubPeerUpdated = fFlag; }
  bool CheckPeerStatus(long lCurrentDate);

  CString GetDescription(void) const { return m_strDescription; }
  void SetDescription(CString strDescription) { m_strDescription = strDescription; }
  CString GetDisplaySymbol(void) const { return m_strDisplaySymbol; }
  void SetDisplaySymbol(CString strDisplaySymbol) { m_strDisplaySymbol = strDisplaySymbol; }
  CString GetType(void) const { return m_strType; }
  void SetType(CString strType) { m_strType = strType; }
  CString GetMic(void) const { return m_strMic; }
  void SetMic(CString strMic) { m_strMic = strMic; }
  CString GetFigi(void) const { return m_strFigi; }
  void SetFigi(CString strFigi) { m_strFigi = strFigi; }
  CString GetCurrency(void) const { return m_strCurrency; }
  void SetCurrency(CString strCurrency) { m_strCurrency = strCurrency; }

  CString GetAddress(void) const { return m_strAddress; }
  void SetAddress(CString strAddress) { m_strAddress = strAddress; }
  CString GetCity(void) const { return m_strCity; }
  void SetCity(CString strCity) { m_strCity = strCity; }
  CString GetCountry(void) const { return m_strCountry; }
  void SetCountry(CString strCountry) { m_strCountry = strCountry; }
  CString GetCusip(void) const { return m_strCusip; }
  void SetCusip(CString strCusip) { m_strCusip = strCusip; }
  CString GetSedol(void) const { return m_strSedol; }
  void SetSedol(CString strSedol) { m_strSedol = strSedol; }
  long GetEmployeeTotal(void) const noexcept { return m_lEmployeeTotal; }
  void SetEmployeeTotal(long lEmployeeTotal) noexcept { m_lEmployeeTotal = lEmployeeTotal; }
  CString GetListedExchange(void) const { return m_strListedExchange; }
  void SetListedExchange(CString strListedExchange) { m_strListedExchange = strListedExchange; }
  CString GetGgroup(void) const { return m_strGgroup; }
  void SetGgroup(CString strGgroup) { m_strGgroup = strGgroup; }
  CString GetGind(void) const { return m_strGind; }
  void SetGind(CString strGind) { m_strGind = strGind; }
  CString GetGsector(void) const { return m_strGsector; }
  void SetGsector(CString strGsector) { m_strGsector = strGsector; }
  CString GetGsubind(void) const { return m_strGsubind; }
  void SetGsubind(CString strGsubind) { m_strGsubind = strGsubind; }
  CString GetIPODate(void) const { return m_strIPODate; }
  void SetIPODate(CString strIPODate) { m_strIPODate = strIPODate; }
  CString GetIsin(void) const { return m_strIsin; }
  void SetIsin(CString strIsin) { m_strIsin = strIsin; }
  double GetMarketCapitalization(void) const noexcept { return m_dMarketCapitalization; }
  void SetMarketCapitalization(double dMarketCapitalization) noexcept { m_dMarketCapitalization = dMarketCapitalization; }
  CString GetNaics(void) const { return m_strNaics; }
  void SetNaics(CString strNaics) { m_strNaics = strNaics; }
  CString GetNaicsNationalIndustry(void) const { return m_strNaicsNationalIndustry; }
  void SetNaicsNationalIndustry(CString strNaicsNationalIndustry) { m_strNaicsNationalIndustry = strNaicsNationalIndustry; }
  CString GetNaicsSector(void) const { return m_strNaicsSector; }
  void SetNaicsSector(CString strNaicsSector) { m_strNaicsSector = strNaicsSector; }
  CString GetNaicsSubsector(void) const { return m_strNaicsSubsector; }
  void SetNaicsSubsector(CString strNaicsSubsector) { m_strNaicsSubsector = strNaicsSubsector; }
  CString GetName(void) const { return m_strName; }
  void SetName(CString strName) { m_strName = strName; }
  CString GetPhone(void) const { return m_strPhone; }
  void SetPhone(CString strPhone) { m_strPhone = strPhone; }
  double GetShareOutstanding(void) const noexcept { return m_dShareOutstanding; }
  void SetShareOutstanding(double dShareOutstanding) noexcept { m_dShareOutstanding = dShareOutstanding; }
  CString GetState(void) const { return m_strState; }
  void SetState(CString strState) { m_strState = strState; }
  CString GetTicker(void) const { return m_strTicker; }
  void SetTicker(CString strTicker) { m_strTicker = strTicker; }
  CString GetWebURL(void) const { return m_strWebURL; }
  void SetWebURL(CString strWebURL) { m_strWebURL = strWebURL; }
  CString GetLogo(void) const { return m_strLogo; }
  void SetLogo(CString strLogo) { m_strLogo = strLogo; }
  CString GetFinnhubIndustry(void) const { return m_strFinnhubIndustry; }
  void SetFinnhubIndustry(CString strFinnhubIndustry) { m_strFinnhubIndustry = strFinnhubIndustry; }
  CString GetPeer(void) const { return m_strPeer; }
  void SetPeer(CString strPeer) { m_strPeer = strPeer; }
  long GetProfileUpdateDate(void) const noexcept { return m_lProfileUpdateDate; }
  void SetProfileUpdateDate(long lProfileUpdateDate) noexcept { m_lProfileUpdateDate = lProfileUpdateDate; }
  long GetLastRTDataUpdateDate(void) const noexcept { return m_lLastRTDataUpdateDate; }
  void SetLastRTDataUpdateDate(long lDate) noexcept { m_lLastRTDataUpdateDate = lDate; }
  long GetPeerUpdateDate(void) const noexcept { return m_lPeerUpdateDate; }
  void SetPeerUpdateDate(long lDate) noexcept { m_lPeerUpdateDate = lDate; }
  long GetLastEPSSurpriseUpdateDate(void) const noexcept { return m_lLastEPSSurpriseUpdateDate; }
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
  bool m_fEPSSurpriseUpdated;
  atomic_bool m_fEPSSurpriseNeedSave;

protected:
  // Finnhub symbol信息
  CString m_strDescription;
  CString m_strDisplaySymbol;
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
  bool m_fProfileUpdated; // 公司简介已更新
  bool m_fFinnhubPeerUpdated; // 同业公司数据已更新

private:
  static int s_iRatio;
};

typedef shared_ptr<CWorldStock> CWorldStockPtr;