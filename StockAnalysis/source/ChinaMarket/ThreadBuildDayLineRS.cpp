///////////////////////////////////////////////////////////////////////////////////
//
// �����gl_lrelativeStrongEndDate��gl_lDay�����ǿ���̡߳�
//
// ���̵߳���ThreadBuildDayLineRSOfDate�̣߳�Ŀǰ�������ͬʱ����8���̡߳�
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ThreadStatus.h"
#include"Thread.h"

#include"ChinaMarket.h"
#include"WorldMarket.h"

using namespace std;
#include<thread>

UINT ThreadBuildDayLineRS(not_null<CChinaMarket*> pMarket, long startCalculatingDate) {
	gl_ThreadStatus.SetCalculatingDayLineRS(true);
	long lToday = startCalculatingDate;

	const long year = lToday / 10000;
	const long month = lToday / 100 - year * 100;
	const long day = lToday - year * 10000 - month * 100;
	CTime ctCurrent(year, month, day, 12, 0, 0);
	const CTimeSpan oneDay(1, 0, 0, 0);

	gl_systemMessage.PushInformationMessage(_T("��ʼ�����Ʊ���ǿ��"));
	time_t tStart = 0, tEnd = 0;
	time(&tStart);
	do {
		if (pMarket->IsWorkingDay(ctCurrent)) { // ���������������޽��ף��Թ�
			// ���ù����̣߳�ִ��ʵ�ʼ��㹤���� ���๤���̵߳����ȼ�Ϊ��ͣ��������Ա�ֻ֤����CPU�Ŀ���ʱ�䡣
			// ÿ�ε���ʱ�����µľֲ����������������̺߳�ִ�з��붯����detach��������Դ��ϵͳ�ڹ����߳�ִ�������л��ա�
			pMarket->CreatingThreadBuildDayLineRSOfDate(lToday);
		}
		ctCurrent += oneDay;
		lToday = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
	} while (lToday <= pMarket->GetMarketDate()); // ��������ǰ���ڣ��������գ�

	while (gl_ThreadStatus.IsBackGroundthreadsWorking()) Sleep(100); // �ȴ����еĹ����߳̽���

	if (!gl_systemStatus.IsExitingCalculatingRS()) { // ���˳������˼�������
		pMarket->SetRSEndDate(pMarket->GetMarketDate());
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
		gl_systemStatus.SetExitingCalculatingRS(false);// ����Ǽ��������ֹ�ˣ���������ֹ��ʶ��
		gl_systemMessage.PushInformationMessage(_T("��ֹ�����¼����������ǿ�ȵĹ���"));
	}
	gl_ThreadStatus.SetCalculatingDayLineRS(false); // ���߳�˳���˳������ڷ�����״̬

	return 11;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����������ڵ��������ǿ�ȡ�
//
//
/////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadBuildDayLineRSOfDate(not_null<CChinaMarket*> pMarket, long lDate) {
	gl_BackGroundTaskThread.Wait();
	gl_ThreadStatus.IncreaseBackGroundWorkingthreads();     // ���ڹ������߳�����һ
	if (!gl_systemStatus.IsExitingSystem() && !gl_systemStatus.IsExitingCalculatingRS()) {
		pMarket->BuildDayLineRS(lDate);  // ����ʵ��ִ�к���
	}
	gl_ThreadStatus.DecreaseBackGroundWorkingthreads(); // ���ڹ������߳�����һ
	gl_BackGroundTaskThread.Signal();

	return 12;
}