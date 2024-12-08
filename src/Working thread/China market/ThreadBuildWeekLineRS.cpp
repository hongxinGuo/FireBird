///////////////////////////////////////////////////////////////////////////////////
//
// 计算从gl_lRelativeStrongEndDate至gl_lDay的相对强度线程。
//
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"

#undef max //Note 包含concurrencpp.h之前，需要undefined max
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
		// 调用工作线程，执行实际计算工作。 此类工作线程的优先级为最低，这样可以保证只利用CPU的空闲时间。
		// 每次调用时生成新的局部变量，启动工作线程后执行分离动作（detach），其资源由系统在工作线程执行完后进行回收。
		gl_BackgroundWorkingThread.acquire();
		auto result = gl_runtime.thread_executor()->submit([lToday] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			ASSERT(GetCurrentMonday(lToday) == lToday); // 确保此日期为星期一

			if (!gl_systemConfiguration.IsExitingSystem() && !gl_systemConfiguration.IsExitingCalculatingRS()) {
				gl_dataContainerChinaStock.BuildWeekLineRS(lToday);
			}
			gl_BackgroundWorkingThread.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
		results.emplace_back(std::move(result));
		ctCurrent += sevenDays;
		lToday = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
	} while (lToday <= pMarket->GetMarketDate()); // 计算至当前日期（包括今日）

	for (auto& result2 : results) {
		result2.get(); // 在此等待所有工作线程完成
	}

	if (!gl_systemConfiguration.IsExitingCalculatingRS()) {
		// 如果顺利完成了计算任务
		// 显示花费的时间
		auto end = chrono::time_point_cast<chrono::seconds>(chrono::steady_clock::now());
		const long tDiffer = (end - start).count();
		const long hour = tDiffer / 3600;
		const long min = tDiffer / 60 - hour * 60;
		const long second = tDiffer - hour * 3600 - min * 60;
		char buffer[100];
		sprintf_s(buffer, _T("计算股票周线相对强度用时%02d小时%02d分钟%02d秒"), hour, min, second);
		const CString str = buffer;
		gl_systemMessage.PushInformationMessage(str);
	}
	else {
		gl_systemConfiguration.SetExitingCalculatingRS(false); // 如果是计算过程中止了，则重置中止标识。
		gl_systemMessage.PushInformationMessage(_T("中止了重新计算日线相对强度的过程"));
	}
	pMarket->SetCalculatingWeekLineRS(false); // 本线程顺利退出，处于非运行状态

	return 30;
}
