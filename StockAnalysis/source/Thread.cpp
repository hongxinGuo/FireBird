#include"globedef.h"

#include"DayLine.h"
#include"Market.h"

#include"Thread.h"

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
UINT ThreadCalculatingRTDataProc(LPVOID) {
  ASSERT(gl_ChinaStockMarket.SystemReady()); // ���ñ������߳�ʱ�������ú��г���
  ASSERT(!gl_ThreadStatus.IsSavingTempData()); // �����������̻߳���

  gl_ThreadStatus.SetCalculatingRTData(true);
  if (gl_ThreadStatus.IsRTDataNeedCalculate()) { // ֻ���г���ʼ̬���úú󣬲�������ʵʱ���ݡ�
    gl_ChinaStockMarket.CalculateRTData();
    gl_ThreadStatus.SetRTDataNeedCalculate(false);
  }
  gl_ThreadStatus.SetCalculatingRTData(false);

  return 3;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// �˹����߳���δʵ��
//
//
///////////////////////////////////////////////////////////////////////////////////////
UINT ThreadUpdatingDataBaseProc(LPVOID) {
  //

  return 9;
}




UINT ThreadSaveTempRTDataProc(LPVOID)
{
  ASSERT(gl_ChinaStockMarket.SystemReady()); // ���ñ������߳�ʱ�������ú��г���
  ASSERT(!gl_ThreadStatus.IsCalculatingRTData()); // �����������̻߳���

  gl_ThreadStatus.SetSavingTempData(true);

  gl_ChinaStockMarket.SaveTodayTempData();

  ASSERT(!gl_ThreadStatus.IsCalculatingRTData()); // �ٴ�ȷ��һ��
  gl_ThreadStatus.SetSavingTempData(false);

  return 4;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���߳���ϵͳ�����к���15:05�Զ����ѣ�ÿ��ִֻ��һ��
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadCompileTodayStocks(LPVOID) {
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

  int i;
  i = gl_ChinaStockMarket.CompileCurrentTradeDayStocks(lCurrentTradeDay);
  char buffer[30];
  sprintf_s(buffer, "%d", i);
  CString str;
  str = _T("���մ�����");
  str += buffer;
  str += _T("����Ʊ");
  gl_systemMessage.PushInformationMessage(str);
  gl_ChinaStockMarket.CalculateOneDayRelativeStrong(lCurrentTradeDay);
  if (gl_systemTime.GetTime() > 150000) {   // ����й����б�����
    gl_ChinaStockMarket.SaveStockCodeDataBase();  // ���´��롣
    gl_ChinaStockMarket.UpdateOptionDataBase();   // ����״̬
    gl_ChinaStockMarket.SetTodayStockCompiledFlag(true);  // ���ý����Ѵ����ʶ
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
// ����װ�ع����߳�
//
// �����ݿ���װ����Ӧ��Ʊ���������ݣ�Ȼ���������ǿ��
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

  return 7;
}