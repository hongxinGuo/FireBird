#include"globedef.h"

#include"DayLine.h"
#include"Market.h"

#include"Thread.h"

using namespace std;
#include<memory>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理实时数据的工作线程。
//
// 此线程只操作各股票的m_dequeRTData变量，这样可以将数据同步的问题减到最小。
// 此线程由CMarket类的SchedulingTaskPerSecond()函数调用，每秒一次。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadCalculatingRTDataProc(LPVOID) {
  ASSERT(gl_ChinaStockMarket.SystemReady()); // 调用本工作线程时必须设置好市场。
  ASSERT(!gl_ThreadStatus.IsSavingTempData()); // 此两个工作线程互斥

  gl_ThreadStatus.SetCalculatingRTData(true);
  if (gl_ThreadStatus.IsRTDataNeedCalculate()) { // 只有市场初始态设置好后，才允许处理实时数据。
    gl_ChinaStockMarket.CalculateRTData();
    gl_ThreadStatus.SetRTDataNeedCalculate(false);
  }
  gl_ThreadStatus.SetCalculatingRTData(false);

  return 3;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// 此工作线程尚未实现
//
//
///////////////////////////////////////////////////////////////////////////////////////
UINT ThreadUpdatingDataBaseProc(LPVOID) {
  //

  return 9;
}




UINT ThreadSaveTempRTDataProc(LPVOID)
{
  ASSERT(gl_ChinaStockMarket.SystemReady()); // 调用本工作线程时必须设置好市场。
  ASSERT(!gl_ThreadStatus.IsCalculatingRTData()); // 此两个工作线程互斥

  gl_ThreadStatus.SetSavingTempData(true);

  gl_ChinaStockMarket.SaveTodayTempData();

  ASSERT(!gl_ThreadStatus.IsCalculatingRTData()); // 再次确认一下
  gl_ThreadStatus.SetSavingTempData(false);

  return 4;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读取网易日线历史数据的线程。
//
// 读取数据时多试几次，防止网络延迟导致数据截断。目前在读完数据后，在测试两次，都没有数据后才返回。
//
// 有时fReadingInProcess == false的断言会失败，但程序中只有两处设置此变量，故估计是不同线程同时操作的原因，需要改为
// 同步事件模式唤醒此线程为好。研究之。（在调用此线程前就设置，就不会出现故障了，可见时启动线程时会出现延时所致）。
//
// 此线程最终的功能，要比现在只是提取数据要多一些，应该加入解码和存储功能（研究之，功能多会涉及更多的数据同步问题，需要权衡）。
// 亦即此线程的功能为：
// 1.从管道处读取需要提取日线历史数据的股票代码，将该代码处理成网易日线服务器所要求的格式，然后发送给日线服务器；
// 2.等待一段时间后（100ms）开始从服务器处接收数据。
// 3.将接收到的数据解码，成功的话存入相应股票的日线容器中，最后设置相应的标识。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadReadingNeteaseDayLineProc(LPVOID) {
  static int siDelayTime = 600;
  static bool fStarted = false;
  CInternetSession session;
  CHttpFile* pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char* pChar = gl_stDayLineInquire.buffer;
  CString str;

  const clock_t tt = clock();
  ASSERT(gl_ThreadStatus.IsNeteaseDayLineReadingInProcess());    // 调用此线程时已经设置了此标识
  try {
    gl_ThreadStatus.SetNeteaseDayLineReadingInProcess(true);
    gl_stDayLineInquire.fError = false;
    gl_stDayLineInquire.lByteRead = 0;
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)gl_stDayLineInquire.strInquire));
    Sleep(siDelayTime);
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_stDayLineInquire.lByteRead += iCount;
        }
      } while (iCount > 0);
      Sleep(30); // 等待50毫秒后再读一次，确认没有新数据后去读第三次，否则继续读。
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        gl_stDayLineInquire.lByteRead += iCount;
      }
      else {
        Sleep(30); // 等待50毫秒后读第三次，确认没有新数据后才返回，否则继续读。
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_stDayLineInquire.lByteRead += iCount;
        }
        else fDone = true;
      }
    }
    gl_stDayLineInquire.buffer[gl_stDayLineInquire.lByteRead] = 0x000;
    gl_ThreadStatus.SetDayLineDataReady(true);
  }
  catch (CInternetException * e) {  // 出现错误的话，简单报错即可，无需处理
    e->Delete();
    gl_stDayLineInquire.fError = true;
    gl_ThreadStatus.SetDayLineDataReady(false);
    gl_stDayLineInquire.lByteRead = 0;
  }
  if (pFile) pFile->Close();
  if (pFile) delete pFile;
  gl_ThreadStatus.SetNeteaseDayLineReadingInProcess(false);
  if (!fStarted) {
    fStarted = true;
    siDelayTime = 50;
  }

  gl_ChinaStockMarket.gl_DayLineReadingTime = clock() - tt;

  return 2;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 此线程由系统在收市后于15:05自动唤醒，每日只执行一次
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadCompileTodayStocks(LPVOID) {
  ASSERT(gl_ChinaStockMarket.SystemReady()); // 调用本工作线程时必须设置好市场。

  time_t time = 0;
  switch (gl_systemTime.GetDayOfWeek()) {
  case 6: // 星期六
    time = gl_systemTime.Gett_time() - 24 * 3600; //
    break;
  case 0: //星期日
    time = gl_systemTime.Gett_time() - 2 * 24 * 3600; //
    break;
  default: // 其他
    time = gl_systemTime.Gett_time();
    break;
  }
  tm tm_;
  localtime_s(&tm_, &time);
  long lCurrentTradeDay = 0;
  if (tm_.tm_hour < 9) {
    lCurrentTradeDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday - 1;
  }
  else {
    lCurrentTradeDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
  }

  int i;
  i = gl_ChinaStockMarket.CompileCurrentTradeDayStocks(lCurrentTradeDay);
  char buffer[30];
  sprintf_s(buffer, "%d", i);
  CString str;
  str = _T("今日处理了");
  str += buffer;
  str += _T("个股票");
  gl_systemMessage.PushInformationMessage(str);
  gl_ChinaStockMarket.CalculateOneDayRelativeStrong(lCurrentTradeDay);
  if (gl_systemTime.GetTime() > 150000) {   // 如果中国股市闭市了
    gl_ChinaStockMarket.SaveStockCodeDataBase();  // 更新代码。
    gl_ChinaStockMarket.UpdateOptionDataBase();   // 更新状态
    gl_ChinaStockMarket.SetTodayStockCompiledFlag(true);  // 设置今日已处理标识
  }

  return 5;
}

UINT ThreadSaveDayLineProc(LPVOID) {
  gl_ChinaStockMarket.SaveDayLineData();

  gl_ThreadStatus.SetSavingDayLineInProcess(false);

  return 6;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 日线装载工作线程
//
// 从数据库中装入相应股票的日线数据，然后计算各相对强度
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadLoadDayLineProc(LPVOID) {
  CSetDayLine setDayLine;
  CDayLinePtr pDayLine;

  setDayLine.m_strFilter = _T("[StockCode] = '");
  setDayLine.m_strFilter += gl_ChinaStockMarket.m_pCurrentStock->GetStockCode();
  setDayLine.m_strFilter += _T("'");
  setDayLine.m_strSort = _T("[Time]");
  setDayLine.Open();
  gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.clear();
  while (!setDayLine.IsEOF()) {
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetData(&setDayLine);
    gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.push_back(pDayLine);
    setDayLine.MoveNext();
  }

  // 计算各相对强度
  double dTempRS = 0;
  const long lTotalNumber = gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.size();
  for (int i = 3; i < lTotalNumber; i++) {
    dTempRS = 0;
    for (int j = i - 3; j < i; j++) {
      dTempRS += gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.at(j)->GetRelativeStrong();
    }
    gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.at(i)->m_d3DayRS = dTempRS / 3;
  }
  dTempRS = 0;
  for (int i = 5; i < lTotalNumber; i++) {
    dTempRS = 0;
    for (int j = i - 5; j < i; j++) {
      dTempRS += gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.at(j)->GetRelativeStrong();
    }
    gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.at(i)->m_d5DayRS = dTempRS / 5;
  }
  for (int i = 10; i < lTotalNumber; i++) {
    dTempRS = 0;
    for (int j = i - 10; j < i; j++) {
      dTempRS += gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.at(j)->GetRelativeStrong();
    }
    gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.at(i)->m_d10DayRS = dTempRS / 10;
  }
  for (int i = 30; i < lTotalNumber; i++) {
    dTempRS = 0;
    for (int j = i - 30; j < i; j++) {
      dTempRS += gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.at(j)->GetRelativeStrong();
    }
    gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.at(i)->m_d30DayRS = dTempRS / 30;
  }
  for (int i = 60; i < lTotalNumber; i++) {
    dTempRS = 0;
    for (int j = i - 60; j < i; j++) {
      dTempRS += gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.at(j)->GetRelativeStrong();
    }
    gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.at(i)->m_d60DayRS = dTempRS / 60;
  }
  for (int i = 120; i < lTotalNumber; i++) {
    dTempRS = 0;
    for (int j = i - 120; j < i; j++) {
      dTempRS += gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.at(j)->GetRelativeStrong();
    }
    gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.at(i)->m_d120DayRS = dTempRS / 120;
  }

  gl_ChinaStockMarket.m_pCurrentStock->SetDayLineLoaded(true);
  setDayLine.Close();

  return 7;
}