//////////////////////////////////////////////////////////////////////////////////////
//
// �洢������ʷ���ݿ�ʱ�����ƻ���ͬ�����⣬���µ����ݿ��д��ھ�����ʱ�����µ���;�ͻᷢ�����ݿ�ͬ�����Ļ��⣬
// �����Ҷ�MySQL���ݿⲻ̫��Ϥ���޷��ҵ�����������ʶ����������ź������ֱ����������ݿ�Ϊ�գ�ϵͳ����)ʱ,
// ʹ�ö��̷߳�ʽ�洢���ݣ��������ݿ��д��ھ�����ʱ��ʹ�õ�һ�̴߳洢��
// ���ַ�ʽ����Ӱ���ٶȣ���Ϊ��������ʱ���������Ǻ�С�ġ�
//
// ���յĽ������������Ҫ�о��������ͬ�����⡣
//
//
/////////////////////////////////////////////////////////////////////////////////////////

#include"Market.h"
#include"TransferSharedPtr.h"

UINT ThreadSaveDayLineOfOneStock(LPVOID pParam) {
  // ���ݹ�����ΪЯ������ָ��Ľṹ������ָ���޷�����pParamֱ�Ӵ��ݹ���
  CStockPtr pStock;
  CString str;
  strTransferSharedPtr* pTransfer = nullptr;
  bool fDataSaved = false;
  gl_SaveOneStockDayLine.Wait(); // ʹ�ö��߳�ģʽ
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
  pStock->ClearDayLineContainer();
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