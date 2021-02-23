#pragma once

#include"stdafx.h"
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
  bool UpdateEPSSurpriseDB(void);

  void UpdateDayLine(vector<CDayLinePtr>& vDayLine);
  void UpdateEPSSurprise(vector<CEPSSurprisePtr>& vEPSSurprise);

  long GetLastRTDataUpdateDate(void) noexcept { return m_lLastRTDataUpdateDate; }
  void SetLastRTDataUpdateDate(long lDate) noexcept { m_lLastRTDataUpdateDate = lDate; }
  long GetPeerUpdateDate(void) noexcept { return m_lPeerUpdateDate; }
  void SetPeerUpdateDate(long lDate) noexcept { m_lPeerUpdateDate = lDate; }
  void SetLastEPSSurpriseUpdateDate(long lDate) noexcept { m_lLastEPSSurpriseUpdateDate = lDate; }
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

  bool CheckPeerStatus(long lCurrentDate);

  CString GetSymbol(void) { return m_strSymbol; }
  CString GetTicker(void) { return m_strTicker; }

  CString GetFinnhubDayLineInquiryString(time_t tCurrentTime);
  CString GetTiingoDayLineInquiryString(long lCurrentDate);

  // ʵʱ������
  time_t GetTransactionTime(void) noexcept { return m_TransactionTime; }
  void SetTransactionTime(time_t time) noexcept { m_TransactionTime = time; }
  long GetLastClose(void) noexcept { return m_lLastClose; }
  void SetLastClose(long lValue) noexcept { m_lLastClose = lValue; }
  long GetOpen(void) noexcept { return m_lOpen; }
  void SetOpen(long lValue) noexcept { m_lOpen = lValue; }
  long GetHigh(void) noexcept { return m_lHigh; }
  void SetHigh(long lValue) noexcept { m_lHigh = lValue; }
  long GetLow(void) noexcept { return m_lLow; }
  void SetLow(long lValue) noexcept { m_lLow = lValue; }
  long GetNew(void) noexcept { return m_lNew; }
  void SetNew(long lValue) noexcept { m_lNew = lValue; }
  INT64 GetAmount(void) noexcept { return m_llAmount; }
  void SetAmount(INT64 llValue) noexcept { m_llAmount = llValue; }
  INT64 GetVolume(void) noexcept { return m_llVolume; }
  void SetVolume(INT64 llValue) noexcept { m_llVolume = llValue; }
  long GetUpDown(void) noexcept { return m_lUpDown; }
  void SetUpDown(long lValue) noexcept { m_lUpDown = lValue; }
  double GetUpDownRate(void) noexcept { return m_dUpDownRate; }
  void SetUpDownRate(double dValue) noexcept { m_dUpDownRate = dValue; }
  double GetChangeHandRate(void) noexcept { return m_dChangeHandRate; }
  void SetChangeHandRate(double dValue) noexcept { m_dChangeHandRate = dValue; }
  void SetTotalValue(INT64 llValue) noexcept { m_llTotalValue = llValue; }
  INT64 GetTotalValue(void) noexcept { return m_llTotalValue; }
  void SetCurrentValue(INT64 llValue) noexcept { m_llCurrentValue = llValue; }
  INT64 GetCurrentValue(void) noexcept { return m_llCurrentValue; }


  bool IsUSMarket(void);

public:
  // Finnhub symbol��Ϣ
  CString m_strDescription;
  CString m_strDisplaySymbol;
  CString m_strSymbol;
  CString m_strExchangeCode; // ��Ʊ�������������룬����Ϊ��US��
  CString m_strType;
  CString m_strMic;
  CString m_strFigi;
  CString m_strCurrency;

  // Finnhub profile��Ϣ
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
  long m_lProfileUpdateDate; // ���¼���������
  long m_lLastRTDataUpdateDate; // ����ʵʱ���ݸ�������
  long m_lPeerUpdateDate; // ����ʵʱ���ݸ�������
  long m_lLastEPSSurpriseUpdateDate; // ����EPS Surprise��������

  // Tiingo Symbol��Ϣ
  CString m_strTiingoPermaTicker; // Tiingo���ô����ʶ
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

  // ϵͳ������Ϣ

  // ʵʱ������
  time_t m_TransactionTime; // ʵʱ���ݽ���ʱ��
  long m_lLastClose; // ��0.001Ԫ�Ƶ����̼�
  long m_lOpen; // ��0.001Ԫ�ƵĿ��̼�
  long m_lHigh; // ��0.001Ԫ�Ƶ���߼�
  long m_lLow; // ��0.001Ԫ�Ƶ���ͼ�
  long m_lNew; // ��0.001Ԫ�Ƶ����¼�
  long m_lUpDown; // �ǵ�ֵ
  double m_dUpDownRate; // �ǵ���
  INT64 m_llVolume;	// ��1�ɼƵĳɽ���
  INT64 m_llAmount; // ��Ԫ�Ƶĳɽ����
  double m_dChangeHandRate; // ������
  INT64 m_llTotalValue;	// ����ֵ����λ����Ԫ
  INT64 m_llCurrentValue;	// ��ͨ��ֵ����λ����Ԫ

  vector<CDayLinePtr> m_vDayLine;
  vector<CEPSSurprisePtr> m_vEPSSurprise;
  bool m_fEPSSurpriseNeedUpdate;
  atomic_bool m_fEPSSurpriseNeedSave;
  // ����洢������
  bool m_fInquiryStockProfile; // Ҫ����¹�˾���
  bool m_fFinnhubPeerUpdated; // ͬҵ��˾�����Ѹ���

protected:
  // ����洢������
};

typedef shared_ptr<CWorldStock> CWorldStockPtr;