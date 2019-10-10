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
#include"Market.h"

UINT ThreadReadingTengxunRTDataProc(LPVOID) {
  CInternetSession session;
  CHttpFile* pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char* pChar = gl_stTengxunRTDataInquire.buffer;

  const clock_t tt = clock();

  try {
    gl_ThreadStatus.SetReadingTengxunRTData(true);  //
    gl_stTengxunRTDataInquire.fError = false;
    gl_stTengxunRTDataInquire.lByteRead = 0;
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)gl_stTengxunRTDataInquire.strInquire));
    Sleep(100); // 腾讯服务器100ms延迟即可。
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_stTengxunRTDataInquire.lByteRead += iCount;
        }
      } while (iCount > 0);
      Sleep(30); // 等待30毫秒后再读一次，确认没有新数据后才返回。
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        gl_stTengxunRTDataInquire.lByteRead += iCount;
      }
      else fDone = true;
    }
    gl_stTengxunRTDataInquire.buffer[gl_stTengxunRTDataInquire.lByteRead] = 0x000;
    gl_ThreadStatus.SetTengxunRTDataReceived(true);
  }
  catch (CInternetException * e) {
    e->Delete();
    gl_stTengxunRTDataInquire.fError = true;
    gl_ThreadStatus.SetTengxunRTDataReceived(false);
  }
  if (pFile) pFile->Close();
  if (pFile) delete pFile;
  gl_ThreadStatus.SetReadingTengxunRTData(false);

  gl_ChinaStockMarket.SetReadingTengxunRTDataTime(clock() - tt);

  return 10;
}