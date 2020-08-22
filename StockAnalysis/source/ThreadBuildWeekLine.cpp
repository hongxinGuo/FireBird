///////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"
#include"Thread.h"

#include"ChinaMarket.h"

using namespace std;
#include<thread>

UINT ThreadBuildWeekLine(CChinaMarket* pMarket, long lStartDay) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_ThreadStatus.SetCreatingWeekLine(true);

  long lStartMonday = GetCurrentMonday(lStartDay);
  long year = lStartMonday / 10000;
  long month = lStartMonday / 100 - (lStartMonday / 10000) * 100;
  long mday = lStartMonday - (lStartMonday / 100) * 100;
  CTime ctCurrent(year, month, mday, 12, 0, 0), ctNext;
  const CTimeSpan ts7Day(7, 0, 0, 0);
  long lCurrentMonday = lStartMonday;

  if (lStartDay > 19900101) { // Ŀǰ�������ֻ�����������ɱ�������
    ASSERT(lStartMonday == GetCurrentMonday(pMarket->GetFormatedMarketDay()));
    do {
      pMarket->DeleteWeekLine(lCurrentMonday);
      ctCurrent += ts7Day;
      lCurrentMonday = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
    } while (lCurrentMonday <= pMarket->GetFormatedMarketDay());
  }
  else {
    pMarket->DeleteWeekLine();
  }

  // �����ǰ�����߱�
  pMarket->DeleteCurrentWeekWeekLine();

  pMarket->BuildWeekLine(lStartMonday);

  // �����µĵ�ǰ������
  pMarket->BuildCurrentWeekWeekLineTable();

  gl_ThreadStatus.SetCreatingWeekLine(false);
  gl_ThreadStatus.DecreaseRunningThread();

  return 25;
}

UINT ThreadBuildWeekLineOfStock(CChinaStockPtr pStock, long lStartDay) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_SemaphoreBackGroundTaskThreads.Wait();
  gl_ThreadStatus.IncreaseBackGroundWorkingthreads();
  if (!gl_fExitingSystem) pStock->BuildWeekLine(lStartDay);

  gl_ThreadStatus.DecreaseBackGroundWorkingthreads();
  gl_SemaphoreBackGroundTaskThreads.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 26;
}

UINT ThreadBuildWeekLineOfCurrentWeek(CChinaMarket* pMarket) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_SemaphoreBackGroundTaskThreads.Wait();
  gl_ThreadStatus.IncreaseBackGroundWorkingthreads();

  pMarket->BuildWeekLineOfCurrentWeek();

  gl_ThreadStatus.DecreaseBackGroundWorkingthreads();
  gl_SemaphoreBackGroundTaskThreads.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 32;
}

UINT ThreadBuildCurrentWeekWeekLineTable(CChinaMarket* pMarket) {
  gl_ThreadStatus.IncreaseRunningThread();

  // �����ǰ�����߱�
  pMarket->DeleteCurrentWeekWeekLine();

  // �����µĵ�ǰ������
  pMarket->BuildCurrentWeekWeekLineTable();

  gl_ThreadStatus.DecreaseRunningThread();

  return 33;
}