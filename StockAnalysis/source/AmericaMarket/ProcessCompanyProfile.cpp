#include"stdafx.h"
#include"globedef.h"
#include"WebData.h"
#include"ProcessCompanyProfile.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

static char s_buffer[2048 * 1024];

bool ConvertToJSon(ptree& pt, CWebDataPtr pWebData) {
  for (int i = 0; i < pWebData->GetBufferLength(); i++) {
    s_buffer[i] = pWebData->GetData(i);
  }
  s_buffer[pWebData->GetBufferLength()] = 0x000;
  string strTemp = s_buffer;
  stringstream ss(strTemp);

  try {
    read_json(ss, pt);
  }
  catch (ptree_error& e) {
    return false;
  }
  return true;
}

bool ProcessCompanyProfile(CWebDataPtr pWebData) {
  CCompanyProfilePtr pProfile = make_shared<CCompanyProfile>();
  ptree pt;
  string s;
  CString strTicker = _T("");

  if (pWebData->GetBufferLength() < 20) {
    pProfile = gl_pAmericaStakeMarket->GetCurrentProcessingCompanyProfile();
    pProfile->m_lCompanyProfileUpdateDate = gl_pAmericaStakeMarket->GetFormatedMarketDate();
    pProfile->m_fUpdateDatabase = true;
    return false; // 没有公司简介
  }
  if (!ConvertToJSon(pt, pWebData)) return false;
  try {
    s = pt.get<string>(_T("ticker"));
    strTicker = s.c_str();
  }
  catch (ptree_error& e) {
    return false;
  }
  pProfile = gl_pAmericaStakeMarket->GetCurrentProcessingCompanyProfile();
  pProfile->m_strTicker = strTicker;
  try {
    s = pt.get<string>(_T("country"));
    pProfile->m_strCountry = s.c_str();
    s = pt.get<string>(_T("currency"));
    pProfile->m_strCurrency = s.c_str();
    s = pt.get<string>(_T("exchange"));
    pProfile->m_strExchange = s.c_str();
    s = pt.get<string>(_T("name"));
    pProfile->m_strName = s.c_str();
    s = pt.get<string>(_T("finnhubIndustry"));
    pProfile->m_strFinnhubIndustry = s.c_str();
    s = pt.get<string>(_T("logo"));
    pProfile->m_strLogo = s.c_str();
    s = pt.get<string>(_T("marketCapitalization"));
    pProfile->m_dMarketCapitalization = atof(s.c_str());
    s = pt.get<string>(_T("phone"));
    pProfile->m_strPhone = s.c_str();
    pProfile->m_dShareOutstanding = pt.get<double>(_T("shareOutstanding"));
    s = pt.get<string>(_T("weburl"));
    pProfile->m_strWebURL = s.c_str();
    s = pt.get<string>(_T("ipo"));
    pProfile->m_strIPODate = s.c_str();
    pProfile->m_lCompanyProfileUpdateDate = gl_pAmericaStakeMarket->GetFormatedMarketDate();
  }
  catch (ptree_error& e) {
    pProfile->m_fUpdateDatabase = true;
    return false;
  }
  pProfile->m_fUpdateDatabase = true;
  return true;
}

bool ProcessCompanySymbol(CWebDataPtr pWebData) {
  CCompanyProfilePtr pProfile = make_shared<CCompanyProfile>();
  ptree pt, pt2;
  string s;

  if (!ConvertToJSon(pt, pWebData)) return false;
  for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
    pProfile = make_shared<CCompanyProfile>();
    pt2 = it->second;
    s = pt2.get<string>(_T("description"));
    pProfile->m_strDescription = s.c_str();
    s = pt2.get<string>(_T("displaySymbol"));
    pProfile->m_strDisplaySymbol = s.c_str();
    s = pt2.get<string>(_T("symbol"));
    pProfile->m_strSymbol = s.c_str();
    s = pt2.get<string>(_T("type"));
    pProfile->m_strType = s.c_str();
    s = pt2.get<string>(_T("currency"));
    pProfile->m_strCurrency = s.c_str();
    if (!gl_pAmericaStakeMarket->IsCompanyProfile(pProfile->m_strSymbol)) { // 新代码？
      gl_pAmericaStakeMarket->AddCompanyProfile(pProfile);
    }
  }
  return true;
}

bool ProcessStakeCandle(CWebDataPtr pWebData, CCompanyProfilePtr& pCompany) {
  vector<CDayLinePtr> vDayLine;
  ptree pt, pt2, pt3;
  string s;
  double dTemp = 0;
  long lTemp = 0;
  INT64 llTemp = 0;
  time_t tTemp = 0;
  CDayLinePtr pDayLine = nullptr;
  int i = 0;

  if (!ConvertToJSon(pt, pWebData)) return false;
  s = pt.get<string>(_T("s"));
  if (s.compare(_T("ok")) != 0) return false;
  try {
    pt2 = pt.get_child(_T("t"));
  }
  catch (ptree_error& e) {
    return false;
  }
  for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
    pt3 = it->second;
    tTemp = pt3.get_value<time_t>();
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetTime(tTemp);
    lTemp = FormatToDate(tTemp);
    pDayLine->SetDate(lTemp);
    vDayLine.push_back(pDayLine);
  }
  try {
    pt2 = pt.get_child(_T("c"));
  }
  catch (ptree_error& e) {
    return true;
  }
  i = 0;
  for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
    pt3 = it->second;
    dTemp = pt3.get_value<double>();
    pDayLine = vDayLine.at(i++);
    pDayLine->SetClose(dTemp * 1000);
  }
  try {
    pt2 = pt.get_child(_T("o"));
  }
  catch (ptree_error& e) {
    return true;
  }
  i = 0;
  for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
    pt3 = it->second;
    dTemp = pt3.get_value<double>();
    pDayLine = vDayLine.at(i++);
    pDayLine->SetOpen(dTemp * 1000);
  }
  try {
    pt2 = pt.get_child(_T("h"));
  }
  catch (ptree_error& e) {
    return true;
  }
  i = 0;
  for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
    pt3 = it->second;
    dTemp = pt3.get_value<double>();
    pDayLine = vDayLine.at(i++);
    pDayLine->SetHigh(dTemp * 1000);
  }
  try {
    pt2 = pt.get_child(_T("l"));
  }
  catch (ptree_error& e) {
    return true;
  }
  i = 0;
  for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
    pt3 = it->second;
    dTemp = pt3.get_value<double>();
    pDayLine = vDayLine.at(i++);
    pDayLine->SetLow(dTemp * 1000);
  }
  try {
    pt2 = pt.get_child(_T("v"));
  }
  catch (ptree_error& e) {
    return true;
  }
  i = 0;
  for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
    pt3 = it->second;
    llTemp = pt3.get_value<INT64>();
    pDayLine = vDayLine.at(i++);
    pDayLine->SetVolume(llTemp);
  }
  pCompany->UpdateDayLine(vDayLine);
  pCompany->SetDayLineNeedUpdate(false);
  pCompany->SetDayLineNeedSaving(true);
  return true;
}