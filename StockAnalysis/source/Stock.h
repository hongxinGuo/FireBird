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
	CStock( void );
	~CStock ( void );
	void Reset( void );

	void operator=( CStock & );

public:

	bool IsChoiced( void ) { return m_fChoiced; }
	void SetChoicedFlag( bool fChoiced ) { m_fChoiced = fChoiced; }

	bool IsMinLineUpdated( void ) { return ( m_fMinLineUpdated ); }
	void SetMinLineUpdated( bool fUpdate ) { m_fMinLineUpdated = fUpdate; }

	bool IsDayLineUpdated( void ) { return ( m_fDayKLineUpdated ); }
	void SetDayLineUpdated( bool fUpdate ) { m_fDayKLineUpdated = fUpdate; }

	void UpdataCurrentStatus(CStockRTDataPtr pRTData);
	bool LoadDayLine(CSetDayLine * psetDayLine);
	
  // ����ʵʱ����, �ɹ����߳�ClientThreadCalculatingRTDataProc����
  bool CalculateRTData(void);
  bool AnalysisingGuaDan(CStockRTDataPtr pCurrentRTData, CStockRTDataPtr pLastRTData, int nTransactionType, double dCurrentTransactionPrice);

	bool SaveRealTimeData(CSetRealTimeData * psetRT);

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
  // ������Ϣ
	WORD      m_wMarket;                // �г�����
  bool      m_fActive;                // �����Ƿ���ܵ�ʵʱ����
	CString		m_strStockCode;						// ֤ȯ���룬
	CString		m_strStockName;						// ֤ȯ����
  int       m_iStockCode;             // ֤ȯ����ֵ
	short			m_nHand;									// ÿ�ֹ���

	bool			m_fDayLineLoaded;					// �Ƿ�װ������������

	vector<CDayLinePtr>				m_vDayLine;			// ������������

  bool      m_fStartCalculating;  // ʵʱ���ݿ�ʼ�����ʶ����һ��ʵʱ����ֻ��������ʼ��ϵͳ���������ڼ��㡣�ӵڶ������ݿ�ʼ�������Ч��

	// ʵʱ����
	time_t    m_Time;
	long	    m_lLastClose;		// ��0.001Ԫ�Ƶ����̼�
	long		  m_lOpen;				// ��0.001Ԫ�ƵĿ��̼�		
	long		  m_lHigh;				// ��0.001Ԫ�Ƶ���߼�
	long		  m_lLow;					// ��0.001Ԫ�Ƶ���ͼ�
	long		  m_lNew;					// ��0.001Ԫ�Ƶ����¼�
	long long	m_lVolume;			// ��1�ɼƵĳɽ���
	long long m_lAmount;			// ��Ԫ�Ƶĳɽ����
  double    m_dRelativeStrong; // ��λ��1%
  long      m_lTransactionNumber; // �������յĳɽ�����
  long      m_lTransactionNumberBelow5000; // �������յ���5000�ɵĳɽ�����
  long      m_lTransactionNumberBelow50000; // 
  long      m_lTransactionNumberBelow200000; // 
  long      m_lTransactionNumberAbove200000; // 
	long		  m_lPBuy[5];			// ���̼ۡ���λ��0.001Ԫ
	long			m_lVBuy[5];			// ����������λ����
	long		  m_lPSell[5];		// ���̼ۡ���λ��0.001Ԫ
	long			m_lVSell[5];		// ����������λ����

	// ���±������ڷ������������������
  long      m_lSellPrice;     // �����۸�
  long      m_lBuyPrice;      // ����۸�

	long long	m_lAttackBuyAmount;		// ����������
	long long m_lAttackSellAmount;	// �����������
	long			m_lCurrentVolume;
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
 
public:

  deque<COneDealPtr>    m_dequeDeal;        // ����ɽ���Ϣ����

  // �ҵ��ľ��������
  map<long, long>       m_mapGuaDan;        // ����map�ṹ�洢�ҵ��ľ������������Ϊ��λ������Ϊ�ҵ�����
  long                  m_lLastClose2;        // �����̡���λ��0.01Ԫ��
  CStockRTDataPtr       m_pLastRTData;        // ��m_dequeRTData��������һ��ʵʱ���ݡ�
  long                  m_lCurrentGuadanTransactionVolume; // ��ǰ�ҵ�������������Ŀǰ��ʱ�䣬����ʵʱ���ݶ�����ǰ�����ݵ�ʱ�䣩
  double                m_dCurrentGuaDanTransactionPrice; // ��ǰ�ɽ��۸�
  int                   m_nCurrentTransactionType; // ��ǰ�������ͣ�ǿ�򡢽��������롣��������
  long                  m_lCurrentCanselSellVolume;
  long                  m_lCurrentCanselBuyVolume;
protected:

  bool                      m_fDayLineNeededSaving;   // ���������Ƿ���Ҫ�洢
  CCriticalSection          m_DayLineNeedSacingLock;

  deque<CStockRTDataPtr>		m_dequeRTStockData;  // ʵʱ���ݶ���
  CCriticalSection          m_RTDataLock;   // ʵʱ���ݶ��е�ͬ����

 	
	bool			                m_fChoiced;									// �˹�Ʊ�Ƿ�����ѡ��Ʊ.
	bool			                m_fMinLineUpdated;					// ����ķ��������Ƿ���¹�.
	bool			                m_fDayKLineUpdated;					// ��������������Ƿ���¹�.

};

typedef shared_ptr<CStock> CStockPtr;

