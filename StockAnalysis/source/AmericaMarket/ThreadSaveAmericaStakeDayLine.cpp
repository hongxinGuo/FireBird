//////////////////////////////////////////////////////////////////////////////////////
//
// �洢������ʷ���ݿ�ʱ�����ƻ���ͬ�����⣬���µ����ݿ��д��ھ�����ʱ�����µ���;�ͻᷢ�����ݿ�ͬ�����Ļ��⣬
// �����Ҷ�MySQL���ݿⲻ̫��Ϥ���޷��ҵ�����������ʶ����������ź������ֱ������������ݿ�Ϊ�գ�ϵͳ����)ʱ,
// ʹ�ö��̷߳�ʽ�洢���ݣ��������ݿ��д��ھ�����ʱ��ʹ�õ�һ�̴߳洢��
// ���ַ�ʽ����Ӱ���ٶȣ���Ϊ��������ʱ���������Ǻ�С�ġ�
//
// ���յĽ������������Ҫ�о��������ͬ�����⡣
//
//
/////////////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"

#include"AmericaStake.h"

UINT ThreadUpdateAmericaStakeDayLineDB(not_null<CAmericaStakePtr> pStake) {
  CString str;
  gl_ThreadStatus.IncreaseRunningThread();

  gl_ThreadStatus.IncreaseSavingDayLineThreads();
  gl_SaveAmericaOneStockDayLine.Wait();
  if (!gl_fExitingSystem) {
    pStake->SaveDayLine();
    pStake->UpdateDayLineStartEndDate();
    pStake->m_fUpdateDatabase = true;
    pStake->UnloadDayLine();
    str = pStake->GetSymbol() + _T("�������ϴ洢���");
    gl_systemMessage.PushDayLineInfoMessage(str);
  }
  gl_ThreadStatus.DecreaseSavingDayLineThreads();
  gl_SaveAmericaOneStockDayLine.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 36;
}