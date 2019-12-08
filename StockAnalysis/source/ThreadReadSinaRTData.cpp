/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 新浪实时行情接口读取线程。
//
// 新浪的服务器延迟不超过100ms，故而在等待100ms后即可开始读取接收到的数据。
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
    Sleep(100); // 新浪服务器100ms延迟即可。
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          pSinaRTWebData->AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(30); // 等待30毫秒后再读一次，确认没有新数据后才返回。
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        pSinaRTWebData->AddByteReaded(iCount);
      }
      else fDone = true;
    }
    *pChar = 0x000; // 最后以0x000结尾
    pSinaRTWebData->SetWebDataReceived(true);

    // 将读取的腾讯实时数据放入腾讯实时网络数据缓冲区中，并设置相关标识。
    char* p = pSinaRTWebData->GetBufferAddr();
    CRTWebDataPtr pRTWebData = make_shared<CRTWebData>();
    pRTWebData->m_pDataBuffer = new char[pSinaRTWebData->GetByteReaded() + 1]; // 缓冲区需要多加一个字符长度（最后那个0x000）。
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