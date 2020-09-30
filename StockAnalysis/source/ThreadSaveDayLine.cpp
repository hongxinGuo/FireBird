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

UINT ThreadSaveDLBasicInfoOfStock(CChinaStockPtr pStock) {
  CString str;
  bool fDataSaved = false;
  gl_ThreadStatus.IncreaseRunningThread();

  gl_ThreadStatus.IncreaseSavingDLThreads();
  gl_SaveOneStockDL.Wait(); //ʹ�ö��߳�ģʽ����������ȫ����ʷ����ʱʹ��4���̣߳�������ʷ����ʱֻʹ��һ���̣߳���ʱʹ�ö���̷߳��������ֻ�����󣩡�
  if (!gl_fExitingSystem) {
    fDataSaved = pStock->SaveDLBasicInfo();
    if (fDataSaved) {
      pStock->UpdateDLStartEndDate();
      pStock->SetDLDBUpdated(true);
    }
    pStock->UnloadDL();
    if (fDataSaved) {
      str = pStock->GetStockCode() + _T("�������ϴ洢���");
      gl_systemMessage.PushDLInfoMessage(str);
    }
  }
  gl_ThreadStatus.DecreaseSavingDLThreads();
  gl_SaveOneStockDL.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 15;
}