#pragma once

#include"stdafx.h"

#include"VirtualMarket.h"

using namespace std;
#include<map>
#include<vector>

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

protected:
  vector<CString> m_vFinnHubInquiringStr;
  long m_lPrefixIndex; // ��ǰ��ѯ״̬

  bool m_fSymbolUpdated; // ÿ�ո��¹�˾�����
};

typedef shared_ptr<CAmericaStakeMarket> CAmericaStakeMarketPtr;
