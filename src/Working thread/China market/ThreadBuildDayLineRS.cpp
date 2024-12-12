///////////////////////////////////////////////////////////////////////////////////
//
// �����gl_lRelativeStrongEndDate��gl_lDate�����ǿ���̡߳�
//
// ���߳�Ŀǰֻ�������˸��߳�ͬʱִ�С�
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"
#include"ConCurrenCppHeader.h"

import FireBird.Market.China;

import FireBird.System.Message;
import FireBird.System.ThreadStatus;

UINT ThreadBuildDayLineRS(const CChinaMarketPtr& pMarket, long startCalculatingDate) {
	pMarket->SetCalculatingDayLineRS(true);
	long lThatDate = startCalculatingDate;

	const long year = lThatDate / 10000;
	const long month = lThatDate / 100 - year * 100;
	const long day = lThatDate - year * 10000 - month * 100;
	CTime ctCurrent(year, month, day, 12, 0, 0);
	const CTimeSpan oneDay(1, 0, 0, 0);

	gl_systemMessage.PushInformationMessage(_T("��ʼ�����Ʊ���ǿ��"));
	vector<result<void>> vResults;

	auto start = chrono::time_point_cast<chrono::seconds>(chrono::steady_clock::now());
	do {
		if (pMarket->IsWorkingDay(ctCurrent)) {
			// ���������������޽��ף��Թ�
			// ���ù����̣߳�ִ��ʵ�ʼ��㹤���� ���๤���̵߳����ȼ�Ϊ��ͣ��������Ա�ֻ֤����CPU�Ŀ���ʱ�䡣
			// ��Ҫ���ƹ����̵߳Ĳ�������
			gl_BackgroundWorkingThread.acquire();
			auto result = gl_runtime.thread_executor()->submit([lThatDate] {
				gl_ThreadStatus.IncreaseBackGroundWorkingThread();
				if (!gl_systemConfiguration.IsExitingSystem() && !gl_systemConfiguration.IsExitingCalculatingRS()) {
					gl_dataContainerChinaStock.BuildDayLineRS(lThatDate); // ����ʵ��ִ�к���
				}
				gl_BackgroundWorkingThread.release();
				gl_ThreadStatus.DecreaseBackGroundWorkingThread();
			});
			vResults.emplace_back(std::move(result));
		}
		ctCurrent += oneDay;
		lThatDate = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
	} while (lThatDate <= pMarket->GetMarketDate()); // ��������ǰ���ڣ��������գ�
	for (auto& result2 : vResults) {
		result2.get(); // ������ȴ����е��߳�ִ�����
	}

	if (!gl_systemConfiguration.IsExitingCalculatingRS()) {
		// ���˳������˼�������
		pMarket->SetRSEndDate(pMarket->GetMarketDate());
		pMarket->SetUpdateOptionDB(true); // ����ѡ�����ݿ�
		// ��ʾ���ѵ�ʱ��
		auto end = chrono::time_point_cast<chrono::seconds>(chrono::steady_clock::now());
		const long tDiffer = (end - start).count();
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
