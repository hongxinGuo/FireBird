/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ����ӿڶ�ȡ�̡߳�
//
// ���˵ķ������ӳٲ�����100ms���ʶ��ڵȴ�100ms�󼴿ɿ�ʼ��ȡ���յ������ݡ�
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
    Sleep(100); // ���˷�����100ms�ӳټ��ɡ�
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_SinaRTWebData.AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(30); // �ȴ�30������ٶ�һ�Σ�ȷ��û�������ݺ�ŷ��ء�
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        gl_SinaRTWebData.AddByteReaded(iCount);
      }
      else fDone = true;
    }
    *pChar = 0x000; // �����0x000��β
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