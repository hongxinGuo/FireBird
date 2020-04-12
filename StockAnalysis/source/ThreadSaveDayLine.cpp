//////////////////////////////////////////////////////////////////////////////////////
//
// 存储日线历史数据库时，估计还是同步问题，导致当数据库中存在旧数据时，更新到中途就会发生数据库同步锁的互斥，
// 由于我对MySQL数据库不太熟悉，无法找到解决方法，故而采用两个信号量来分别处理。即当数据库为空（系统初置)时,
// 使用多线程方式存储数据；而当数据库中存在旧数据时，使用单一线程存储。
// 这种方式并不影响速度，因为更新数据时，数据量是很小的。
//
// 最终的解决方法，还是要研究如何消除同步问题。
//
//
/////////////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"

UINT ThreadSaveDayLineOfOneStock(CChinaStockPtr pStock) {
  CString str;
  bool fDataSaved = false;
  gl_ThreadStatus.IncreaseRunningThread();

  gl_ThreadStatus.IncreaseSavingDayLineThreads();
  gl_SaveOneStockDayLine.Wait(); //使用多线程模式（重新生成全部历史日线时使用4个线程；更新历史日线时只使用一个线程，此时使用多个线程服务器出现互斥错误）。
  if (!gl_ExitingSystem) {
    fDataSaved = pStock->SaveDayLine();
    pStock->SetDayLineLoaded(false);
    pStock->ClearDayLineContainer();
    if (fDataSaved) {
      str = pStock->GetStockCode() + _T("日线资料存储完成");
      gl_systemMessage.PushDayLineInfoMessage(str);
    }
  }
  gl_ThreadStatus.DecreaseSavingDayLineThreads();
  gl_SaveOneStockDayLine.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 15;
}