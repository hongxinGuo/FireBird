#include"stdafx.h"
#include"globedef.h"
#include"accessory.h"

#include"ProcessTiingoWebData.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

bool CompareDayLineDate(CDayLinePtr& p1, CDayLinePtr& p2);

bool ProcessTiingoStockDayLine(CWebDataPtr pWebData, CAmericaStakePtr& pStake) {
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