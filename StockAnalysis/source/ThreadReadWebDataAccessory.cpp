#include"stdafx.h"
#include"globedef.h"
#include"Market.h"

#include"thread.h"
#include "ThreadReadWebDataAccessory.h"

CWebDataReceivedPtr TransferWebDataToBuffer(CWebData* pWebData) {
  char* p = pWebData->GetBufferAddr();
  CWebDataReceivedPtr pWebDataReceived = make_shared<CWebDataReceived>();
  pWebDataReceived->m_pDataBuffer = new char[pWebData->GetByteReaded() + 1]; // ��������Ҫ���һ���ַ����ȣ�����Ǹ�0x000����
  pWebDataReceived->m_lBufferLength = pWebData->GetByteReaded();
  char* pbuffer = pWebDataReceived->m_pDataBuffer;
  for (int i = 0; i < pWebData->GetByteReaded() + 1; i++) {
    *pbuffer++ = *p++;
  }
  return CWebDataReceivedPtr();
}

void ThreadReadWebData(CWebData* pWebData, long lStartDelayTime, long lSecondDelayTime) {
  CInternetSession session;
  CHttpFile* pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char* pChar = pWebData->GetBufferAddr();

  try {
    pWebData->SetReadingWebData(true);
    pWebData->SetByteReaded(0);
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)pWebData->GetInquiringString(), 1,
                                                     INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE));
    Sleep(lStartDelayTime); // ������500ms�ӳټ��ɡ�
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          pWebData->AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(lSecondDelayTime); // �ȴ�50������ٶ�һ�Σ�ȷ��û�������ݺ�ŷ��ء�
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        pWebData->AddByteReaded(iCount);
      }
      else fDone = true;
    }
    *pChar = 0x000; // �����0x000��β

    // ����ȡ�����ݷ������ݻ������У���������ر�ʶ��
    CWebDataReceivedPtr pWebDataReceived = TransferWebDataToBuffer(pWebData);
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
  pWebData->SetReadingWebData(false);
}