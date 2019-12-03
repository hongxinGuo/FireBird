#include"stdafx.h"
#include"globedef.h"
#include"thread.h"
#include"market.h"

#include "SinaRTWebData.h"

CSinaRTWebData::CSinaRTWebData() : CWebData() {
  m_strWebDataInquirePrefix = _T("http://hq.sinajs.cn/list=");
  m_strWebDataInquireSuffix = _T("");
}

CSinaRTWebData::~CSinaRTWebData() {
}

void CSinaRTWebData::ProcessCurrentWebData(void) {
  CSingleLock sl(&gl_ProcessSinaRTDataQueue);

  if (IsWebDataReceived()) {
    if (IsReadingSucceed()) { //����ͨ��һ��˳����
      sl.Lock();
      if (sl.IsLocked()) {
        ResetCurrentPos();
        if (SucceedReadingAndStoringWebData()) {
          ProcessWebDataStored();
        }
      }
      sl.Unlock();
    }
  }
  else {  // ����ͨ�ų��ִ���
    ReportCommunicationError();
  }
}

bool CSinaRTWebData::SucceedReadingAndStoringOneWebData(void) {
  CRTDataPtr pRTData = make_shared<CRTData>();
  if (pRTData->ReadSinaData(this)) {
    pRTData->SetDataSource(__SINA_RT_WEB_DATA__); // ������ʵʱ��������������յ�������
    gl_QueueSinaRTData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
    //gl_QueueSinaRTDataForSave.PushRTData(pRTData); // ͬʱ������洢ʵʱ���ݶ���
    return true;
  }
  return false;
}

void CSinaRTWebData::ProcessWebDataStored(void) {
  //������ĺ������붨ʱ���ȴ���ÿ3��ִ��һ�μ��ɡ��������������κ����飬
  //gl_ChinaStockMarket.DistributeRTDataReceivedFromWebToProperStock();
}

void CSinaRTWebData::ReportDataError(void) {
  TRACE("��������,�׵�����\n");
  CString str;
  str = _T("����ʵʱ��������");
  gl_systemMessage.PushInformationMessage(str);
}

void CSinaRTWebData::ReportCommunicationError(void) {
  TRACE("Error reading http file ��hq.sinajs.cn\n");
  CString str;
  str = _T("Error reading http file ��hq.sinajs.cn");
  gl_systemMessage.PushInformationMessage(str);
}

void CSinaRTWebData::InquireNextWebData(void) {
  static int iCountUp = 0;
  CRTDataPtr pRTData = nullptr;
  static int iTotalInquiringStocks = 0;

  CString strMiddle = _T("");

  // ������һ���ι�Ʊʵʱ����
  if (gl_ChinaStockMarket.IsCheckTodayActiveStock() || !gl_ChinaStockMarket.SystemReady()) { // �������Ѱ�ҽ��ջ�Ծ��Ʊ�ڼ䣨9:10--9:29, 11:31--12:59),��ʹ��ȫ�ֹ�Ʊ��
    iTotalInquiringStocks += GetInquiringStr(strMiddle, 900, false);
    if (iTotalInquiringStocks > 36000) {
      if (!gl_ChinaStockMarket.SystemReady()) { // ���ϵͳ��δ���úã�����ʾϵͳ׼��
        gl_systemMessage.PushInformationMessage(_T("���ϵͳ��ʼ��"));
      }
      gl_ChinaStockMarket.SetSystemReady(true); // ���еĹ�Ʊʵʱ���ݶ���ѯ���飬���ջ�Ծ��Ʊ���Ѿ��������ʶ����Խ������������ˡ�
    }
  }
  else { // ����ʱʹ�ý��ջ�Ծ��Ʊ��
    GetInquiringStr(strMiddle, 900, true);
  }
  CreateTotalInquiringString(strMiddle);

  SetWebDataReceived(false);
  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
  StartReadingThread();
}

int CSinaRTWebData::GetInquiringStr(CString& strInquire, long lTotalNumber, bool fSkipUnactiveStock) {
  return gl_ChinaStockMarket.GetSinaInquiringStockStr(strInquire, lTotalNumber, fSkipUnactiveStock);
}

void CSinaRTWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadSinaRTData, nullptr);
}

bool CSinaRTWebData::ReportStatus(long lNumberOfData) {
  TRACE("����%d������ʵʱ����\n", lNumberOfData);
  return true;
}