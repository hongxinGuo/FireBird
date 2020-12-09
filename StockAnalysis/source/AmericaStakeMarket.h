#pragma once

#include"stdafx.h"

#include"VirtualMarket.h"
#include"CompanyProfile.h"
#include"WebData.h"

// FinnHub���������ֵ��Ϊ���ȼ����жϱ�׼����ֵ������ȼ��ߣ�
enum {
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

struct FinnHubInquiry {
public:
  int m_iPriority; // ���ȼ�
  long m_iIndex; // ָ��
  bool operator() (FinnHubInquiry temp1, FinnHubInquiry temp2) {
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
  void GetFinnHubDataFromWeb(void);
  virtual void ResetMarket(void) override;
  void Reset(void);

  bool SchedulingTaskPerSecond(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer1Minute(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer1Hour(long lSecond, long lCurrentTime);
  bool TaskResetMarket(long lCurrentTime);

  bool TaskUpdateTodaySymbol(void);
  bool TaskSaveCompanySymbolDB(void);

  bool TaskUpdateCompanyProfile(void);
  bool TaskUpdateCompanyProfileDB(void);

  bool TaskUpdateDayLine(void);

  bool IsCompanyProfile(CString strProfile);
  bool IsCompanyProfileUpdated(void);
  CCompanyProfilePtr GetCompanyProfile(CString strTicker);
  CCompanyProfilePtr GetCurrentProcessingCompanyProfile(void) { return m_vCompanyProfile.at(m_lCurrentProfilePos); }
  void AddCompanyProfile(CCompanyProfilePtr pProfile);

  // ����״̬
  long GetCurrentPrefixIndex(void) noexcept { return m_lPrefixIndex; }

  void SetWaitingFinnHubData(bool fFlag) noexcept { m_fWaitingFinnHubData = fFlag; }
  bool IsWaitingFinHubData(void) noexcept { bool f = m_fWaitingFinnHubData; return f; }

  void SetFinnInquiry(long lOrder);
  long GetFinnInquiry(void);

  // ���ݿ����
  bool LoadCompanyProfile(void);
  bool SaveCompnayProfile(void);

protected:
  vector<CCompanyProfilePtr> m_vCompanyProfile;
  map<CString, long> m_mapCompanyProfile;
  long m_lLastTotalCompanyProfile;
  long m_lTotalCompanyProfile;
  long m_lCurrentProfilePos;
  long m_lCurrentUpdateDayLinePos;

  vector<CString> m_vFinnHubInquiringStr;
  long m_lPrefixIndex; // ��ǰ��ѯ״̬
  priority_queue<FinnHubInquiry, vector<FinnHubInquiry>, FinnHubInquiry> m_qWebInquiry; // �������ݲ�ѯ�������(�����ȼ���
  atomic_bool m_fWaitingFinnHubData;

  bool m_fCompanyProfileUpdated; // ÿ�ո��¹�˾���
  bool m_fStakeDayLineUpdated; // ÿ�ո��¹�˾���
  bool m_fSymbolUpdated; // ÿ�ո��¹�˾�����
  bool m_fSymbolProceeded;

  bool m_fInquiringComprofileData; // ��ѯ��˾�����
  bool m_fInquiringStakeCandle;
};

typedef shared_ptr<CAmericaStakeMarket> CAmericaStakeMarketPtr;
