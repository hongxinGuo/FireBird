#include"globedef.h"

#include"DayLine.h"
#include"Market.h"

#include"Accessory.h"

#include"ClientThread.h"

using namespace std;
#include<memory>

///////////////////////////////////////////////////////////////////////////////////
//
// 计算从gl_lrelativeStrongEndDay至gl_lDay的相对强度线程。
//
// 此线程调用ClientThreadCalculateRelativeStrongAtThisDayProc线程，最多同时生成16个线程。
//
//
///////////////////////////////////////////////////////////////////////////////////
UINT ClientThreadCalculateRelativeStrongProc(LPVOID) {
  gl_ThreadStatus.SetCalculateDayLineRS(true);

  const long year = gl_ChinaStockMarket.GetRelativeStrongEndDay() / 10000;
  const long month = gl_ChinaStockMarket.GetRelativeStrongEndDay() / 100 - year * 100;
  const long day = gl_ChinaStockMarket.GetRelativeStrongEndDay() - year * 10000 - month * 100;

  CTime ctCurrent(year, month, day, 12, 0, 0);

  long lToday = gl_ChinaStockMarket.GetRelativeStrongEndDay();

  const CTimeSpan oneDay(1, 0, 0, 0);

  if (lToday >= gl_systemTime.GetDay()) return(true);

  time_t tStart = 0, tEnd = 0;
  time(&tStart);
  do {
    gl_ChinaStockMarket.SetRelativeStrongEndDay(lToday); // 设置最后日期。
    if ((ctCurrent.GetDayOfWeek() != 1) // sunday， CTime用1--7标识星期日至星期六
      && (ctCurrent.GetDayOfWeek() != 7)) { // saturday，sunday and saturday no data, so skiped.
      while (!gl_ThreadStatus.IsCalculatingRSThreadAvailable());  // 等待有可用的线程（最多同时生成16个工作线程，再要生成线程就要等待已生成的结束才行）
      // 调用工作线程，执行实际计算工作。 此类工作线程的优先级为最低，这样可以保证只利用CPU的空闲时间。
      AfxBeginThread(ClientThreadCalculateRealtiveStrongAtThisDayProc, (LPVOID)lToday, THREAD_PRIORITY_LOWEST);
    }
    if (gl_fExiting) return true;
    if (gl_fExitingCalculatingRelativeStrong) return true;
    ctCurrent += oneDay;
    lToday = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
  } while (lToday < gl_systemTime.GetDay());

  while (gl_ThreadStatus.IsCalculatingRSThreadRunning()); // 等待所有的工作线程结束

  gl_ChinaStockMarket.UpdateOptionDataBase(); // 更新选项数据库

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
  gl_systemMessage.PushDayLineInfoMessage(str);

  gl_ChinaStockMarket.SetCalculatingRS(false);  // 计算相对强度的状态为假
  gl_ThreadStatus.SetCalculateDayLineRS(false); // 本线程顺利退出，处于非运行状态

  return 8;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 计算给定日期的日线相对强度。
//
// pParam： 给定的日期（长整型）
//
//
/////////////////////////////////////////////////////////////////////////////////////////
UINT ClientThreadCalculateRealtiveStrongAtThisDayProc(LPVOID pParam) {
  long lToday = (long)pParam;

  gl_ThreadStatus.IncreaseNunberOfCalculatingRSThreads();     // 正在工作的线程数加一 

  gl_ChinaStockMarket.CalculateOneDayRelativeStrong(lToday);  // 调用实际执行函数

  gl_ThreadStatus.DecreaseNumberOfCalculatingRSThreads(); // 正在工作的线程数减一

  return 11;
}