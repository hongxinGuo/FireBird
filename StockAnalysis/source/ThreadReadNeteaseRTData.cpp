/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ����ӿڶ�ȡ�̡߳�
//
// ���׵ķ������ӳٲ�����100ms���ʶ��ڵȴ�100ms�󼴿ɿ�ʼ��ȡ���յ������ݡ�
//
// �˹����߳���Ϊ����ʵʱ�����̵߳ĸ����Ͳ��䣬�ʶ���Ҫ��ϵͳ׼����Ϻ󷽿�ִ�С�
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"
#include"Market.h"

UINT ThreadReadNeteaseRTData(LPVOID pParam) {
  CNeteaseRTWebData* pNeteaseRTWebData = (CNeteaseRTWebData*)pParam;

  CInternetSession session;
  CHttpFile* pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char* pChar = pNeteaseRTWebData->GetBufferAddr();

  try {
    pNeteaseRTWebData->SetReadingWebData(true);  //
    pNeteaseRTWebData->SetReadingSucceed(true);
    pNeteaseRTWebData->SetByteReaded(0);
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)pNeteaseRTWebData->GetInquiringString()));
    Sleep(200); // ��Ѷ������100ms�ӳټ��ɡ�
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          pNeteaseRTWebData->AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(50); // �ȴ�30������ٶ�һ�Σ�ȷ��û�������ݺ�ŷ��ء�
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        pNeteaseRTWebData->AddByteReaded(iCount);
      }
      else fDone = true;
    }
    *pChar = 0x000;
    pNeteaseRTWebData->SetWebDataReceived(true);
    pNeteaseRTWebData->ResetCurrentPos();

    // ����ȡ������ʵʱ���ݷ�������ʵʱ�������ݻ������У���������ر�ʶ��
    char* p = pNeteaseRTWebData->GetBufferAddr();
    CRTWebDataPtr pRTWebData = make_shared<CRTWebData>();
    pRTWebData->m_pDataBuffer = new char[pNeteaseRTWebData->GetByteReaded() + 1]; // ��������Ҫ���һ���ַ����ȣ�����Ǹ�0x000����
    pRTWebData->m_lBufferLength = pNeteaseRTWebData->GetByteReaded();
    char* pbuffer = pRTWebData->m_pDataBuffer;
    for (int i = 0; i < pNeteaseRTWebData->GetByteReaded() + 1; i++) {
      *pbuffer++ = *p++;
    }
    gl_QueueNeteaseRTWebData.PushRTWebData(pRTWebData);
  }
  catch (CInternetException * e) {
    e->Delete();
    pNeteaseRTWebData->SetReadingSucceed(false);
    pNeteaseRTWebData->SetWebDataReceived(false);
  }
  if (pFile) pFile->Close();
  if (pFile) {
    delete pFile;
    pFile = nullptr;
  }
  pNeteaseRTWebData->SetReadingWebData(false);

  return 13;
}