#pragma once

#include"stdafx.h"

#include"VirtualMarket.h"
#include"CompanySymbol.h"
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

  __QUOTE__ = 1, // ʵʱ�������ȼ����
  __CANDLES__ = 2, // ��ʷ�������ȼ���

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
  bool TaskSaveCompanySymbol(void);
  bool TaskUpdateComProfile(void);
  bool TaskSaveCompanyProfile(void);

  bool IsCompanySymbol(CString strSymbol);
  void AddCompanySymbol(CCompanySymbolPtr pSymbol);
  void AddCompanyProfile(CCompanyProfilePtr pProfile);

  // ����״̬
  long GetCurrentPrefixIndex(void) noexcept { return m_lPrefixIndex; }

  void SetWaitingFinnHubData(bool fFlag) noexcept { m_fWaitingFinnHubData = fFlag; }
  bool IsWaitingFinHubData(void) noexcept { bool f = m_fWaitingFinnHubData; return f; }

  void SetFinnInquiry(long lOrder);
  long GetFinnInquiry(void);

  // ���ݿ����
  bool LoadCompanySymbol(void);
  bool SaveCompnaySymbol(void);
  bool LoadCompanyProfile(void);

protected:
  vector<CCompanySymbolPtr> m_vCompanySymbol;
  map<CString, long> m_mapConpanySymbol;
  long m_lLastTotalCompanySymbol;
  long m_lTotalCompanySymbol;

  vector<CCompanyProfilePtr> m_vCompanyProfile;
  long m_lTotalCompanyProfile;
  long m_lCurrentProfilePos;
  bool m_fInquiringComprofileData; // ��ѯ��˾�����

  vector<CString> m_vFinnHubInquiringStr;
  long m_lPrefixIndex; // ��ǰ��ѯ״̬
  priority_queue<FinnHubInquiry, vector<FinnHubInquiry>, FinnHubInquiry> m_qWebInquiry; // �������ݲ�ѯ�������(�����ȼ���
  atomic_bool m_fWaitingFinnHubData;

  bool m_fSymbolUpdated; // ÿ�ո��¹�˾�����
  bool m_fCompanyProfileUpdated; // ÿ�ո��¹�˾���
};

typedef shared_ptr<CAmericaStakeMarket> CAmericaStakeMarketPtr;
