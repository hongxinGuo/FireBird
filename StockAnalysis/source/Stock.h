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
#include"SetDayLineToday.h"
#include"SetRealTimeData.h"
#include"SetStockCode.h"

#include"RTData.h"
#include"DayLine.h"
#include"OneDeal.h"

#include"StockBasicInfo.h"
#include"StockCalculatedInfo.h"

using namespace std;
#include<vector>
#include<memory>
#include<queue>
#include<deque>
#include<map>
#include<array>

// ֤ȯ�������ݰ�
class CStock final : public CObject {
public:
  CStock(void);
  ~CStock(void);
  void Reset(void);

public:
  // ������Ϣ��ȡ������
  time_t GetTransactionTime(void) { return m_stockBasicInfo.GetTransactionTime(); }
  void SetTransactionTime(time_t time) { m_stockBasicInfo.SetTransactionTime(time); m_stockCalculatedInfo.SetTransactionTime(time); }
  WORD GetMarket(void) { return m_stockBasicInfo.GetMarket(); }
  void SetMarket(WORD wValue) { m_stockBasicInfo.SetMarket(wValue); m_stockCalculatedInfo.SetMarket(wValue); }
  CString GetStockCode(void) { return m_stockBasicInfo.GetStockCode(); }
  void SetStockCode(CString str) { m_stockBasicInfo.SetStockCode(str); m_stockCalculatedInfo.SetStockCode(str); }
  CString GetStockName(void) { return m_stockBasicInfo.GetStockName(); }
  void SetStockName(CString str) { m_stockBasicInfo.SetStockName(str); }
  long GetOffset(void) { return m_stockBasicInfo.GetOffset(); }
  void SetOffset(long lValue) { m_stockBasicInfo.SetOffset(lValue); }
  long GetDayLineStartDay(void) { return m_stockBasicInfo.GetDayLineStartDay(); }
  void SetDayLineStartDay(long lDay) { m_stockBasicInfo.SetDayLineStartDay(lDay); }
  long GetDayLineEndDay(void) { return m_stockBasicInfo.GetDayLineEndDay(); }
  void SetDayLineEndDay(long lDay) { m_stockBasicInfo.SetDayLineEndDay(lDay); }
  long GetIPOStatus(void) { return m_stockBasicInfo.GetIPOStatus(); }
  void SetIPOStatus(long lValue) { m_stockBasicInfo.SetIPOStatus(lValue); }
  void SetLastClose(long lValue) { m_stockBasicInfo.SetLastClose(lValue); }
  long GetLastClose(void) { return m_stockBasicInfo.GetLastClose(); }
  long GetOpen(void) { return m_stockBasicInfo.GetOpen(); }
  void SetOpen(long lValue) { m_stockBasicInfo.SetOpen(lValue); }
  long GetHigh(void) { return m_stockBasicInfo.GetHigh(); }
  void SetHigh(long lValue) { m_stockBasicInfo.SetHigh(lValue); }
  long GetLow(void) { return m_stockBasicInfo.GetLow(); }
  void SetLow(long lValue) { m_stockBasicInfo.SetLow(lValue); }
  long GetNew(void) { return m_stockBasicInfo.GetNew(); }
  void SetNew(long lValue) { m_stockBasicInfo.SetNew(lValue); }
  INT64 GetAmount(void) { return m_stockBasicInfo.GetAmount(); }
  void SetAmount(INT64 llValue) { m_stockBasicInfo.SetAmount(llValue); }
  INT64 GetVolume(void) { return m_stockBasicInfo.GetVolume(); }
  void SetVolume(INT64 llValue) { m_stockBasicInfo.SetVolume(llValue); }
  long GetUpDown(void) { return m_stockBasicInfo.GetUpDown(); }
  void SetUpDown(long lValue) { m_stockBasicInfo.SetUpDown(lValue); }
  double GetUpDownRate(void) { return m_stockBasicInfo.GetUpDownRate(); }
  void SetUpDownRate(double dValue) { m_stockBasicInfo.SetUpDownRate(dValue); }
  void SetTotalValue(INT64 llValue) { m_stockBasicInfo.SetTotalValue(llValue); }
  INT64 GetTotalValue(void) { return m_stockBasicInfo.GetTotalValue(); }
  void SetCurrentValue(INT64 llValue) { m_stockBasicInfo.SetCurrentValue(llValue); }
  INT64 GetCurrentValue(void) { return m_stockBasicInfo.GetCurrentValue(); }
  long GetPBuy(int iIndex) { return m_stockBasicInfo.GetPBuy(iIndex); }
  long GetVBuy(int iIndex) { return m_stockBasicInfo.GetVBuy(iIndex); }
  long GetPSell(int iIndex) { return m_stockBasicInfo.GetPSell(iIndex); }
  long GetVSell(int iIndex) { return m_stockBasicInfo.GetVSell(iIndex); }
  void SetPBuy(int iIndex, long value) { m_stockBasicInfo.SetPBuy(iIndex, value); }
  void SetVBuy(int iIndex, long value) { m_stockBasicInfo.SetVBuy(iIndex, value); }
  void SetPSell(int iIndex, long value) { m_stockBasicInfo.SetPSell(iIndex, value); }
  void SetVSell(int iIndex, long value) { m_stockBasicInfo.SetVSell(iIndex, value); }
  double GetRelativeStrong(void) { return m_stockBasicInfo.GetRelativeStrong(); }
  void SetRelativeStrong(double value) { m_stockBasicInfo.SetRelativeStrong(value); }

  // ���������Ϣ��ȡ������
  INT64 GetAttackBuyAmount(void) { return m_stockCalculatedInfo.GetAttackBuyAmount(); }
  void SetAttackBuyAmount(INT64 value) { m_stockCalculatedInfo.SetAttackBuyAmount(value); }
  INT64 GetAttackSellAmount(void) { return m_stockCalculatedInfo.GetAttackSellAmount(); }
  void SetAttackSellAmount(INT64 value) { m_stockCalculatedInfo.SetAttackSellAmount(value); }
  INT64 GetOrdinaryBuyVolume(void) { return m_stockCalculatedInfo.GetOrdinaryBuyVolume(); }
  void SetOrdinaryBuyVolume(INT64 value) { m_stockCalculatedInfo.SetOrdinaryBuyVolume(value); }
  void IncreaseOrdinaryBuyVolume(INT64 value) { m_stockCalculatedInfo.IncreaseOrdinaryBuyVolume(value); }
  INT64 GetOrdinarySellVolume(void) { return m_stockCalculatedInfo.GetOrdinarySellVolume(); }
  void SetOrdinarySellVolume(INT64 value) { m_stockCalculatedInfo.SetOrdinarySellVolume(value); }
  void IncreaseOrdinarySellVolume(INT64 value) { m_stockCalculatedInfo.IncreaseOrdinarySellVolume(value); }
  INT64 GetAttackBuyVolume(void) { return m_stockCalculatedInfo.GetAttackBuyVolume(); } // �������롣�ɽ��۸�����һ�۵����������ۡ����������������ǿ������
  void SetAttackBuyVolume(INT64 value) { m_stockCalculatedInfo.SetAttackBuyVolume(value); }
  void IncreaseAttackBuyVolume(INT64 value) { m_stockCalculatedInfo.IncreaseAttackBuyVolume(value); }
  INT64 GetStrongBuyVolume(void) { return m_stockCalculatedInfo.GetStrongBuyVolume(); } // ����ǿ������,�ɽ��۳���֮ǰ����������
  void SetStrongBuyVolume(INT64 value) { m_stockCalculatedInfo.SetStrongBuyVolume(value); }
  void IncreaseStrongBuyVolume(INT64 value) { m_stockCalculatedInfo.IncreaseStrongBuyVolume(value); }
  INT64 GetCurrentAttackBuy(void) { return m_stockCalculatedInfo.GetCurrentAttackBuy(); }
  void SetCurrentAttackBuy(INT64 value) { m_stockCalculatedInfo.SetCurrentAttackBuy(value); }
  long GetCurrentStrongBuy(void) { return m_stockCalculatedInfo.GetCurrentStrongBuy(); }
  void SetCurrentStrongBuy(INT64 value) { m_stockCalculatedInfo.SetCurrentStrongBuy(value); }
  INT64 GetAttackSellVolume(void) { return m_stockCalculatedInfo.GetAttackSellVolume(); } // �����������ɽ��۵�����һ�۵���������ۡ�
  void SetAttackSellVolume(INT64 value) { m_stockCalculatedInfo.SetAttackSellVolume(value); }
  void IncreaseAttackSellVolume(INT64 value) { m_stockCalculatedInfo.IncreaseAttackSellVolume(value); }
  INT64 GetStrongSellVolume(void) { return m_stockCalculatedInfo.GetStrongSellVolume(); }
  void SetStrongSellVolume(INT64 value) { m_stockCalculatedInfo.SetStrongSellVolume(value); }
  void IncreaseStrongSellVolume(INT64 value) { m_stockCalculatedInfo.IncreaseStrongSellVolume(value); }
  long GetCurrentAttackSell(void) { return m_stockCalculatedInfo.GetCurrentAttackSell(); }
  void SetCurrentAttackSell(INT64 value) { m_stockCalculatedInfo.SetCurrentAttackSell(value); }
  long GetCurrentStrongSell(void) { return m_stockCalculatedInfo.GetCurrentStrongSell(); }
  void SetCurrentStrongSell(INT64 value) { m_stockCalculatedInfo.SetCurrentStrongSell(value); }
  INT64 GetUnknownVolume(void) { return m_stockCalculatedInfo.GetUnknownVolume(); }
  void SetUnknownVolume(INT64 value) { m_stockCalculatedInfo.SetUnknownVolume(value); }
  void IncreaseUnknownVolume(INT64 value) { m_stockCalculatedInfo.IncreaseUnknownVolume(value); }
  long GetCurrentUnknown(void) { return m_stockCalculatedInfo.GetCurrentUnknown(); }
  void SetCurrentUnknown(long value) { m_stockCalculatedInfo.SetCurrentUnknown(value); }
  INT64 GetCancelBuyVolume(void) { return m_stockCalculatedInfo.GetCancelBuyVolume(); }
  void SetCancelBuyVolume(INT64 value) { m_stockCalculatedInfo.SetCancelBuyVolume(value); }
  INT64 GetCancelSellVolume(void) { return m_stockCalculatedInfo.GetCancelSellVolume(); }
  void SetCancelSellVolume(INT64 value) { m_stockCalculatedInfo.SetCancelSellVolume(value); }

  void SetTransactionNumber(long value) { m_stockCalculatedInfo.SetTransactionNumber(value); }
  void SetTransactionNumberBelow5000(long value) { m_stockCalculatedInfo.SetTransactionNumberBelow5000(value); }
  void SetTransactionNumberBelow50000(long value) { m_stockCalculatedInfo.SetTransactionNumberBelow50000(value); }
  void SetTransactionNumberBelow200000(long value) { m_stockCalculatedInfo.SetTransactionNumberBelow200000(value); }
  void SetTransactionNumberAbove200000(long value) { m_stockCalculatedInfo.SetTransactionNumberAbove200000(value); }
  void SetAttackBuyBelow50000(INT64 value) { m_stockCalculatedInfo.SetAttackBuyBelow50000(value); }
  void SetAttackBuyBelow200000(INT64 value) { m_stockCalculatedInfo.SetAttackBuyBelow200000(value); }
  void SetAttackBuyAbove200000(INT64 value) { m_stockCalculatedInfo.SetAttackBuyAbove200000(value); }
  void SetAttackSellBelow50000(INT64 value) { m_stockCalculatedInfo.SetAttackSellBelow50000(value); }
  void SetAttackSellBelow200000(INT64 value) { m_stockCalculatedInfo.SetAttackSellBelow200000(value); }
  void SetAttackSellAbove200000(INT64 value) { m_stockCalculatedInfo.SetAttackSellAbove200000(value); }

  long GetTransactionNumber(void) { return m_stockCalculatedInfo.GetTransactionNumber(); }
  long GetTransactionNumberBelow5000(void) { return m_stockCalculatedInfo.GetTransactionNumberBelow5000(); }
  long GetTransactionNumberBelow50000(void) { return m_stockCalculatedInfo.GetTransactionNumberBelow50000(); }
  long GetTransactionNumberBelow200000(void) { return m_stockCalculatedInfo.GetTransactionNumberBelow200000(); }
  long GetTransactionNumberAbove200000(void) { return m_stockCalculatedInfo.GetTransactionNumberAbove200000(); }

  INT64 GetAttackBuyBelow50000(void) { return m_stockCalculatedInfo.GetAttackBuyBelow50000(); }
  INT64 GetAttackBuyBelow200000(void) { return m_stockCalculatedInfo.GetAttackBuyBelow200000(); }
  INT64 GetAttackBuyAbove200000(void) { return m_stockCalculatedInfo.GetAttackBuyAbove200000(); }
  INT64 GetAttackSellBelow50000(void) { return m_stockCalculatedInfo.GetAttackSellBelow50000(); }
  INT64 GetAttackSellBelow200000(void) { return m_stockCalculatedInfo.GetAttackSellBelow200000(); }
  INT64 GetAttackSellAbove200000(void) { return m_stockCalculatedInfo.GetAttackSellAbove200000(); }

  void IncreaseAttackBuyBelow50000(INT64 value) { m_stockCalculatedInfo.IncreaseAttackBuyBelow50000(value); }
  void IncreaseAttackBuyBelow200000(INT64 value) { m_stockCalculatedInfo.IncreaseAttackBuyBelow200000(value); }
  void IncreaseAttackBuyAbove200000(INT64 value) { m_stockCalculatedInfo.IncreaseAttackBuyAbove200000(value); }
  void IncreaseAttackSellBelow50000(INT64 value) { m_stockCalculatedInfo.IncreaseAttackSellBelow50000(value); }
  void IncreaseAttackSellBelow200000(INT64 value) { m_stockCalculatedInfo.IncreaseAttackSellBelow200000(value); }
  void IncreaseAttackSellAbove200000(INT64 value) { m_stockCalculatedInfo.IncreaseAttackSellAbove200000(value); }

  int  GetCurrentTransactionType(void) { return m_nCurrentTransactionType; }
  void SetCurrentTransactionType(int nType) { m_nCurrentTransactionType = nType; }
  INT64 GetCurrentTransationVolume(void) { return m_lCurrentGuadanTransactionVolume; }
  void SetCurrentTransationVolume(INT64 value) { m_lCurrentGuadanTransactionVolume = value; }

  // ���µ�ǰ������״̬
  void UpdateStatus(CRTDataPtr pRTData);

  // ����״̬��ʶ��ȡ������
  bool IsActive(void) { return m_fActive; }
  void SetActive(bool fFlag) { m_fActive = fFlag; }
  bool IsChoiced(void) { return m_fChoiced; }
  void SetChoiced(bool fChoiced) { m_fChoiced = fChoiced; }
  bool IsMinLineUpdated(void) { return (m_fMinLineUpdated); }
  void SetMinLineUpdated(bool fUpdate) { m_fMinLineUpdated = fUpdate; }
  bool IsDayLineUpdated(void) { return (m_fDayLineUpdated); }
  void SetDayLineUpdated(bool fUpdate) { m_fDayLineUpdated = fUpdate; }
  bool IsDayLineLoaded(void) { return m_fDayLineLoaded; }
  void SetDayLineLoaded(bool fFlag) { m_fDayLineLoaded = fFlag; }
  // ��һ��ʵʱ�����жϺ�����
  bool HaveFirstRTData(void) { return m_fHaveFirstRTData; }
  bool SetHavingFirstRTData(bool fFlag) { if (m_fHaveFirstRTData || !fFlag) return false; m_fHaveFirstRTData = fFlag; return true; }

  bool IsTodayDataActive(void); //������߼ۡ���ͼۡ��ɽ����ͳɽ������жϣ������Ϊ�㣬����Ϊ�˹ɽ���û����Ч���ݡ���Ȼ��m_fActiveΪ��״̬�¡�
  bool IsTodayDataChanged(void); // �����߼ۡ���ͼۡ��ɽ����ͳɽ����������ݲ�Ϊ�㣬�򷵻��档

  // ���ڴ���������ʷ���ݵĺ���λ�ڲ�ͬ���߳��У��ʶ���Ҫͬ���������ñ�ʶ
  bool IsDayLineNeedUpdate(void) { return m_fDayLineNeedUpdate; }
  void SetDayLineNeedUpdate(bool fFlag);
  bool IsDayLineNeedProcess(void) { return m_fDayLineNeedProcess; }
  void SetDayLineNeedProcess(bool fFlag);
  bool IsDayLineNeedSaving(void) { return m_fDayLineNeedSaving; }
  void SetDayLineNeedSaving(bool fFlag);
  bool IsDayLineNeedSavingAndClearFlag(void);

  bool ProcessNeteaseDayLineData(void);
  void IncreaseCurrentPos(long lValue = 1) { m_lCurrentPos += lValue; m_pCurrentPos += lValue; }
  void ResetCurrentPos(void) { m_pCurrentPos = m_pDayLineBuffer; m_lCurrentPos = 0; }

  // ���ݿ����ȡ�ʹ洢
  bool SaveDayLine(void);
  void UpdateDayLineStartEndDay(void);
  void SaveBasicInfo(CSetDayLine& setDayLine);
  void SaveCalculatedInfo(CSetDayLineInfo& setDayLineInfo);
  void SaveTempInfo(CSetDayLineToday& setDayLineToday);
  void LoadAndCalculateTempInfo(CSetDayLineToday& setDayLineToday);
  // ����װ�غ������ɹ����߳�ThreadLoadDayLine����
  bool LoadDayLineAndDayLineInfo(void); // �˺�������
  bool LoadDayLine(CSetDayLine& setDayLine);
  bool LoadDayLineInfo(CSetDayLineInfo& setDayLine);
  // �洢ʵʱ�������ݽ����ݿ⡣��Ŀǰ��֮���ã�
  bool SaveRealTimeData(CSetRealTimeData& setRT);
  void SaveStockCodeDB(CSetStockCode& setStockCode);
  void AppendStockCodeDB(CSetStockCode& setStockCode);
  bool LoadStockCodeDB(CSetStockCode& setStockCode);

  // �ҵ����
  double GetCurrentGuadanTransactionPrice(void) { return m_dCurrentGuadanTransactionPrice; }
  void SetCurrentGuadanTransactionPrice(double dValue) { m_dCurrentGuadanTransactionPrice = dValue; }
  long GetGuadan(long lPrice) { return m_mapGuadan.at(lPrice); }
  void SetGuadan(long lPrice, long lVolume) { m_mapGuadan[lPrice] = lVolume; }
  bool HaveGuadan(long lPrice);

  // �������ǿ�ȼ���
  bool CalculateDayLineRS(void);
  bool CalculateDayLineRS(long lNumber);

  // ����ʵʱ���ݸ�����, �ɹ����߳�ThreadCalculateRTData����
  bool ProcessRTData(void);
  bool ProcessOneRTData(CRTDataPtr pRTData);
  void CalculateOneDeal(CRTDataPtr pRTData, long lCurrentGuadanTransactionPrice);
  void IncreaseTransactionNumber(void);
  void CalculateOneRTData(CRTDataPtr pRTData);
  void CalculateOrdinaryBuySell(long lCurrentGuadanTransactionPrice);
  void CalculateAttackBuy(void);
  void CalculateStrongBuy(void);
  void CalculateAttackBuyVolume(void);
  void CalculateAttackSell(void);
  void CalculateStrongSell(void);
  void CalculateAttackSellVolume(void);
  void SetLastRTDataPtr(CRTDataPtr pLastRTData) { m_pLastRTData = pLastRTData; }
  CRTDataPtr GetLastRTDataPtr(void) { return m_pLastRTData; }
  void InitializeCalculatingRTDataEnvionment(CRTDataPtr pRTData);

  bool AnalysisGuadan(CRTDataPtr pCurrentRTData, long lCurrentTransactionPrice);
  void SelectGuadanThatNeedToCalculate(CRTDataPtr pCurrentRTData, long lCurrentTransactionPrice, array<bool, 10>& fNeedCheck);
  void SetCurrentGuadan(CRTDataPtr pCurrentRTData);
  void CheckGuadan(CRTDataPtr pCurrentRTData, array<bool, 10>& fNeedCheck);
  void CheckSellGuadan(array<bool, 10>& fNeedCheck, int i);
  void CheckBuyGuadan(array<bool, 10>& fNeedCheck, int i);
  bool CheckCurrentRTData();
  void ShowCurrentTransaction(void);
  void ShowCurrentInformationofCancelingGuadan(void);
  void ReportGuadanTransaction(void);
  void ReportGuadan(void);

  // ����ͬ�����ƴ�ȡʵʱ����
  void PushRTData(CRTDataPtr pData);
  CRTDataPtr PopRTData(void);
  CRTDataPtr GetRTDataAtHead(void); // �����������������
  long GetRTDataDequeSize(void);
  // ��մ洢ʵʱ���ݵĶ���
  void ClearRTDataDeque(void);

#ifdef _DEBUG
  virtual	void AssertValid() const;
  virtual	void Dump(CDumpContext& dc) const;
#endif

public:
  // ����ר�ú���
  void __TestSetGuadanDeque(long lPrice, long lVolume) { m_mapGuadan[lPrice] = lVolume; } // Ԥ�����ùҵ���

public:

  vector<CDayLinePtr>	m_vDayLine; // ������������
  char* m_pDayLineBuffer; // ���߶�ȡ������
  long m_lDayLineBufferLength;
  char* m_pCurrentPos;
  long m_lCurrentPos;

protected:
  CStockBasicInfo m_stockBasicInfo;
  CStockCalculatedInfo m_stockCalculatedInfo;

  atomic_bool m_fDayLineNeedUpdate; // ������Ҫ���¡�Ĭ��Ϊ��
  atomic_bool m_fDayLineNeedProcess; // �Ѵ������϶�ȡ��������ʷ���ݣ��ȴ�����
  atomic_bool m_fDayLineNeedSaving; // ������ʷ�����Ѵ����ȴ��洢��
  atomic_bool m_fDayLineLoaded; // �Ƿ�װ������������

  bool m_fActive;	// �Ƿ����������ݶ��롣������ʵʱ���鴦����������������ʷ���ݴ����������á�

  bool m_fHaveFirstRTData; // ʵʱ���ݿ�ʼ�����ʶ����һ��ʵʱ����ֻ��������ʼ��ϵͳ���������ڼ��㡣�ӵڶ������ݿ�ʼ�������Ч��

  bool m_fChoiced;// �˹�Ʊ�Ƿ�����ѡ��Ʊ.
  bool m_fMinLineUpdated; // ����ķ��������Ƿ���¹�.
  bool m_fDayLineUpdated; // ��������������Ƿ���¹�.

  // �ҵ��ľ��������
  map<long, long> m_mapGuadan;// ����map�ṹ�洢�ҵ��ľ������������Ϊ��λ������Ϊ�ҵ�����
  CRTDataPtr m_pLastRTData; // ��m_dequeRTData��������һ��ʵʱ���ݡ�
  INT64 m_lCurrentGuadanTransactionVolume; // ��ǰ�ҵ�������������Ŀǰ��ʱ�䣬����ʵʱ���ݶ�����ǰ�����ݵ�ʱ�䣩
  double m_dCurrentGuadanTransactionPrice; // ��ǰ�ɽ��۸�
  int m_nCurrentTransactionType; // ��ǰ�������ͣ�ǿ�򡢽��������롣��������
  INT64 m_lCurrentCanselSellVolume;
  INT64 m_lCurrentCanselBuyVolume;

  queue<COneDealPtr> m_queueDeal; // ����ɽ���Ϣ���У�Ŀǰ��δʹ�ã���

  deque<CRTDataPtr> m_dequeRTData; // ʵʱ���ݶ��С�Ŀǰ����ʹ��˫����У���Ϊ�����������õ������������Ǹ�ΪqueueΪ�á�
  CCriticalSection m_RTDataLock; // ʵʱ���ݶ��е�ͬ����

private:
  bool m_fDebugLoadDayLineFirst; // �����á���ֹDayLine���DayLineInfo��װ��������
};

typedef shared_ptr<CStock> CStockPtr;
