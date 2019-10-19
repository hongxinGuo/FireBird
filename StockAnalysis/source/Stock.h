#pragma once

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
#include"SetRealTimeData.h"
#include"RTData.h"
#include"DayLine.h"
#include"OneDeal.h"

using namespace std;
#include<vector>
#include<memory>
#include<queue>
#include<deque>
#include<map>
#include<array>

// ֤ȯ�������ݰ�
class CStock : public CObject {
public:
  CStock(void);
  ~CStock(void);
  void Reset(void);

  void operator=(CStock&);

public:
  WORD GetMarket(void) noexcept { return m_wMarket; }
  void SetMarket(WORD wValue) noexcept { m_wMarket = wValue; }
  bool IsActive(void) noexcept { return m_fActive; }
  void SetActive(bool fFlag) noexcept { m_fActive = fFlag; }
  CString GetStockCode(void) { return m_strStockCode; }
  void SetStockCode(CString str) { m_strStockCode = str; }
  long GetCode(void) noexcept { return m_iStockCode; }
  void SetCode(long lValue) { m_iStockCode = lValue; }
  CString GetStockName(void) { return m_strStockName; }
  void SetStockName(CString str) { m_strStockName = str; }

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
  void SetTotalValue(INT64 llValue) noexcept { m_llTotalValue = llValue; }
  INT64 GetTotalValue(void) noexcept { return m_llTotalValue; }
  void SetCurrentValue(INT64 llValue) noexcept { m_llCurrentValue = llValue; }
  INT64 GetCurrentValue(void) noexcept { return m_llCurrentValue; }
  long GetPBuy(int iIndex) { return m_lPBuy.at(iIndex); }
  long GetVBuy(int iIndex) { return m_lVBuy.at(iIndex); }
  long GetPSell(int iIndex) { return m_lPSell.at(iIndex); }
  long GetVSell(int iIndex) { return m_lVSell.at(iIndex); }

  INT64 GetAttackBuyAmount(void) noexcept { return m_lAttackBuyAmount; }
  INT64 GetAttackSellAmount(void) noexcept { return m_lAttackSellAmount; }
  INT64 GetOrdinaryBuyVolume(void) noexcept { return m_lOrdinaryBuyVolume; }
  INT64 GetOrdinarySellVolume(void) noexcept { return m_lOrdinarySellVolume; }
  INT64 GetAttackBuyVolume(void) noexcept { return m_lAttackBuyVolume; }		// �������롣�ɽ��۸�����һ�۵����������ۡ����������������ǿ������
  INT64 GetStrongBuyVolume(void) noexcept { return m_lStrongBuyVolume; }		// ����ǿ������,�ɽ��۳���֮ǰ����������
  INT64 GetCurrentAttackBuy(void) noexcept { return m_lCurrentAttackBuy; }
  long GetCurrentStrongBuy(void) noexcept { return m_lCurrentStrongBuy; }
  INT64 GetAttackSellVolume(void) noexcept { return m_lAttackSellVolume; }			// �����������ɽ��۵�����һ�۵���������ۡ�
  INT64 GetStrongSellVolume(void) noexcept { return m_lStrongSellVolume; }
  long GetCurrentAttackSell(void) noexcept { return m_lCurrentAttackSell; }
  long GetCurrentStrongSell(void) noexcept { return m_lCurrentStrongSell; }
  INT64 GetUnknownVolume(void) noexcept { return m_lUnknownVolume; }
  long GetCurrentUnknown(void) noexcept { return m_lCurrentUnknown; }
  INT64 GetCancelBuyVolume(void) noexcept { return m_lCancelBuyVolume; }
  INT64 GetCancelSellVolume(void) noexcept { return m_lCancelSellVolume; }

  int  GetCurrentTransactionType(void) noexcept { return m_nCurrentTransactionType; }
  void SetCurrentTransactionType(int nType) noexcept { m_nCurrentTransactionType = nType; }
  INT64 GetCurrentTransationVolume(void) noexcept { return m_lCurrentGuadanTransactionVolume; }

  double GetRelativeStrong(void) noexcept { return m_dRelativeStrong; }
  long GetTransactionNumber(void) noexcept { return m_lTransactionNumber; }
  long GetTransactionNumberBelow5000(void) noexcept { return m_lTransactionNumberBelow5000; }
  long GetTransactionNumberBelow50000(void) noexcept { return m_lTransactionNumberBelow50000; }
  long GetTransactionNumberBelow200000(void) noexcept { return m_lTransactionNumberBelow200000; }
  long GetTransactionNumberAbove200000(void) noexcept { return m_lTransactionNumberAbove200000; }

  INT64 GetAttackBuyBelow50000(void) noexcept { return m_lAttackBuyBelow50000; }
  INT64 GetAttackBuyBelow200000(void) noexcept { return m_lAttackBuyBelow200000; }
  INT64 GetAttackBuyAbove200000(void) noexcept { return m_lAttackBuyAbove200000; }
  INT64 GetAttackSellBelow50000(void) noexcept { return m_lAttackSellBelow50000; }
  INT64 GetAttackSellBelow200000(void) noexcept { return m_lAttackSellBelow200000; }
  INT64 GetAttackSellAbove200000(void) noexcept { return m_lAttackSellAbove200000; }

  void SetAttackBuyAmount(INT64 value) noexcept { m_lAttackBuyAmount = value; }
  void SetAttackSellAmount(INT64 value) noexcept { m_lAttackSellAmount = value; }
  void SetOrdinaryBuyVolume(INT64 value) noexcept { m_lOrdinaryBuyVolume = value; }
  void SetOrdinarySellVolume(INT64 value) noexcept { m_lOrdinarySellVolume = value; }
  void SetAttackBuyVolume(INT64 value) noexcept { m_lAttackBuyVolume = value; }		// �������롣�ɽ��۸�����һ�۵����������ۡ����������������ǿ������
  void SetStrongBuyVolume(INT64 value) noexcept { m_lStrongBuyVolume = value; }		// ����ǿ������,�ɽ��۳���֮ǰ����������
  void SetCurrentAttackBuy(long value) noexcept { m_lCurrentAttackBuy = value; }
  void SetCurrentStrongBuy(long value) noexcept { m_lCurrentStrongBuy = value; }
  void SetAttackSellVolume(INT64 value) noexcept { m_lAttackSellVolume = value; }			// �����������ɽ��۵�����һ�۵���������ۡ�
  void SetStrongSellVolume(INT64 value) noexcept { m_lStrongSellVolume = value; }
  void SetCurrentAttackSell(long value) noexcept { m_lCurrentAttackSell = value; }
  void SetCurrentStrongSell(long value) noexcept { m_lCurrentStrongSell = value; }
  void SetUnknownVolume(INT64 value) noexcept { m_lUnknownVolume = value; }
  void SetCurrentUnknown(long value) noexcept { m_lCurrentUnknown = value; }
  void SetCancelBuyVolume(INT64 value) noexcept { m_lCancelBuyVolume = value; }
  void SetCancelSellVolume(INT64 value) noexcept { m_lCancelSellVolume = value; }

  void SetCurrentTransationVolume(INT64 value) noexcept { m_lCurrentGuadanTransactionVolume = value; }

  void SetTransactionNumber(long value) noexcept { m_lTransactionNumber = value; }
  void SetTransactionNumberBelow5000(long value) noexcept { m_lTransactionNumberBelow5000 = value; }
  void SetTransactionNumberBelow50000(long value) noexcept { m_lTransactionNumberBelow50000 = value; }
  void SetTransactionNumberBelow200000(long value) noexcept { m_lTransactionNumberBelow200000 = value; }
  void SetTransactionNumberAbove200000(long value) noexcept { m_lTransactionNumberAbove200000 = value; }

  void SetAttackBuyBelow50000(INT64 value) noexcept { m_lAttackBuyBelow50000 = value; }
  void SetAttackBuyBelow200000(INT64 value) noexcept { m_lAttackBuyBelow200000 = value; }
  void SetAttackBuyAbove200000(INT64 value) noexcept { m_lAttackBuyAbove200000 = value; }
  void SetAttackSellBelow50000(INT64 value) noexcept { m_lAttackSellBelow50000 = value; }
  void SetAttackSellBelow200000(INT64 value) noexcept { m_lAttackSellBelow200000 = value; }
  void SetAttackSellAbove200000(INT64 value) noexcept { m_lAttackSellAbove200000 = value; }

  void ClearRTDataDeque(void);  // ��մ洢ʵʱ���ݵĶ���

  // �ҵ����
  double GetCurrentGuaDanTransactionPrice(void) noexcept { return m_dCurrentGuaDanTransactionPrice; }
  long GetGuaDan(long lPrice) { return m_mapGuaDan.at(lPrice); }
  void SetGuaDan(long lPrice, long lVolume) { m_mapGuaDan[lPrice] = lVolume; }
  bool HaveGuaDan(long lPrice) { if (m_mapGuaDan.find(lPrice) == m_mapGuaDan.end()) return false; return true; }

  bool IsChoiced(void) noexcept { return m_fChoiced; }
  void SetChoicedFlag(bool fChoiced) noexcept { m_fChoiced = fChoiced; }

  bool IsMinLineUpdated(void) noexcept { return (m_fMinLineUpdated); }
  void SetMinLineUpdated(bool fUpdate) noexcept { m_fMinLineUpdated = fUpdate; }

  bool IsDayLineUpdated(void) noexcept { return (m_fDayKLineUpdated); }
  void SetDayLineUpdated(bool fUpdate) noexcept { m_fDayKLineUpdated = fUpdate; }

  bool IsDayLineLoaded(void) noexcept { return m_fDayLineLoaded; }
  void SetDayLineLoaded(bool fFlag) noexcept { m_fDayLineLoaded = fFlag; }

  bool IsStartCalculating(void) noexcept { return m_fStartCalculating; }
  bool SetStartCalculating(bool fFlag) noexcept { if (m_fStartCalculating || !fFlag) return false; m_fStartCalculating = fFlag; return true; }

  void UpdateStatus(CRTDataPtr pRTData); // ���µ�ǰ������״̬

  // ����װ�غ������ɹ����߳�ThreadLoadDayLine����
  bool LoadDayLine(void);
  bool LoadDayLine(CSetDayLine* psetDayLine);
  bool LoadDayLineInfo(CSetDayLineInfo* psetDayLine);
  bool CalculateDayLineRS(void);
  bool CalculateDayLineRS(long lNumber);

  // ����ʵʱ���ݸ�����, �ɹ����߳�ThreadCalculateRTData����
  bool ProcessRTData(void);
  bool ProcessOneRTData(CRTDataPtr pRTData);
  void IncreaseTransactionNumber(void);
  void CalculateOneRTData(CRTDataPtr pRTData);
  void CalculateOrdinaryBuySell(long lCurrentGuaDanTransactionPrice);
  void CalculateAttackBuy(void);
  void CalculateStrongBuy(void);
  void CalculateAttackBuyVolume(void);
  void CalculateAttackSell(void);
  void CalculateStrongSell(void);
  void CalculateAttackSellVolume(void);
  void InitializeCalculatingRTDataEnvionment(CRTDataPtr pRTData);
  bool AnalysisGuaDan(CRTDataPtr pCurrentRTData, long lCurrentTransactionPrice);
  bool CheckCurrentRTData();
  void ShowCurrentTransaction(void);
  void ShowCurrentInformationofCancelingGuaDan(void);
  void ReportGuaDanTransaction(void);
  void ReportGuaDan(void);
  void SetLastRTDataPtr(CRTDataPtr pLastRTData) noexcept { m_pLastRTData = pLastRTData; }

  // �洢ʵʱ�������ݽ����ݿ⡣��Ŀǰ��֮���ã�
  bool SaveRealTimeData(CSetRealTimeData* psetRT);

  // ����ͬ�����ƴ�ȡʵʱ����
  void PushRTData(CRTDataPtr pData);
  CRTDataPtr PopRTData(void);
  CRTDataPtr GetRTDataAtHead(void); // �����������������
  long GetRTDataDequeSize(void);

  // ���ڴ���������ʷ���ݵĺ���λ�ڲ�ͬ���߳��У��ʶ���Ҫͬ���������ñ�ʶ
  void SetDayLineNeedSavingFlag(bool fFlag);
  bool IsDayLineNeedSaving(void);

#ifdef _DEBUG
  virtual	void AssertValid() const;
  virtual	void Dump(CDumpContext& dc) const;
#endif

public:
  // ����ר�ú���
  void TestSetGuaDanDeque(long lPrice, long lVolume) { m_mapGuaDan[lPrice] = lVolume; } // Ԥ�����ùҵ���

public:

  vector<CDayLinePtr>				m_vDayLine;			// ������������

protected:
  // ������Ϣ
  WORD      m_wMarket;                // �г�����
  bool      m_fActive;                // �����Ƿ���ܵ�ʵʱ����
  CString		m_strStockCode;						// ֤ȯ���룬
  CString 	m_strStockName;						// ֤ȯ����
  int       m_iStockCode;             // ֤ȯ����ֵ
  short			m_nHand;									// ÿ�ֹ���

  // ʵʱ����,��Ҫ���ϸ���
  time_t    m_TransactionTime;         // ʵʱ���ݽ���ʱ��
  long	    m_lLastClose;		// ��0.001Ԫ�Ƶ����̼�
  long		  m_lOpen;				// ��0.001Ԫ�ƵĿ��̼�
  long		  m_lHigh;				// ��0.001Ԫ�Ƶ���߼�
  long		  m_lLow;					// ��0.001Ԫ�Ƶ���ͼ�
  long		  m_lNew;					// ��0.001Ԫ�Ƶ����¼�
  INT64	    m_llVolume;			// ��1�ɼƵĳɽ���
  INT64     m_llAmount;			// ��Ԫ�Ƶĳɽ����
  INT64	    m_llTotalValue;					// ����ֵ����λ����Ԫ
  INT64     m_llCurrentValue;				// ��ͨ��ֵ����λ����Ԫ
  array<long, 5>      m_lPBuy;			// ���̼ۡ���λ��0.001Ԫ
  array<long, 5>			m_lVBuy;			// ����������λ����
  array<long, 5>		  m_lPSell;		// ���̼ۡ���λ��0.001Ԫ
  array<long, 5>			m_lVSell;		// ����������λ����

    // ���±������ڷ������������������
  INT64	    m_lAttackBuyAmount;		// ����������
  INT64     m_lAttackSellAmount;	// �����������
  INT64	    m_lCurrentVolume;

  double    m_dRelativeStrong; // ��λ��1%
  long      m_lTransactionNumber; // �������յĳɽ�����
  long      m_lTransactionNumberBelow5000; // �������յ���5000�ɵĳɽ�����
  long      m_lTransactionNumberBelow50000; //
  long      m_lTransactionNumberBelow200000; //
  long      m_lTransactionNumberAbove200000; //

  INT64     m_lOrdinaryBuyVolume;
  INT64     m_lOrdinarySellVolume;
  INT64     m_lAttackBuyVolume;		// �������롣�ɽ��۸�����һ�۵����������ۡ����������������ǿ������
  INT64	    m_lStrongBuyVolume;		// ����ǿ������,�ɽ��۳���֮ǰ����������
  long    	m_lCurrentAttackBuy;
  long    	m_lCurrentStrongBuy;
  INT64     m_lAttackSellVolume;			// �����������ɽ��۵�����һ�۵���������ۡ�
  INT64     m_lStrongSellVolume;		// ����ǿ������,�ɽ��۵���֮ǰ���������
  long      m_lCurrentAttackSell;
  long      m_lCurrentStrongSell;
  INT64	    m_lUnknownVolume;
  long      m_lCurrentUnknown;
  INT64	    m_lCancelBuyVolume;		// �򵥳�����
  INT64     m_lCancelSellVolume;			// ����������

  INT64	    m_lAttackBuyBelow50000;
  INT64	    m_lAttackBuyBelow200000;
  INT64	    m_lAttackBuyAbove200000;
  INT64	    m_lAttackSellBelow50000;
  INT64	    m_lAttackSellBelow200000;
  INT64	    m_lAttackSellAbove200000;

  queue<COneDealPtr>    m_queueDeal;        // ����ɽ���Ϣ���У�Ŀǰ��δʹ�ã���

  // �ҵ��ľ��������
  map<long, long>       m_mapGuaDan;        // ����map�ṹ�洢�ҵ��ľ������������Ϊ��λ������Ϊ�ҵ�����
  CRTDataPtr            m_pLastRTData;        // ��m_dequeRTData��������һ��ʵʱ���ݡ�
  INT64                 m_lCurrentGuadanTransactionVolume; // ��ǰ�ҵ�������������Ŀǰ��ʱ�䣬����ʵʱ���ݶ�����ǰ�����ݵ�ʱ�䣩
  double                m_dCurrentGuaDanTransactionPrice; // ��ǰ�ɽ��۸�
  int                   m_nCurrentTransactionType; // ��ǰ�������ͣ�ǿ�򡢽��������롣��������
  INT64                 m_lCurrentCanselSellVolume;
  INT64                 m_lCurrentCanselBuyVolume;

  bool			            m_fDayLineLoaded;					// �Ƿ�װ������������

  bool                  m_fDayLineNeededSaving;   // ���������Ƿ���Ҫ�洢
  CCriticalSection      m_DayLineNeedSavingLock;  // ������ʶ��ͬ����

  deque<CRTDataPtr>     m_dequeRTData;  // ʵʱ���ݶ��С�Ŀǰ����ʹ��˫����У���Ϊ�����������õ������������Ǹ�ΪqueueΪ�á�
  CCriticalSection      m_RTDataLock;   // ʵʱ���ݶ��е�ͬ����

  bool                  m_fStartCalculating;  // ʵʱ���ݿ�ʼ�����ʶ����һ��ʵʱ����ֻ��������ʼ��ϵͳ���������ڼ��㡣�ӵڶ������ݿ�ʼ�������Ч��

  bool			            m_fChoiced;									// �˹�Ʊ�Ƿ�����ѡ��Ʊ.
  bool			            m_fMinLineUpdated;					// ����ķ��������Ƿ���¹�.
  bool			            m_fDayKLineUpdated;					// ��������������Ƿ���¹�.
};

typedef shared_ptr<CStock> CStockPtr;
