#include"globedef.h"

UINT ThreadUpdateStakeDB(not_null<CAmericaStakeMarket*> pMarket) {
  gl_SaveAmericaOneStockDayLine.Wait();
  while (gl_ThreadStatus.IsSavingDayLine()) Sleep(1); // ���̹߳���ʱ������ȴ��洢�������ݵ��̹߳�����ɡ�
  pMarket->UpdateStakeDB();
  gl_SaveAmericaOneStockDayLine.Signal();

  return 37;
}