#include"stdafx.h"
#include"globedef.h"
#include"ChinaMarket.h"

#include"Thread.h"
#include "TengxunWebRTData.h"

CTengxunRTWebData::CTengxunRTWebData() : CWebData() {
  m_strWebDataInquirePrefix = _T("http://qt.gtimg.cn/q=");
  m_strWebDataInquireSuffix = _T("");
  m_strConnection = _T("TengxunRT");
}

CTengxunRTWebData::~CTengxunRTWebData() {
}

void CTengxunRTWebData::InquireNextWebData(void) {
  CString strMiddle = _T("");
  ASSERT(gl_ChinaStockMarket.SystemReady());

  // ������һ���ι�Ʊʵʱ���ݡ�
  // ������Ѷʵʱ����ʱ��������������ڵĹ�Ʊ���룬�������᷵��v_pv_none_match="1";������ϵͳ���ϣ�
  // �ʶ�����ֻʹ����Ч��Ʊ���롣
  if (!gl_ChinaStockMarket.SystemReady()) { // ���ϵͳ��δ׼���ã���ʹ��ȫ�ֹ�Ʊ��
    strMiddle = GetNextInquiringStr(900, false);
  }
  else { // ����ʱʹ�ý��ջ�Ծ��Ʊ��
    strMiddle = GetNextInquiringStr(900, false); // Ŀǰ��ʱ����ʹ��ȫ����Ʊ��
  }

  CreateTotalInquiringString(strMiddle);
  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
  StartReadingThread();
}

CString CTengxunRTWebData::GetNextInquiringStr(long lTotalNumber, bool fSkipUnactiveStock) {
  return gl_ChinaStockMarket.GetTengxunInquiringStockStr(lTotalNumber, fSkipUnactiveStock);
}

void CTengxunRTWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadTengxunRTData, this);
}

bool CTengxunRTWebData::ReportStatus(long lNumberOfData) {
  TRACE("����%d����Ѷʵʱ����\n", lNumberOfData);
  return true;
}