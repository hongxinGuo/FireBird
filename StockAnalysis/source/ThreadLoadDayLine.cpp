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

  gl_ThreadStatus.IncreaseRunningThread();
  pStock->UnloadDayLine();
  // װ����������
  pStock->LoadDayLine(pStock->GetStockCode());
  // ��������ǿ�ȣ���ָ�����ǿ��ΪĬ��ֵ��
  pStock->CalculateDayLineRelativeStrongIndex();
  pStock->SetDayLineLoaded(true);
  gl_ThreadStatus.DecreaseRunningThread();

  return 16;
}