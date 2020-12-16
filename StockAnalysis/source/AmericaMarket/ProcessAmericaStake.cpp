#include"stdafx.h"
#include"globedef.h"
#include"WebData.h"
#include"ProcessAmericaStake.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

static char s_buffer[2048 * 1024];

bool ConvertToJSon(ptree& pt, CWebDataPtr pWebData) {
  char* pbuffer = new char[2048 * 1024];
  for (int i = 0; i < pWebData->GetBufferLength(); i++) {
    pbuffer[i] = pWebData->GetData(i);
  }
  pbuffer[pWebData->GetBufferLength()] = 0x000;
  string strTemp = pbuffer;
  stringstream ss(strTemp);

  delete pbuffer;
  try {
    read_json(ss, pt);
  }
  catch (ptree_error&) {
    return false;
  }
  return true;
}

bool ProcessAmericaStakeProfile(CWebDataPtr pWebData, CAmericaStakePtr& pStake) {
  ptree pt;
  string s;

  if (pWebData->GetBufferLength() < 20) {
    pStake->m_lProfileUpdateDate = gl_pAmericaStakeMarket->GetFormatedMarketDate();
    pStake->m_fUpdateDatabase = true;
    return false; // 没有公司简介
  }
  if (!ConvertToJSon(pt, pWebData)) return false;
  s = pt.get<string>(_T("address"));
  pStake->m_strAddress = s.c_str();
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
  pStake->m_lProfileUpdateDate = gl_pAmericaStakeMarket->GetFormatedMarketDate();
  pStake->m_fUpdateDatabase = true;
  return true;
}

bool ProcessAmericaStakeProfile2(CWebDataPtr pWebData, CAmericaStakePtr& pStake) {
  ptree pt;
  string s;

  TRACE("处理%s简介\n", pStake->m_strSymbol.GetBuffer());
  if (pWebData->GetBufferLength() < 20) {
    pStake->m_lProfileUpdateDate = gl_pAmericaStakeMarket->GetFormatedMarketDate();
    pStake->m_fUpdateDatabase = true;
    return false; // 没有公司简介
  }
  if (!ConvertToJSon(pt, pWebData)) return false;
  s = pt.get<string>(_T("ticker"));
  if (s.size() > 0) pStake->m_strTicker = s.c_str();
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
  pStake->m_lProfileUpdateDate = gl_pAmericaStakeMarket->GetFormatedMarketDate();
  pStake->m_fUpdateDatabase = true;
  return true;
}

bool ProcessAmericaStakeSymbol(CWebDataPtr pWebData) {
  CAmericaStakePtr pStake = make_shared<CAmericaStake>();
  ptree pt, pt2;
  string s;

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
    s = pt2.get<string>(_T("currency"));
    if (s.size() > 0) pStake->m_strCurrency = s.c_str();
    if (!gl_pAmericaStakeMarket->IsAmericaStake(pStake->m_strSymbol)) { // 新代码？
      gl_pAmericaStakeMarket->AddAmericaStake(pStake);
    }
  }

  return true;
}

bool ProcessAmericaStakeCandle(CWebDataPtr pWebData, CAmericaStakePtr& pStake) {
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
    TRACE("%S", str);
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
      TRACE("%S", str);
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
  pStake->UpdateDayLine(vDayLine);
  pStake->SetDayLineNeedUpdate(false);
  pStake->SetDayLineNeedSaving(true);
  pStake->m_fUpdateDatabase = true;
  return true;
}

bool ProcessAmericaStakeQuote(CWebDataPtr pWebData, CAmericaStakePtr& pStake) {
  string s;
  ptree pt;
  double dTemp = 0;
  time_t tt = 0;

  if (!ConvertToJSon(pt, pWebData)) {
    return false;
  }
  dTemp = pt.get<double>(_T("pc"));
  pStake->SetLastClose(dTemp * 1000);
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

bool ProcessAmericaForexExchange(CWebDataPtr pWebData, vector<CString>& vExchange) {
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

bool ProcessAmericaForexSymbol(CWebDataPtr pWebData, vector<CForexSymbolPtr>& vForexSymbol) {
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