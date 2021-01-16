#include"stdafx.h"
#include"globedef.h"
#include"accessory.h"

#include"ProcessTiingoWebData.h"

using namespace std;
#include<algorithm>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

bool CompareDayLineDate(CDayLinePtr& p1, CDayLinePtr& p2);

bool ProcessTiingoStockSymbol(CWebDataPtr pWebData, vector<CAmericaStakePtr>& vStake) {
  CAmericaStakePtr pStake = make_shared<CAmericaStake>();
  ptree pt, pt2;
  string s;
  int iCount = 0;
  CString str, strNumber;
  char buffer[30];
  long year, month, day;
  int i = 0;

  if (!ConvertToJSon(pt, pWebData)) return false;
  for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
    pStake = make_shared<CAmericaStake>();
    pt2 = it->second;
    s = pt2.get<string>(_T("permaTicker"));
    if (s.size() > 0) pStake->m_strTiingoPermaTicker = s.c_str();
    s = pt2.get<string>(_T("ticker"));
    transform(s.begin(), s.end(), s.begin(), toupper);
    pStake->m_strSymbol = s.c_str();
    pStake->m_fIsActive = pt2.get<bool>(_T("isActive"));
    pStake->m_fIsADR = pt2.get<bool>(_T("isADR"));
    s = pt2.get<string>(_T("industry"));
    if (s.size() > 0) pStake->m_strTiingoIndustry = s.c_str();
    s = pt2.get<string>(_T("sector"));
    if (s.size() > 0) pStake->m_strTiingoSector = s.c_str();
    s = pt2.get<string>(_T("sicCode"));
    if (s.at(0) == 'F') {
    }
    else {
      pStake->m_iSICCode = atoi(s.c_str());
    }
    s = pt2.get<string>(_T("sicIndustry"));
    if (s.size() > 0) pStake->m_strSICIndustry = s.c_str();
    s = pt2.get<string>(_T("sicSector"));
    if (s.size() > 0) pStake->m_strSICSector = s.c_str();
    s = pt2.get<string>(_T("companyWebsite"));
    if (s.size() > 0) pStake->m_strCompanyWebSite = s.c_str();
    s = pt2.get<string>(_T("secFilingWebsite"));
    if (s.size() > 0) pStake->m_strSECFilingWebSite = s.c_str();
    s = pt2.get<string>(_T("statementLastUpdated"));
    if (s.size() > 0) str = s.c_str();
    sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
    pStake->m_lStatementUpdateDate = year * 10000 + month * 100 + day;
    s = pt2.get<string>(_T("dailyLastUpdated"));
    if (s.size() > 0) str = s.c_str();
    sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
    pStake->m_lDailyDataUpdateDate = year * 10000 + month * 100 + day;
    vStake.push_back(pStake);
    iCount++;
  }
  TRACE("今日Tiingo Company Symbol总数为%d\n", iCount);
  sprintf_s(buffer, _T("%6d"), iCount);
  strNumber = buffer;
  str = _T("今日Tiingo Company Symbol总数为") + strNumber;
  gl_systemMessage.PushInnerSystemInformationMessage(str);
  return true;
}

bool ProcessTiingoStockDayLine(CWebDataPtr pWebData, CAmericaStakePtr& pStake) {
  vector<CDayLinePtr> vDayLine;
  ptree pt, pt2;
  string s;
  double dTemp = 0;
  long lTemp = 0;
  CDayLinePtr pDayLine = nullptr;
  CString str;
  long year, month, day;

  if (!ConvertToJSon(pt, pWebData)) { // 工作线程故障
    str = _T("Tiingo下载");
    str += pStake->m_strSymbol;
    str += _T("日线故障\n");
    TRACE("%s", str.GetBuffer());
    gl_systemMessage.PushInnerSystemInformationMessage(str);
    return false;
  }

  try {
    for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
      pDayLine = make_shared<CDayLine>();
      pt2 = it->second;
      s = pt2.get<string>(_T("date"));
      str = s.c_str();
      sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
      lTemp = year * 10000 + month * 100 + day;
      pDayLine->SetDate(lTemp);
      dTemp = pt2.get<double>(_T("close"));
      pDayLine->SetClose(dTemp * 1000);
      dTemp = pt2.get<double>(_T("open"));
      pDayLine->SetOpen(dTemp * 1000);
      dTemp = pt2.get<double>(_T("high"));
      pDayLine->SetHigh(dTemp * 1000);
      dTemp = pt2.get<double>(_T("low"));
      pDayLine->SetLow(dTemp * 1000);
      lTemp = pt2.get<long>(_T("volume"));
      pDayLine->SetVolume(lTemp);
      vDayLine.push_back(pDayLine);
    }
  }
  catch (ptree_error&) {
    return false; // 数据解析出错的话，则放弃。
  }
  sort(vDayLine.begin(), vDayLine.end(), CompareDayLineDate); // 以日期早晚顺序排列。
  for (auto& pDayLine : vDayLine) {
    pDayLine->SetMarketString(pStake->m_strListedExchange);
    pDayLine->SetStakeCode(pStake->GetSymbol());
    pDayLine->SetStakeName(pStake->GetTicker());
  }
  pStake->UpdateDayLine(vDayLine);
  pStake->SetDayLineNeedUpdate(false);
  pStake->SetDayLineNeedSaving(true);
  pStake->m_fUpdateDatabase = true;
  return true;
}