#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include "SinaRTWebInquiry.h"

using namespace std;
#include<thread>

CSinaRTWebInquiry::CSinaRTWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T("http://hq.sinajs.cn/list=");
  m_strWebDataInquireSuffix = _T("");
  m_strConnection = _T("SinaRT");
  m_lInquiringNumber = 850; // ����ʵʱ���ݲ�ѯ����Ĭ��ֵ
}

CSinaRTWebInquiry::~CSinaRTWebInquiry() {
}

bool CSinaRTWebInquiry::PrepareNextInquiringStr(void) {
  CRTDataPtr pRTData = nullptr;

  CString strMiddle = _T("");

  // ������һ���ι�Ʊʵʱ����
  if (gl_ChinaStockMarket.IsCheckActiveStock() || !gl_ChinaStockMarket.IsSystemReady()) { // �������Ѱ�ҽ��ջ�Ծ��Ʊ�ڼ䣨9:10--9:29, 11:31--12:59),��ʹ��ȫ�ֹ�Ʊ��
    strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber, false);
    if (!gl_ChinaStockMarket.IsSystemReady()) {
      gl_ChinaStockMarket.CheckMarketReady();
      if (gl_ChinaStockMarket.IsSystemReady()) gl_systemMessage.PushInformationMessage(_T("�й���Ʊ�г���ʼ�����"));
    }
  }
  else { // ����ʱʹ�ý��ջ�Ծ��Ʊ��
    strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber, true);
  }
  gl_ChinaStockMarket.SetStockCodeForInquiringSinaRTData(strMiddle.Left(8)); // ֻ��ȡ��һ����Ʊ����
  CreateTotalInquiringString(strMiddle);

  return true;
}

CString CSinaRTWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
  return gl_ChinaStockMarket.GetSinaInquiringStockStr(lTotalNumber, fSkipUnactiveStock);
}

void CSinaRTWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadSinaRTData, this);
  thread1.detach();
}

bool CSinaRTWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("����%d������ʵʱ����\n", lNumberOfData);
  return true;
}