#include"Market.h"
#include"TransferSharedPtr.h"

UINT ThreadSaveDayLineOfOneStock(LPVOID pParam) {
  // ���ݹ�����ΪЯ������ָ��Ľṹ������ָ���޷�����pParamֱ�Ӵ��ݹ���
  CStockPtr pStock;
  CString str;
  strTransferSharedPtr* pTransfer = nullptr;

  CSingleLock singleLock(&gl_SaveOneStockDayLine);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    if (gl_ExitingSystem) {
      pTransfer = (strTransferSharedPtr*)pParam;
      delete pTransfer;
      pTransfer = nullptr;
      singleLock.Unlock();
      return 6;
    }
    gl_ThreadStatus.IncreaseNunberOfSavingDayLineThreads();
    pTransfer = (strTransferSharedPtr*)pParam;
    pStock = pTransfer->m_pStock;
    pStock->SaveDayLine();
    pStock->SetDayLineLoaded(false);
    gl_ChinaStockMarket.m_iDayLineNeedSave--;
    pStock->m_vDayLine.clear();
    delete pTransfer;
    pTransfer = nullptr;
    gl_ThreadStatus.DecreaseNumberOfSavingDayLineThreads();

    singleLock.Unlock();
    return 6;
  }
}