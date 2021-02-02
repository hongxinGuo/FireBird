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
  CString strMiddle = _T("");

  // ������һ���ι�Ʊʵʱ����
  if (gl_pChinaStakeMarket->IsCheckActiveStock() || !gl_pChinaStakeMarket->IsSystemReady()) { // �������Ѱ�ҽ��ջ�Ծ��Ʊ�ڼ䣨9:10--9:29, 11:31--12:59),��ʹ��ȫ�ֹ�Ʊ��
    strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber, false);
  }
  else { // ����ʱʹ�ý��ջ�Ծ��Ʊ��
    strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber, true);
  }
  gl_pChinaStakeMarket->SetStockCodeForInquiringRTData(strMiddle.Left(8)); // ֻ��ȡ��һ����Ʊ���롣���˴����ʽΪ��sh600000�����˸��ַ���
  CreateTotalInquiringString(strMiddle);

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��������ʵʱ������ȡ������
// Ŀǰֻ��ȡǰ12000����Ʊ��ʵʱ���ݡ�
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CSinaRTWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
  return gl_pChinaStakeMarket->GetSinaStockInquiringStr(lTotalNumber, fSkipUnactiveStock);
}

void CSinaRTWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadSinaRTData, this);
  thread1.detach();
}

bool CSinaRTWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("����%d������ʵʱ����\n", lNumberOfData);
  return true;
}