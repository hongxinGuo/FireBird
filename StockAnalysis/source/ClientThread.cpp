
#include"globedef.h"

#include"DayLine.h"
#include"Market.h"

#include"Accessory.h"

#include"ClientThread.h"

using namespace std;
#include<memory>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理实时数据的工作线程。
//
// 此线程只操作各股票的m_dequeRTData变量，这样可以将数据同步的问题减到最小。
// 此线程在系统启动后就一直工作着，直到系统退出时才跟随系统一起退出。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ClientThreadCalculatingRTDataProc(LPVOID pParam) {

  while (true) { //此线程永远执行，直到得到系统通知
    if (gl_fExiting) { // 
      return 0; // 
    }
    if (gl_ChinaStockMarket.MarketReady() && gl_systemStatus.IsRTDataNeedCalculate()) { // 只有市场初始态设置好后，才允许处理实时数据。
      gl_ChinaStockMarket.CalculateRTData();
      gl_systemStatus.SetRTDataNeedCalculate(false); 
    }
    Sleep(50); // 暂停50毫秒。当计算繁忙时，无所谓是否暂停；当没有计算任务时，此50毫秒能够保证此线程不过多占用系统计算能力。
    // 研究使用挂起唤醒机制节约计算能力。
    // 采用事件方式。实现之。
  }
  return 2;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// 此工作线程尚未实现
//
//
///////////////////////////////////////////////////////////////////////////////////////
UINT ClientThreadUpdatingDataBaseProc(LPVOID pParam) {
  // 

  return 6;
}

///////////////////////////////////////////////////////////////////////////////////
//
// 计算从gl_lrelativeStrongEndDay至gl_lDay的相对强度线程。
//
// 
///////////////////////////////////////////////////////////////////////////////////
UINT ClientThreadCalculateRelativeStrongProc(LPVOID pParam) {
  const long year = gl_ChinaStockMarket.GetRelativeStrongEndDay() / 10000;
  const long month = gl_ChinaStockMarket.GetRelativeStrongEndDay() / 100 - year * 100;
  const long day = gl_ChinaStockMarket.GetRelativeStrongEndDay() - year * 10000 - month * 100;

  CTime ctCurrent(year, month, day, 12, 0, 0);

  DWORD dwToday = gl_ChinaStockMarket.GetRelativeStrongEndDay();

  const CTimeSpan oneDay(1, 0, 0, 0);

  if (dwToday >= gl_systemTime.GetDay()) return(true);

  time_t tStart, tEnd;
  time(&tStart);
  do {
    gl_ChinaStockMarket.SetRelativeStrongEndDay(dwToday); // 设置最后日期。
    if ((ctCurrent.GetDayOfWeek() != 1) // sunday
      && (ctCurrent.GetDayOfWeek() != 7)) { // saturday，sunday and saturday no data, so skiped.
      gl_ChinaStockMarket.CalculateOneDayRelativeStrong(dwToday);
    }
    if (gl_fExiting) return true;
    ctCurrent += oneDay;
    dwToday = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
  } while (dwToday < gl_systemTime.GetDay());

  gl_ChinaStockMarket.UpdateOptionDataBase();

  time(&tEnd);
  const long tDiffer = tEnd - tStart;
  const long hour = tDiffer / 3600;
  const long min = tDiffer / 60 - hour * 60;
  const long second = tDiffer - hour * 3600 - min * 60;
  char buffer[100];
  sprintf_s(buffer, "计算股票相对强度用时%02d小时%02d分钟%02d秒", hour, min, second);
  CString str = gl_systemTime.GetTimeString();
  str += buffer;
  gl_systemMessage.PushDayLineInfoMessage(str);

  return 1;
}


UINT ClientThreadReadingRTDataProc(LPVOID pParam) {
  CInternetSession session;
  CHttpFile * pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char * pChar = gl_stRTDataInquire.buffer;

  clock_t tt = clock();

  try {
    gl_systemStatus.SetRTDataReadingInProcess(true);
    gl_stRTDataInquire.fError = false;
    gl_stRTDataInquire.lByteRead = 0;
    pFile = dynamic_cast<CHttpFile *>(session.OpenURL((LPCTSTR)gl_stRTDataInquire.strInquire));
    Sleep(100); // 新浪服务器100ms延迟即可。
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_stRTDataInquire.lByteRead += iCount;
        }
      } while (iCount > 0);
      Sleep(30); // 等待30毫秒后再读一次，确认没有新数据后才返回。
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        gl_stRTDataInquire.lByteRead += iCount;
      }
      else fDone = true;
    }
    gl_stRTDataInquire.buffer[gl_stRTDataInquire.lByteRead] = 0x000;
    gl_systemStatus.SetRTDataReceived(true);
  }
  catch (CInternetException * e) {
    e->Delete();
    gl_stRTDataInquire.fError = true;
    gl_systemStatus.SetRTDataReceived(false);
  }
  if (pFile) pFile->Close();
  if (pFile) delete pFile;
  gl_systemStatus.SetRTDataReadingInProcess(false);

  gl_RTReadingTime = clock() - tt;

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
// 此线程最终的功能，要比现在只是提取数据要多一些，应该加入解码和存储功能。
// 亦即此线程的功能为：
// 1.从管道处读取需要提取日线历史数据的股票代码，将该代码处理成网易日线服务器所要求的格式，然后发送给日线服务器；
// 2.等待一段时间后（100ms）开始从服务器处接收数据。
// 3.将接收到的数据解码，成功的话存入相应股票的日线容器中，最后设置相应的标识。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ClientThreadReadDayLineProc(LPVOID pParam) {
  static int siDelayTime = 600;
  static bool fStarted = false;
  CInternetSession session;
  CHttpFile * pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char * pChar = gl_stDayLineInquire.buffer;
  CString str;

  clock_t tt = clock();
  
  try {
    gl_systemStatus.SetReadingInProcess(true);
    gl_stDayLineInquire.fError = false;
    gl_stDayLineInquire.lByteRead = 0;
    pFile = dynamic_cast<CHttpFile *>(session.OpenURL((LPCTSTR)gl_stDayLineInquire.strInquire));
    // 不能采用下述读取文件长度的方式。读取出来的数值不对。
    // pFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, str);
    Sleep(siDelayTime);
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_stDayLineInquire.lByteRead += iCount;
        }
      } while (iCount > 0);
      Sleep(50); // 等待50毫秒后再读一次，确认没有新数据后去读第三次，否则继续读。
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        gl_stDayLineInquire.lByteRead += iCount;
      }
      else {
        Sleep(50); // 等待50毫秒后读第三次，确认没有新数据后才返回，否则继续读。
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_stDayLineInquire.lByteRead += iCount;
        }
        else fDone = true;
      }
    }
    gl_stDayLineInquire.buffer[gl_stDayLineInquire.lByteRead] = 0x000;
    gl_systemStatus.SetDayLineDataReady(true);
  }
  catch (CInternetException * e) {
    e->Delete();
    gl_stDayLineInquire.fError = true;
    gl_systemStatus.SetDayLineDataReady(false);
    gl_stDayLineInquire.lByteRead = 0;
  }
  if (pFile) pFile->Close();
  if (pFile) delete pFile;
  gl_systemStatus.SetReadingInProcess(false);
  if (!fStarted) {
    fStarted = true;
    siDelayTime = 50;
  }

  gl_DayLineReadingTime = clock() - tt;

  return 5;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 此线程由系统在收市后于15:05自动唤醒，每日只执行一次
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ClientThreadCompileTodayStocks(LPVOID pParam) {

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

  int i = gl_ChinaStockMarket.GetTotalStock();
  gl_ChinaStockMarket.CompileCurrentTradeDayStocks(lCurrentTradeDay);
  i = gl_ChinaStockMarket.GetTotalStock();
  gl_ChinaStockMarket.CalculateOneDayRelativeStrong(lCurrentTradeDay);
  gl_ChinaStockMarket.UpdateStockCodeDataBase(); // 更新代码。
  gl_ChinaStockMarket.SetTodayStockCompiledFlag(true);

  return 7;
}


UINT ClientThreadSaveDayLineProc(LPVOID pParam) {

  gl_ChinaStockMarket.SaveDayLineData();

  gl_systemStatus.SetDataBaseInProcess(false);

  return 8;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 日线装载工作线程
//
// 从数据库中装入相应股票的日线数据，然后计算各相对强度
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ClientthreadLoadDayLineProc(LPVOID pParam) {
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

  return 9;
}

