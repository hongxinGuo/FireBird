#include"globedef.h"

UINT ThreadUpdateForexSymbolDB(not_null<CAmericaStakeMarket*> pMarket) {
  gl_SaveAmericaOneStockDayLine.Wait();
  while (gl_ThreadStatus.IsSavingDayLine()) Sleep(100); // ���̹߳���ʱ������ȴ��洢�������ݵ��̹߳�����ɡ�
  pMarket->UpdateForexSymbolDB();
  gl_SaveAmericaOneStockDayLine.Signal();

  return 39;
}