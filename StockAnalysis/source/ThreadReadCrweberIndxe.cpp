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

UINT ThreadReadCrweberIndex(LPVOID) {
  CInternetSession session;
  CHttpFile* pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char* pChar = gl_CrweberIndexWebData.GetBufferAddr();

  try {
    gl_CrweberIndexWebData.SetReadingWebData(true);
    gl_CrweberIndexWebData.SetReadingSucceed(true);
    gl_CrweberIndexWebData.SetByteReaded(0);
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)gl_CrweberIndexWebData.GetInquiringString()));
    Sleep(500); // 服务器500ms延迟即可。
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_CrweberIndexWebData.AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(50); // 等待50毫秒后再读一次，确认没有新数据后才返回。
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        gl_CrweberIndexWebData.AddByteReaded(iCount);
      }
      else fDone = true;
    }
    *pChar = 0x000; // 最后以0x000结尾
    gl_CrweberIndexWebData.SetWebDataReceived(true);
  }
  catch (CInternetException * e) {
    e->Delete();
    gl_CrweberIndexWebData.SetReadingSucceed(false);
    gl_CrweberIndexWebData.SetWebDataReceived(false);
  }
  if (pFile) pFile->Close();
  if (pFile) delete pFile;
  gl_CrweberIndexWebData.SetReadingWebData(false);

  return 12;
}