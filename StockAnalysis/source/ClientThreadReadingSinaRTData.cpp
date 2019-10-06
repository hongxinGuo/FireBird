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
    Sleep(100); // ���˷�����100ms�ӳټ��ɡ�
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_stRTDataInquire.lByteRead += iCount;
        }
      } while (iCount > 0);
      Sleep(30); // �ȴ�30������ٶ�һ�Σ�ȷ��û�������ݺ�ŷ��ء�
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