#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"
#include"Market.h"

#include "SinaWebRTData.h"

CSinaWebRTData::CSinaWebRTData() : CWebData() {
  m_strWebDataInquirePrefix = _T("http://hq.sinajs.cn/list=");
  m_strWebDataInquireSuffix = _T("");
}

CSinaWebRTData::~CSinaWebRTData() {
}

void CSinaWebRTData::InquireNextWebData(void) {
  CRTDataPtr pRTData = nullptr;
  static int iTotalInquiringStocks = 0;

  CString strMiddle = _T("");

  // ������һ���ι�Ʊʵʱ����
  if (gl_ChinaStockMarket.IsCheckTodayActiveStock() || !gl_ChinaStockMarket.SystemReady()) { // �������Ѱ�ҽ��ջ�Ծ��Ʊ�ڼ䣨9:10--9:29, 11:31--12:59),��ʹ��ȫ�ֹ�Ʊ��
    iTotalInquiringStocks += 900;
    strMiddle = GetNextInquiringStr(900, false);
    if (iTotalInquiringStocks > gl_ChinaStockMarket.GetTotalStock() * 3) {
      if (!gl_ChinaStockMarket.SystemReady()) { // ���ϵͳ��δ���úã�����ʾϵͳ׼��
        gl_systemMessage.PushInformationMessage(_T("���ϵͳ��ʼ��"));
      }
      gl_ChinaStockMarket.SetSystemReady(true); // ���еĹ�Ʊʵʱ���ݶ���ѯ���飬���ջ�Ծ��Ʊ���Ѿ��������ʶ����Խ������������ˡ�
    }
  }
  else { // ����ʱʹ�ý��ջ�Ծ��Ʊ��
    strMiddle = GetNextInquiringStr(900, true);
  }
  CreateTotalInquiringString(strMiddle);

  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
  StartReadingThread();
}

CString CSinaWebRTData::GetNextInquiringStr(long lTotalNumber, bool fSkipUnactiveStock) {
  return gl_ChinaStockMarket.GetSinaInquiringStockStr(lTotalNumber, fSkipUnactiveStock);
}

void CSinaWebRTData::StartReadingThread(void) {
  // ׼��ʹ��thread��׼���д
  //static thread run(ThreadReadSinaRTData, this);
  AfxBeginThread(ThreadReadSinaRTData, this);
}

bool CSinaWebRTData::ReportStatus(long lNumberOfData) {
  TRACE("����%d������ʵʱ����\n", lNumberOfData);
  return true;
}