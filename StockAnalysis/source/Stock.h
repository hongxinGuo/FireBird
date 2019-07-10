#pragma once

enum {
  __NO_TRANSACTION__ = 0,
  __ATTACK_BUY__ = 1,
  __STRONG_BUY__ = 2,
  __ORDINARY_BUY__ = 3,
  __UNKNOWN_BUYSELL__ = 4,
  __ORDINARY_SELL__ = 5,
  __STRONG_SELL__ = 6,
  __ATTACK_SELL__ = 7
};

#include"SetDayLine.h"

#include"SetRealTimeData.h"
#include"RTData.h"
#include"DayLine.h"
#include"OneDeal.h"

using namespace std;
#include<vector>
#include<memory>
#include<deque>
#include<map>

// ֤ȯ�������ݰ�
class CStock : public CObject {
public:
  CStock(void);
  ~CStock(void);
  void Reset(void);

  void operator=(CStock &);

public:
  WORD GetMarket(void) { return m_wMarket; }
  void SetMarket(WORD wValue) { m_wMarket = wValue; }
  bool IsActive(void) { return m_fActive; }
  void SetActive(bool fFlag) { m_fActive = fFlag; }
  CString GetStockCode(void) { return m_strStockCode; }
  void SetStockCode(CString str) { m_strStockCode = str; }
  long GetCode(void) { return m_iStockCode; }
  void SetCode(long lValue) { m_iStockCode = lValue; }
  CString GetStockName(void) { return m_strStockName; }
  void SetStockName(CString str) { m_strStockName = str; }

  time_t GetTime(void) { return m_Time; }
  void SetTime(time_t time) { m_Time = time; }
  long GetLastClose(void) { return m_lLastClose; }
  void SetLastClose(long lValue) { m_lLastClose = lValue; }
  long GetOpen(void) { return m_lOpen; }
  void SetOpen(long lValue) { m_lOpen = lValue; }
  long GetHigh(void) { return m_lHigh; }
  void SetHigh(long lValue) { m_lHigh = lValue; }
  long GetLow(void) { return m_lLow; }
  void SetLow(long lValue) { m_lLow = lValue; }
  long GetNew(void) { return m_lNew; }
  void SetNew(long lValue) { m_lNew = lValue; }
  long long GetAmount(void) { return m_lAmount; }
  void SetAmount(long long llValue) { m_lAmount = llValue; }
  long long GetVolume(void) { return m_lVolume; }
  void SetVolume(long long llValue) { m_lVolume = llValue; }

  long long GetAttackBuyAmount(void) { return m_lAttackBuyAmount; }
  long long GetAttackSellAmount(void) { return m_lAttackSellAmount; }
  long GetOrdinaryBuyVolume(void) { return m_lOrdinaryBuyVolume; }
  long GetOrdinarySellVolume(void) { return m_lOrdinarySellVolume; }
  long GetAttackBuyVolume(void) { return m_lAttackBuyVolume; }		// �������롣�ɽ��۸�����һ�۵����������ۡ����������������ǿ������
  long GetStrongBuyVolume(void) { return m_lStrongBuyVolume; }		// ����ǿ������,�ɽ��۳���֮ǰ����������
  long GetCurrentAttackBuy(void) { return m_lCurrentAttackBuy; }
  long GetCurrentStrongBuy(void) { return m_lCurrentStrongBuy; }
  long GetAttackSellVolume(void) { return m_lAttackSellVolume; }			// �����������ɽ��۵�����һ�۵���������ۡ�
  long GetStrongSellVolume(void) { return m_lStrongSellVolume; }
  long GetCurrentAttackSell(void) { return m_lCurrentAttackSell; }
  long GetCurrentStrongSell(void) { return m_lCurrentStrongSell; }
  long GetUnknownVolume(void) { return m_lUnknownVolume; }
  long GetCurrentUnknown(void) { return m_lCurrentUnknown; }
  long GetCancelBuyVolume(void) { return m_lCancelBuyVolume; }
  long GetCancelSellVolume(void) { return m_lCancelSellVolume; }

  double GetRelativeStrong(void) { return m_dRelativeStrong; }
  long GetTransactionNumber(void) { return m_lTransactionNumber; }
  long GetTransactionNumberBelow5000(void) { return m_lTransactionNumberBelow5000; }
  long GetTransactionNumberBelow50000(void) { return m_lTransactionNumberBelow50000; }
  long GetTransactionNumberBelow200000(void) { return m_lTransactionNumberBelow200000; }
  long GetTransactionNumberAbove200000(void) { return m_lTransactionNumberAbove200000; }

  long GetAttackBuyBelow50000(void) { return m_lAttackBuyBelow50000; }
  long GetAttackBuyBelow200000(void) { return m_lAttackBuyBelow200000; }
  long GetAttackBuyAbove200000(void) { return m_lAttackBuyAbove200000; }
  long GetAttackSellBelow50000(void) { return m_lAttackSellBelow50000; }
  long GetAttackSellBelow200000(void) { return m_lAttackSellBelow200000; }
  long GetAttackSellAbove200000(void) { return m_lAttackSellAbove200000; }

	bool IsChoiced( void ) { return m_fChoiced; }
	void SetChoicedFlag( bool fChoiced ) { m_fChoiced = fChoiced; }

	bool IsMinLineUpdated( void ) { return ( m_fMinLineUpdated ); }
	void SetMinLineUpdated( bool fUpdate ) { m_fMinLineUpdated = fUpdate; }

	bool IsDayLineUpdated( void ) { return ( m_fDayKLineUpdated ); }
	void SetDayLineUpdated( bool fUpdate ) { m_fDayKLineUpdated = fUpdate; }

  bool IsDayLineLoaded(void) { return m_fDayLineLoaded; }
  void SetDayLineLoaded(bool fFlag) { m_fDayLineLoaded = fFlag; }

  bool IsStartCalculating(void) { return m_fStartCalculating; }
  bool SetStartCalculating(bool fFlag) { if (m_fStartCalculating || !fFlag) return false; m_fStartCalculating = fFlag; return true; }

	void UpdataCurrentStatus(CStockRTDataPtr pRTData);
	bool LoadDayLine(CSetDayLine * psetDayLine);
	
  // ����ʵʱ����, �ɹ����߳�ClientThreadCalculatingRTDataProc����
  bool CalculateRTData(void);
  bool AnalysisingGuaDan(CStockRTDataPtr pCurrentRTData, CStockRTDataPtr pLastRTData, int nTransactionType, double dCurrentTransactionPrice);

	bool SaveRealTimeData(CSetRealTimeData * psetRT);

  // ����ͬ�����ƴ�ȡʵʱ����
  void PushRTStockData(CStockRTDataPtr pData);
  CStockRTDataPtr PopRTStockData(void);
  long GetRTDataDequeSize(void);

  // ���ڴ���������ʷ���ݵĺ���λ�ڲ�ͬ���߳��У��ʶ���Ҫͬ���������ñ�ʶ
  void SetDayLineNeedSavingFlag(bool fFlag);
  bool IsDayLineNeedSaving(void);
		
#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif

public:

	vector<CDayLinePtr>				m_vDayLine;			// ������������
 
protected:
  // ������Ϣ
  WORD      m_wMarket;                // �г�����
  bool      m_fActive;                // �����Ƿ���ܵ�ʵʱ����
  CString		m_strStockCode;						// ֤ȯ���룬
  CString		m_strStockName;						// ֤ȯ����
  int       m_iStockCode;             // ֤ȯ����ֵ
  short			m_nHand;									// ÿ�ֹ���


  // ʵʱ����
  time_t    m_Time;
  long	    m_lLastClose;		// ��0.001Ԫ�Ƶ����̼�
  long		  m_lOpen;				// ��0.001Ԫ�ƵĿ��̼�		
  long		  m_lHigh;				// ��0.001Ԫ�Ƶ���߼�
  long		  m_lLow;					// ��0.001Ԫ�Ƶ���ͼ�
  long		  m_lNew;					// ��0.001Ԫ�Ƶ����¼�
  long long	m_lVolume;			// ��1�ɼƵĳɽ���
  long long m_lAmount;			// ��Ԫ�Ƶĳɽ����

  long		  m_lPBuy[5];			// ���̼ۡ���λ��0.001Ԫ
  long			m_lVBuy[5];			// ����������λ����
  long		  m_lPSell[5];		// ���̼ۡ���λ��0.001Ԫ
  long			m_lVSell[5];		// ����������λ����

    // ���±������ڷ������������������
  long long	m_lAttackBuyAmount;		// ����������
  long long m_lAttackSellAmount;	// �����������
  long			m_lCurrentVolume;

  double    m_dRelativeStrong; // ��λ��1%
  long      m_lTransactionNumber; // �������յĳɽ�����
  long      m_lTransactionNumberBelow5000; // �������յ���5000�ɵĳɽ�����
  long      m_lTransactionNumberBelow50000; // 
  long      m_lTransactionNumberBelow200000; // 
  long      m_lTransactionNumberAbove200000; // 

  long      m_lOrdinaryBuyVolume;
  long      m_lOrdinarySellVolume;
  long			m_lAttackBuyVolume;		// �������롣�ɽ��۸�����һ�۵����������ۡ����������������ǿ������
  long			m_lStrongBuyVolume;		// ����ǿ������,�ɽ��۳���֮ǰ����������
  long			m_lCurrentAttackBuy;
  long			m_lCurrentStrongBuy;
  long			m_lAttackSellVolume;			// �����������ɽ��۵�����һ�۵���������ۡ�
  long			m_lStrongSellVolume;		// ����ǿ������,�ɽ��۵���֮ǰ���������
  long			m_lCurrentAttackSell;
  long			m_lCurrentStrongSell;
  long			m_lUnknownVolume;
  long			m_lCurrentUnknown;
  long			m_lCancelBuyVolume;		// �򵥳�����
  long			m_lCancelSellVolume;			// ����������

  long			m_lAttackBuyBelow50000;
  long			m_lAttackBuyBelow200000;
  long			m_lAttackBuyAbove200000;
  long			m_lAttackSellBelow50000;
  long			m_lAttackSellBelow200000;
  long			m_lAttackSellAbove200000;

  //
  long long	m_lFirstDataVolume;			  // ���ڴ洢��һ��ʵʱ���ݵĳɽ���

  deque<COneDealPtr>    m_dequeDeal;        // ����ɽ���Ϣ����

  // �ҵ��ľ��������
  map<long, long>       m_mapGuaDan;        // ����map�ṹ�洢�ҵ��ľ������������Ϊ��λ������Ϊ�ҵ�����
  CStockRTDataPtr       m_pLastRTData;        // ��m_dequeRTData��������һ��ʵʱ���ݡ�
  long                  m_lCurrentGuadanTransactionVolume; // ��ǰ�ҵ�������������Ŀǰ��ʱ�䣬����ʵʱ���ݶ�����ǰ�����ݵ�ʱ�䣩
  double                m_dCurrentGuaDanTransactionPrice; // ��ǰ�ɽ��۸�
  int                   m_nCurrentTransactionType; // ��ǰ�������ͣ�ǿ�򡢽��������롣��������
  long                  m_lCurrentCanselSellVolume;
  long                  m_lCurrentCanselBuyVolume;
  
  bool			            m_fDayLineLoaded;					// �Ƿ�װ������������

  bool                  m_fDayLineNeededSaving;   // ���������Ƿ���Ҫ�洢
  CCriticalSection      m_DayLineNeedSacingLock;

  deque<CStockRTDataPtr>m_dequeRTStockData;  // ʵʱ���ݶ���
  CCriticalSection      m_RTDataLock;   // ʵʱ���ݶ��е�ͬ����

  bool                  m_fStartCalculating;  // ʵʱ���ݿ�ʼ�����ʶ����һ��ʵʱ����ֻ��������ʼ��ϵͳ���������ڼ��㡣�ӵڶ������ݿ�ʼ�������Ч��
 	
	bool			            m_fChoiced;									// �˹�Ʊ�Ƿ�����ѡ��Ʊ.
	bool			            m_fMinLineUpdated;					// ����ķ��������Ƿ���¹�.
	bool			            m_fDayKLineUpdated;					// ��������������Ƿ���¹�.

};

typedef shared_ptr<CStock> CStockPtr;

