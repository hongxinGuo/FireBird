#include"globedef.h"

#include"ChinaMarket.h"
#include"Thread.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 日线装载工作线程
//
// 从数据库中装入相应股票的日线数据，然后计算各相对强度
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadLoadDayLine(not_null<CChinaStake*> pStake) {
  gl_ThreadStatus.IncreaseRunningThread();
  pStake->UnloadDayLine();
  // 装入日线数据
  pStake->LoadDayLine(pStake->GetStakeCode());
  // 计算各相对强度（以指数相对强度为默认值）
  pStake->CalculateDayLineRSIndex();
  pStake->SetDayLineLoaded(true);
  gl_ThreadStatus.DecreaseRunningThread();

  return 16;
}