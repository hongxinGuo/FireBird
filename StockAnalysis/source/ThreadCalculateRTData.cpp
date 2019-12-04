///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理实时数据的工作线程。
//
// 此线程只操作各股票的m_queueRTData变量，这样可以将数据同步的问题减到最小。
// 此线程由CMarket类的SchedulingTaskPerSecond()函数调用，每秒一次。
//
// 此线程与存储实时临时数据的线程互斥。
//
// 返回值为3
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"Market.h"

UINT ThreadCalculateRTData(LPVOID) {
  ASSERT(gl_ChinaStockMarket.SystemReady()); // 调用本工作线程时必须设置好市场。
  ASSERT(!gl_ThreadStatus.IsSavingTempData()); // 此两个工作线程互斥

  gl_ThreadStatus.SetCalculatingRTData(true);
  if (gl_ThreadStatus.IsRTDataNeedCalculate()) { // 只有市场初始态设置好后，才允许处理实时数据。
    gl_ChinaStockMarket.ProcessRTData();
    gl_ThreadStatus.SetRTDataNeedCalculate(false);
  }
  gl_ThreadStatus.SetCalculatingRTData(false);

  return 3;
}