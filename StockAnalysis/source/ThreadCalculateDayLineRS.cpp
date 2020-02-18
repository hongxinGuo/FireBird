///////////////////////////////////////////////////////////////////////////////////
//
// 计算从gl_lrelativeStrongEndDay至gl_lDay的相对强度线程。
//
// 此线程调用ThreadCalculateRelativeStrongAtThisDayProc线程，最多同时生成16个线程。
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"ChinaMarket.h"
#include"Thread.h"

UINT ThreadCalculateDayLineRS(LPVOID startCalculatingDay) {
  gl_ThreadStatus.SetCalculatingDayLineRS(true);
  long lToday = (long)startCalculatingDay;

  const long year = lToday / 10000;
  const long month = lToday / 100 - year * 100;
  const long day = lToday - year * 10000 - month * 100;
  CTime ctCurrent(year, month, day, 12, 0, 0);
  const CTimeSpan oneDay(1, 0, 0, 0);

  if (lToday > gl_ChinaStockMarket.GetDay()) return(true);

  time_t tStart = 0, tEnd = 0;
  time(&tStart);
  do {
    if (gl_ChinaStockMarket.IsWorkingDay(ctCurrent)) { // 星期六和星期日无交易，略过
      // 调用工作线程，执行实际计算工作。 此类工作线程的优先级为最低，这样可以保证只利用CPU的空闲时间。
      AfxBeginThread(ThreadCalculateThisDayRS, (LPVOID)lToday, THREAD_PRIORITY_LOWEST);
    }
    ctCurrent += oneDay;
    lToday = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
  } while (lToday <= gl_ChinaStockMarket.GetDay()); // 计算至当前日期（包括今日）

  while (gl_ThreadStatus.IsCalculatingRS()) Sleep(1); // 等待所有的工作线程结束

  if (!gl_fExitingCalculatingRS) { // 如果顺利完成了计算任务
    gl_ChinaStockMarket.SetRelativeStrongEndDay(gl_ChinaStockMarket.GetDay());
    gl_ChinaStockMarket.SetUpdateOptionDB(true); // 更新选项数据库
    // 显示花费的时间
    time(&tEnd);
    const long tDiffer = tEnd - tStart;
    const long hour = tDiffer / 3600;
    const long min = tDiffer / 60 - hour * 60;
    const long second = tDiffer - hour * 3600 - min * 60;
    char buffer[100];
    sprintf_s(buffer, "计算股票相对强度用时%02d小时%02d分钟%02d秒", hour, min, second);
    CString str;
    str = buffer;
    gl_systemMessage.PushInformationMessage(str);
  }
  else {
    gl_fExitingCalculatingRS = false;// 如果是计算过程中止了，则重置中止标识。
    gl_systemMessage.PushInformationMessage(_T("中止了重新计算日线相对强度的过程"));
  }
  gl_ThreadStatus.SetCalculatingDayLineRS(false); // 本线程顺利退出，处于非运行状态

  return 11;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 计算给定日期的日线相对强度。使用C++11mutex和condition_variable构造的Semaphore。
//
// pParam： 给定的日期（长整型）
//
//
/////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadCalculateThisDayRS(LPVOID thisDay) {
  long lToday;

  gl_ThreadStatus.IncreaseNunberOfCalculatingRSThreads();     // 正在工作的线程数加一
  gl_SemaphoreCalculateDayLineRS.Wait();
  if (!gl_ExitingSystem && !gl_fExitingCalculatingRS) {
    lToday = (long)thisDay;
    gl_ChinaStockMarket.CalculateOneDayRelativeStrong(lToday);  // 调用实际执行函数
  }
  gl_ThreadStatus.DecreaseNumberOfCalculatingRSThreads(); // 正在工作的线程数减一
  gl_SemaphoreCalculateDayLineRS.Signal();

  return 12;
}