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
  gl_SaveAmericaOneStockDayLine.Signal();
  gl_ThreadStatus.DecreaseSavingDayLineThreads();
  gl_ThreadStatus.DecreaseRunningThread();

  return 36;
}

UINT ThreadUpdateAmericaStakeDayLineDB2(not_null<CAmericaMarket*> pMarket) {
  CString str;
  CAmericaStakePtr pStake = nullptr;

  gl_ThreadStatus.IncreaseRunningThread();
  gl_ThreadStatus.IncreaseSavingDayLineThreads();
  for (long i = 0; i < pMarket->GetTotalStock(); i++) {
    pStake = pMarket->GetStock(i);
    if (pStake->IsDayLineNeedSavingAndClearFlag()) { // �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
      if (pStake->GetDayLineSize() > 0) {
        if (pStake->HaveNewDayLineData()) {
          pStake->SaveDayLine();
          pStake->UpdateDayLineStartEndDate();
          pStake->m_fUpdateDatabase = true;
          pStake->UnloadDayLine();
          str = pStake->GetSymbol() + _T("�������ϴ洢���");
          gl_systemMessage.PushDayLineInfoMessage(str);
          TRACE("����%s��������\n", pStake->GetSymbol().GetBuffer());
        }
        else pStake->UnloadDayLine(); // ������ִ�д洢����ʱ�����ﻹҪ����ж���������ݡ���洢���������߳��Ժ��ִ�У��ʶ������ڴ�ͳһִ��ɾ��������
      }
      else { // �������Ϊ�й�Ʊ���룬���˴�����δ����
        CString str1 = pStake->GetSymbol();
        str1 += _T(" Ϊδ���й�Ʊ����");
        gl_systemMessage.PushDayLineInfoMessage(str1);
      }
    }
    if (gl_fExitingSystem) {
      break; // ������������˳�����ֹͣ�洢��
    }
  }
  gl_ThreadStatus.DecreaseSavingDayLineThreads();
  gl_ThreadStatus.DecreaseRunningThread();

  return 42;
}