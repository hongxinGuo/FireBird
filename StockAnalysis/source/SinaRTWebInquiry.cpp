#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include "SinaRTWebInquiry.h"

CSinaRTWebInquiry::CSinaRTWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T("http://hq.sinajs.cn/list=");
  m_strWebDataInquireSuffix = _T("");
  m_strConnection = _T("SinaRT");
}

CSinaRTWebInquiry::~CSinaRTWebInquiry() {
}

void CSinaRTWebInquiry::InquireNextWebData(void) {
  CRTDataPtr pRTData = nullptr;
  static int iTotalInquiringStocks = 0;
  const int iInquiringNumber = 850;

  CString strMiddle = _T("");

  // ������һ���ι�Ʊʵʱ����
  if (gl_ChinaStockMarket.IsCheckActiveStock() || !gl_ChinaStockMarket.SystemReady()) { // �������Ѱ�ҽ��ջ�Ծ��Ʊ�ڼ䣨9:10--9:29, 11:31--12:59),��ʹ��ȫ�ֹ�Ʊ��
    iTotalInquiringStocks += iInquiringNumber;
    strMiddle = GetNextInquiringStr(iInquiringNumber, false);
    if (iTotalInquiringStocks > gl_ChinaStockMarket.GetTotalStock() * 3) {
      if (!gl_ChinaStockMarket.SystemReady()) { // ���ϵͳ��δ���úã�����ʾϵͳ׼��
        gl_systemMessage.PushInformationMessage(_T("�й���Ʊ�г���ʼ�����"));
      }
      gl_ChinaStockMarket.SetSystemReady(true); // ���еĹ�Ʊʵʱ���ݶ���ѯ���飬���ջ�Ծ��Ʊ���Ѿ��������ʶ����Խ������������ˡ�
    }
  }
  else { // ����ʱʹ�ý��ջ�Ծ��Ʊ��
    strMiddle = GetNextInquiringStr(iInquiringNumber, true);
  }
  gl_ChinaStockMarket.SetStockCodeForInquiringSinaRTData(strMiddle.Left(8));
  CreateTotalInquiringString(strMiddle);

  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
  StartReadingThread();
}

CString CSinaRTWebInquiry::GetNextInquiringStr(long lTotalNumber, bool fSkipUnactiveStock) {
  return gl_ChinaStockMarket.GetSinaInquiringStockStr(lTotalNumber, fSkipUnactiveStock);
}

void CSinaRTWebInquiry::StartReadingThread(void) {
  AfxBeginThread(ThreadReadSinaRTData, this);
}

bool CSinaRTWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("����%d������ʵʱ����\n", lNumberOfData);
  return true;
}