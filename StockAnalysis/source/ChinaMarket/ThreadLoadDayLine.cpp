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
UINT ThreadLoadDayLine(not_null<CChinaStock*> pStake) {
  gl_ThreadStatus.IncreaseRunningThread();
  pStake->UnloadDayLine();
  // װ����������
  pStake->LoadDayLine(pStake->GetStockCode());
  // ��������ǿ�ȣ���ָ�����ǿ��ΪĬ��ֵ��
  pStake->CalculateDayLineRSIndex();
  pStake->SetDayLineLoaded(true);
  gl_ThreadStatus.DecreaseRunningThread();

  return 16;
}