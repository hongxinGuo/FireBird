/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ����ӿڶ�ȡ�̡߳�
//
// ���˵ķ������ӳٲ�����100ms���ʶ��ڵȴ�100ms�󼴿ɿ�ʼ��ȡ���յ������ݡ�
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"Market.h"

UINT ThreadReadSinaRTData(LPVOID pParam) {
  CSinaWebRTData* pSinaWebRTData = (CSinaWebRTData*)pParam;
  CInternetSession session;
  CHttpFile* pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char* pChar = pSinaWebRTData->GetBufferAddr();

  const clock_t tt = clock();

  try {
    pSinaWebRTData->SetReadingWebData(true);
    pSinaWebRTData->SetByteReaded(0);
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)pSinaWebRTData->GetInquiringString()));
    Sleep(100); // ���˷�����100ms�ӳټ��ɡ�
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          pSinaWebRTData->AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(30); // �ȴ�30������ٶ�һ�Σ�ȷ��û�������ݺ�ŷ��ء�
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        pSinaWebRTData->AddByteReaded(iCount);
      }
      else fDone = true;
    }
    *pChar = 0x000; // �����0x000��β

    // ����ȡ������ʵʱ���ݷ�������ʵʱ�������ݻ������У���������ر�ʶ��
    char* p = pSinaWebRTData->GetBufferAddr();
    CWebDataReceivedPtr pWebDataReceived = make_shared<CWebDataReceived>();
    pWebDataReceived->m_pDataBuffer = new char[pSinaWebRTData->GetByteReaded() + 1]; // ��������Ҫ���һ���ַ����ȣ�����Ǹ�0x000����
    pWebDataReceived->m_lBufferLength = pSinaWebRTData->GetByteReaded();
    char* pbuffer = pWebDataReceived->m_pDataBuffer;
    for (int i = 0; i < pSinaWebRTData->GetByteReaded() + 1; i++) {
      *pbuffer++ = *p++;
    }
    gl_QueueSinaWebRTData.PushWebRTData(pWebDataReceived);
  }
  catch (CInternetException * e) {
    e->Delete();
  }
  if (pFile) pFile->Close();
  if (pFile) {
    delete pFile;
    pFile = nullptr;
  }
  pSinaWebRTData->SetReadingWebData(false);

  gl_ChinaStockMarket.SetReadingSinaRTDataTime(clock() - tt);

  return 1;
}