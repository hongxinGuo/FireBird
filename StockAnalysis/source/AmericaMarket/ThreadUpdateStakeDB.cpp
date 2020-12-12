#include"globedef.h"

UINT ThreadUpdateStakeDB(not_null<CAmericaStakeMarket*> pMarket) {
  while (gl_ThreadStatus.IsSavingDayLine()) Sleep(100); // 此线程工作时，必须等待存储日线数据的线程工作完成。
  pMarket->UpdateStakeDB();

  return 37;
}