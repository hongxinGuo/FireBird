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
  char* pChar = gl_SinaRTWebData.m_buffer;

  const clock_t tt = clock();

  try {
    gl_SinaRTWebData.SetReadingWebData(true);
    gl_ThreadStatus.SetReadingSinaRTData(true);  //
    gl_SinaRTWebData.m_fError = false;
    gl_SinaRTWebData.m_lByteRead = 0;
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)gl_SinaRTWebData.m_strInquire));
    Sleep(100); // 新浪服务器100ms延迟即可。
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_SinaRTWebData.m_lByteRead += iCount;
        }
      } while (iCount > 0);
      Sleep(30); // 等待30毫秒后再读一次，确认没有新数据后才返回。
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        gl_SinaRTWebData.m_lByteRead += iCount;
      }
      else fDone = true;
    }
    gl_SinaRTWebData.m_buffer[gl_SinaRTWebData.m_lByteRead] = 0x000;
    gl_ThreadStatus.SetSinaRTDataReceived(true);
    gl_SinaRTWebData.SetWebDataReceived(true);
  }
  catch (CInternetException * e) {
    e->Delete();
    gl_SinaRTWebData.m_fError = true;
    gl_ThreadStatus.SetSinaRTDataReceived(false);
    gl_SinaRTWebData.SetWebDataReceived(false);
  }
  if (pFile) pFile->Close();
  if (pFile) delete pFile;
  gl_ThreadStatus.SetReadingSinaRTData(false);
  gl_SinaRTWebData.SetReadingWebData(false);

  gl_ChinaStockMarket.SetReadingSinaRTDataTime(clock() - tt);

  return 1;
}