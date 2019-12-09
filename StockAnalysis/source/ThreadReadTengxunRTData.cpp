/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��Ѷʵʱ����ӿڶ�ȡ�̡߳�
//
// ��Ѷ�ķ������ӳٲ�����100ms���ʶ��ڵȴ�100ms�󼴿ɿ�ʼ��ȡ���յ������ݡ�
//
// �˹����߳���Ϊ����ʵʱ�����̵߳ĸ����Ͳ��䣬�ʶ���Ҫ��ϵͳ׼����Ϻ󷽿�ִ�С�
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"
#include"Market.h"

UINT ThreadReadTengxunRTData(LPVOID pParam) {
  CTengxunWebRTData* pTengxunWebData = (CTengxunWebRTData*)pParam;
  CInternetSession session;
  CHttpFile* pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char* pChar = pTengxunWebData->GetBufferAddr();

  const clock_t tt = clock();

  try {
    pTengxunWebData->SetReadingWebData(true);  //
    pTengxunWebData->SetReadingSucceed(true);
    pTengxunWebData->SetByteReaded(0);
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)pTengxunWebData->GetInquiringString()));
    Sleep(100); // ��Ѷ������100ms�ӳټ��ɡ�
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          pTengxunWebData->AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(30); // �ȴ�30������ٶ�һ�Σ�ȷ��û�������ݺ�ŷ��ء�
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        pTengxunWebData->AddByteReaded(iCount);
      }
      else fDone = true;
    }
    *pChar = 0x000;

    // ����ȡ����Ѷʵʱ���ݷ�����Ѷʵʱ�������ݻ������У���������ر�ʶ��
    char* p = pTengxunWebData->GetBufferAddr();
    CWebRTDataPtr pWebRTData = make_shared<CWebRTData>();
    pWebRTData->m_pDataBuffer = new char[pTengxunWebData->GetByteReaded() + 1]; // ��������Ҫ���һ���ַ����ȣ�����Ǹ�0x000����
    pWebRTData->m_lBufferLength = pTengxunWebData->GetByteReaded();
    char* pbuffer = pWebRTData->m_pDataBuffer;
    for (int i = 0; i < pTengxunWebData->GetByteReaded() + 1; i++) {
      *pbuffer++ = *p++;
    }
    gl_QueueTengxunWebRTData.PushWebRTData(pWebRTData);

    pTengxunWebData->SetWebDataReceived(true);
  }
  catch (CInternetException * e) {
    e->Delete();
    pTengxunWebData->SetReadingSucceed(false);
    pTengxunWebData->SetWebDataReceived(false);
  }
  if (pFile) pFile->Close();
  if (pFile) {
    delete pFile;
    pFile = nullptr;
  }
  pTengxunWebData->SetReadingWebData(false);

  gl_ChinaStockMarket.SetReadingTengxunRTDataTime(clock() - tt);

  return 10;
}