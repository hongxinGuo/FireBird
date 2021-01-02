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
  ptree pt, pt2;
  string s;
  double dTemp = 0;
  long lTemp = 0;
  INT64 llTemp = 0;
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
  pStake->SetIPOStatus(__STAKE_IPOED__);
  sort(vDayLine.begin(), vDayLine.end(), CompareDayLineDate); // 以日期早晚顺序排列。
  pStake->UpdateDayLine(vDayLine);
  pStake->SetDayLineNeedUpdate(false);
  pStake->SetDayLineNeedSaving(true);
  pStake->m_fUpdateDatabase = true;
  return true;
}