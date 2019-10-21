#include"stdafx.h"
#include"globedef.h"
#include"thread.h"
#include"market.h"

#include "SinaRTWebData.h"

bool CSinaRTWebData::sm_fCreatedOnce = false; // ��ʼʱû�����ɹ�ʵ��

CSinaRTWebData::CSinaRTWebData() : CWebData() {
  if (sm_fCreatedOnce) ASSERT(0); // ����Ѿ����ɹ�һ��ʵ���ˣ��򱨴�
  else sm_fCreatedOnce = true;

  m_strWebDataInquirePrefix = _T("http://hq.sinajs.cn/list=");
  m_strWebDataInquireSuffix = _T("");
}

CSinaRTWebData::~CSinaRTWebData() {
}

bool CSinaRTWebData::SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount)
{
  CRTDataPtr pRTData = make_shared<CRTData>();
  if (pRTData->ReadSinaData(pCurrentPos, iCount)) {
    gl_QueueRTData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
    return true;
  }
  return false;
}

void CSinaRTWebData::ProcessWebDataStored(void) {
  gl_ChinaStockMarket.ProcessRTDataReceivedFromWeb();
}

void CSinaRTWebData::ReportDataError(void)
{
  TRACE("��������,�׵�����\n");
  CString str;
  str = _T("��������");
  gl_systemMessage.PushInformationMessage(str);
}

void CSinaRTWebData::ReportCommunicationError(void)
{
  TRACE("Error reading http file ��hq.sinajs.cn\n");
  CString str;
  str = _T("Error reading http file ��hq.sinajs.cn");
  gl_systemMessage.PushInformationMessage(str);
}

void CSinaRTWebData::InquireNextWebData(void)
{
  static int iCountUp = 0;
  CRTDataPtr pRTData = nullptr;

  CString strTemp = _T("");

  // ������һ���ι�Ʊʵʱ����
  if (gl_ChinaStockMarket.IsCheckTodayActiveStock() || !gl_ChinaStockMarket.SystemReady()) { // �������Ѱ�ҽ��ջ�Ծ��Ʊ�ڼ䣨9:10--9:29, 11:31--12:59),��ʹ��ȫ�ֹ�Ʊ��
    gl_SinaRTWebData.m_strInquire = m_strWebDataInquirePrefix; // ���ò�ѯ����ʵʱ���ݵ��ַ���ͷ
    if (gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(strTemp)) {
      if (gl_ChinaStockMarket.CountLoopRTDataInquiring()) {  // ��������ȫ���Ʊ��
        if (!gl_ChinaStockMarket.SystemReady()) { // ���ϵͳ��δ���úã�����ʾϵͳ׼��
          gl_systemMessage.PushInformationMessage(_T("���ϵͳ��ʼ��"));
        }
        gl_ChinaStockMarket.SetSystemReady(true); // ���еĹ�Ʊʵʱ���ݶ���ѯ���飬���ջ�Ծ��Ʊ���Ѿ��������ʶ����Խ������������ˡ�
        gl_ChinaStockMarket.ResetIT();
      }
    }
    gl_SinaRTWebData.m_strInquire += strTemp;
  }
  else { // ����ʱʹ�ý��ջ�Ծ��Ʊ��
    gl_SinaRTWebData.m_strInquire = m_strWebDataInquirePrefix;
    GetInquiringStockStr(gl_SinaRTWebData.m_strInquire);
    gl_SinaRTWebData.m_strInquire += m_strWebDataInquireSuffix;
  }
  SetWebDataReceived(false);
  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
  StartReadingThread();
}

int CSinaRTWebData::GetInquiringStockStr(CString& strInquire) {
  return gl_ChinaStockMarket.GetSinaInquiringStockStr(strInquire);
}

void CSinaRTWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadSinaRTData, nullptr);
}