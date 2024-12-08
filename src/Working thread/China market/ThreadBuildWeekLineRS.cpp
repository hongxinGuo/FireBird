///////////////////////////////////////////////////////////////////////////////////
//
// �����gl_lRelativeStrongEndDate��gl_lDay�����ǿ���̡߳�
//
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"

#undef max //Note ����concurrencpp.h֮ǰ����Ҫundefined max
#include"concurrencpp/concurrencpp.h"
using namespace concurrencpp;

#include"SystemMessage.h"

 
import FireBird.Accessory.TimeConvert;
import FireBird.System.ThreadStatus;
#include"ChinaMarket.h"

UINT ThreadBuildWeekLineRS(const CChinaMarketPtr& pMarket, long startCalculatingDate) {
	pMarket->SetCalculatingWeekLineRS(true);
	long lToday = GetPrevMonday(startCalculatingDate);

	const long year = lToday / 10000;
	const long month = lToday / 100 - year * 100;
	const long day = lToday - year * 10000 - month * 100;
	CTime ctCurrent(year, month, day, 12, 0, 0);
	const CTimeSpan sevenDays(7, 0, 0, 0);

	vector<result<void>> results;
	auto start = chrono::time_point_cast<chrono::seconds>(chrono::steady_clock::now());
	do {
		// ���ù����̣߳�ִ��ʵ�ʼ��㹤���� ���๤���̵߳����ȼ�Ϊ��ͣ��������Ա�ֻ֤����CPU�Ŀ���ʱ�䡣
		// ÿ�ε���ʱ�����µľֲ����������������̺߳�ִ�з��붯����detach��������Դ��ϵͳ�ڹ����߳�ִ�������л��ա�
		gl_BackgroundWorkingThread.acquire();
		auto result = gl_runtime.thread_executor()->submit([lToday] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			ASSERT(GetCurrentMonday(lToday) == lToday); // ȷ��������Ϊ����һ

			if (!gl_systemConfiguration.IsExitingSystem() && !gl_systemConfiguration.IsExitingCalculatingRS()) {
				gl_dataContainerChinaStock.BuildWeekLineRS(lToday);
			}
			gl_BackgroundWorkingThread.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
		results.emplace_back(std::move(result));
		ctCurrent += sevenDays;
		lToday = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
	} while (lToday <= pMarket->GetMarketDate()); // ��������ǰ���ڣ��������գ�

	for (auto& result2 : results) {
		result2.get(); // �ڴ˵ȴ����й����߳����
	}

	if (!gl_systemConfiguration.IsExitingCalculatingRS()) {
		// ���˳������˼�������
		// ��ʾ���ѵ�ʱ��
		auto end = chrono::time_point_cast<chrono::seconds>(chrono::steady_clock::now());
		const long tDiffer = (end - start).count();
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
