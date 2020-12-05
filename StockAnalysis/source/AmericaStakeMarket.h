#pragma once

#include"stdafx.h"

#include"VirtualMarket.h"

// FinnHub���������ֵ��Ϊ���ȼ����жϱ�׼����ֵ������ȼ��ߣ�
enum {
  __COMPANY_PROFILE2__ = 101,
  __COMPANY_SYMBOLS__ = 102,
  __MARKET_NEWS__ = 103,
  __COMPANY_NEWS__ = 104,
  __NEWS_SETIMENTS__ = 105,
  __PEERS__ = 106,
  __BASIC_FINANCIALS__ = 107,

  __QUOTE__ = 1, // ʵʱ�������ȼ����
  __CANDLES__ = 2, // ��ʷ�������ȼ���

  __FOREX_EXCHANGE__ = 201,
  __FOREX_SYMBOLS__ = 202,
  __FOREX_CANDLES__ = 3, // ��ʷ�������ȼ���
  __FOREX_ALL_RATES__ = 204,
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
  void GetFinnHubDataFromWeb(void);
  virtual void ResetMarket(void) override;
  void Reset(void);

  bool SchedulingTaskPerSecond(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer1Minute(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer1Hour(long lSecond, long lCurrentTime);
  bool TaskResetMarket(long lCurrentTime);

  bool TaskUpdateTodaySymbol(void);

  // ����״̬
  long GetCurrentPrefixIndex(void) noexcept { return m_lPrefixIndex; }

  void SetWaitingFinnHubData(bool fFlag) noexcept { m_fWaitingFinnHubData = fFlag; }
  bool IsWaitingFinHubData(void) noexcept { bool f = m_fWaitingFinnHubData; return f; }

protected:
  vector<CString> m_vFinnHubInquiringStr;
  long m_lPrefixIndex; // ��ǰ��ѯ״̬
  priority_queue<int> m_qWebInquiry; // �������ݲ�ѯ�������(�����ȼ���
  atomic_bool m_fWaitingFinnHubData;

  bool m_fSymbolUpdated; // ÿ�ո��¹�˾�����
};

typedef shared_ptr<CAmericaStakeMarket> CAmericaStakeMarketPtr;
