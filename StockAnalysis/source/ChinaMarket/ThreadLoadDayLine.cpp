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
UINT ThreadLoadDayLine(not_null<CChinaStock*> pStock) {
  gl_ThreadStatus.IncreaseRunningThread();
  pStock->UnloadDayLine();
  // װ����������
  pStock->LoadDayLine(pStock->GetStockCode());
  // ��������ǿ�ȣ���ָ�����ǿ��ΪĬ��ֵ��
  pStock->CalculateDayLineRSIndex();
  pStock->SetDayLineLoaded(true);
  gl_ThreadStatus.DecreaseRunningThread();

  return 16;
}