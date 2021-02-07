#include"stdafx.h"
#include"globedef.h"
#include"ChinaMarket.h"

#include"Thread.h"
#include "TengxunRTWebInquiry.h"

using namespace std;
#include<thread>

CTengxunRTWebInquiry::CTengxunRTWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T("http://qt.gtimg.cn/q=");
  m_strWebDataInquireSuffix = _T("");
  m_strConnection = _T("TengxunRT");
  m_lInquiringNumber = 900; // ��Ѷʵʱ���ݲ�ѯĬ��ֵ
}

CTengxunRTWebInquiry::~CTengxunRTWebInquiry() {
}

bool CTengxunRTWebInquiry::PrepareNextInquiringStr(void) {
  CString strMiddle = _T("");
  ASSERT(gl_pChinaStockMarket->IsSystemReady());
  // ������һ���ι�Ʊʵʱ���ݡ�
  // ������Ѷʵʱ����ʱ��������������ڵĹ�Ʊ���룬�������᷵��v_pv_none_match="1";������ϵͳ���ϣ�
  // �ʶ�����ֻʹ����Ч��Ʊ���롣
  strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber, false); // Ŀǰ��ʱ����ʹ��ȫ����Ʊ��
  CreateTotalInquiringString(strMiddle);
  return true;
}

CString CTengxunRTWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSystemReady) {
  return gl_pChinaStockMarket->GetTengxunInquiringStockStr(lTotalNumber, gl_pChinaStockMarket->GetTotalStock(), true);
}

void CTengxunRTWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadTengxunRTData, this);
  thread1.detach();
}

bool CTengxunRTWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("����%d����Ѷʵʱ����\n", lNumberOfData);
  return true;
}