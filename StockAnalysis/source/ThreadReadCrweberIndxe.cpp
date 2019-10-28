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
//#include"Stock.h"
#include"Market.h"
#include "Thread.h"

#include"TransferSharedPtr.h"

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
    Sleep(1000); // ������500ms�ӳټ��ɡ�
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_CrweberIndexWebData.AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(50); // �ȴ�50������ٶ�һ�Σ�ȷ��û�������ݺ�ŷ��ء�
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        gl_CrweberIndexWebData.AddByteReaded(iCount);
      }
      else fDone = true;
    }
    *pChar = 0x000; // �����0x000��β
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

UINT ThreadSaveDayLineOfOneStock(LPVOID pParam)
{
  // ���ݹ�����ΪЯ������ָ��Ľṹ��
  static int counter = 0;
  CStockPtr pStock;
  strTransferSharedPtr* pTransfer = nullptr;

  gl_ThreadStatus.IncreaseNunberOfSavingDayLineThreads();
  CSingleLock singleLock(&gl_SaveOneStockDayLine);
  singleLock.Lock();
  CCriticalSection cs;
  CSingleLock s(&cs);
  s.Lock();
  if (s.IsLocked()) {
    counter++;
    if (counter > 16) {
      int a = 0; // �����ã�counter��Ӧ�ô���16
    }
    pTransfer = (strTransferSharedPtr*)pParam;
    pStock = pTransfer->m_pStock;
    s.Unlock();
  }

  gl_ChinaStockMarket.SaveDayLine(pStock);

  s.Lock();
  if (s.IsLocked()) {
    counter--;
    pStock->m_vDayLine.clear();
    delete pTransfer;
    s.Unlock();
  }
  singleLock.Unlock();
  gl_ThreadStatus.DecreaseNumberOfSavingDayLineThreads();

  return 13;
}