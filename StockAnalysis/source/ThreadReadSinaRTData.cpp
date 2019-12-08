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
  CSinaRTWebData* pSinaRTWebData = (CSinaRTWebData*)pParam;
  CInternetSession session;
  CHttpFile* pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char* pChar = pSinaRTWebData->GetBufferAddr();

  const clock_t tt = clock();

  try {
    pSinaRTWebData->SetReadingWebData(true);
    pSinaRTWebData->SetReadingSucceed(true);
    pSinaRTWebData->SetByteReaded(0);
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)pSinaRTWebData->GetInquiringString()));
    Sleep(100); // ���˷�����100ms�ӳټ��ɡ�
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          pSinaRTWebData->AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(30); // �ȴ�30������ٶ�һ�Σ�ȷ��û�������ݺ�ŷ��ء�
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        pSinaRTWebData->AddByteReaded(iCount);
      }
      else fDone = true;
    }
    *pChar = 0x000; // �����0x000��β
    pSinaRTWebData->SetWebDataReceived(true);

    // ����ȡ����Ѷʵʱ���ݷ�����Ѷʵʱ�������ݻ������У���������ر�ʶ��
    char* p = pSinaRTWebData->GetBufferAddr();
    CRTWebDataPtr pRTWebData = make_shared<CRTWebData>();
    pRTWebData->m_pDataBuffer = new char[pSinaRTWebData->GetByteReaded() + 1]; // ��������Ҫ���һ���ַ����ȣ�����Ǹ�0x000����
    pRTWebData->m_lBufferLength = pSinaRTWebData->GetByteReaded();
    char* pbuffer = pRTWebData->m_pDataBuffer;
    for (int i = 0; i < pSinaRTWebData->GetByteReaded() + 1; i++) {
      *pbuffer++ = *p++;
    }
    gl_QueueSinaRTWebData.PushRTWebData(pRTWebData);
  }
  catch (CInternetException * e) {
    e->Delete();
    pSinaRTWebData->SetReadingSucceed(false);
    pSinaRTWebData->SetWebDataReceived(false);
    pSinaRTWebData->SetReadingWebData(false);
  }
  if (pFile) pFile->Close();
  if (pFile) {
    delete pFile;
    pFile = nullptr;
  }
  pSinaRTWebData->SetReadingWebData(false);

  gl_ChinaStockMarket.SetReadingSinaRTDataTime(clock() - tt);

  return 1;
}