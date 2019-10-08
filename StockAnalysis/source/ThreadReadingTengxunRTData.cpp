/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��Ѷʵʱ����ӿڶ�ȡ�̡߳�
//
// ���˵ķ������ӳٲ�����100ms���ʶ��ڵȴ�100ms�󼴿ɿ�ʼ��ȡ���յ������ݡ�
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"

#include"Market.h"

#include"Accessory.h"

#include"Thread.h"

UINT ThreadReadingTengxunRTDataProc(LPVOID) {
  CInternetSession session;
  CHttpFile* pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char* pChar = gl_stTengxunRTDataInquire.buffer;

  const clock_t tt = clock();

  try {
    gl_ThreadStatus.SetTengxunRTDataReadingInProcess(true);  //
    gl_stTengxunRTDataInquire.fError = false;
    gl_stTengxunRTDataInquire.lByteRead = 0;
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)gl_stTengxunRTDataInquire.strInquire));
    Sleep(100); // ��Ѷ������100ms�ӳټ��ɡ�
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_stTengxunRTDataInquire.lByteRead += iCount;
        }
      } while (iCount > 0);
      Sleep(30); // �ȴ�30������ٶ�һ�Σ�ȷ��û�������ݺ�ŷ��ء�
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
  gl_ThreadStatus.SetTengxunRTDataReadingInProcess(false);

  gl_ChinaStockMarket.gl_RTReadingTime = clock() - tt;

  return 10;
}