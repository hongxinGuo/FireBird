#include"globedef.h"

#include"DayLine.h"
#include"Market.h"

#include"Accessory.h"

#include"ClientThread.h"

using namespace std;
#include<memory>

///////////////////////////////////////////////////////////////////////////////////
//
// �����gl_lrelativeStrongEndDay��gl_lDay�����ǿ���̡߳�
//
// ���̵߳���ClientThreadCalculateRelativeStrongAtThisDayProc�̣߳����ͬʱ����16���̡߳�
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
    gl_ChinaStockMarket.SetRelativeStrongEndDay(lToday); // ����������ڡ�
    if ((ctCurrent.GetDayOfWeek() != 1) // sunday�� CTime��1--7��ʶ��������������
      && (ctCurrent.GetDayOfWeek() != 7)) { // saturday��sunday and saturday no data, so skiped.
      while (!gl_ThreadStatus.IsCalculatingRSThreadAvailable());  // �ȴ��п��õ��̣߳����ͬʱ����16�������̣߳���Ҫ�����߳̾�Ҫ�ȴ������ɵĽ������У�
      // ���ù����̣߳�ִ��ʵ�ʼ��㹤���� ���๤���̵߳����ȼ�Ϊ��ͣ��������Ա�ֻ֤����CPU�Ŀ���ʱ�䡣
      AfxBeginThread(ClientThreadCalculateRealtiveStrongAtThisDayProc, (LPVOID)lToday, THREAD_PRIORITY_LOWEST);
    }
    if (gl_fExiting) return true;
    if (gl_fExitingCalculatingRelativeStrong) return true;
    ctCurrent += oneDay;
    lToday = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
  } while (lToday < gl_systemTime.GetDay());

  while (gl_ThreadStatus.IsCalculatingRSThreadRunning()); // �ȴ����еĹ����߳̽���

  gl_ChinaStockMarket.UpdateOptionDataBase(); // ����ѡ�����ݿ�

  // ��ʾ���ѵ�ʱ��
  time(&tEnd);
  const long tDiffer = tEnd - tStart;
  const long hour = tDiffer / 3600;
  const long min = tDiffer / 60 - hour * 60;
  const long second = tDiffer - hour * 3600 - min * 60;
  char buffer[100];
  sprintf_s(buffer, "�����Ʊ���ǿ����ʱ%02dСʱ%02d����%02d��", hour, min, second);
  CString str;
  str = buffer;
  gl_systemMessage.PushDayLineInfoMessage(str);

  gl_ChinaStockMarket.SetCalculatingRS(false);  // �������ǿ�ȵ�״̬Ϊ��
  gl_ThreadStatus.SetCalculateDayLineRS(false); // ���߳�˳���˳������ڷ�����״̬

  return 8;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����������ڵ��������ǿ�ȡ�
//
// pParam�� ���������ڣ������ͣ�
//
//
/////////////////////////////////////////////////////////////////////////////////////////
UINT ClientThreadCalculateRealtiveStrongAtThisDayProc(LPVOID pParam) {
  long lToday = (long)pParam;

  gl_ThreadStatus.IncreaseNunberOfCalculatingRSThreads();     // ���ڹ������߳�����һ 

  gl_ChinaStockMarket.CalculateOneDayRelativeStrong(lToday);  // ����ʵ��ִ�к���

  gl_ThreadStatus.DecreaseNumberOfCalculatingRSThreads(); // ���ڹ������߳�����һ

  return 11;
}