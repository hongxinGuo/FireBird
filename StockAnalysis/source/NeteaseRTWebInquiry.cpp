/////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ļ�file�ж�ȡ������ʽʵʱ���ݣ�����ֵ�����������Ƿ���ָ�ʽ����
// ��ʼ��Ϊ��һ��{��������Ϊ�����ڶ���}��������������账���򱻴�����ַ�Ϊ','�����û�������ˣ��򱻴�����ַ�Ϊ' '��
//
// Ҫ��ȡ�������飬�������ݽӿڣ�http://api.money.126.net/data/feed/0601872
//
// _ntes_quote_callback({"0601872":{"code": "0601872", "percent": 0.038251, "high": 5.72, "askvol3": 311970, "askvol2": 257996,
//                      "askvol5": 399200, "askvol4": 201000, "price": 5.7, "open": 5.53, "bid5": 5.65, "bid4": 5.66, "bid3": 5.67,
//                       "bid2": 5.68, "bid1": 5.69, "low": 5.51, "updown": 0.21, "type": "SH", "symbol": "601872", "status": 0,
//                       "ask4": 5.73, "bidvol3": 234700, "bidvol2": 166300, "bidvol1": 641291, "update": "2019/11/04 15:59:54",
//                       "bidvol5": 134500, "bidvol4": 96600, "yestclose": 5.49, "askvol1": 396789, "ask5": 5.74, "volume": 78750304,
//                       "ask1": 5.7, "name": "\u62db\u5546\u8f6e\u8239", "ask3": 5.72, "ask2": 5.71, "arrow": "\u2191",
//                        "time": "2019/11/04 15:59:52", "turnover": 443978974} });
//
// ����ʵʱ����ȱ�ٹؼ��Եĳɽ����һ��ʶ��޷���Ϊ�������ݣ�ֻ����Ϊ�����á�
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include"stdafx.h"
#include"globedef.h"
#include"ChinaMarket.h"

#include"Thread.h"
#include "NeteaseRTWebInquiry.h"

using namespace std;
#include<thread>

CNeteaseRTWebInquiry::CNeteaseRTWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T("http://api.money.126.net/data/feed/");
  m_strWebDataInquireSuffix = _T("");
  m_strConnection = _T("NeteaseRT");
  m_fReportStatus = false;
  m_lInquiringNumber = 700; // ����ʵʱ���ݲ�ѯĬ��ֵ
}

CNeteaseRTWebInquiry::~CNeteaseRTWebInquiry() {
}

bool CNeteaseRTWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("����%d������ʵʱ����\n", lNumberOfData);
  return true;
}

bool CNeteaseRTWebInquiry::PrepareNextInquiringStr(void) {
  CString strMiddle = _T("");
  bool fSkipUnactiveStock = true;

  // ������һ���ι�Ʊʵʱ����
  if (gl_pChinaStakeMarket->IsSystemReady()) fSkipUnactiveStock = true;
  else fSkipUnactiveStock = false;
  strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber, fSkipUnactiveStock); // Ŀǰ����ʹ��ȫ����Ʊ��
  gl_pChinaStakeMarket->SetStockCodeForInquiringRTData(strMiddle.Left(7)); // ֻ��ȡ��һ����Ʊ����.���״����ʽΪ��0600000�����߸��ַ�
  CreateTotalInquiringString(strMiddle);

  return true;
}

CString CNeteaseRTWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
  CString str = gl_pChinaStakeMarket->GetNeteaseStakeInquiringStr(lTotalNumber, 0, gl_pChinaStakeMarket->GetTotalStock(), fSkipUnactiveStock);
  return str;
}

void CNeteaseRTWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadNeteaseRTData, this);
  thread1.detach();
}