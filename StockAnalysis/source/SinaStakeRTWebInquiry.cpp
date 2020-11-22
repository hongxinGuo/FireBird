#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include "SinaStakeRTWebInquiry.h"

using namespace std;
#include<thread>

CSinaStakeRTWebInquiry::CSinaStakeRTWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T("http://hq.sinajs.cn/list=");
  m_strWebDataInquireSuffix = _T("");
  m_strConnection = _T("SinaRT");
  m_lInquiringNumber = 850; // 新浪实时数据查询数量默认值
}

CSinaStakeRTWebInquiry::~CSinaStakeRTWebInquiry() {
}

bool CSinaStakeRTWebInquiry::PrepareNextInquiringStr(void) {
  CWebRTDataPtr pRTData = nullptr;

  CString strMiddle = _T("");

  // 申请下一批次股票实时数据
  strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber, false);
  CreateTotalInquiringString(strMiddle);

  return true;
}

CString CSinaStakeRTWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool) {
  return gl_pChinaStakeMarket->GetSinaStakeInquiringStr(lTotalNumber);
}

void CSinaStakeRTWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadSinaStakeRTData, this);
  thread1.detach();
}

bool CSinaStakeRTWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("读入%d个新浪实时数据\n", lNumberOfData);
  return true;
}