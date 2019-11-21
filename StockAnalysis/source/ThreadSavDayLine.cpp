#include"Market.h"
#include"TransferSharedPtr.h"

UINT ThreadSaveDayLineOfOneStock(LPVOID pParam) {
  // ���ݹ�����ΪЯ������ָ��Ľṹ������ָ���޷�����pParamֱ�Ӵ��ݹ���
  CStockPtr pStock;
  CString str;
  strTransferSharedPtr* pTransfer = nullptr;

  CSingleLock singleLock(&gl_SaveOneStockDayLine);
  singleLock.Lock();
  gl_ThreadStatus.IncreaseNunberOfSavingDayLineThreads();
  pTransfer = (strTransferSharedPtr*)pParam;
  pStock = pTransfer->m_pStock;
  pStock->SaveDayLine();
  pStock->SetDayLineLoaded(false);
  pStock->m_vDayLine.clear();
  delete pTransfer;
  gl_ThreadStatus.DecreaseNumberOfSavingDayLineThreads();

  return 6;
}