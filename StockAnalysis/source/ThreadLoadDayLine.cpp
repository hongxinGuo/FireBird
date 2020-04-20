#include"globedef.h"

#include"ChinaMarket.h"
#include"Thread.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����װ�ع����߳�
//
// �����ݿ���װ����Ӧ��Ʊ���������ݣ�Ȼ���������ǿ��
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadLoadDayLine(CChinaStockPtr pStock) {
  ASSERT(pStock != nullptr);
  ASSERT(!pStock->IsDayLineLoaded());

  gl_ThreadStatus.IncreaseRunningThread();
  pStock->UnloadDayLine();
  // װ����������
  pStock->LoadDayLine();
  // ��������ǿ�ȣ���ָ�����ǿ��ΪĬ��ֵ��
  pStock->CalculateDayLineRelativeStrongIndex();
  pStock->SetDayLineLoaded(true);
  gl_ThreadStatus.DecreaseRunningThread();

  return 16;
}