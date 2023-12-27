///////////////////////////////////////////////////////////////////////////////////
//
// �����gl_lRelativeStrongEndDate��gl_lDate�����ǿ���̡߳�
//
// ���̵߳���ThreadBuildDayLineRSOfDate�̣߳�Ŀǰֻ�������˸��߳�ͬʱִ�С�
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ThreadStatus.h"
#include"Thread.h"

#include"ChinaMarket.h"
#include"WorldMarket.h"

UINT ThreadBuildDayLineRS(const not_null<CChinaMarketPtr>& pMarket, long startCalculatingDate) {
	pMarket->SetCalculatingDayLineRS(true);
	long lThatDate = startCalculatingDate;

	const long year = lThatDate / 10000;
	const long month = lThatDate / 100 - year * 100;
	const long day = lThatDate - year * 10000 - month * 100;
	CTime ctCurrent(year, month, day, 12, 0, 0);
	const CTimeSpan oneDay(1, 0, 0, 0);

	gl_systemMessage.PushInformationMessage(_T("��ʼ�����Ʊ���ǿ��"));
	time_t tStart = 0, tEnd = 0;
	time(&tStart);
	do {
		if (pMarket->IsWorkingDay(ctCurrent)) {
			// ���������������޽��ף��Թ�
			// ���ù����̣߳�ִ��ʵ�ʼ��㹤���� ���๤���̵߳����ȼ�Ϊ��ͣ��������Ա�ֻ֤����CPU�Ŀ���ʱ�䡣
			// ÿ�ε���ʱ�����µľֲ����������������̺߳�ִ�з��붯����detach��������Դ��ϵͳ�ڹ����߳�ִ�������л��ա�
			pMarket->CreateThreadBuildDayLineRSOfDate(lThatDate);
		}
		ctCurrent += oneDay;
		lThatDate = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
	} while (lThatDate <= pMarket->GetMarketDate()); // ��������ǰ���ڣ��������գ�

	while (gl_ThreadStatus.IsBackGroundThreadsWorking()) Sleep(100); // �ȴ����еĹ����߳̽���

	if (!gl_systemConfiguration.IsExitingCalculatingRS()) {
		// ���˳������˼�������
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
		const CString str = buffer;
		gl_systemMessage.PushInformationMessage(str);
	}
	else {
		gl_systemConfiguration.SetExitingCalculatingRS(false); // ����Ǽ��������ֹ�ˣ���������ֹ��ʶ��
		gl_systemMessage.PushInformationMessage(_T("��ֹ�����¼����������ǿ�ȵĹ���"));
	}
	pMarket->SetCalculatingDayLineRS(false); // ���߳�˳���˳������ڷ�����״̬

	return 11;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����������ڵ��������ǿ�ȡ�
// ʹ�û���������Ʋ����߳�����Ŀǰ�������˸��߳�ִ��
//
/////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadBuildDayLineRSOfDate(const not_null<CChinaMarketPtr>& pMarket, long lDate) {
	gl_ThreadStatus.IncreaseBackGroundWorkingThread();
	gl_BackgroundWorkingThread.acquire();
	if (!gl_systemConfiguration.IsExitingSystem() && !gl_systemConfiguration.IsExitingCalculatingRS()) {
		gl_dataContainerChinaStock.BuildDayLineRS(lDate); // ����ʵ��ִ�к���
	}
	gl_BackgroundWorkingThread.release();
	gl_ThreadStatus.DecreaseBackGroundWorkingThread();

	return 12;
}
