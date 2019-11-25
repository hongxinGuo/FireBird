#include"Market.h"
#include"TransferSharedPtr.h"

UINT ThreadSaveDayLineOfOneStock(LPVOID pParam) {
  // 传递过来的为携带智能指针的结构（智能指针无法经由pParam直接传递过来
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