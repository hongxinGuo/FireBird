///////////////////////////////////////////////////////////////////////////////////
//
// �����gl_lrelativeStrongEndDate��gl_lDay�����ǿ���̡߳�
//
// ���̵߳���ThreadBuildWeekLineRSOfDate�̣߳�Ŀǰ�������ͬʱ����8���̡߳�
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include"globedef.h"
#include"Thread.h"
#include"Accessory.h"

#include"ChinaMarket.h"

using namespace std;
#include<thread>

UINT ThreadBuildWeekLineRS(not_null<CChinaMarket*> pMarket, long startCalculatingDate) {
  gl_ThreadStatus.SetCalculatingWeekLineRS(true);
  long lToday = GetPrevMonday(startCalculatingDate);

  const long year = lToday / 10000;
  const long month = lToday / 100 - year * 100;
  const long day = lToday - year * 10000 - month * 100;
  CTime ctCurrent(year, month, day, 12, 0, 0);
  const CTimeSpan sevenDay(7, 0, 0, 0);

  time_t tStart = 0, tEnd = 0;
  time(&tStart);
  do {
    // ���ù����̣߳�ִ��ʵ�ʼ��㹤���� ���๤���̵߳����ȼ�Ϊ��ͣ��������Ա�ֻ֤����CPU�Ŀ���ʱ�䡣
    // ÿ�ε���ʱ�����µľֲ����������������̺߳�ִ�з��붯����detach��������Դ��ϵͳ�ڹ����߳�ִ�������л��ա�
    pMarket->CreatingThreadBuildWeekLineRSOfDate(lToday);
    ctCurrent += sevenDay;
    lToday = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
  } while (lToday <= pMarket->GetFormatedMarketDate()); // ��������ǰ���ڣ��������գ�

  while (gl_ThreadStatus.IsBackGroundthreadsWorking()) Sleep(100); // �ȴ����еĹ����߳̽���

  if (!gl_fExitingCalculatingRS) { // ���˳������˼�������
    // ��ʾ���ѵ�ʱ��
    time(&tEnd);
    const long tDiffer = tEnd - tStart;
    const long hour = tDiffer / 3600;
    const long min = tDiffer / 60 - hour * 60;
    const long second = tDiffer - hour * 3600 - min * 60;
    char buffer[100];
    sprintf_s(buffer, _T("�����Ʊ�������ǿ����ʱ%02dСʱ%02d����%02d��"), hour, min, second);
    CString str;
    str = buffer;
    gl_systemMessage.PushInformationMessage(str);
  }
  else {
    gl_fExitingCalculatingRS = false;// ����Ǽ��������ֹ�ˣ���������ֹ��ʶ��
    gl_systemMessage.PushInformationMessage(_T("��ֹ�����¼����������ǿ�ȵĹ���"));
  }
  gl_ThreadStatus.SetCalculatingWeekLineRS(false); // ���߳�˳���˳������ڷ�����״̬

  return 30;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����������ڵ��������ǿ�ȡ�
//
//
/////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadBuildWeekLineRSOfDate(not_null<CChinaMarket*> pMarket, long lDate) {
  gl_SemaphoreBackGroundTaskThreads.Wait();
  gl_ThreadStatus.IncreaseBackGroundWorkingthreads();     // ���ڹ������߳�����һ
  const long year = lDate / 10000;
  const long month = lDate / 100 - year * 100;
  const long day = lDate - year * 10000 - month * 100;
  CTime ctCurrent(year, month, day, 12, 0, 0);
  const CTimeSpan oneDay(1, 0, 0, 0);

  ASSERT(GetCurrentMonday(lDate) == lDate); // ȷ��������Ϊ����һ

  if (!gl_fExitingSystem && !gl_fExitingCalculatingRS) {
    pMarket->BuildWeekLineRS(lDate);
  }
  gl_ThreadStatus.DecreaseBackGroundWorkingthreads(); // ���ڹ������߳�����һ
  gl_SemaphoreBackGroundTaskThreads.Signal();

  return 31;
}