#include"stdafx.h"

#include"globedef.h"
#include"Market.h"

#include"CWebData.h"

bool CWebData::GetWebData(void)
{
  static int iCountUp = 0;
  CRTDataPtr pRTData = nullptr;
  long i = 0;

  if (!IsReadingWebData()) {
    ProcessWebData();

    CString strTemp = _T("");

    // ������һ���ι�Ʊʵʱ����
    if (gl_ChinaStockMarket.IsCheckTodayActiveStock() || !gl_ChinaStockMarket.SystemReady()) { // �������Ѱ�ҽ��ջ�Ծ��Ʊ�ڼ䣨9:10--9:29, 11:31--12:59),��ʹ��ȫ�ֹ�Ʊ��
      gl_stSinaRTDataInquire.strInquire = m_strSinaRTDataInquire; // ���ò�ѯ����ʵʱ���ݵ��ַ���ͷ
      if (CreateSinaRTDataInquiringStr(strTemp)) {
        if (++m_lCountLoopRTDataInquiring >= 3) {  // ��������ȫ���Ʊ��
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
    // AfxBeginThread(ThreadReadSinaRTData, nullptr);
  }

  return true;
}

void CWebData::ProcessWebData(void) {
  CRTDataPtr pRTData = nullptr;
  char* pCurrentPos = nullptr;
  long i = 0;

  if (IsWebDataReceived()) {
    if (IsReadingSucceed()) { //����ͨ��һ��˳����
      pCurrentPos = m_buffer;
      long  iCount = 0;
      while (iCount < m_lByteRead) { // ����ʵʱ���ݻ���û�д��󣬲���Ҫ�׵����һ�������ˡ�
        pRTData = make_shared<CRTData>();
        if (pRTData->ReadSinaData(pCurrentPos, m_lByteRead)) {
          i++;
          gl_QueueRTData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
        }
        else {
          TRACE("����ʵʱ��������,�׵�����\n");
          CString str;
          str = _T("����ʵʱ��������");
          gl_systemMessage.PushInformationMessage(str);
          iCount = m_lByteRead; // ��������ݿ��ܳ����⣬�׵����á�
        }
      }
      TRACE("����%d������ʵʱ����\n", i);
      // ������յ���ʵʱ����
      gl_ChinaStockMarket.ProcessRTDataReceivedFromWeb();
    }
    else {  // ����ͨ�ų��ִ���
      TRACE("Error reading http file ��hq.sinajs.cn\n");
      CString str;
      str = _T("Error reading http file ��hq.sinajs.cn");
      gl_systemMessage.PushInformationMessage(str);
    }
  }

}

void CWebData::SetWebDataReceived(bool fFlag) {
  CSingleLock singleLock(&m_WebDataReceivedLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fWebDataReceived = fFlag;
    singleLock.Unlock();
  }
}

bool CWebData::IsWebDataReceived(void) {
  CSingleLock singleLock(&m_WebDataReceivedLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fWebDataReceived;
    singleLock.Unlock();
    return fFlag;
  }
  ASSERT(0);
  return false; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
}

void CWebData::SetReadingWebData(bool fFlag) {
  CSingleLock singleLock(&m_ReadingWebDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fReadingWebData = fFlag;
    singleLock.Unlock();
  }
}

bool CWebData::IsReadingWebData(void) {
  CSingleLock singleLock(&m_ReadingWebDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fReadingWebData;
    singleLock.Unlock();
    return fFlag;
  }
  ASSERT(0);
  return false; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
}
