/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Crweber.com����ӿڶ�ȡ�̡߳�
//
// Crweber.com�ķ������ӳٱȽϳ����ʶ��ڵȴ�500ms��ʼ��ȡ���յ������ݡ�
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"Market.h"
#include "Thread.h"
#include"ThreadReadWebDataAccessory.h"

UINT ThreadReadCrweberIndex(LPVOID pParam) {
  CCrweberIndexWebData* pCrweberWebData = (CCrweberIndexWebData*)(pParam);
  CInternetSession session;
  CHttpFile* pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char* pChar = pCrweberWebData->GetBufferAddr();

  try {
    pCrweberWebData->SetReadingWebData(true);
    pCrweberWebData->SetByteReaded(0);
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)pCrweberWebData->GetInquiringString(), 1,
                                                     INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE));
    Sleep(500); // ������500ms�ӳټ��ɡ�
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          pCrweberWebData->AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(50); // �ȴ�50������ٶ�һ�Σ�ȷ��û�������ݺ�ŷ��ء�
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        pCrweberWebData->AddByteReaded(iCount);
      }
      else fDone = true;
    }
    *pChar = 0x000; // �����0x000��β

    // ����ȡ��Crweber.com���ݷ������ݻ������У���������ر�ʶ��
    /*
    char* p = pCrweberWebData->GetBufferAddr();
    CWebDataReceivedPtr pWebDataReceived = make_shared<CWebDataReceived>();
    pWebDataReceived->m_pDataBuffer = new char[pCrweberWebData->GetByteReaded() + 1]; // ��������Ҫ���һ���ַ����ȣ�����Ǹ�0x000����
    pWebDataReceived->m_lBufferLength = pCrweberWebData->GetByteReaded();
    char* pbuffer = pWebDataReceived->m_pDataBuffer;
    for (int i = 0; i < pCrweberWebData->GetByteReaded() + 1; i++) {
      *pbuffer++ = *p++;
    }
    */
    CWebDataReceivedPtr pWebDataReceived = TransferWebDataToBuffer(pCrweberWebData);
    gl_QueueCrweberdotcomWebData.PushWebRTData(pWebDataReceived);
  }
  catch (CInternetException*) {
    TRACE(_T("net error\n"));
  }
  if (pFile) pFile->Close();
  if (pFile) {
    delete pFile;
    pFile = nullptr;
  }
  pCrweberWebData->SetReadingWebData(false);

  return 12;
}