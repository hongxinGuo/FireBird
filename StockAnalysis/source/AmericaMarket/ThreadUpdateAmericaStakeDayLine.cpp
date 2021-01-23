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

#include"WorldStock.h"

UINT ThreadUpdateWorldStockDayLineDB(not_null<CWorldMarket*> pMarket) {
  CString str;
  CWorldStockPtr pStock = nullptr;

  gl_ThreadStatus.IncreaseRunningThread();
  gl_ThreadStatus.IncreaseSavingDayLineThreads();
  gl_UpdateWorldMarketDB.Wait();
  for (long i = 0; i < pMarket->GetTotalStock(); i++) {
    pStock = pMarket->GetStock(i);
    if (pStock->IsDayLineNeedSavingAndClearFlag()) { // �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
      if (pStock->GetDayLineSize() > 0) {
        if (pStock->HaveNewDayLineData()) {
          pStock->SaveDayLine();
          pStock->UpdateDayLineStartEndDate();
          pStock->m_fUpdateDatabase = true;
          str = pStock->GetSymbol() + _T("�������ϴ洢���");
          gl_systemMessage.PushDayLineInfoMessage(str);
          TRACE("����%s��������\n", pStock->GetSymbol().GetBuffer());
        }
        pStock->UnloadDayLine();
      }
    }
    if (gl_fExitingSystem) {
      break; // ������������˳�����ֹͣ�洢��
    }
  }
  gl_UpdateWorldMarketDB.Signal();
  gl_ThreadStatus.DecreaseSavingDayLineThreads();
  gl_ThreadStatus.DecreaseRunningThread();

  return 42;
}