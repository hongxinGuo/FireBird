///////////////////////////////////////////////////////////////////////////////////
//
// 计算从gl_lRelativeStrongEndDate至gl_lDate的相对强度线程。
//
// 此线程目前只允许最多八个线程同时执行。
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

	gl_systemMessage.PushInformationMessage(_T("开始计算股票相对强度"));
	vector<result<void>> vResults;

	auto start = chrono::time_point_cast<chrono::seconds>(chrono::steady_clock::now());
	do {
		if (pMarket->IsWorkingDay(ctCurrent)) {
			// 星期六和星期日无交易，略过
			// 调用工作线程，执行实际计算工作。 此类工作线程的优先级为最低，这样可以保证只利用CPU的空闲时间。
			// 需要限制工作线程的并行数量
			gl_BackgroundWorkingThread.acquire();
			auto result = gl_runtime.thread_executor()->submit([lThatDate] {
				gl_ThreadStatus.IncreaseBackGroundWorkingThread();
				if (!gl_systemConfiguration.IsExitingSystem() && !gl_systemConfiguration.IsExitingCalculatingRS()) {
					gl_dataContainerChinaStock.BuildDayLineRS(lThatDate); // 调用实际执行函数
				}
				gl_BackgroundWorkingThread.release();
				gl_ThreadStatus.DecreaseBackGroundWorkingThread();
			});
			vResults.emplace_back(std::move(result));
		}
		ctCurrent += oneDay;
		lThatDate = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
	} while (lThatDate <= pMarket->GetMarketDate()); // 计算至当前日期（包括今日）
	for (auto& result2 : vResults) {
		result2.get(); // 在这里等待所有的线程执行完毕
	}

	if (!gl_systemConfiguration.IsExitingCalculatingRS()) {
		// 如果顺利完成了计算任务
		pMarket->SetRSEndDate(pMarket->GetMarketDate());
		pMarket->SetUpdateOptionDB(true); // 更新选项数据库
		// 显示花费的时间
		auto end = chrono::time_point_cast<chrono::seconds>(chrono::steady_clock::now());
		const long tDiffer = (end - start).count();
		const long hour = tDiffer / 3600;
		const long min = tDiffer / 60 - hour * 60;
		const long second = tDiffer - hour * 3600 - min * 60;
		char buffer[100];
		sprintf_s(buffer, _T("计算股票相对强度用时%02d小时%02d分钟%02d秒"), hour, min, second);
		const CString str = buffer;
		gl_systemMessage.PushInformationMessage(str);
	}
	else {
		gl_systemConfiguration.SetExitingCalculatingRS(false); // 如果是计算过程中止了，则重置中止标识。
		gl_systemMessage.PushInformationMessage(_T("中止了重新计算日线相对强度的过程"));
	}
	pMarket->SetCalculatingDayLineRS(false); // 本线程顺利退出，处于非运行状态

	return 11;
}
