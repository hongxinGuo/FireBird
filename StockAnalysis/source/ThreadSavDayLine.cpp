#include"Market.h"

UINT ThreadSaveDayLineOfOneStock(LPVOID pParam) {
  // 传递过来的为携带智能指针的结构（智能指针无法经由pParam直接传递过来
  // 或者传递过来的是从智能指针中取出的原始指针，注意不能再用之生成智能指针（否则自动解析）
  CStock* pStock;
  CString str;

  CSingleLock singleLock(&gl_SaveOneStockDayLine);
  singleLock.Lock();

  gl_ThreadStatus.IncreaseNunberOfSavingDayLineThreads();
  pStock = (CStock*)pParam;
  pStock->SaveDayLine();
  pStock->SetDayLineLoaded(false);
  pStock->m_vDayLine.clear();
  gl_ThreadStatus.DecreaseNumberOfSavingDayLineThreads();
  ASSERT(pStock != nullptr);
  singleLock.Unlock();

  return 6;
}