///////////////////////////////////////////////////////////////////////////////////
//
// �����gl_lrelativeStrongEndDate��gl_lDay�����ǿ���̡߳�
//
// ���̵߳���ThreadBuildDayLineRSOfDay�̣߳�Ŀǰ�������ͬʱ����8���̡߳�
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"
#include"Thread.h"

#include"ChinaMarket.h"

using namespace std;
#include<thread>

UINT ThreadBuildDayLineRS(CChinaMarket* pMarket, long startCalculatingDay) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_ThreadStatus.SetCalculatingDayLineRS(true);
  long lToday = startCalculatingDay;

  const long year = lToday / 10000;
  const long month = lToday / 100 - year * 100;
  const long day = lToday - year * 10000 - month * 100;
  CTime ctCurrent(year, month, day, 12, 0, 0);
  const CTimeSpan oneDay(1, 0, 0, 0);

  time_t tStart = 0, tEnd = 0;
  time(&tStart);
  do {
    if (pMarket->IsWorkingDay(ctCurrent)) { // ���������������޽��ף��Թ�
      // ���ù����̣߳�ִ��ʵ�ʼ��㹤���� ���๤���̵߳����ȼ�Ϊ��ͣ��������Ա�ֻ֤����CPU�Ŀ���ʱ�䡣
      // ÿ�ε���ʱ�����µľֲ����������������̺߳�ִ�з��붯����detach��������Դ��ϵͳ�ڹ����߳�ִ�������л��ա�
      pMarket->RunningThreadBuildDayLineRSOfDay(lToday);
    }
    ctCurrent += oneDay;
    lToday = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
  } while (lToday <= pMarket->GetFormatedMarketDate()); // ��������ǰ���ڣ��������գ�

  while (gl_ThreadStatus.IsBackGroundthreadsWorking()) Sleep(100); // �ȴ����еĹ����߳̽���

  if (!gl_fExitingCalculatingRS) { // ���˳������˼�������
    pMarket->SetRelativeStrongEndDate(pMarket->GetFormatedMarketDate());
    pMarket->SetUpdateOptionDB(true); // ����ѡ�����ݿ�
    // ��ʾ���ѵ�ʱ��
    time(&tEnd);
    const long tDiffer = tEnd - tStart;
    const long hour = tDiffer / 3600;
    const long min = tDiffer / 60 - hour * 60;
    const long second = tDiffer - hour * 3600 - min * 60;
    char buffer[100];
    sprintf_s(buffer, _T("�����Ʊ���ǿ����ʱ%02dСʱ%02d����%02d��"), hour, min, second);
    CString str;
    str = buffer;
    gl_systemMessage.PushInformationMessage(str);
  }
  else {
    gl_fExitingCalculatingRS = false;// ����Ǽ��������ֹ�ˣ���������ֹ��ʶ��
    gl_systemMessage.PushInformationMessage(_T("��ֹ�����¼����������ǿ�ȵĹ���"));
  }
  gl_ThreadStatus.SetCalculatingDayLineRS(false); // ���߳�˳���˳������ڷ�����״̬
  gl_ThreadStatus.DecreaseRunningThread();

  return 11;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����������ڵ��������ǿ�ȡ�
//
//
/////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadBuildDayLineRSOfDay(CChinaMarket* pMarket, long thisDay) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_SemaphoreBackGroundTaskThreads.Wait();
  gl_ThreadStatus.IncreaseBackGroundWorkingthreads();     // ���ڹ������߳�����һ
  if (!gl_fExitingSystem && !gl_fExitingCalculatingRS) {
    pMarket->BuildDayLineRSOfDay(thisDay);  // ����ʵ��ִ�к���
  }
  gl_ThreadStatus.DecreaseBackGroundWorkingthreads(); // ���ڹ������߳�����һ
  gl_SemaphoreBackGroundTaskThreads.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 12;
}