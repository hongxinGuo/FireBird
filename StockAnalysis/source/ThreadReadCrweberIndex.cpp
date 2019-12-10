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
#include "Thread.h"

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

    // 将读取的新浪实时数据放入新浪实时网络数据缓冲区中，并设置相关标识。
    char* p = pCrweberWebData->GetBufferAddr();
    CWebRTDataPtr pWebRTData = make_shared<CWebRTData>();
    pWebRTData->m_pDataBuffer = new char[pCrweberWebData->GetByteReaded() + 1]; // 缓冲区需要多加一个字符长度（最后那个0x000）。
    pWebRTData->m_lBufferLength = pCrweberWebData->GetByteReaded();
    char* pbuffer = pWebRTData->m_pDataBuffer;
    for (int i = 0; i < pCrweberWebData->GetByteReaded() + 1; i++) {
      *pbuffer++ = *p++;
    }
    gl_QueueCrweberdotcomWebData.PushWebRTData(pWebRTData);
  }
  catch (CInternetException * e) {
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