///////////////////////////////////////////////////////////////////////////////////
//
// �����gl_lrelativeStrongEndDay��gl_lDay�����ǿ���̡߳�
//
// ���̵߳���ThreadCalculateRelativeStrongAtThisDayProc�̣߳����ͬʱ����16���̡߳�
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"Market.h"
#include"Thread.h"

UINT ThreadCalculateRS(LPVOID) {
  gl_ThreadStatus.SetCalculatingDayLineRS(true);

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
    if (gl_systemTime.IsWorkingDay(ctCurrent)) { // ���������������޽��ף��Թ�
      gl_ThreadStatus.IncreaseNunberOfCalculatingRSThreads();     // ���ڹ������߳�����һ
      // ���ù����̣߳�ִ��ʵ�ʼ��㹤���� ���๤���̵߳����ȼ�Ϊ��ͣ��������Ա�ֻ֤����CPU�Ŀ���ʱ�䡣
      AfxBeginThread(ThreadCalculateThisDayRS, (LPVOID)lToday, THREAD_PRIORITY_LOWEST);
    }
    ctCurrent += oneDay;
    lToday = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
  } while (lToday <= gl_systemTime.GetDay()); // ��������ǰ���ڣ��������գ�

  while (gl_ThreadStatus.IsCalculatingRS()) Sleep(1); // �ȴ����еĹ����߳̽���

  gl_ChinaStockMarket.UpdateOptionDB(); // ����ѡ�����ݿ�

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
  gl_systemMessage.PushInformationMessage(str);

  gl_ThreadStatus.SetCalculatingDayLineRS(false); // ���߳�˳���˳������ڷ�����״̬
  gl_fExitingCalculatingRS = false; // ����Ǽ��������ֹ�ˣ���������ֹ��ʶ��

  return 11;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����������ڵ��������ǿ�ȡ�ʹ��C++11mutex��condition_variable�����Semaphore��
//
// pParam�� ���������ڣ������ͣ�
//
//
/////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadCalculateThisDayRS(LPVOID pParam) {
  long lToday;

  gl_SemaphoreCalculateDayLineRS.Wait();
  if (!gl_ExitingSystem && !gl_fExitingCalculatingRS) {
    lToday = (long)pParam;
    gl_ChinaStockMarket.CalculateOneDayRelativeStrong(lToday);  // ����ʵ��ִ�к���
  }
  gl_ThreadStatus.DecreaseNumberOfCalculatingRSThreads(); // ���ڹ������߳�����һ
  gl_SemaphoreCalculateDayLineRS.Signal();

  return 12;
}