/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网易实时行情接口读取线程。
//
// 网易的服务器延迟不超过100ms，故而在等待100ms后即可开始读取接收到的数据。
//
// 此工作线程作为新浪实时行情线程的辅助和补充，故而需要在系统准备完毕后方可执行。
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
    Sleep(100); // 网易服务器100ms延迟即可。
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          pNeteaseWebRTData->AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(30); // 等待30毫秒后再读一次，确认没有新数据后才返回。
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        pNeteaseWebRTData->AddByteReaded(iCount);
      }
      else fDone = true;
    }
    *pChar = 0x000;

    // 将读取的网易实时数据放入网易实时网络数据缓冲区中，并设置相关标识。
    char* p = pNeteaseWebRTData->GetBufferAddr();
    CWebDataReceivedPtr pWebDataReceived = make_shared<CWebDataReceived>();
    pWebDataReceived->m_pDataBuffer = new char[pNeteaseWebRTData->GetByteReaded() + 1]; // 缓冲区需要多加一个字符长度（最后那个0x000）。
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