#include "AmericaStakeMarket.h"
#include"thread.h"

#include"WebInquirer.h"

CAmericaStakeMarket::CAmericaStakeMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("CAmericaStakeMarket�г�����ֻ�������һ��ʵ��\n");
  }

  m_strMarketId = _T("FinnHub������Ϣ");
  m_lMarketTimeZone = 4 * 3600; // ��������ʹ��������׼ʱ�䡣
  CalculateTime();

  m_vFinnHubInquiringStr.resize(1000);

  // FinnHubǰ׺�ַ����ڴ�Ԥ��֮
  m_vFinnHubInquiringStr.at(__COMPANY_PROFILE2__) = _T("https://finnhub.io/api/v1/stock/profile2?symbol="); // ��˾���
  m_vFinnHubInquiringStr.at(__COMPANY_SYMBOLS__) = _T("https://finnhub.io/api/v1/stock/symbol?exchange=US"); // ���ô��뼯
  m_vFinnHubInquiringStr.at(__QUOTE__) = _T("https://finnhub.io/api/v1/quote?symbol="); // ĳ������Ľ���
  m_vFinnHubInquiringStr.at(__CANDLES__) = _T("https://finnhub.io/api/v1/stock/candle?symbol="); // ��ʷ����ͼ

  m_lPrefixIndex = -1; //

  Reset();
}

CAmericaStakeMarket::~CAmericaStakeMarket() {
}

void CAmericaStakeMarket::Reset(void) {
  m_fSymbolUpdated = false; // ÿ����Ҫ���´���
}

bool CAmericaStakeMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask();

  static time_t s_timeLast = 0;
  const long lCurrentTime = GetFormatedMarketTime();

  //����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
  if (GetMarketTime() > s_timeLast) {
    SchedulingTaskPerSecond(GetMarketTime() - s_timeLast, lCurrentTime);
    s_timeLast = GetMarketTime();
  }

  return true;
}

void CAmericaStakeMarket::GetFinnHubDataFromWeb(void) {
  static bool s_fWaitData = false;
  CWebDataPtr pWebData = nullptr;

  if (s_fWaitData) {// �Ѿ��������������룿
    if (!IsWaitingFinHubData()) {
      s_fWaitData = false;
      if (gl_WebInquirer.GetFinnHubDataSize() > 0) { // ����������ݽ��յ���
        // ����ǰ��������
        switch (m_lPrefixIndex) {
        case __COMPANY_PROFILE2__:
        break;
        case  __COMPANY_SYMBOLS__:
        pWebData = gl_WebInquirer.PopFinnHubData();
        break;
        case  __MARKET_NEWS__:
        break;
        case __COMPANY_NEWS__:
        break;
        case __NEWS_SETIMENTS__:
        break;
        case __PEERS__:
        break;
        case __BASIC_FINANCIALS__:
        break;
        case __QUOTE__:
        break;
        case __CANDLES__:
        break;
        case __FOREX_EXCHANGE__:
        break;
        case __FOREX_SYMBOLS__:
        break;
        case __FOREX_CANDLES__:
        break;
        case __FOREX_ALL_RATES__:
        break;
        default:
        break;
        }
      }
    }
  }
  else { // û����������������Ļ�
    if (m_qWebInquiry.size() > 0) { // ������ȴ���
      m_lPrefixIndex = m_qWebInquiry.top();
      m_qWebInquiry.pop();
      gl_pFinnhubWebInquiry->SetInquiryingStrPrefix(m_vFinnHubInquiringStr.at(m_lPrefixIndex)); // ����ǰ׺
      switch (m_lPrefixIndex) {
      case __COMPANY_PROFILE2__:
      break;
      case  __COMPANY_SYMBOLS__:
      break;
      case  __MARKET_NEWS__:
      break;
      case __COMPANY_NEWS__:
      break;
      case __NEWS_SETIMENTS__:
      break;
      case __PEERS__:
      break;
      case __BASIC_FINANCIALS__:
      break;
      case __QUOTE__:
      break;
      case __CANDLES__:
      break;
      case __FOREX_EXCHANGE__:
      break;
      case __FOREX_SYMBOLS__:
      break;
      case __FOREX_CANDLES__:
      break;
      case __FOREX_ALL_RATES__:
      break;
      default:
      break;
      }
      gl_pFinnhubWebInquiry->GetWebData();
      s_fWaitData = true;
      SetWaitingFinnHubData(true);
    }
  }
}

void CAmericaStakeMarket::ResetMarket(void) {
  Reset();
  CString str = _T("����America Stake Market��������׼ʱ�䣺");
  str += GetStringOfMarketTime();
  gl_systemMessage.PushInformationMessage(str);
}

bool CAmericaStakeMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
  static int s_iCount = 1;

  SchedulingTaskPer1Hour(lSecond, lCurrentTime);
  SchedulingTaskPer1Minute(lSecond, lCurrentTime);

  if (s_iCount-- < 0) { // ÿ��������һ��FinnHub����
    s_iCount = 1;
    GetFinnHubDataFromWeb();
  }

  TaskUpdateTodaySymbol();

  return true;
}

bool CAmericaStakeMarket::SchedulingTaskPer1Hour(long lSecond, long lCurrentTime) {
  static int i1MinuteCounter = 3599;  // һ����һ�εļ�����

  i1MinuteCounter -= lSecond;
  if (i1MinuteCounter < 0) {
    i1MinuteCounter = 3599;
    TaskResetMarket(lCurrentTime);
    //TaskMaintainDatabase(lCurrentTime);

    return true;
  }
  else return false;
}

bool CAmericaStakeMarket::SchedulingTaskPer1Minute(long lSecond, long lCurrentTime) {
  static int i1MinuteCounter = 59;  // һСʱһ�εļ�����

  TaskResetMarket(lCurrentTime);

  // �Զ���ѯcrweber.com
  i1MinuteCounter -= lSecond;
  if (i1MinuteCounter < 0) {
    i1MinuteCounter = 59;
    return true;
  }
  else {
    return false;
  }
}

bool CAmericaStakeMarket::TaskResetMarket(long lCurrentTime) {
  // �г�ʱ���ĵ�����ϵͳ
  if (IsPermitResetMarket()) { // �����������ϵͳ
    if ((lCurrentTime >= 29000) && (lCurrentTime < 30000)) { // ���г�ʱ���һ���������г� // ����������ʱ��Ϊ������ʱ��������ʱ����ʱ����
      SetResetMarket(true);// ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
      SetPermitResetMarket(false); // ���첻����������ϵͳ��
    }
  }
  return true;
}

bool CAmericaStakeMarket::TaskUpdateTodaySymbol(void) {
  if (!m_fSymbolUpdated) {
    m_qWebInquiry.push(__COMPANY_SYMBOLS__);
    m_fSymbolUpdated = true;
    return true;
  }
  return false;
}