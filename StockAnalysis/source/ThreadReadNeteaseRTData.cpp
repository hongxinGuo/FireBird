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
  CNeteaseWebRTData* pNeteaseWebRTData = (CNeteaseWebRTData*)pParam;

  CInternetSession session;
  CHttpFile* pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char* pChar = pNeteaseWebRTData->GetBufferAddr();

  try {
    pNeteaseWebRTData->SetReadingWebData(true);  //
    pNeteaseWebRTData->SetByteReaded(0);
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)pNeteaseWebRTData->GetInquiringString()));
    Sleep(100); // ���׷�����100ms�ӳټ��ɡ�
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          pNeteaseWebRTData->AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(30); // �ȴ�30������ٶ�һ�Σ�ȷ��û�������ݺ�ŷ��ء�
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        pNeteaseWebRTData->AddByteReaded(iCount);
      }
      else fDone = true;
    }
    *pChar = 0x000;

    // ����ȡ������ʵʱ���ݷ�������ʵʱ�������ݻ������У���������ر�ʶ��
    char* p = pNeteaseWebRTData->GetBufferAddr();
    CWebDataReceivedPtr pWebDataReceived = make_shared<CWebDataReceived>();
    pWebDataReceived->m_pDataBuffer = new char[pNeteaseWebRTData->GetByteReaded() + 1]; // ��������Ҫ���һ���ַ����ȣ�����Ǹ�0x000����
    pWebDataReceived->m_lBufferLength = pNeteaseWebRTData->GetByteReaded();
    char* pbuffer = pWebDataReceived->m_pDataBuffer;
    for (int i = 0; i < pNeteaseWebRTData->GetByteReaded() + 1; i++) {
      *pbuffer++ = *p++;
    }
    gl_QueueNeteaseWebRTData.PushWebRTData(pWebDataReceived);

    pNeteaseWebRTData->ResetCurrentPos();
  }
  catch (CInternetException * e) {
    e->Delete();
  }
  if (pFile) pFile->Close();
  if (pFile) {
    delete pFile;
    pFile = nullptr;
  }
  pNeteaseWebRTData->SetReadingWebData(false);

  return 13;
}