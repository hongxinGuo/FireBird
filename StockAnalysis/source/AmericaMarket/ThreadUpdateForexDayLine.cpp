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

#include"FinnhubForexSymbol.h"

UINT ThreadUpdateForexDayLineDB(not_null<CFinnhubForexSymbol*> pSymbol) {
  CString str;
  gl_ThreadStatus.IncreaseRunningThread();

  gl_ThreadStatus.IncreaseSavingDayLineThreads();
  gl_SaveForexDayLine.Wait();
  if (!gl_fExitingSystem) {
    pSymbol->SaveDayLine();
    pSymbol->UpdateDayLineStartEndDate();
    pSymbol->m_fUpdateDatabase = true;
    pSymbol->UnloadDayLine();
    str = pSymbol->GetSymbol() + _T("�������ϴ洢���");
    gl_systemMessage.PushDayLineInfoMessage(str);
  }
  gl_ThreadStatus.DecreaseSavingDayLineThreads();
  gl_SaveForexDayLine.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 38;
}