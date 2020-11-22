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
  m_lInquiringNumber = 850; // ����ʵʱ���ݲ�ѯ����Ĭ��ֵ
}

CSinaStakeRTWebInquiry::~CSinaStakeRTWebInquiry() {
}

bool CSinaStakeRTWebInquiry::PrepareNextInquiringStr(void) {
  CWebRTDataPtr pRTData = nullptr;

  CString strMiddle = _T("");

  // ������һ���ι�Ʊʵʱ����
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
  TRACE("����%d������ʵʱ����\n", lNumberOfData);
  return true;
}