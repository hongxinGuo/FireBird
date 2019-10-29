#include"Market.h"
#include"TransferSharedPtr.h"

UINT ThreadSaveDayLineOfOneStock(LPVOID pParam)
{
  // ���ݹ�����ΪЯ������ָ��Ľṹ��ֻ��ָ���޷�����pParamֱ�Ӵ��ݹ���
  CStockPtr pStock;
  CString str;
  strTransferSharedPtr* pTransfer = nullptr;

  CSingleLock singleLock(&gl_SaveOneStockDayLine);
  singleLock.Lock();

  if (gl_ExitingSystem.IsTrue()) {
    pTransfer = (strTransferSharedPtr*)pParam;
    delete pTransfer;
    singleLock.Unlock();
    return 13;
  }
  gl_ThreadStatus.IncreaseNunberOfSavingDayLineThreads();
  pTransfer = (strTransferSharedPtr*)pParam;
  pStock = pTransfer->m_pStock;

  gl_ChinaStockMarket.SaveDayLine(pStock);

  pStock->SetDayLineLoaded(false);
  pStock->m_vDayLine.clear();
  delete pTransfer;
  gl_ThreadStatus.DecreaseNumberOfSavingDayLineThreads();
  singleLock.Unlock();

  return 6;
}