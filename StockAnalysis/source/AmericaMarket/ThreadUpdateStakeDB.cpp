#include"globedef.h"

UINT ThreadUpdateStakeDB(not_null<CAmericaStakeMarket*> pMarket) {
  while (gl_ThreadStatus.IsSavingDayLine()) Sleep(100); // ���̹߳���ʱ������ȴ��洢�������ݵ��̹߳�����ɡ�
  pMarket->UpdateStakeDB();

  return 37;
}