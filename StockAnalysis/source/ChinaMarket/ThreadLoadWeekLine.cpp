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
UINT ThreadLoadWeekLine(not_null<CChinaStock*> pStake) {
  gl_ThreadStatus.IncreaseRunningThread();
  pStake->UnloadWeekLine();
  // װ����������
  pStake->LoadWeekLine();
  // ��������ǿ�ȣ���ָ�����ǿ��ΪĬ��ֵ��
  pStake->CalculateWeekLineRSIndex();
  pStake->SetWeekLineLoaded(true);
  gl_ThreadStatus.DecreaseRunningThread();

  return 29;
}