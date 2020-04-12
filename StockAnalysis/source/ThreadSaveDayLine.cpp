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
#include"globedef.h"

UINT ThreadSaveDayLineOfOneStock(CChinaStockPtr pStock) {
  CString str;
  bool fDataSaved = false;
  gl_ThreadStatus.IncreaseRunningThread();

  gl_ThreadStatus.IncreaseSavingDayLineThreads();
  gl_SaveOneStockDayLine.Wait(); //ʹ�ö��߳�ģʽ����������ȫ����ʷ����ʱʹ��4���̣߳�������ʷ����ʱֻʹ��һ���̣߳���ʱʹ�ö���̷߳��������ֻ�����󣩡�
  if (!gl_ExitingSystem) {
    fDataSaved = pStock->SaveDayLine();
    pStock->SetDayLineLoaded(false);
    pStock->ClearDayLineContainer();
    if (fDataSaved) {
      str = pStock->GetStockCode() + _T("�������ϴ洢���");
      gl_systemMessage.PushDayLineInfoMessage(str);
    }
  }
  gl_ThreadStatus.DecreaseSavingDayLineThreads();
  gl_SaveOneStockDayLine.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 15;
}