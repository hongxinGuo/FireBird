///////////////////////////////////////////////////////////////////////////////////
//
// 计算从gl_lRelativeStrongEndDate至gl_lDate的相对强度线程。
//
// 此线程调用ThreadBuildDayLineRSOfDate线程，目前只允许最多八个线程同时执行。
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

	gl_systemMessage.PushInformationMessage(_T("开始计算股票相对强度"));
	time_t tStart = 0, tEnd = 0;
	time(&tStart);
	do {
		if (pMarket->IsWorkingDay(ctCurrent)) {
			// 星期六和星期日无交易，略过
			// 调用工作线程，执行实际计算工作。 此类工作线程的优先级为最低，这样可以保证只利用CPU的空闲时间。
			// 每次调用时生成新的局部变量，启动工作线程后执行分离动作（detach），其资源由系统在工作线程执行完后进行回收。
			pMarket->CreateThreadBuildDayLineRSOfDate(lThatDate);
		}
		ctCurrent += oneDay;
		lThatDate = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
	} while (lThatDate <= pMarket->GetMarketDate()); // 计算至当前日期（包括今日）

	while (gl_ThreadStatus.IsBackGroundThreadsWorking()) Sleep(100); // 等待所有的工作线程结束

	if (!gl_systemConfiguration.IsExitingCalculatingRS()) {
		// 如果顺利完成了计算任务
		pMarket->SetRSEndDate(pMarket->GetMarketDate());
		pMarket->SetUpdateOptionDB(true); // 更新选项数据库
		// 显示花费的时间
		time(&tEnd);
		const long tDiffer = tEnd - tStart;
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

//////////////////////////////////////////////////////////////////////////////////////////
//
// 计算给定日期的日线相对强度。
// 使用互斥变量控制并行线程数。目前允许最多八个线程执行
//
/////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadBuildDayLineRSOfDate(const not_null<CChinaMarketPtr>& pMarket, long lDate) {
	gl_ThreadStatus.IncreaseBackGroundWorkingThread();
	gl_BackgroundWorkingThread.acquire();
	if (!gl_systemConfiguration.IsExitingSystem() && !gl_systemConfiguration.IsExitingCalculatingRS()) {
		gl_dataContainerChinaStock.BuildDayLineRS(lDate); // 调用实际执行函数
	}
	gl_BackgroundWorkingThread.release();
	gl_ThreadStatus.DecreaseBackGroundWorkingThread();

	return 12;
}
