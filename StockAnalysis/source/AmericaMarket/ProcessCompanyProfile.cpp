#include"stdafx.h"
#include"globedef.h"
#include"WebData.h"
#include"ProcessCompanyProfile.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

static char s_buffer[2048 * 1024];

bool ProcessCompanyProfile(CWebDataPtr pWebData) {
  CCompanyProfilePtr pProfile = make_shared<CCompanyProfile>();
  ptree pt;
  for (int i = 0; i < pWebData->GetBufferLength(); i++) {
    s_buffer[i] = pWebData->GetData(i);
  }
  s_buffer[pWebData->GetBufferLength()] = 0x000;
  string strTemp = s_buffer;
  stringstream ss(strTemp);
  string s;
  CString strTicker = _T("");

  try {
    read_json(ss, pt);
  }
  catch (ptree_error& e) {
    return false;
  }
  try {
    s = pt.get<string>(_T("ticker"));
    strTicker = s.c_str();
  }
  catch (ptree_error& e) {
    return false;
  }
  if (gl_pAmericaStakeMarket->IsCompanyProfile(strTicker)) {
    pProfile = gl_pAmericaStakeMarket->GetCompanyProfile(strTicker);
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
      s = pt.get<string>(_T("ipo"));
      pProfile->m_strIPODate = s.c_str();
      s = pt.get<string>(_T("logo"));
      pProfile->m_strLogo = s.c_str();
      s = pt.get<string>(_T("marketCapitalization"));
      pProfile->m_lMarketCapitalization = atoi(s.c_str());
      s = pt.get<string>(_T("phone"));
      pProfile->m_strPhone = s.c_str();
      pProfile->m_dShareOutstanding = pt.get<double>(_T("shareOutstanding"));
      s = pt.get<string>(_T("weburl"));
      pProfile->m_strWebURL = s.c_str();
      pProfile->m_lCompanyProfileUpdateDate = gl_pAmericaStakeMarket->GetFormatedMarketDate();
    }
    catch (ptree_error& e) {
      return false;
    }
    pProfile->m_fUpdateDatabase = true;
  }
  else { // 发现新代码（不应该的事情
    TRACE("Company Profilef发现新代码%s\n", strTicker.GetBuffer());
  }
  return true;
}

bool ProcessCompanySymbol(CWebDataPtr pWebData) {
  CCompanyProfilePtr pProfile = make_shared<CCompanyProfile>();
  ptree pt, pt2;
  for (int i = 0; i < pWebData->GetBufferLength(); i++) {
    s_buffer[i] = pWebData->GetData(i);
  }
  s_buffer[pWebData->GetBufferLength()] = 0x000;
  string strTemp = s_buffer;
  stringstream ss(strTemp);
  string s;
  string sTime;
  try {
    read_json(ss, pt);
  }
  catch (ptree_error& e) {
    return false;
  }
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