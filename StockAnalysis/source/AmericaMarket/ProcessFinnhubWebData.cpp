#include"stdafx.h"
#include"globedef.h"
#include"accessory.h"

#include"WebData.h"
#include"EconomicCalendar.h"
#include"ProcessFinnhubWebData.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

static char s_buffer[1024 * 1024 * 8];

bool CompareEPSSurprise(CEPSSurprisePtr& p1, CEPSSurprisePtr& p2) { return (p1->m_lDate < p2->m_lDate); }
bool CompareDayLineDate(CDayLinePtr& p1, CDayLinePtr& p2) { return p1->GetFormatedMarketDate() < p2->GetFormatedMarketDate(); }

bool ProcessFinnhubStockProfile(CWebDataPtr pWebData, CAmericaStakePtr& pStake) {
  ptree pt;
  string s;

  if (pWebData->GetBufferLength() < 20) {
    pStake->m_lProfileUpdateDate = gl_pAmericaMarket->GetFormatedMarketDate();
    pStake->m_fUpdateDatabase = true;
    return false; // 没有公司简介
  }
  if (!ConvertToJSon(pt, pWebData)) return false;
  try {
    s = pt.get<string>(_T("address"));
    pStake->m_strAddress = s.c_str();
  }
  catch (ptree_error&) {
    pStake->m_lProfileUpdateDate = gl_pAmericaMarket->GetFormatedMarketDate();
    pStake->m_fUpdateDatabase = true;
    return false; // 没有公司简介
  }
  s = pt.get<string>(_T("city"));
  pStake->m_strCity = s.c_str();
  s = pt.get<string>(_T("country"));
  if (s.size() > 0) pStake->m_strCountry = s.c_str();
  s = pt.get<string>(_T("currency"));
  if (s.size() > 0) pStake->m_strCurrency = s.c_str();
  s = pt.get<string>(_T("cusip"));
  if (s.size() > 0) pStake->m_strCusip = s.c_str();
  s = pt.get<string>(_T("sedol"));
  if (s.size() > 0) pStake->m_strSedol = s.c_str();
  s = pt.get<string>(_T("description"));
  if (s.size() > 0) pStake->m_strDescription = s.c_str();
  s = pt.get<string>(_T("exchange"));
  if (s.size() > 0) pStake->m_strExchange = s.c_str();
  s = pt.get<string>(_T("ggroup"));
  if (s.size() > 0) pStake->m_strGgroup = s.c_str();
  s = pt.get<string>(_T("gind"));
  if (s.size() > 0) pStake->m_strGind = s.c_str();
  s = pt.get<string>(_T("gsector"));
  if (s.size() > 0) pStake->m_strGsector = s.c_str();
  s = pt.get<string>(_T("gsubind"));
  if (s.size() > 0) pStake->m_strGsubind = s.c_str();
  s = pt.get<string>(_T("ipo"));
  if (s.size() > 0) pStake->m_strIPODate = s.c_str();
  s = pt.get<string>(_T("isin"));
  if (s.size() > 0) pStake->m_strIsin = s.c_str();
  s = pt.get<string>(_T("marketCapitalization"));
  if (s.size() > 0) pStake->m_dMarketCapitalization = atof(s.c_str());

  s = pt.get<string>(_T("naics"));
  if (s.size() > 0) pStake->m_strNaics = s.c_str();
  s = pt.get<string>(_T("naicsNationalIndustry"));
  if (s.size() > 0) pStake->m_strNaicsNationalIndustry = s.c_str();
  s = pt.get<string>(_T("naicsSector"));
  if (s.size() > 0) pStake->m_strNaicsSector = s.c_str();
  s = pt.get<string>(_T("naicsSubsector"));
  if (s.size() > 0) pStake->m_strNaicsSubsector = s.c_str();
  s = pt.get<string>(_T("name"));
  if (s.size() > 0) pStake->m_strName = s.c_str();
  s = pt.get<string>(_T("phone"));
  if (s.size() > 0) pStake->m_strPhone = s.c_str();

  s = pt.get<string>(_T("shareOutstanding"));
  if (s.size() > 0) pStake->m_dShareOutstanding = atof(s.c_str());
  s = pt.get<string>(_T("state"));
  if (s.size() > 0) pStake->m_strState = s.c_str();
  s = pt.get<string>(_T("ticker"));
  if (s.size() > 0) pStake->m_strTicker = s.c_str();
  s = pt.get<string>(_T("weburl"));
  if (s.size() > 0) pStake->m_strWebURL = s.c_str();

  s = pt.get<string>(_T("logo"));
  if (s.size() > 0) pStake->m_strLogo = s.c_str();
  s = pt.get<string>(_T("finnhubIndustry"));
  if (s.size() > 0) pStake->m_strFinnhubIndustry = s.c_str();
  pStake->m_lProfileUpdateDate = gl_pAmericaMarket->GetFormatedMarketDate();
  pStake->m_fUpdateDatabase = true;
  return true;
}

bool ProcessFinnhubStockProfile2(CWebDataPtr pWebData, CAmericaStakePtr& pStake) {
  ptree pt;
  string s;

  TRACE("处理%s简介\n", pStake->m_strSymbol.GetBuffer());
  if (pWebData->GetBufferLength() < 20) {
    pStake->m_lProfileUpdateDate = gl_pAmericaMarket->GetFormatedMarketDate();
    pStake->m_fUpdateDatabase = true;
    return false; // 没有公司简介
  }
  if (!ConvertToJSon(pt, pWebData)) return false;
  try {
    s = pt.get<string>(_T("ticker"));
    if (s.size() > 0) pStake->m_strTicker = s.c_str();
  }
  catch (ptree_error&) {
    pStake->m_lProfileUpdateDate = gl_pAmericaMarket->GetFormatedMarketDate();
    pStake->m_fUpdateDatabase = true;
    return false; // 没有公司简介
  }
  s = pt.get<string>(_T("country"));
  if (s.size() > 0) pStake->m_strCountry = s.c_str();
  s = pt.get<string>(_T("currency"));
  if (s.size() > 0) pStake->m_strCurrency = s.c_str();
  s = pt.get<string>(_T("exchange"));
  if (s.size() > 0) pStake->m_strExchange = s.c_str();
  s = pt.get<string>(_T("name"));
  if (s.size() > 0) pStake->m_strName = s.c_str();
  s = pt.get<string>(_T("finnhubIndustry"));
  if (s.size() > 0) pStake->m_strFinnhubIndustry = s.c_str();
  s = pt.get<string>(_T("logo"));
  if (s.size() > 0) pStake->m_strLogo = s.c_str();
  s = pt.get<string>(_T("marketCapitalization"));
  if (s.size() > 0) pStake->m_dMarketCapitalization = atof(s.c_str());
  s = pt.get<string>(_T("phone"));
  if (s.size() > 0) pStake->m_strPhone = s.c_str();
  if (s.size() > 0) pStake->m_dShareOutstanding = pt.get<double>(_T("shareOutstanding"));
  s = pt.get<string>(_T("weburl"));
  if (s.size() > 0) pStake->m_strWebURL = s.c_str();
  s = pt.get<string>(_T("ipo"));
  if (s.size() > 0) pStake->m_strIPODate = s.c_str();
  return true;
}

bool CompareAmericaStake(CAmericaStakePtr p1, CAmericaStakePtr p2) { return (p1->m_strSymbol.Compare(p2->m_strSymbol) < 0); }

bool ProcessFinnhubStockSymbol(CWebDataPtr pWebData, vector<CAmericaStakePtr>& vStake) {
  CAmericaStakePtr pStake = make_shared<CAmericaStake>();
  ptree pt, pt2;
  string s;
  bool fFoundNewSymbol = false;

  if (!ConvertToJSon(pt, pWebData)) return false;
  for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
    pStake = make_shared<CAmericaStake>();
    pt2 = it->second;
    s = pt2.get<string>(_T("description"));
    if (s.size() > 0) pStake->m_strDescription = s.c_str();
    s = pt2.get<string>(_T("displaySymbol"));
    pStake->m_strDisplaySymbol = s.c_str();
    s = pt2.get<string>(_T("symbol"));
    pStake->m_strSymbol = s.c_str();
    s = pt2.get<string>(_T("type"));
    if (s.size() > 0) pStake->m_strType = s.c_str();
    s = pt2.get<string>(_T("mic"));
    if (s.size() > 0) pStake->m_strMic = s.c_str();
    s = pt2.get<string>(_T("figi"));
    if (s.size() > 0) pStake->m_strFigi = s.c_str();
    s = pt2.get<string>(_T("currency"));
    if (s.size() > 0) pStake->m_strCurrency = s.c_str();
    if (!gl_pAmericaMarket->IsAmericaStake(pStake->m_strSymbol)) { // 新代码？
      vStake.push_back(pStake);
      fFoundNewSymbol = true;
    }
  }
  sort(vStake.begin(), vStake.end(), CompareAmericaStake);

  return fFoundNewSymbol;
}

bool ProcessFinnhubStockCandle(CWebDataPtr pWebData, CAmericaStakePtr& pStake) {
  vector<CDayLinePtr> vDayLine;
  ptree pt, pt2, pt3;
  string s;
  double dTemp = 0;
  long lTemp = 0;
  INT64 llTemp = 0;
  time_t tTemp = 0;
  CDayLinePtr pDayLine = nullptr;
  int i = 0;
  CString str;

  if (!ConvertToJSon(pt, pWebData)) { // 工作线程故障
    str = _T("下载");
    str += pStake->m_strSymbol;
    str += _T("日线故障\n");
    TRACE("%s", str.GetBuffer());
    gl_systemMessage.PushInformationMessage(str);
    gl_systemMessage.PushInnerSystemInformationMessage(str);
    return false;
  }
  try {
    s = pt.get<string>(_T("s"));
    if (s.compare(_T("no_data")) == 0) { // 没有日线数据，无需检查此股票的日线和实时数据
      pStake->SetIPOStatus(__STAKE_NULL__);
      pStake->m_fUpdateDatabase = true;
      return true;
    }
    if (s.compare(_T("ok")) != 0) {
      str = _T("下载");
      str += pStake->m_strSymbol;
      str += _T("日线返回值不为ok\n");
      TRACE("%s", str.GetBuffer());
      gl_systemMessage.PushInformationMessage(str);
      gl_systemMessage.PushInnerSystemInformationMessage(str);
      return false;
    }
  }
  catch (ptree_error&) { // 这种请况是此代码出现问题。如服务器返回"error":"you don't have access this resource."
    pStake->SetIPOStatus(__STAKE_NULL__);
    pStake->m_fUpdateDatabase = true;
    return true;
  }
  try {
    pt2 = pt.get_child(_T("t"));
    for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
      pt3 = it->second;
      tTemp = pt3.get_value<time_t>();
      pDayLine = make_shared<CDayLine>();
      pDayLine->SetMarketString(pStake->m_strExchange);
      pDayLine->SetStakeCode(pStake->GetSymbol());
      pDayLine->SetStakeName(pStake->GetTicker());
      pDayLine->SetTime(tTemp);
      lTemp = FormatToDate(tTemp);
      pDayLine->SetDate(lTemp);
      vDayLine.push_back(pDayLine);
    }
  }
  catch (ptree_error&) {
    return false;
  }
  try {
    pt2 = pt.get_child(_T("c"));
    i = 0;
    for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
      pt3 = it->second;
      dTemp = pt3.get_value<double>();
      pDayLine = vDayLine.at(i++);
      pDayLine->SetClose(dTemp * 1000);
    }
  }
  catch (ptree_error&) {
  }
  try {
    pt2 = pt.get_child(_T("o"));
    i = 0;
    for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
      pt3 = it->second;
      dTemp = pt3.get_value<double>();
      pDayLine = vDayLine.at(i++);
      pDayLine->SetOpen(dTemp * 1000);
    }
  }
  catch (ptree_error&) {
  }
  try {
    pt2 = pt.get_child(_T("h"));
    i = 0;
    for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
      pt3 = it->second;
      dTemp = pt3.get_value<double>();
      pDayLine = vDayLine.at(i++);
      pDayLine->SetHigh(dTemp * 1000);
    }
  }
  catch (ptree_error&) {
  }
  try {
    pt2 = pt.get_child(_T("l"));
    i = 0;
    for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
      pt3 = it->second;
      dTemp = pt3.get_value<double>();
      pDayLine = vDayLine.at(i++);
      pDayLine->SetLow(dTemp * 1000);
    }
  }
  catch (ptree_error&) {
  }
  try {
    pt2 = pt.get_child(_T("v"));
    i = 0;
    for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
      pt3 = it->second;
      llTemp = pt3.get_value<INT64>();
      pDayLine = vDayLine.at(i++);
      pDayLine->SetVolume(llTemp);
    }
  }
  catch (ptree_error&) {
  }
  pStake->SetIPOStatus(__STAKE_IPOED__);
  sort(vDayLine.begin(), vDayLine.end(), CompareDayLineDate); // 以日期早晚顺序排列。
  pStake->UpdateDayLine(vDayLine);
  pStake->SetDayLineNeedUpdate(false);
  pStake->SetDayLineNeedSaving(true);
  pStake->m_fUpdateDatabase = true;
  return true;
}

bool ProcessFinnhubStockQuote(CWebDataPtr pWebData, CAmericaStakePtr& pStake) {
  string s;
  ptree pt;
  double dTemp = 0;
  time_t tt = 0;

  if (!ConvertToJSon(pt, pWebData)) {
    return false;
  }
  try {
    dTemp = pt.get<double>(_T("pc"));
    pStake->SetLastClose(dTemp * 1000);
  }
  catch (ptree_error&) { // 数据格式不对，跳过。
    return false;
  }
  dTemp = pt.get<double>(_T("o"));
  pStake->SetOpen(dTemp * 1000);
  dTemp = pt.get<double>(_T("h"));
  pStake->SetHigh(dTemp * 1000);
  dTemp = pt.get<double>(_T("l"));
  pStake->SetLow(dTemp * 1000);
  dTemp = pt.get<double>(_T("c"));
  pStake->SetNew(dTemp * 1000);
  tt = pt.get<time_t>(_T("t"));
  pStake->SetTransactionTime(tt);
  pStake->SetActive(true); // 本日有实时数据

  return true;
}

bool ProcessFinnhubForexExchange(CWebDataPtr pWebData, vector<CString>& vExchange) {
  ptree pt, pt2;
  string s;
  CString str = _T("");

  if (!ConvertToJSon(pt, pWebData)) return false;
  for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
    pt2 = it->second;
    s = pt2.get_value<string>();
    str = s.c_str();
    vExchange.push_back(str);
  }

  return true;
}

bool ProcessFinnhubForexSymbol(CWebDataPtr pWebData, vector<CForexSymbolPtr>& vForexSymbol) {
  CForexSymbolPtr pSymbol = make_shared<CForexSymbol>();
  ptree pt, pt2;
  string s;

  if (!ConvertToJSon(pt, pWebData)) return false;
  for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
    pSymbol = make_shared<CForexSymbol>();
    pt2 = it->second;
    s = pt2.get<string>(_T("description"));
    if (s.size() > 0) pSymbol->m_strDescription = s.c_str();
    s = pt2.get<string>(_T("displaySymbol"));
    pSymbol->m_strDisplaySymbol = s.c_str();
    s = pt2.get<string>(_T("symbol"));
    pSymbol->m_strSymbol = s.c_str();
    vForexSymbol.push_back(pSymbol);
  }

  return true;
}

bool ProcessFinnhubForexCandle(CWebDataPtr pWebData, CForexSymbolPtr& pForexSymbol) {
  vector<CDayLinePtr> vDayLine;
  ptree pt, pt2, pt3;
  string s;
  double dTemp = 0;
  long lTemp = 0;
  INT64 llTemp = 0;
  time_t tTemp = 0;
  CDayLinePtr pDayLine = nullptr;
  int i = 0;
  CString str;

  if (!ConvertToJSon(pt, pWebData)) { // 工作线程故障
    str = _T("下载");
    str += pForexSymbol->m_strSymbol;
    str += _T("日线故障\n");
    TRACE("%s", str.GetBuffer());
    gl_systemMessage.PushInformationMessage(str);
    gl_systemMessage.PushInnerSystemInformationMessage(str);
    return false;
  }
  try {
    s = pt.get<string>(_T("s"));
    if (s.compare(_T("no_data")) == 0) { // 没有日线数据，无需检查此股票的日线和实时数据
      pForexSymbol->SetIPOStatus(__STAKE_NULL__);
      pForexSymbol->m_fUpdateDatabase = true;
      return true;
    }
    if (s.compare(_T("ok")) != 0) {
      str = _T("下载");
      str += pForexSymbol->m_strSymbol;
      str += _T("日线返回值不为ok\n");
      TRACE("%s", str.GetBuffer());
      gl_systemMessage.PushInformationMessage(str);
      gl_systemMessage.PushInnerSystemInformationMessage(str);
      return false;
    }
  }
  catch (ptree_error&) { // 这种请况是此代码出现问题。如服务器返回"error":"you don't have access this resource."
    pForexSymbol->SetIPOStatus(__STAKE_NULL__);
    pForexSymbol->m_fUpdateDatabase = true;
    return true;
  }
  try {
    pt2 = pt.get_child(_T("t"));
    for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
      pt3 = it->second;
      tTemp = pt3.get_value<time_t>();
      pDayLine = make_shared<CDayLine>();
      pDayLine->SetMarketString(pForexSymbol->m_strExchange);
      pDayLine->SetStakeCode(pForexSymbol->GetSymbol());
      pDayLine->SetTime(tTemp);
      lTemp = FormatToDate(tTemp);
      pDayLine->SetDate(lTemp);
      vDayLine.push_back(pDayLine);
    }
  }
  catch (ptree_error&) {
    return false;
  }
  try {
    pt2 = pt.get_child(_T("c"));
    i = 0;
    for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
      pt3 = it->second;
      dTemp = pt3.get_value<double>();
      pDayLine = vDayLine.at(i++);
      pDayLine->SetClose(dTemp * 1000);
    }
  }
  catch (ptree_error&) {
  }
  try {
    pt2 = pt.get_child(_T("o"));
    i = 0;
    for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
      pt3 = it->second;
      dTemp = pt3.get_value<double>();
      pDayLine = vDayLine.at(i++);
      pDayLine->SetOpen(dTemp * 1000);
    }
  }
  catch (ptree_error&) {
  }
  try {
    pt2 = pt.get_child(_T("h"));
    i = 0;
    for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
      pt3 = it->second;
      dTemp = pt3.get_value<double>();
      pDayLine = vDayLine.at(i++);
      pDayLine->SetHigh(dTemp * 1000);
    }
  }
  catch (ptree_error&) {
  }
  try {
    pt2 = pt.get_child(_T("l"));
    i = 0;
    for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
      pt3 = it->second;
      dTemp = pt3.get_value<double>();
      pDayLine = vDayLine.at(i++);
      pDayLine->SetLow(dTemp * 1000);
    }
  }
  catch (ptree_error&) {
  }
  try {
    pt2 = pt.get_child(_T("v"));
    i = 0;
    for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
      pt3 = it->second;
      llTemp = pt3.get_value<INT64>();
      pDayLine = vDayLine.at(i++);
      pDayLine->SetVolume(llTemp);
    }
  }
  catch (ptree_error&) {
    // 有些外汇交易不提供成交量，忽略就可以了
  }
  pForexSymbol->SetIPOStatus(__STAKE_IPOED__);
  sort(vDayLine.begin(), vDayLine.end(), CompareDayLineDate);
  pForexSymbol->UpdateDayLine(vDayLine);
  pForexSymbol->SetDayLineNeedUpdate(false);
  pForexSymbol->SetDayLineNeedSaving(true);
  pForexSymbol->m_fUpdateDatabase = true;
  return true;
}

bool ProcessFinnhubCountryList(CWebDataPtr pWebData, vector<CCountryPtr>& vCountry) {
  CCountryPtr pCountry = nullptr;
  ptree pt, pt2;
  string s;

  if (!ConvertToJSon(pt, pWebData)) return false;
  for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
    pCountry = make_shared<CCountry>();
    pt2 = it->second;
    try {
      s = pt2.get<string>(_T("code2"));
      if (s.size() > 0) pCountry->m_strCode2 = s.c_str();
      s = pt2.get<string>(_T("code3"));
      pCountry->m_strCode3 = s.c_str();
      s = pt2.get<string>(_T("codeNo"));
      pCountry->m_strCodeNo = s.c_str();
      s = pt2.get<string>(_T("country"));
      pCountry->m_strCountry = s.c_str();
      s = pt2.get<string>(_T("currency"));
      pCountry->m_strCurrency = s.c_str();
      s = pt2.get<string>(_T("currencyCode"));
      pCountry->m_strCurrencyCode = s.c_str();
    }
    catch (ptree_error&) {
    }
    vCountry.push_back(pCountry);
  }
  return true;
}

bool ProcessFinnhubStockPeer(CWebDataPtr pWebData, CAmericaStakePtr& pStake) {
  char buffer[1000]{};
  int i = 0;

  ASSERT(pWebData->GetBufferLength() < 1000);
  for (i = 0; i < pWebData->GetBufferLength(); i++) {
    buffer[i] = pWebData->GetData(i);
  }
  if (i > 200) {
    buffer[200] = 0x000;
  }
  else {
    buffer[pWebData->GetBufferLength()] = 0x000;
  }
  pStake->m_strPeer = buffer;

  return true;
}

bool ProcessFinnhubEconomicCalendar(CWebDataPtr pWebData, vector<CEconomicCalendarPtr>& vEconomicCalendar) {
  CEconomicCalendarPtr pEconomicCalendar = nullptr;
  ptree pt, pt2;
  string s;

  if (!ConvertToJSon(pt, pWebData)) return false;
  for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
    pEconomicCalendar = make_shared<CEconomicCalendar>();
    pt2 = it->second;
    try {
      s = pt2.get<string>(_T("country"));
      if (s.size() > 0) pEconomicCalendar->m_strCountry = s.c_str();
      s = pt2.get<string>(_T("event"));
      pEconomicCalendar->m_strEvent = s.c_str();
      s = pt2.get<string>(_T("impact"));
      pEconomicCalendar->m_strImpact = s.c_str();
      pEconomicCalendar->m_dEstimate = pt2.get<double>(_T("estimate"));
      pEconomicCalendar->m_dEstimate = pt2.get<double>(_T("actual"));
      pEconomicCalendar->m_dPrev = pt2.get<double>(_T("prev"));
      s = pt2.get<string>(_T("time"));
      pEconomicCalendar->m_strTime = s.c_str();
      s = pt2.get<string>(_T("unit"));
      pEconomicCalendar->m_strUnit = s.c_str();
    }
    catch (ptree_error&) {
    }
    vEconomicCalendar.push_back(pEconomicCalendar);
  }
  return true;
}

bool ProcessFinnhubEPSSurprise(CWebDataPtr pWebData, vector<CEPSSurprisePtr>& vEPSSurprise) {
  ptree pt, pt2;
  string s;
  CTime time;
  CEPSSurprisePtr pEPSSurprise = nullptr;
  long year, month, day;
  CString str;

  if (!ConvertToJSon(pt, pWebData)) return false;
  for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
    pEPSSurprise = make_shared<CEPSSurprise>();
    pt2 = it->second;
    try {
      s = pt2.get<string>(_T("symbol"));
      pEPSSurprise->m_strSymbol = s.c_str();
      s = pt2.get<string>(_T("period"));
      str = s.c_str();
      sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
      pEPSSurprise->m_lDate = year * 10000 + month * 100 + day;
      pEPSSurprise->m_dEstimate = pt2.get<double>(_T("estimate"));
      pEPSSurprise->m_dActual = pt2.get<double>(_T("actual"));
    }
    catch (ptree_error&) {
    }
    vEPSSurprise.push_back(pEPSSurprise);
  }
  sort(vEPSSurprise.begin(), vEPSSurprise.end(), CompareEPSSurprise); // 以日期早晚顺序排列。

  return true;
}