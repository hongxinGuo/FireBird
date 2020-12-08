#include"stdafx.h"
#include"globedef.h"

#include"ReadString.h"

#include"ProcessCompanyProfile.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;
char buffer[2048 * 1024];

bool ProcessCompanyProfile(CWebDataPtr pWebData) {
  CCompanyProfilePtr pProfile = make_shared<CCompanyProfile>();
  ptree pt;
  for (int i = 0; i < pWebData->GetBufferLength(); i++) {
    buffer[i] = pWebData->GetData(i);
  }
  buffer[pWebData->GetBufferLength()] = 0x000;
  string strTemp = buffer;
  stringstream ss(strTemp);
  string s;
  string sTime;
  try {
    read_json(ss, pt);
  }
  catch (ptree_error& e) {
    return false;
  }
  s = pt.get<string>(_T("country"));
  pProfile->m_strCountry = s.c_str();
  s = pt.get<string>(_T("currency"));
  pProfile->m_strCurrency = s.c_str();
  s = pt.get<string>(_T("exchange"));
  pProfile->m_strExchange = s.c_str();
  s = pt.get<string>(_T("finnhubIndustry"));
  pProfile->m_strFinnhubIndustry = s.c_str();
  sTime = pt.get<string>(_T("ipo"));
  s = pt.get<string>(_T("logo"));
  pProfile->m_strLogo = s.c_str();
  s = pt.get<string>(_T("marketCapitalization"));
  pProfile->m_lMarketCapitalization = atoi(s.c_str());
  s = pt.get<string>(_T("phone"));
  pProfile->m_strPhone = s.c_str();
  pProfile->m_dShareOutstanding = pt.get<double>(_T("shareOutstanding"));
  s = pt.get<string>(_T("ticker"));
  pProfile->m_strTicker = s.c_str();
  s = pt.get<string>(_T("weburl"));
  pProfile->m_strWebURL = s.c_str();
  if (gl_pAmericaStakeMarket->IsCompanySymbol(pProfile->m_strTicker)) { // 不是新代码？
    gl_pAmericaStakeMarket->AddCompanyProfile(pProfile);
    return true;
  }
  else { // 发现新代码（不应该的事情
    TRACE("Company Profilef发现新代码%s\n", pProfile->m_strTicker.GetBuffer());
  }
  return false;
}