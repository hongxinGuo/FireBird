///////////////////////////////////////////////////////////////////////////////////
//
// �����gl_lRelativeStrongEndDate��gl_lDay�����ǿ���̡߳�
//
// ���̵߳���ThreadBuildWeekLineRSOfDate�̣߳�Ŀǰ�������ͬʱ����8���̡߳�
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include <concurrencpp/executors/thread_pool_executor.h>

#include"Thread.h"
#include"TimeConvert.h"
#include"ThreadStatus.h"
#include"ChinaMarket.h"

UINT ThreadBuildWeekLineRS(const not_null<CChinaMarketPtr>& pMarket, long startCalculatingDate) {
	pMarket->SetCalculatingWeekLineRS(true);
	long lToday = GetPrevMonday(startCalculatingDate);

	const long year = lToday / 10000;
	const long month = lToday / 100 - year * 100;
	const long day = lToday - year * 10000 - month * 100;
	CTime ctCurrent(year, month, day, 12, 0, 0);
	const CTimeSpan sevenDays(7, 0, 0, 0);

	time_t tStart = 0, tEnd = 0;
	time(&tStart);
	do {
		// ���ù����̣߳�ִ��ʵ�ʼ��㹤���� ���๤���̵߳����ȼ�Ϊ��ͣ��������Ա�ֻ֤����CPU�Ŀ���ʱ�䡣
		// ÿ�ε���ʱ�����µľֲ����������������̺߳�ִ�з��붯����detach��������Դ��ϵͳ�ڹ����߳�ִ�������л��ա�
		gl_runtime.background_executor()->post([lToday] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_BackgroundWorkingThread.acquire();
			ASSERT(GetCurrentMonday(lToday) == lToday); // ȷ��������Ϊ����һ

			if (!gl_systemConfiguration.IsExitingSystem() && !gl_systemConfiguration.IsExitingCalculatingRS()) {
				gl_dataContainerChinaStock.BuildWeekLineRS(lToday);
			}
			gl_BackgroundWorkingThread.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
		ctCurrent += sevenDays;
		lToday = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
	} while (lToday <= pMarket->GetMarketDate()); // ��������ǰ���ڣ��������գ�

	while (gl_ThreadStatus.IsBackGroundThreadsWorking()) Sleep(100); // �ȴ����еĹ����߳̽���

	if (!gl_systemConfiguration.IsExitingCalculatingRS()) {
		// ���˳������˼�������
		// ��ʾ���ѵ�ʱ��
		time(&tEnd);
		const long tDiffer = tEnd - tStart;
		const long hour = tDiffer / 3600;
		const long min = tDiffer / 60 - hour * 60;
		const long second = tDiffer - hour * 3600 - min * 60;
		char buffer[100];
		sprintf_s(buffer, _T("�����Ʊ�������ǿ����ʱ%02dСʱ%02d����%02d��"), hour, min, second);
		const CString str = buffer;
		gl_systemMessage.PushInformationMessage(str);
	}
	else {
		gl_systemConfiguration.SetExitingCalculatingRS(false); // ����Ǽ��������ֹ�ˣ���������ֹ��ʶ��
		gl_systemMessage.PushInformationMessage(_T("��ֹ�����¼����������ǿ�ȵĹ���"));
	}
	pMarket->SetCalculatingWeekLineRS(false); // ���߳�˳���˳������ڷ�����״̬

	return 30;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����������ڵ��������ǿ�ȡ�
// �������˸������̲߳��С�
//
/////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadBuildWeekLineRSOfDate(long lDate) {
	gl_ThreadStatus.IncreaseBackGroundWorkingThread();
	gl_BackgroundWorkingThread.acquire();
	ASSERT(GetCurrentMonday(lDate) == lDate); // ȷ��������Ϊ����һ

	if (!gl_systemConfiguration.IsExitingSystem() && !gl_systemConfiguration.IsExitingCalculatingRS()) {
		gl_dataContainerChinaStock.BuildWeekLineRS(lDate);
	}
	gl_BackgroundWorkingThread.release();
	gl_ThreadStatus.DecreaseBackGroundWorkingThread();

	return 31;
}
