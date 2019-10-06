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
#include"globedef.h"

#include"Market.h"

#include"Accessory.h"

#include"ClientThread.h"

UINT ClientThreadReadingSinaRTDataProc(LPVOID) {
  CInternetSession session;
  CHttpFile* pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char* pChar = gl_stRTDataInquire.buffer;

  const clock_t tt = clock();

  try {
    gl_ThreadStatus.SetSinaRTDataReadingInProcess(true);  // 
    gl_stRTDataInquire.fError = false;
    gl_stRTDataInquire.lByteRead = 0;
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)gl_stRTDataInquire.strInquire));
    Sleep(100); // 新浪服务器100ms延迟即可。
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_stRTDataInquire.lByteRead += iCount;
        }
      } while (iCount > 0);
      Sleep(30); // 等待30毫秒后再读一次，确认没有新数据后才返回。
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        gl_stRTDataInquire.lByteRead += iCount;
      }
      else fDone = true;
    }
    gl_stRTDataInquire.buffer[gl_stRTDataInquire.lByteRead] = 0x000;
    gl_ThreadStatus.SetRTDataReceived(true);
  }
  catch (CInternetException * e) {
    e->Delete();
    gl_stRTDataInquire.fError = true;
    gl_ThreadStatus.SetRTDataReceived(false);
  }
  if (pFile) pFile->Close();
  if (pFile) delete pFile;
  gl_ThreadStatus.SetSinaRTDataReadingInProcess(false);

  gl_ChinaStockMarket.gl_RTReadingTime = clock() - tt;

  return 1;
}