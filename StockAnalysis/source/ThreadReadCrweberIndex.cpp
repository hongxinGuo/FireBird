/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Crweber.com行情接口读取线程。
//
// Crweber.com的服务器延迟比较长，故而在等待500ms后开始读取接收到的数据。
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
    Sleep(500); // 服务器500ms延迟即可。
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          pCrweberWebData->AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(50); // 等待50毫秒后再读一次，确认没有新数据后才返回。
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        pCrweberWebData->AddByteReaded(iCount);
      }
      else fDone = true;
    }
    *pChar = 0x000; // 最后以0x000结尾

    // 将读取的Crweber.com数据放入数据缓冲区中，并设置相关标识。
    /*
    char* p = pCrweberWebData->GetBufferAddr();
    CWebDataReceivedPtr pWebDataReceived = make_shared<CWebDataReceived>();
    pWebDataReceived->m_pDataBuffer = new char[pCrweberWebData->GetByteReaded() + 1]; // 缓冲区需要多加一个字符长度（最后那个0x000）。
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