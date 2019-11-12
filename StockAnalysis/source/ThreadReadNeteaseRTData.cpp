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

UINT ThreadReadNeteaseRTData(LPVOID) {
  CInternetSession session;
  CHttpFile* pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char* pChar = gl_NeteaseRTWebData.GetBufferAddr();

  try {
    gl_NeteaseRTWebData.SetReadingWebData(true);  //
    gl_NeteaseRTWebData.SetReadingSucceed(true);
    gl_NeteaseRTWebData.SetByteReaded(0);
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)gl_NeteaseRTWebData.GetInquiringString()));
    Sleep(200); // 腾讯服务器100ms延迟即可。
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_NeteaseRTWebData.AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(50); // 等待30毫秒后再读一次，确认没有新数据后才返回。
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        gl_NeteaseRTWebData.AddByteReaded(iCount);
      }
      else fDone = true;
    }
    *pChar = 0x000;
    gl_NeteaseRTWebData.SetWebDataReceived(true);
    gl_NeteaseRTWebData.ResetCurrentPos();
  }
  catch (CInternetException * e) {
    e->Delete();
    gl_NeteaseRTWebData.SetReadingSucceed(false);
    gl_NeteaseRTWebData.SetWebDataReceived(false);
  }
  if (pFile) pFile->Close();
  if (pFile) delete pFile;
  gl_NeteaseRTWebData.SetReadingWebData(false);

  return 13;
}