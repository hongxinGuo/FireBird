/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ����ӿڶ�ȡ�̡߳�
//
// ���׵ķ������ӳٲ�����100ms���ʶ��ڵȴ�100ms�󼴿ɿ�ʼ��ȡ���յ������ݡ�
//
// �˹����߳���Ϊ����ʵʱ�����̵߳ĸ����Ͳ��䣬�ʶ���Ҫ��ϵͳ׼����Ϻ󷽿�ִ�С�
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
    Sleep(200); // ��Ѷ������100ms�ӳټ��ɡ�
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_NeteaseRTWebData.AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(50); // �ȴ�30������ٶ�һ�Σ�ȷ��û�������ݺ�ŷ��ء�
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