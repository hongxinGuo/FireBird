#include"Market.h"
#include"TransferSharedPtr.h"

UINT ThreadSaveDayLineOfOneStock(LPVOID pParam) {
  // 传递过来的为携带智能指针的结构（智能指针无法经由pParam直接传递过来
  CStockPtr pStock;
  CString str;
  strTransferSharedPtr* pTransfer = nullptr;
  bool fDataSaved = false;

  gl_SaveOneStockDayLine.Wait();
  if (gl_ExitingSystem) {
    pTransfer = static_cast<strTransferSharedPtr*>(pParam);
    delete pTransfer;
    pTransfer = nullptr;
    gl_SaveOneStockDayLine.Signal();
    return 6;
  }
  gl_ThreadStatus.IncreaseNunberOfSavingDayLineThreads();
  pTransfer = static_cast<strTransferSharedPtr*>(pParam);
  pStock = pTransfer->m_pStock;
  fDataSaved = pStock->SaveDayLine();
  pStock->SetDayLineLoaded(false);
  pStock->m_vDayLine.clear();
  delete pTransfer;
  pTransfer = nullptr;
  gl_ThreadStatus.DecreaseNumberOfSavingDayLineThreads();
  if (fDataSaved) {
    str = pStock->GetStockCode() + _T("日线资料存储完成");
    gl_systemMessage.PushDayLineInfoMessage(str);
  }
  gl_SaveOneStockDayLine.Signal();
  return 6;
}