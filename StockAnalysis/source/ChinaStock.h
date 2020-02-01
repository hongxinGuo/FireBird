#pragma once

// �ɽ��ľ����������Ϊ���֣��򣬽�������ǿ�򣬡����Ǽ�λΪ��һλ�ã����������λ�������������ҳɽ���λ������һ����������
// ǿ���λ�����������ҳɽ���λ���ٸ����������ж�����֮�����ơ�
enum {
  __ATTACK_BUY__ = 1,
  __STRONG_BUY__ = 2,
  __ORDINARY_BUY__ = 3,
  __UNKNOWN_BUYSELL__ = 4,
  __ORDINARY_SELL__ = 5,
  __STRONG_SELL__ = 6,
  __ATTACK_SELL__ = 7,
  __NO_TRANSACTION__ = 8
};

#include"SetDayLine.h"
#include"SetDayLineInfo.h"
#include"SetDayLineToday.h"
#include"SetRealTimeData.h"
#include"SetStockCode.h"

#include"RTData.h"

#include"DayLine.h"
#include"OneDeal.h"

#include"NeteaseDayLineWebInquiry.h"

using namespace std;
#include<vector>
#include<memory>
#include<queue>
#include<deque>
#include<map>
#include<array>
#include<mutex>

// ֤ȯ�������ݰ�
class CChinaStock final : public CObject {
public:
  CChinaStock(void);
  ~CChinaStock(void);
  void Reset(void);

public:
  // ���ݿ����
  void SaveBasicInfo(CSetDayLine& psetDayLine);
  void SaveTempInfo(CSetDayLineToday& setDayLineToday);

  void UpdateStatus(CRTDataPtr pRTData);

  WORD GetMarket(void) noexcept { return m_wMarket; }
  void SetMarket(WORD wValue) noexcept { m_wMarket = wValue; }
  CString GetStockCode(void) { return m_strStockCode; }
  void SetStockCode(CString str) { m_strStockCode = str; }
  CString GetStockName(void) { return m_strStockName; }
  void SetStockName(CString str) { m_strStockName = str; }

  long GetOffset(void) noexcept { return m_nOffsetInContainer; }
  void SetOffset(long lValue) noexcept { m_nOffsetInContainer = lValue; }
  long GetDayLineStartDay(void) noexcept { return m_lDayLineStartDay; }
  void SetDayLineStartDay(long lDay) noexcept { m_lDayLineStartDay = lDay; }
  long GetDayLineEndDay(void) noexcept { return m_lDayLineEndDay; }
  void SetDayLineEndDay(long lDay) noexcept { m_lDayLineEndDay = lDay; }
  long GetIPOStatus(void) noexcept { return m_lIPOed; }
  void SetIPOStatus(long lValue) noexcept { m_lIPOed = lValue; }

  // ����ʵʱ���ݣ���Ҫ����
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
  long GetHighLimit(void) noexcept { return m_lHighLimit; }
  void SetHighLimit(long lValue) noexcept { m_lHighLimit = lValue; }
  long GetLowLimit(void) noexcept { return m_lLowLimit; }
  void SetLowLimit(long lValue) noexcept { m_lLowLimit = lValue; }
  long GetPBuy(int iIndex) { return m_lPBuy.at(iIndex); }
  long GetVBuy(int iIndex) { return m_lVBuy.at(iIndex); }
  long GetPSell(int iIndex) { return m_lPSell.at(iIndex); }
  long GetVSell(int iIndex) { return m_lVSell.at(iIndex); }
  void SetPBuy(int iIndex, long value) { m_lPBuy.at(iIndex) = value; }
  void SetVBuy(int iIndex, long value) { m_lVBuy.at(iIndex) = value; }
  void SetPSell(int iIndex, long value) { m_lPSell.at(iIndex) = value; }
  void SetVSell(int iIndex, long value) { m_lVSell.at(iIndex) = value; }
  double GetRelativeStrong(void) noexcept { return m_dRelativeStrong; }
  void SetRelativeStrong(double value) noexcept { m_dRelativeStrong = value; }

  INT64 GetCurrentCancelBuyVolume(void) noexcept { return m_lCurrentCanselBuyVolume; }
  INT64 GetCurrentCancelSellVolume(void) noexcept { return m_lCurrentCanselSellVolume; }

  int  GetCurrentTransactionType(void) noexcept { return m_nCurrentTransactionType; }
  void SetCurrentTransactionType(int nType) noexcept { m_nCurrentTransactionType = nType; }
  INT64 GetCurrentTransationVolume(void) noexcept { return m_lCurrentGuadanTransactionVolume; }
  void SetCurrentTransationVolume(INT64 value) noexcept { m_lCurrentGuadanTransactionVolume = value; }

  INT64 GetAttackBuyAmount(void) noexcept { return m_lAttackBuyAmount; }
  INT64 GetAttackSellAmount(void) noexcept { return m_lAttackSellAmount; }
  INT64 GetOrdinaryBuyVolume(void) noexcept { return m_lOrdinaryBuyVolume; }
  INT64 GetOrdinarySellVolume(void) noexcept { return m_lOrdinarySellVolume; }
  INT64 GetAttackBuyVolume(void) noexcept { return m_lAttackBuyVolume; } // �������롣�ɽ��۸�����һ�۵����������ۡ����������������ǿ������
  INT64 GetStrongBuyVolume(void) noexcept { return m_lStrongBuyVolume; } // ����ǿ������,�ɽ��۳���֮ǰ����������
  INT64 GetCurrentAttackBuy(void) noexcept { return m_lCurrentAttackBuy; }
  INT64 GetCurrentStrongBuy(void) noexcept { return m_lCurrentStrongBuy; }
  INT64 GetAttackSellVolume(void) noexcept { return m_lAttackSellVolume; } // �����������ɽ��۵�����һ�۵���������ۡ�
  INT64 GetStrongSellVolume(void) noexcept { return m_lStrongSellVolume; }
  INT64 GetCurrentAttackSell(void) noexcept { return m_lCurrentAttackSell; }
  INT64 GetCurrentStrongSell(void) noexcept { return m_lCurrentStrongSell; }
  INT64 GetUnknownVolume(void) noexcept { return m_lUnknownVolume; }
  INT64 GetCurrentUnknown(void) noexcept { return m_lCurrentUnknown; }
  INT64 GetCancelBuyVolume(void) noexcept { return m_lCancelBuyVolume; }
  INT64 GetCancelSellVolume(void) noexcept { return m_lCancelSellVolume; }

  INT64 GetTransactionNumber(void) noexcept { return m_lTransactionNumber; }
  INT64 GetTransactionNumberBelow5000(void) noexcept { return m_lTransactionNumberBelow5000; }
  void IncreaseTransactionNumberBelow5000(INT64 value = 1) noexcept { m_lTransactionNumberBelow5000 += value; }
  INT64 GetTransactionNumberBelow50000(void) noexcept { return m_lTransactionNumberBelow50000; }
  void IncreaseTransactionNumberBelow50000(INT64 value = 1) noexcept { m_lTransactionNumberBelow50000 += value; }
  INT64 GetTransactionNumberBelow200000(void) noexcept { return m_lTransactionNumberBelow200000; }
  void IncreaseTransactionNumberBelow200000(INT64 value = 1) noexcept { m_lTransactionNumberBelow200000 += value; }
  INT64 GetTransactionNumberAbove200000(void) noexcept { return m_lTransactionNumberAbove200000; }
  void IncreaseTransactionNumberAbove200000(INT64 value = 1) noexcept { m_lTransactionNumberAbove200000 += value; }

  INT64 GetAttackBuyBelow50000(void) noexcept { return m_lAttackBuyBelow50000; }
  INT64 GetAttackBuyBelow200000(void) noexcept { return m_lAttackBuyBelow200000; }
  INT64 GetAttackBuyAbove200000(void) noexcept { return m_lAttackBuyAbove200000; }
  INT64 GetAttackSellBelow50000(void) noexcept { return m_lAttackSellBelow50000; }
  INT64 GetAttackSellBelow200000(void) noexcept { return m_lAttackSellBelow200000; }
  INT64 GetAttackSellAbove200000(void) noexcept { return m_lAttackSellAbove200000; }

  void SetAttackBuyAmount(INT64 value) noexcept { m_lAttackBuyAmount = value; }
  void SetAttackSellAmount(INT64 value) noexcept { m_lAttackSellAmount = value; }
  void SetOrdinaryBuyVolume(INT64 value) noexcept { m_lOrdinaryBuyVolume = value; }
  void IncreaseOrdinaryBuyVolume(INT64 value) noexcept { m_lOrdinaryBuyVolume += value; }
  void SetOrdinarySellVolume(INT64 value) noexcept { m_lOrdinarySellVolume = value; }
  void IncreaseOrdinarySellVolume(INT64 value) noexcept { m_lOrdinarySellVolume += value; }
  void SetAttackBuyVolume(INT64 value) noexcept { m_lAttackBuyVolume = value; } // �������롣�ɽ��۸�����һ�۵����������ۡ����������������ǿ������
  void IncreaseAttackBuyVolume(INT64 value) noexcept { m_lAttackBuyVolume += value; }
  void SetStrongBuyVolume(INT64 value) noexcept { m_lStrongBuyVolume = value; } // ����ǿ������,�ɽ��۳���֮ǰ����������
  void IncreaseStrongBuyVolume(INT64 value) noexcept { m_lStrongBuyVolume += value; }
  void SetCurrentAttackBuy(INT64 value) noexcept { m_lCurrentAttackBuy = value; }
  void SetCurrentStrongBuy(INT64 value) noexcept { m_lCurrentStrongBuy = value; }
  void SetAttackSellVolume(INT64 value) noexcept { m_lAttackSellVolume = value; } // �����������ɽ��۵�����һ�۵���������ۡ�
  void IncreaseAttackSellVolume(INT64 value) noexcept { m_lAttackSellVolume += value; }
  void SetStrongSellVolume(INT64 value) noexcept { m_lStrongSellVolume = value; }
  void IncreaseStrongSellVolume(INT64 value) noexcept { m_lStrongSellVolume += value; }
  void SetCurrentAttackSell(INT64 value) noexcept { m_lCurrentAttackSell = value; }
  void SetCurrentStrongSell(INT64 value) noexcept { m_lCurrentStrongSell = value; }
  void SetUnknownVolume(INT64 value) noexcept { m_lUnknownVolume = value; }
  void IncreaseUnknownVolume(INT64 value) noexcept { m_lUnknownVolume += value; }
  void SetCurrentUnknown(INT64 value) noexcept { m_lCurrentUnknown = value; }
  void SetCancelBuyVolume(INT64 value) noexcept { m_lCancelBuyVolume = value; }
  void IncreaseCancelBuyVolume(INT64 value) noexcept { m_lCancelBuyVolume += value; }
  void SetCancelSellVolume(INT64 value) noexcept { m_lCancelSellVolume = value; }
  void IncreaseCancelSellVolume(INT64 value) noexcept { m_lCancelSellVolume += value; }

  void SetTransactionNumber(INT64 value) noexcept { m_lTransactionNumber = value; }
  void SetTransactionNumberBelow5000(INT64 value) noexcept { m_lTransactionNumberBelow5000 = value; }
  void SetTransactionNumberBelow50000(INT64 value) noexcept { m_lTransactionNumberBelow50000 = value; }
  void SetTransactionNumberBelow200000(INT64 value) noexcept { m_lTransactionNumberBelow200000 = value; }
  void SetTransactionNumberAbove200000(INT64 value) noexcept { m_lTransactionNumberAbove200000 = value; }

  void SetAttackBuyBelow50000(INT64 value) noexcept { m_lAttackBuyBelow50000 = value; }
  void IncreaseAttackBuyBelow50000(INT64 value) noexcept { m_lAttackBuyBelow50000 += value; }
  void SetAttackBuyBelow200000(INT64 value) noexcept { m_lAttackBuyBelow200000 = value; }
  void IncreaseAttackBuyBelow200000(INT64 value) noexcept { m_lAttackBuyBelow200000 += value; }
  void SetAttackBuyAbove200000(INT64 value) noexcept { m_lAttackBuyAbove200000 = value; }
  void IncreaseAttackBuyAbove200000(INT64 value) noexcept { m_lAttackBuyAbove200000 += value; }
  void SetAttackSellBelow50000(INT64 value) noexcept { m_lAttackSellBelow50000 = value; }
  void IncreaseAttackSellBelow50000(INT64 value) noexcept { m_lAttackSellBelow50000 += value; }
  void SetAttackSellBelow200000(INT64 value) noexcept { m_lAttackSellBelow200000 = value; }
  void IncreaseAttackSellBelow200000(INT64 value) noexcept { m_lAttackSellBelow200000 += value; }
  void SetAttackSellAbove200000(INT64 value) noexcept { m_lAttackSellAbove200000 = value; }
  void IncreaseAttackSellAbove200000(INT64 value) noexcept { m_lAttackSellAbove200000 += value; }

  // ���µ�ǰ������״̬
  void SetLastSavedVolume(INT64 llVolume) noexcept { m_llLastSavedVolume = llVolume; }
  INT64 GetLastSavedVolume(void) noexcept { return m_llLastSavedVolume; }

  // ����״̬��ʶ��ȡ������
  bool IsActive(void) noexcept { return m_fActive; }
  void SetActive(bool fFlag) noexcept { m_fActive = fFlag; }
  bool IsChoiced(void) noexcept { return m_fChoiced; }
  void SetChoiced(bool fChoiced) noexcept { m_fChoiced = fChoiced; }
  bool IsMinLineUpdated(void) noexcept { return (m_fMinLineUpdated); }
  void SetMinLineUpdated(bool fUpdate) noexcept { m_fMinLineUpdated = fUpdate; }
  bool IsDayLineUpdated(void) noexcept { return (m_fDayLineUpdated); }
  void SetDayLineUpdated(bool fUpdate) noexcept { m_fDayLineUpdated = fUpdate; }
  bool IsDayLineDBUpdated(void) noexcept { return (m_fDayLineDBUpdated); }
  void SetDayLineDBUpdated(bool fUpdate) noexcept { m_fDayLineDBUpdated = fUpdate; }
  bool IsDayLineLoaded(void) noexcept { return m_fDayLineLoaded; }
  void SetDayLineLoaded(bool fFlag) noexcept { m_fDayLineLoaded = fFlag; }
  // ��һ��ʵʱ�����жϺ�����
  bool HaveFirstRTData(void) noexcept { return m_fHaveFirstRTData; }
  bool SetHavingFirstRTData(bool fFlag) noexcept { if (m_fHaveFirstRTData || !fFlag) return false; m_fHaveFirstRTData = fFlag; return true; }
  void SetNeedProcessRTData(bool fFlag) noexcept { m_fNeedProcessRTData = fFlag; }
  bool IsNeedProcessRTData(void) noexcept { return m_fNeedProcessRTData; }

  bool IsTodayDataActive(void); //������߼ۡ���ͼۡ��ɽ����ͳɽ������жϣ������Ϊ�㣬����Ϊ�˹ɽ���û����Ч���ݡ���Ȼ��m_fActiveΪ��״̬�¡�
  bool IsTodayDataChanged(void); // �����߼ۡ���ͼۡ��ɽ����ͳɽ����������ݲ�Ϊ�㣬�򷵻��档

  // ���ڴ���������ʷ���ݵĺ���λ�ڲ�ͬ���߳��У��ʶ���Ҫͬ���������ñ�ʶ
  bool IsDayLineNeedUpdate(void) noexcept { return m_fDayLineNeedUpdate; }
  void SetDayLineNeedUpdate(bool fFlag);
  bool IsDayLineNeedProcess(void) noexcept { return m_fDayLineNeedProcess; }
  void SetDayLineNeedProcess(bool fFlag);
  bool IsDayLineNeedSaving(void) noexcept { return m_fDayLineNeedSaving; }
  void SetDayLineNeedSaving(bool fFlag);
  bool IsDayLineNeedSavingAndClearFlag(void);

  bool TransferNeteaseDayLineWebDataToBuffer(CNeteaseDayLineWebInquiry* pNeteaseWebDayLineData);
  bool ProcessNeteaseDayLineData(void);
  bool SkipNeteaseDayLineInformationHeader(void);
  void SetTodayActive(WORD wMarket, CString strStockCode, CString strStockName);
  void StoreDayLine(vector<CDayLinePtr>& vTempDayLine);
  void ReportDayLineDownLoaded(void);
  void IncreaseCurrentPos(INT64 lValue = 1) noexcept { m_llCurrentPos += lValue; m_pCurrentPos += lValue; }
  void ResetCurrentPos(void) noexcept { m_pCurrentPos = m_pDayLineBuffer; m_llCurrentPos = 0; }

  // ���ݿ����ȡ�ʹ洢
  bool SaveDayLine(void);
  void UpdateDayLineStartEndDay(void);
  void SaveCalculatedInfo(CSetDayLineInfo& setDayLineInfo);
  void LoadAndCalculateTempInfo(CSetDayLineToday& setDayLineToday);
  // ����װ�غ������ɹ����߳�ThreadLoadDayLine����
  bool LoadDayLineAndDayLineInfo(void); // �˺�������
  bool LoadDayLine(CSetDayLine& setDayLine);
  bool LoadDayLineInfo(CSetDayLineInfo& setDayLine);
  void SaveStockCodeDB(CSetStockCode& setStockCode);
  void AppendStockCodeDB(CSetStockCode& setStockCode);
  bool LoadStockCodeDB(CSetStockCode& setStockCode);
  void SetCheckingDayLineStatus(void);

  // �ҵ����
  double GetCurrentGuadanTransactionPrice(void) noexcept { return m_dCurrentGuadanTransactionPrice; }
  void SetCurrentGuadanTransactionPrice(double dValue) noexcept { m_dCurrentGuadanTransactionPrice = dValue; }
  INT64 GetGuadan(INT64 lPrice) { return m_mapGuadan.at(lPrice); }
  void SetGuadan(INT64 lPrice, INT64 lVolume) noexcept { m_mapGuadan[lPrice] = lVolume; }
  bool HaveGuadan(INT64 lPrice);

  // �������ǿ�ȼ���
  bool CalculateDayLineRS(void);
  bool CalculateDayLineRS(INT64 lNumber);

  // ����ʵʱ���ݸ�����, �ɹ����߳�ThreadCalculateRTData����
  bool ProcessRTData(void);
  bool ProcessOneRTData(CRTDataPtr pRTData);
  void CalculateOneDeal(CRTDataPtr pRTData, INT64 lCurrentGuadanTransactionPrice);
  void IncreaseTransactionNumber(void);
  void CalculateOneRTData(CRTDataPtr pRTData);
  void CalculateOrdinaryBuySell(INT64 lCurrentGuadanTransactionPrice);
  void CalculateAttackBuy(void);
  void CalculateStrongBuy(void);
  void CalculateAttackBuyVolume(void);
  void CalculateAttackSell(void);
  void CalculateStrongSell(void);
  void CalculateAttackSellVolume(void);
  void SetLastRTData(CRTDataPtr pLastRTData) noexcept { m_pLastRTData = pLastRTData; }
  CRTDataPtr GetLastRTData(void) noexcept { return m_pLastRTData; }
  void InitializeCalculatingRTDataEnvionment(CRTDataPtr pRTData);

  bool AnalysisGuadan(CRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice);
  void SelectGuadanThatNeedToCalculate(CRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice, array<bool, 10>& fNeedCheck);
  void SetCurrentGuadan(CRTDataPtr pCurrentRTData);
  void CheckGuadan(CRTDataPtr pCurrentRTData, array<bool, 10>& fNeedCheck);
  void CheckSellGuadan(array<bool, 10>& fNeedCheck, int i);
  void CheckBuyGuadan(array<bool, 10>& fNeedCheck, int i);
  bool CheckCurrentRTData();
  void ShowCurrentTransaction(void);
  void ShowCurrentInformationofCancelingGuadan(void);
  void ReportGuadanTransaction(void);
  void ReportGuadan(void);

  void PushRTData(CRTDataPtr pData);
  CRTDataPtr PopRTData(void);
  CRTDataPtr GetRTDataAtHead(void); // �����������������
  INT64 GetRTDataQueueSize(void);
  // ��մ洢ʵʱ���ݵĶ���
  void ClearRTDataDeque(void);

  // ������ʷ����
  size_t GetDayLineSize(void) { return m_vDayLine.size(); }
  bool HaveNewDayLineData(void);
  void ClearDayLineContainer(void) noexcept { m_vDayLine.clear(); }
  bool StoreDayLine(CDayLinePtr pDayLine) noexcept { m_vDayLine.push_back(pDayLine); return true; }
  CDayLinePtr GetDayLine(long lIndex) { return m_vDayLine.at(lIndex); }
  void ShowDayLine(CDC* pDC, CRect rectClient);
  bool RSLineTo(CDC* pDC, CRect rectClient, int i, double dValue);
  void ShowDayLineRS(CDC* pDC, CRect rectClient);
  void ShowDayLine3RS(CDC* pDC, CRect rectClient);
  void ShowDayLine5RS(CDC* pDC, CRect rectClient);
  void ShowDayLine10RS(CDC* pDC, CRect rectClient);
  void ShowDayLine30RS(CDC* pDC, CRect rectClient);
  void ShowDayLine60RS(CDC* pDC, CRect rectClient);
  void ShowDayLine120RS(CDC* pDC, CRect rectClient);

  INT64 GetCurrentPos(void) noexcept { return m_llCurrentPos; }
  char* GetCurrentPosPtr(void) noexcept { return m_pCurrentPos; }
  INT64 GetDayLineBufferLength(void) noexcept { return m_lDayLineBufferLength; }
  char* GetDayLineBufferPtr(void) noexcept { return m_pDayLineBuffer; }

  bool IsVolumeConsistence(void) noexcept;

#ifdef _DEBUG
  virtual	void AssertValid() const;
  virtual	void Dump(CDumpContext& dc) const;
#endif

public:
  // ����ר�ú���
  void __TestSetGuadanDeque(INT64 lPrice, INT64 lVolume) { m_mapGuadan[lPrice] = lVolume; } // Ԥ�����ùҵ���
  void __TestSetDayLineBuffer(INT64 lBufferLength, char* pDayLineBuffer);

public:

protected:
  WORD m_wMarket;	// 1���Ϻ��г���������ϸ�ࣩ��2�������г���������ϸ�ࣩ��3���Ϻ�ָ����4������ָ�������Ϻ����棻6��������С�壻
                // 7���Ϻ�B�ɣ�8������B�ɣ�9���Ϻ��ƴ���; 10�����ڴ�ҵ�壻
                // �Ϻ��г����õ�����ʾ�������г�����˫����ʾ��Ŀǰ��ʱ���ã��Ȳ鿴�漰����Щ������Ҫ�޸ġ�
  CString m_strStockCode; // ��Ʊ���롣��λ��ǰ��λΪ�г�ǰ׺������λΪ���ִ��롣��sh600601��sz000001
  CString m_strStockName; // ��Ʊ����
  CStringW m_strStockNameReadIn; // ����Ĺ�Ʊ���ƣ�UniCode��ʽ��Ŀǰ��δʹ�ã�
  long m_nOffsetInContainer;	// �������е�ƫ����
  long m_lDayLineStartDay;	// ����������ʼ�ա�����Ǵ���������ʷ����ʱ�õ�����ʼ�����գ�
  long m_lDayLineEndDay;	// �������ݸ����ա�����Ǵ���������ʷ����ʱ�õ��������գ�
  long m_lIPOed; // ͨ��������ʷ���߲�ѯ�����ֻ��ǰ׺��Ϣ��û��ʵ�����ݣ�����ȷ��û��ʵ�ʽ��ס�����������£�����ʵʱ���������ݣ�ֻ��Ϊ����ѡ�Ĭ�������Ϊ������
                 // δ���У���Ч��Ʊ���룩Ϊ__STOCK_NULL__������Ϊ__STOCK_IPOED__����ͨ��IPO����δ���л�����Ϊ__STOCK_DELISTED�����������δ���֣������Ժ���
  short	m_nHand;	// ÿ�ֹ���

  // ʵʱ����,��Ҫ���ϸ���
  time_t m_TransactionTime; // ʵʱ���ݽ���ʱ��
  long m_lLastClose; // ��0.001Ԫ�Ƶ����̼�
  long m_lOpen; // ��0.001Ԫ�ƵĿ��̼�
  long m_lHigh; // ��0.001Ԫ�Ƶ���߼�
  long m_lLow; // ��0.001Ԫ�Ƶ���ͼ�
  long m_lNew; // ��0.001Ԫ�Ƶ����¼�
  long m_lHighLimit; // ��ͣ�ۡ���������Ŀǰֻ����Ѷʵʱ�����ܹ��ṩ��
  long m_lLowLimit; // ��ͣ�ۡ���������Ŀǰֻ����Ѷʵʱ�����ܹ��ṩ��
  long m_lUpDown; // �ǵ�ֵ
  double m_dUpDownRate; // �ǵ���
  INT64 m_llVolume;	// ��1�ɼƵĳɽ���
  INT64 m_llAmount; // ��Ԫ�Ƶĳɽ����
  double m_dChangeHandRate; // ������
  INT64 m_llTotalValue;	// ����ֵ����λ����Ԫ
  INT64 m_llCurrentValue;	// ��ͨ��ֵ����λ����Ԫ
  array<long, 5> m_lPBuy;	// ���̼ۡ���λ��0.001Ԫ
  array<long, 5> m_lVBuy;	// ����������λ����
  array<long, 5> m_lPSell; // ���̼ۡ���λ��0.001Ԫ
  array<long, 5> m_lVSell; // ����������λ����
  double m_dRelativeStrong; // ��λ��1%

  // ���±������ڷ������������������
  INT64 m_lAttackBuyAmount; // ����������
  INT64 m_lAttackSellAmount; // �����������
  INT64 m_lCurrentAttackBuy;
  INT64 m_lCurrentStrongBuy;
  INT64 m_lCurrentAttackSell;
  INT64 m_lCurrentStrongSell;

  // ����Ϊ��洢��
  INT64 m_lTransactionNumber; // �������յĳɽ�����
  INT64 m_lTransactionNumberBelow5000; // �������յ���5000�ɵĳɽ�����
  INT64 m_lTransactionNumberBelow50000; //
  INT64 m_lTransactionNumberBelow200000; //
  INT64 m_lTransactionNumberAbove200000; //

  INT64 m_lOrdinaryBuyVolume;
  INT64 m_lOrdinarySellVolume;
  INT64 m_lAttackBuyVolume;	// �������롣�ɽ��۸�����һ�۵����������ۡ����������������ǿ������
  INT64 m_lStrongBuyVolume;	// ����ǿ������,�ɽ��۳���֮ǰ����������
  INT64 m_lAttackSellVolume; // �����������ɽ��۵�����һ�۵���������ۡ�
  INT64 m_lStrongSellVolume; // ����ǿ������,�ɽ��۵���֮ǰ���������
  INT64 m_lUnknownVolume; // ����״̬�ĳɽ���
  INT64 m_lCurrentUnknown; // ��ǰ�ɽ����еĲ���״̬�ɽ���
  INT64 m_lCancelBuyVolume;	// �򵥳�����
  INT64 m_lCancelSellVolume; // ����������

  INT64 m_lAttackBuyBelow50000;
  INT64 m_lAttackBuyBelow200000;
  INT64 m_lAttackBuyAbove200000;
  INT64 m_lAttackSellBelow50000;
  INT64 m_lAttackSellBelow200000;
  INT64 m_lAttackSellAbove200000;

  INT64 m_llLastSavedVolume;

  atomic_bool m_fDayLineNeedUpdate; // ������Ҫ���¡�Ĭ��Ϊ��
  atomic_bool m_fDayLineNeedProcess; // �Ѵ������϶�ȡ��������ʷ���ݣ��ȴ�����
  atomic_bool m_fDayLineNeedSaving; // ������ʷ�����Ѵ����ȴ��洢��
  atomic_bool m_fDayLineLoaded; // �Ƿ�װ������������

  bool m_fActive;	// �Ƿ����������ݶ��롣������ʵʱ���鴦����������������ʷ���ݴ����������á�

  bool m_fHaveFirstRTData; // ʵʱ���ݿ�ʼ�����ʶ����һ��ʵʱ����ֻ��������ʼ��ϵͳ���������ڼ��㡣�ӵڶ������ݿ�ʼ�������Ч��
  bool m_fNeedProcessRTData; //ָ�����Ʊ������㽻�׺͹ҵ����

  bool m_fChoiced;// �˹�Ʊ�Ƿ�����ѡ��Ʊ.
  bool m_fMinLineUpdated; // ����ķ��������Ƿ���¹�.
  bool m_fDayLineUpdated; // ��������������Ƿ���¹�.

  bool m_fDayLineDBUpdated; // ������ʷ���ݿ���±�ʶ

  // �ҵ��ľ��������
  map<INT64, INT64> m_mapGuadan;// ����map�ṹ�洢�ҵ��ľ������������Ϊ��λ������Ϊ�ҵ�����
  CRTDataPtr m_pLastRTData; // ��m_qRTData��������һ��ʵʱ���ݡ�
  INT64 m_lCurrentGuadanTransactionVolume; // ��ǰ�ҵ�������������Ŀǰʱ��Ľ�����������ʵʱ���ݶ�����ǰ�����ݵ�ʱ��Ľ�������
  double m_dCurrentGuadanTransactionPrice; // ��ǰ�ɽ��۸�
  int m_nCurrentTransactionType; // ��ǰ�������ͣ�ǿ�򡢽��������롣��������
  INT64 m_lCurrentCanselSellVolume;
  INT64 m_lCurrentCanselBuyVolume;

  queue<COneDealPtr> m_qDeal; // ����ɽ���Ϣ���У�Ŀǰ��δʹ�ã���

  queue<CRTDataPtr> m_qRTData; // ʵʱ���ݶ��С�
  CCriticalSection m_RTDataLock; // ʵʱ���ݶ��е�ͬ����

  // ������ʷ����
  vector<CDayLinePtr>	m_vDayLine; // ������������
  vector<char> m_vDayLineBuffer; // ���߶�ȡ������
  char* m_pDayLineBuffer; // ���߶�ȡ������
  INT64 m_lDayLineBufferLength;
  char* m_pCurrentPos;
  INT64 m_llCurrentPos;

private:
  bool m_fLoadDayLineFirst; // �����á���ֹDayLine���DayLineInfo��װ��������
};

typedef shared_ptr<CChinaStock> CChinaStockPtr;
