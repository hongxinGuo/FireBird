#include"Market.h"
#include"TransferSharedPtr.h"

UINT ThreadSaveDayLineOfOneStock(LPVOID pParam) {
  // ���ݹ�����ΪЯ������ָ��Ľṹ������ָ���޷�����pParamֱ�Ӵ��ݹ���
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
    str = pStock->GetStockCode() + _T("�������ϴ洢���");
    gl_systemMessage.PushDayLineInfoMessage(str);
  }
  gl_SaveOneStockDayLine.Signal();
  return 6;
}