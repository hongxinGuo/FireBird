#include "AmericaStakeMarket.h"
#include"thread.h"

#include"WebInquirer.h"

#include"SetCompanySymbol.h"

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
  m_vFinnHubInquiringStr.at(__MARKET_NEWS__) = _T("https://finnhub.io/api/v1/news?category=general");
  m_vFinnHubInquiringStr.at(__COMPANY_NEWS__) = _T("https://finnhub.io/api/v1/company-news?symbol=");
  m_vFinnHubInquiringStr.at(__NEWS_SETIMENTS__) = _T("https://finnhub.io/api/v1/news-sentiment?symbol=");
  m_vFinnHubInquiringStr.at(__PEERS__) = _T("https://finnhub.io/api/v1/stock/peers?symbol=");
  m_vFinnHubInquiringStr.at(__BASIC_FINANCIALS__) = _T("https://finnhub.io/api/v1/stock/metric?symbol=");
  m_vFinnHubInquiringStr.at(__SEC_FILINGS__) = _T("https://finnhub.io/api/v1/stock/filings?symbol=");

  m_vFinnHubInquiringStr.at(__QUOTE__) = _T("https://finnhub.io/api/v1/quote?symbol="); // ĳ������Ľ���
  m_vFinnHubInquiringStr.at(__CANDLES__) = _T("https://finnhub.io/api/v1/stock/candle?symbol="); // ��ʷ����ͼ

  m_vFinnHubInquiringStr.at(__FOREX_EXCHANGE__) = _T("https://finnhub.io/api/v1/forex/exchange?");
  m_vFinnHubInquiringStr.at(__FOREX_SYMBOLS__) = _T("https://finnhub.io/api/v1/forex/symbol?exchange=oanda");
  m_vFinnHubInquiringStr.at(__FOREX_CANDLES__) = _T("https://finnhub.io/api/v1/forex/candle?symbol=OANDA:EUR_USD&resolution=D");
  m_vFinnHubInquiringStr.at(__FOREX_ALL_RATES__) = _T("https://finnhub.io/api/v1/forex/rates?base=USD");

  m_lPrefixIndex = -1; //
  Reset();
}

CAmericaStakeMarket::~CAmericaStakeMarket() {
}

void CAmericaStakeMarket::Reset(void) {
  m_fSymbolUpdated = false; // ÿ����Ҫ���´���
  m_lLastTotalCompanySymbol = 0;
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
        pWebData = gl_WebInquirer.PopFinnHubData();
        switch (m_lPrefixIndex) {
        case __COMPANY_PROFILE2__:
        break;
        case  __COMPANY_SYMBOLS__:
        ProcessCompanySymbolData(pWebData);
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
      m_lPrefixIndex = GetFinnInquiry();
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

  LoadCompanySymbol();

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
    SetFinnInquiry(__COMPANY_SYMBOLS__);
    //SetFinnInquiry(__MARKET_NEWS__);
    //SetFinnInquiry(__FOREX_EXCHANGE__);
    //SetFinnInquiry(__FOREX_SYMBOLS__);
    //SetFinnInquiry(__FOREX_CANDLES__);
    //SetFinnInquiry(__FOREX_ALL_RATES__);
    //SetFinnInquiry(__MARKET_NEWS__);
    m_fSymbolUpdated = true;
    return true;
  }
  return false;
}

bool CAmericaStakeMarket::ProcessCompanySymbolData(CWebDataPtr pWebData) {
  CCompanySymbolPtr pSymbol = nullptr;

  if (pWebData->GetCurrentPosData() != '[') return false;
  pWebData->IncreaseCurrentPos();
  while (pWebData->GetCurrentPos() < pWebData->GetBufferLength()) {
    if ((pSymbol = ReadOneSymbol(pWebData)) != nullptr) {
      if (m_mapConpanySymbol.find(pSymbol->m_strSymbol) == m_mapConpanySymbol.end()) { // �´��룿
        m_vCompanySymbol.push_back(pSymbol);
        m_mapConpanySymbol[pSymbol->m_strSymbol] = m_lTotalCompanySymbol++;
      }
    }
    else return false;
    if (!(pWebData->GetCurrentPosData() == ',')) { // �����ˣ�
      if (pWebData->GetCurrentPosData() == ']') return true; // ������
    }
    else {
      pWebData->IncreaseCurrentPos();
    }
  }
  return false;
}

map<CString, long> gl_mapCompanySymbol{ {"description", 0 }, {"displaySymbol", 1}, {"symbol", 2}, {"type", 3}, {"currency", 4} };

CCompanySymbolPtr CAmericaStakeMarket::ReadOneSymbol(CWebDataPtr pWebData) {
  CString strDescription = _T("");
  CString strSymbol;
  CString strDisplaySymbol;
  CString strType;
  CString strCurrency;
  CString strTemp, strTemp2;
  CCompanySymbolPtr pSymbol = make_shared<CCompanySymbol>();
  char buffer[1000];
  int i1 = 0;
  long lCurrentPos = pWebData->GetCurrentPos();
  while (pWebData->GetData(lCurrentPos) != '}') buffer[i1++] = pWebData->GetData(lCurrentPos++);
  buffer[i1] = '}';
  buffer[i1 + 1] = 0x000;

  while (pWebData->GetCurrentPosData() != '{') pWebData->IncreaseCurrentPos();
  pWebData->IncreaseCurrentPos(); // ����ַ�{
  for (int i = 0; i < 5; i++) { // ���������
    strTemp = ReadString(pWebData);
    if (gl_mapCompanySymbol.find(strTemp) != gl_mapCompanySymbol.end()) {
      while (pWebData->GetCurrentPosData() != '"') pWebData->IncreaseCurrentPos();
      strTemp2 = ReadString(pWebData);
      switch (gl_mapCompanySymbol.at(strTemp)) {
      case 0:
      pSymbol->m_strDescription = strTemp2;
      break;
      case 1:
      pSymbol->m_strDisplaySymbol = strTemp2;
      break;
      case 2:
      pSymbol->m_strSymbol = strTemp2;
      break;
      case 3:
      pSymbol->m_strType = strTemp2;
      break;
      case 4:
      pSymbol->m_strCurrency = strTemp2;
      break;
      default:
      break;
      }
      while (pWebData->GetCurrentPosData() != '"') pWebData->IncreaseCurrentPos();
    }
  }
  while (pWebData->GetCurrentPosData() != '}') pWebData->IncreaseCurrentPos();
  pWebData->IncreaseCurrentPos();
  return pSymbol;
}

CString CAmericaStakeMarket::ReadString(CWebDataPtr pWebData) {
  char buffer[100];
  int i = 0;
  CString str = _T("");

  if (pWebData->GetCurrentPosData() != '"') return str; // error
  pWebData->IncreaseCurrentPos();
  while (pWebData->GetCurrentPosData() != '"') {
    buffer[i++] = pWebData->GetCurrentPosData();
    pWebData->IncreaseCurrentPos();
  }
  pWebData->IncreaseCurrentPos();
  buffer[i] = 0x000;
  str = buffer;
  return str;
}

void CAmericaStakeMarket::SetFinnInquiry(long lOrder) {
  FinnHubInquiry inquiry;
  inquiry.m_iIndex = lOrder;
  switch (lOrder) {
  case __COMPANY_PROFILE2__:
  case __COMPANY_SYMBOLS__:
  case __MARKET_NEWS__:
  case __FOREX_EXCHANGE__:
  case __FOREX_SYMBOLS__:
  case __CRYPTO_EXCHANGE__:
  case __CRYPTO_SYMBOL__:
  inquiry.m_iPriority = 100;
  break;
  case __COMPANY_NEWS__:
  case __NEWS_SETIMENTS__:
  case __PEERS__:
  case __BASIC_FINANCIALS__:
  case __SEC_FILINGS__:
  inquiry.m_iPriority = 1;
  break;
  case __QUOTE__: // ʵʱ�������ȼ����
  case __CANDLES__: // ��ʷ�������ȼ���
  case __FOREX_CANDLES__: // ��ʷ�������ȼ���
  case __FOREX_ALL_RATES__:
  case __CRYPTO_CANDLES__:
  inquiry.m_iPriority = 10;
  break;
  default:
  TRACE("����FinnHubInquiry���ȼ�ʱ����δָ֪��%d\n", lOrder);
  inquiry.m_iPriority = 0;
  break;
  }
  m_qWebInquiry.push(inquiry);
}

long CAmericaStakeMarket::GetFinnInquiry(void) {
  FinnHubInquiry inquiry;
  if (m_qWebInquiry.size() > 0) {
    inquiry = m_qWebInquiry.top();
    m_qWebInquiry.pop();
    return inquiry.m_iIndex;
  }
  return -1;
}

bool CAmericaStakeMarket::LoadCompanySymbol(void) {
  CSetCompanySymbol setCompanySymbol;
  CCompanySymbolPtr pCompanySymbol = nullptr;

  setCompanySymbol.Open();
  setCompanySymbol.m_pDatabase->BeginTrans();
  while (!setCompanySymbol.IsEOF()) {
    pCompanySymbol = make_shared<CCompanySymbol>();
    pCompanySymbol->Load(setCompanySymbol);
    m_vCompanySymbol.push_back(pCompanySymbol);
    m_mapConpanySymbol[setCompanySymbol.m_Symbol] = m_lLastTotalCompanySymbol++;
    setCompanySymbol.MoveNext();
  }
  setCompanySymbol.m_pDatabase->CommitTrans();
  setCompanySymbol.Close();
  ASSERT(m_lLastTotalCompanySymbol == m_vCompanySymbol.size());
  return true;
}

bool CAmericaStakeMarket::SaveCompnaySymbol(void) {
  CSetCompanySymbol setCompanySymbol;
  CCompanySymbolPtr pCompanySymbol = nullptr;

  setCompanySymbol.Open();
  setCompanySymbol.m_pDatabase->BeginTrans();
  for (long l = m_lLastTotalCompanySymbol; l < m_vCompanySymbol.size(); l++) {
    pCompanySymbol = m_vCompanySymbol.at(l);
    pCompanySymbol->Save(setCompanySymbol);
  }
  setCompanySymbol.m_pDatabase->CommitTrans();
  setCompanySymbol.Close();
  m_lLastTotalCompanySymbol = m_vCompanySymbol.size();
  return true;
}