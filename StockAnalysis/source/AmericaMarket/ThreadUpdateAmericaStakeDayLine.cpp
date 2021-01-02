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

#include"AmericaStake.h"

UINT ThreadUpdateAmericaStakeDayLineDB(not_null<CAmericaStakePtr> pStock) {
  CString str;

  gl_ThreadStatus.IncreaseRunningThread();
  gl_ThreadStatus.IncreaseSavingDayLineThreads();
  gl_SaveAmericaOneStockDayLine.Wait();
  if (!gl_fExitingSystem) {
    pStock->SaveDayLine();
    pStock->UpdateDayLineStartEndDate();
    pStock->m_fUpdateDatabase = true;
    pStock->UnloadDayLine();
    str = pStock->GetSymbol() + _T("日线资料存储完成");
    gl_systemMessage.PushDayLineInfoMessage(str);
  }
  gl_SaveAmericaOneStockDayLine.Signal();
  gl_ThreadStatus.DecreaseSavingDayLineThreads();
  gl_ThreadStatus.DecreaseRunningThread();

  return 36;
}

UINT ThreadUpdateAmericaStakeDayLineDB2(not_null<CAmericaMarket*> pMarket) {
  CString str;
  CAmericaStakePtr pStock = nullptr;

  gl_ThreadStatus.IncreaseRunningThread();
  gl_ThreadStatus.IncreaseSavingDayLineThreads();
  gl_SaveAmericaOneStockDayLine.Wait();
  for (long i = 0; i < pMarket->GetTotalStock(); i++) {
    pStock = pMarket->GetStock(i);
    if (pStock->IsDayLineNeedSavingAndClearFlag()) { // 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
      if (pStock->GetDayLineSize() > 0) {
        if (pStock->HaveNewDayLineData()) {
          pStock->SaveDayLine();
          pStock->UpdateDayLineStartEndDate();
          pStock->m_fUpdateDatabase = true;
          pStock->UnloadDayLine();
          str = pStock->GetSymbol() + _T("日线资料存储完成");
          gl_systemMessage.PushDayLineInfoMessage(str);
          TRACE("更新%s日线数据\n", pStock->GetSymbol().GetBuffer());
        }
        else pStock->UnloadDayLine(); // 当无需执行存储函数时，这里还要单独卸载日线数据。因存储日线数据线程稍后才执行，故而不能在此统一执行删除函数。
      }
    }
    if (gl_fExitingSystem) {
      break; // 如果程序正在退出，则停止存储。
    }
  }
  gl_SaveAmericaOneStockDayLine.Signal();
  gl_ThreadStatus.DecreaseSavingDayLineThreads();
  gl_ThreadStatus.DecreaseRunningThread();

  return 42;
}