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

UINT ThreadReadSinaRTData(LPVOID) {
  CInternetSession session;
  CHttpFile* pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char* pChar = gl_SinaRTWebData.GetBufferAddr();

  const clock_t tt = clock();

  try {
    gl_SinaRTWebData.SetReadingWebData(true);
    gl_SinaRTWebData.SetReadingSucceed(true);
    gl_SinaRTWebData.SetByteReaded(0);
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)gl_SinaRTWebData.GetInquiringString()));
    Sleep(100); // 新浪服务器100ms延迟即可。
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_SinaRTWebData.AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(30); // 等待30毫秒后再读一次，确认没有新数据后才返回。
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        gl_SinaRTWebData.AddByteReaded(iCount);
      }
      else fDone = true;
    }
    *pChar = 0x000; // 最后以0x000结尾
    gl_SinaRTWebData.SetWebDataReceived(true);
  }
  catch (CInternetException * e) {
    e->Delete();
    gl_SinaRTWebData.SetReadingSucceed(false);
    gl_SinaRTWebData.SetWebDataReceived(false);
    gl_SinaRTWebData.SetReadingWebData(false);
  }
  if (pFile) pFile->Close();
  if (pFile) {
    delete pFile;
    pFile = nullptr;
  }
  gl_SinaRTWebData.SetReadingWebData(false);

  gl_ChinaStockMarket.SetReadingSinaRTDataTime(clock() - tt);

  return 1;
}