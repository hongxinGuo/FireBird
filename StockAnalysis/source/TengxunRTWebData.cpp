#include"stdafx.h"
#include"globedef.h"
#include"Market.h"

#include"thread.h"
#include "TengxunRTWebData.h"

bool CTengxunRTWebData::sm_fCreatedOnce = false; // ��ʼʱû�����ɹ�ʵ��

CTengxunRTWebData::CTengxunRTWebData() : CWebData() {
  if (sm_fCreatedOnce) ASSERT(0); // ����Ѿ����ɹ�һ��ʵ���ˣ��򱨴�
  else sm_fCreatedOnce = true;
}

CTengxunRTWebData::~CTengxunRTWebData() {
}

bool CTengxunRTWebData::SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount)
{
  CRTDataPtr pRTData = make_shared<CRTData>();
  if (pRTData->ReadSinaData(pCurrentPos, iCount)) {
    gl_QueueRTData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
    return true;
  }
  return false;
}

void CTengxunRTWebData::ProcessWebDataStored(void) {
  gl_ChinaStockMarket.ProcessRTDataReceivedFromWeb();
}

void CTengxunRTWebData::ReportDataError(void)
{
  TRACE("��������,�׵�����\n");
  CString str;
  str = _T("��������");
  gl_systemMessage.PushInformationMessage(str);
}

void CTengxunRTWebData::ReportCommunicationError(void)
{
  TRACE("Error reading http file ��hq.sinajs.cn\n");
  CString str;
  str = _T("Error reading http file ��hq.sinajs.cn");
  gl_systemMessage.PushInformationMessage(str);
}

void CTengxunRTWebData::InquireNextWebData(void)
{
  static int iCountUp = 0;
  CRTDataPtr pRTData = nullptr;

  CString strTemp = _T("");

  // ������һ���ι�Ʊʵʱ����
  if (gl_ChinaStockMarket.IsCheckTodayActiveStock() || !gl_ChinaStockMarket.SystemReady()) { // �������Ѱ�ҽ��ջ�Ծ��Ʊ�ڼ䣨9:10--9:29, 11:31--12:59),��ʹ��ȫ�ֹ�Ʊ��
    gl_stSinaRTDataInquire.strInquire = m_strSinaRTDataInquire; // ���ò�ѯ����ʵʱ���ݵ��ַ���ͷ
    if (gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(strTemp)) {
      if (gl_ChinaStockMarket.CountLoopRTDataInquiring()) {  // ��������ȫ���Ʊ��
        if (!gl_ChinaStockMarket.SystemReady()) { // ���ϵͳ��δ���úã�����ʾϵͳ׼��
          gl_systemMessage.PushInformationMessage(_T("���ϵͳ��ʼ��"));
        }
        gl_ChinaStockMarket.SetSystemReady(true); // ���еĹ�Ʊʵʱ���ݶ���ѯ���飬���ջ�Ծ��Ʊ���Ѿ��������ʶ����Խ������������ˡ�
        gl_ChinaStockMarket.ResetIT();
      }
    }
    gl_stSinaRTDataInquire.strInquire += strTemp;
  }
  else { // ����ʱʹ�ý��ջ�Ծ��Ʊ��
    gl_stSinaRTDataInquire.strInquire = m_strSinaRTDataInquire;
    GetInquiringStockStr(gl_stSinaRTDataInquire.strInquire);
  }
  SetWebDataReceived(false);
  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
  StartReadingThread();
}

int CTengxunRTWebData::GetInquiringStockStr(CString& strInquire) {
  return gl_ChinaStockMarket.GetSinaInquiringStockStr(strInquire);
}

void CTengxunRTWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadTengxunRTData, nullptr);
}