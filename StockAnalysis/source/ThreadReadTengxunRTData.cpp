/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 腾讯实时行情接口读取线程。
//
// 腾讯的服务器延迟不超过100ms，故而在等待100ms后即可开始读取接收到的数据。
//
// 此工作线程作为新浪实时行情线程的辅助和补充，故而需要在系统准备完毕后方可执行。
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
    Sleep(100); // 腾讯服务器100ms延迟即可。
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          pTengxunWebData->AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(30); // 等待30毫秒后再读一次，确认没有新数据后才返回。
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        pTengxunWebData->AddByteReaded(iCount);
      }
      else fDone = true;
    }
    *pChar = 0x000;

    // 将读取的腾讯实时数据放入腾讯实时网络数据缓冲区中，并设置相关标识。
    char* p = pTengxunWebData->GetBufferAddr();
    CWebRTDataPtr pWebRTData = make_shared<CWebRTData>();
    pWebRTData->m_pDataBuffer = new char[pTengxunWebData->GetByteReaded() + 1]; // 缓冲区需要多加一个字符长度（最后那个0x000）。
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