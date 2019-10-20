#include"stdafx.h"
#include"globedef.h"
#include"market.h"

#include "CSinaRTWebData.h"

bool CSinaRTWebData::ReadAndSaveWebData(char*& pCurrentPos, long& iCount)
{
  CRTDataPtr pRTData = make_shared<CRTData>();
  if (pRTData->ReadSinaData(pCurrentPos, iCount)) {
    gl_QueueRTData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
    return true;
  }
  return false;
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
  long i = 0;

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