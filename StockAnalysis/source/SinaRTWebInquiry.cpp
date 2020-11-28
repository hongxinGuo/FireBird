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
  CWebRTDataPtr pRTData = nullptr;

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
// Ŀǰֻ��ȡǰ12000����Ʊ��ʵʱ���ݣ�׼��������ȡ����֤ȯʵʱ���ݵĹ��ܡ�
// �����ٶȵ����ƣ�ǰ12000����Ʊ��ʵʱ������ȡ����ﵽÿ����һ�Σ��ʶ�����֤ȯ�Ĳ�ѯֻ�����ÿ���ʱ�䡣
// �����ѯ��ι�Ʊ֮���ٲ�ѯһ������֤ȯ��
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CSinaRTWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
  CString str;
  static int s_iCount = 5;
  if (--s_iCount > 0) {// ��ȡǰ12000����Ʊ��ʵʱ����
    str = gl_pChinaStakeMarket->GetSinaStockInquiringStr(lTotalNumber, fSkipUnactiveStock);
  }
  else {// ��ȡ��������֤ȯ��ʵʱ����
    s_iCount = 5;
    str = gl_pChinaStakeMarket->GetSinaStakeInquiringStr(lTotalNumber, gl_pChinaStakeMarket->GetTotalStock(), fSkipUnactiveStock);
  }

  return str;
}

void CSinaRTWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadSinaRTData, this);
  thread1.detach();
}

bool CSinaRTWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("����%d������ʵʱ����\n", lNumberOfData);
  return true;
}