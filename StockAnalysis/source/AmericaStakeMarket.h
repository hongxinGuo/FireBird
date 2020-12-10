#pragma once

#include"stdafx.h"

#include"VirtualMarket.h"
#include"AmericaStake.h"
#include"WebData.h"

// Finnhub���������ֵ��Ϊ���ȼ����жϱ�׼����ֵ������ȼ��ߣ�
enum {
  __COMPANY_PROFILE__ = 100,
  __COMPANY_PROFILE2__ = 101,
  __COMPANY_SYMBOLS__ = 102,
  __MARKET_NEWS__ = 103,
  __COMPANY_NEWS__ = 104,
  __NEWS_SETIMENTS__ = 105,
  __PEERS__ = 106,
  __BASIC_FINANCIALS__ = 107,
  __SEC_FILINGS__ = 108,

  __STAKE_QUOTE__ = 1, // ʵʱ�������ȼ����
  __STAKE_CANDLES__ = 2, // ��ʷ�������ȼ���

  __FOREX_EXCHANGE__ = 201,
  __FOREX_SYMBOLS__ = 202,
  __FOREX_CANDLES__ = 3, // ��ʷ�������ȼ���
  __FOREX_ALL_RATES__ = 204,

  __CRYPTO_EXCHANGE__ = 301,
  __CRYPTO_SYMBOL__ = 302,
  __CRYPTO_CANDLES__ = 303,
};

struct FinnhubInquiry {
public:
  int m_iPriority; // ���ȼ�
  long m_iIndex; // ָ��
  bool operator() (FinnhubInquiry temp1, FinnhubInquiry temp2) {
    return temp1.m_iPriority < temp2.m_iPriority; // ���ȼ����λ��ǰ��
  }
};

using namespace std;
#include<map>
#include<vector>
#include<queue>
#include<atomic>

class CAmericaStakeMarket : public CVirtualMarket {
public:
  CAmericaStakeMarket();
  virtual ~CAmericaStakeMarket();

  virtual bool SchedulingTask(void) override; // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��
  void GetFinnhubDataFromWeb(void);
  virtual void ResetMarket(void) override;
  void Reset(void);

  bool SchedulingTaskPerSecond(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer10Seconds(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer1Minute(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer1Hour(long lSecond, long lCurrentTime);
  bool TaskResetMarket(long lCurrentTime);

  bool TaskUpdateTodaySymbol(void);
  bool TaskSaveStakeSymbolDB(void);

  bool TaskUpdateAmericaStake(void);
  bool TaskUpdateAmericaStakeDB(void);

  bool TaskUpdateDayLine(void);

  bool TaskSaveDayLineData(void);

  // �������̵߳��ð�������
  virtual bool RunningThreadSaveStakeDayLine(CAmericaStakePtr pStake);

  bool IsAmericaStake(CString strProfile);
  bool IsAmericaStakeUpdated(void);
  CAmericaStakePtr GetAmericaStake(CString strTicker);
  CAmericaStakePtr GetCurrentProcessingAmericaStake(void) { return m_vAmericaStake.at(m_lCurrentProfilePos); }
  void AddAmericaStake(CAmericaStakePtr pStake);

  // ����״̬
  long GetCurrentPrefixIndex(void) noexcept { return m_lPrefixIndex; }

  void SetWaitingFinnhubData(bool fFlag) noexcept { m_fWaitingFinnhubData = fFlag; }
  bool IsWaitingFinHubData(void) noexcept { bool f = m_fWaitingFinnhubData; return f; }

  void SetFinnInquiry(long lOrder);
  long GetFinnInquiry(void);

  // ���ݿ����
  bool LoadAmericaStake(void);
  bool SaveCompnayProfile(void);

protected:
  vector<CAmericaStakePtr> m_vAmericaStake;
  map<CString, long> m_mapAmericaStake;
  long m_lLastTotalAmericaStake;
  long m_lTotalAmericaStake;
  long m_lCurrentProfilePos;
  long m_lCurrentUpdateDayLinePos;

  vector<CString> m_vFinnhubInquiringStr;
  long m_lPrefixIndex; // ��ǰ��ѯ״̬
  priority_queue<FinnhubInquiry, vector<FinnhubInquiry>, FinnhubInquiry> m_qWebInquiry; // �������ݲ�ѯ�������(�����ȼ���
  atomic_bool m_fWaitingFinnhubData;

  bool m_fAmericaStakeUpdated; // ÿ�ո��¹�˾���
  bool m_fStakeDayLineUpdated; // ÿ�ո��¹�˾���
  bool m_fSymbolUpdated; // ÿ�ո��¹�˾�����
  bool m_fSymbolProceeded;

  bool m_fInquiringStakeProfileData; // ��ѯ��˾�����
  bool m_fInquiringStakeCandle;
};

typedef shared_ptr<CAmericaStakeMarket> CAmericaStakeMarketPtr;
