
#include"globedef.h"

#include"DayLine.h"
#include"Market.h"

#include"Accessory.h"

#include"ClientThread.h"

using namespace std;
#include<memory>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ���ݵĹ����̡߳�
//
// ���߳�ֻ��������Ʊ��m_dequeRTData�������������Խ�����ͬ�������������С��
// ���߳���CMarket���SchedulingTaskPerSecond()�������ã�ÿ��һ�Ρ�
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ClientThreadCalculatingRTDataProc(LPVOID pParam) {
  ASSERT(gl_ChinaStockMarket.SystemReady()); // ���ñ������߳�ʱ�������ú��г���
  ASSERT(!gl_systemStatus.IsSavingTempData()); // �����������̻߳���

  gl_systemStatus.SetCalculatingRTData(true);
  if (gl_systemStatus.IsRTDataNeedCalculate()) { // ֻ���г���ʼ̬���úú󣬲�������ʵʱ���ݡ�
    gl_ChinaStockMarket.CalculateRTData();
    gl_systemStatus.SetRTDataNeedCalculate(false); 
  }
  gl_systemStatus.SetCalculatingRTData(false);
 
  return 2;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// �˹����߳���δʵ��
//
//
///////////////////////////////////////////////////////////////////////////////////////
UINT ClientThreadUpdatingDataBaseProc(LPVOID pParam) {
  // 

  return 6;
}

///////////////////////////////////////////////////////////////////////////////////
//
// �����gl_lrelativeStrongEndDay��gl_lDay�����ǿ���̡߳�
//
// 
///////////////////////////////////////////////////////////////////////////////////
UINT ClientThreadCalculateRelativeStrongProc(LPVOID ) {
  const long year = gl_ChinaStockMarket.GetRelativeStrongEndDay() / 10000;
  const long month = gl_ChinaStockMarket.GetRelativeStrongEndDay() / 100 - year * 100;
  const long day = gl_ChinaStockMarket.GetRelativeStrongEndDay() - year * 10000 - month * 100;

  CTime ctCurrent(year, month, day, 12, 0, 0);

  DWORD dwToday = gl_ChinaStockMarket.GetRelativeStrongEndDay();

  const CTimeSpan oneDay(1, 0, 0, 0);

  if (dwToday >= gl_systemTime.GetDay()) return(true);

  time_t tStart = 0, tEnd = 0;
  time(&tStart);
  do {
    gl_ChinaStockMarket.SetRelativeStrongEndDay(dwToday); // ����������ڡ�
    if ((ctCurrent.GetDayOfWeek() != 1) // sunday
      && (ctCurrent.GetDayOfWeek() != 7)) { // saturday��sunday and saturday no data, so skiped.
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
  sprintf_s(buffer, "�����Ʊ���ǿ����ʱ%02dСʱ%02d����%02d��", hour, min, second);
  CString str = gl_systemTime.GetTimeString();
  str += buffer;
  gl_systemMessage.PushDayLineInfoMessage(str);

  return 1;
}

UINT ClientThreadSaveTempRTDataProc(LPVOID )
{
  ASSERT(gl_ChinaStockMarket.SystemReady()); // ���ñ������߳�ʱ�������ú��г���
  ASSERT(!gl_systemStatus.IsCalculatingRTData()); // �����������̻߳���

  gl_systemStatus.SetSavingTempData(true);
  
  gl_ChinaStockMarket.SaveTodayTempData();
  
  ASSERT(!gl_systemStatus.IsCalculatingRTData()); // �ٴ�ȷ��һ��
  gl_systemStatus.SetSavingTempData(false);

  return 10;
}


UINT ClientThreadReadingRTDataProc(LPVOID ) {
  CInternetSession session;
  CHttpFile * pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char * pChar = gl_stRTDataInquire.buffer;

  const clock_t tt = clock();

  try {
    gl_systemStatus.SetRTDataReadingInProcess(true);
    gl_stRTDataInquire.fError = false;
    gl_stRTDataInquire.lByteRead = 0;
    pFile = dynamic_cast<CHttpFile *>(session.OpenURL((LPCTSTR)gl_stRTDataInquire.strInquire));
    Sleep(100); // ���˷�����100ms�ӳټ��ɡ�
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_stRTDataInquire.lByteRead += iCount;
        }
      } while (iCount > 0);
      Sleep(30); // �ȴ�30������ٶ�һ�Σ�ȷ��û�������ݺ�ŷ��ء�
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

  gl_ChinaStockMarket.gl_RTReadingTime = clock() - tt;

  return 4;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ȡ����������ʷ���ݵ��̡߳�
//
// ��ȡ����ʱ���Լ��Σ���ֹ�����ӳٵ������ݽضϡ�Ŀǰ�ڶ������ݺ��ڲ������Σ���û�����ݺ�ŷ��ء�
//
// ��ʱfReadingInProcess == false�Ķ��Ի�ʧ�ܣ���������ֻ���������ô˱������ʹ����ǲ�ͬ�߳�ͬʱ������ԭ����Ҫ��Ϊ
// ͬ���¼�ģʽ���Ѵ��߳�Ϊ�á��о�֮�����ڵ��ô��߳�ǰ�����ã��Ͳ�����ֹ����ˣ��ɼ�ʱ�����߳�ʱ�������ʱ���£���
//
// ���߳����յĹ��ܣ�Ҫ������ֻ����ȡ����Ҫ��һЩ��Ӧ�ü������ʹ洢���ܡ�
// �༴���̵߳Ĺ���Ϊ��
// 1.�ӹܵ�����ȡ��Ҫ��ȡ������ʷ���ݵĹ�Ʊ���룬���ô��봦����������߷�������Ҫ��ĸ�ʽ��Ȼ���͸����߷�������
// 2.�ȴ�һ��ʱ���100ms����ʼ�ӷ��������������ݡ�
// 3.�����յ������ݽ��룬�ɹ��Ļ�������Ӧ��Ʊ�����������У����������Ӧ�ı�ʶ��
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ClientThreadReadDayLineProc(LPVOID ) {
  static int siDelayTime = 600;
  static bool fStarted = false;
  CInternetSession session;
  CHttpFile * pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char * pChar = gl_stDayLineInquire.buffer;
  CString str;

  const clock_t tt = clock();
  
  try {
    gl_systemStatus.SetReadingInProcess(true);
    gl_stDayLineInquire.fError = false;
    gl_stDayLineInquire.lByteRead = 0;
    pFile = dynamic_cast<CHttpFile *>(session.OpenURL((LPCTSTR)gl_stDayLineInquire.strInquire));
    Sleep(siDelayTime);
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          gl_stDayLineInquire.lByteRead += iCount;
        }
      } while (iCount > 0);
      Sleep(30); // �ȴ�50������ٶ�һ�Σ�ȷ��û�������ݺ�ȥ�������Σ������������
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        gl_stDayLineInquire.lByteRead += iCount;
      }
      else {
        Sleep(30); // �ȴ�50�����������Σ�ȷ��û�������ݺ�ŷ��أ������������
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

  gl_ChinaStockMarket.gl_DayLineReadingTime = clock() - tt;

  return 5;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���߳���ϵͳ�����к���15:05�Զ����ѣ�ÿ��ִֻ��һ��
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ClientThreadCompileTodayStocks(LPVOID ) {

  ASSERT(gl_ChinaStockMarket.SystemReady()); // ���ñ������߳�ʱ�������ú��г���

  time_t time = 0;
  switch (gl_systemTime.GetDayOfWeek()) {
  case 6: // ������
    time = gl_systemTime.Gett_time() - 24 * 3600; // 
    break;
  case 0: //������
    time = gl_systemTime.Gett_time() - 2 * 24 * 3600; // 
    break;
  default: // ����
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
  gl_ChinaStockMarket.SaveStockCodeDataBase(); // ���´��롣
  gl_ChinaStockMarket.UpdateOptionDataBase();
  gl_ChinaStockMarket.SetTodayStockCompiledFlag(true);

  return 7;
}


UINT ClientThreadSaveDayLineProc(LPVOID ) {

  gl_ChinaStockMarket.SaveDayLineData();

  gl_systemStatus.SetSavingDayLineInProcess(false);

  return 8;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����װ�ع����߳�
//
// �����ݿ���װ����Ӧ��Ʊ���������ݣ�Ȼ���������ǿ��
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ClientthreadLoadDayLineProc(LPVOID ) {
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

  // ��������ǿ��
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


