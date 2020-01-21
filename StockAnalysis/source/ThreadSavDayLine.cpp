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

#include"ChinaMarket.h"
#include"TransferSharedPtr.h"

UINT ThreadSaveDayLineOfOneStock(LPVOID pParam) {
  // ���ݹ�����ΪЯ������ָ��Ľṹ������ָ���޷�����pParamֱ�Ӵ��ݹ���
  CStockPtr pStock;
  CString str;
  strTransferSharedPtr* pTransfer = nullptr;
  bool fDataSaved = false;

  gl_ThreadStatus.IncreaseNunberOfSavingDayLineThreads();
  gl_SaveOneStockDayLine.Wait(); //ʹ�ö��߳�ģʽ����������ȫ����ʷ����ʱʹ��4���̣߳�������ʷ����ʱֻʹ��һ���̣߳���ʱʹ�ö���̷߳��������ֻ�����󣩡�
  pTransfer = static_cast<strTransferSharedPtr*>(pParam);
  if (!gl_ExitingSystem) {
    pStock = pTransfer->m_pStock;
    fDataSaved = pStock->SaveDayLine();
    pStock->SetDayLineLoaded(false);
    pStock->ClearDayLineContainer();
    if (fDataSaved) {
      str = pStock->GetStockCode() + _T("�������ϴ洢���");
      gl_systemMessage.PushDayLineInfoMessage(str);
    }
  }
  delete pTransfer;
  pTransfer = nullptr;
  gl_ThreadStatus.DecreaseNumberOfSavingDayLineThreads();
  gl_SaveOneStockDayLine.Signal();

  return 15;
}