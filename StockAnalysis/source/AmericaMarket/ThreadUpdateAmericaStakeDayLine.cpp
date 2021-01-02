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

#include"AmericaStake.h"

UINT ThreadUpdateAmericaStakeDayLineDB(not_null<CAmericaStakePtr> pStock) {
  CString str;

  gl_ThreadStatus.IncreaseRunningThread();
  gl_ThreadStatus.IncreaseSavingDayLineThreads();
  gl_SaveAmericaOneStockDayLine.Wait();
  if (!gl_fExitingSystem) {
    pStock->SaveDayLine();
    pStock->UpdateDayLineStartEndDate();
    pStock->m_fUpdateDatabase = true;
    pStock->UnloadDayLine();
    str = pStock->GetSymbol() + _T("�������ϴ洢���");
    gl_systemMessage.PushDayLineInfoMessage(str);
  }
  gl_SaveAmericaOneStockDayLine.Signal();
  gl_ThreadStatus.DecreaseSavingDayLineThreads();
  gl_ThreadStatus.DecreaseRunningThread();

  return 36;
}

UINT ThreadUpdateAmericaStakeDayLineDB2(not_null<CAmericaMarket*> pMarket) {
  CString str;
  CAmericaStakePtr pStock = nullptr;

  gl_ThreadStatus.IncreaseRunningThread();
  gl_ThreadStatus.IncreaseSavingDayLineThreads();
  gl_SaveAmericaOneStockDayLine.Wait();
  for (long i = 0; i < pMarket->GetTotalStock(); i++) {
    pStock = pMarket->GetStock(i);
    if (pStock->IsDayLineNeedSavingAndClearFlag()) { // �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
      if (pStock->GetDayLineSize() > 0) {
        if (pStock->HaveNewDayLineData()) {
          pStock->SaveDayLine();
          pStock->UpdateDayLineStartEndDate();
          pStock->m_fUpdateDatabase = true;
          pStock->UnloadDayLine();
          str = pStock->GetSymbol() + _T("�������ϴ洢���");
          gl_systemMessage.PushDayLineInfoMessage(str);
          TRACE("����%s��������\n", pStock->GetSymbol().GetBuffer());
        }
        else pStock->UnloadDayLine(); // ������ִ�д洢����ʱ�����ﻹҪ����ж���������ݡ���洢���������߳��Ժ��ִ�У��ʶ������ڴ�ͳһִ��ɾ��������
      }
    }
    if (gl_fExitingSystem) {
      break; // ������������˳�����ֹͣ�洢��
    }
  }
  gl_SaveAmericaOneStockDayLine.Signal();
  gl_ThreadStatus.DecreaseSavingDayLineThreads();
  gl_ThreadStatus.DecreaseRunningThread();

  return 42;
}