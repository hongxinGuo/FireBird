///////////////////////////////////////////////////////////////////////////////////
//
// 计算从gl_lrelativeStrongEndDate至gl_lDay的相对强度线程。
//
// 此线程调用ThreadBuildDayLineRSOfDate线程，目前最多允许同时生成8个线程。
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

	gl_systemMessage.PushInformationMessage(_T("开始计算股票相对强度"));
	time_t tStart = 0, tEnd = 0;
	time(&tStart);
	do {
		if (pMarket->IsWorkingDay(ctCurrent)) { // 星期六和星期日无交易，略过
			// 调用工作线程，执行实际计算工作。 此类工作线程的优先级为最低，这样可以保证只利用CPU的空闲时间。
			// 每次调用时生成新的局部变量，启动工作线程后执行分离动作（detach），其资源由系统在工作线程执行完后进行回收。
			pMarket->CreatingThreadBuildDayLineRSOfDate(lToday);
		}
		ctCurrent += oneDay;
		lToday = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
	} while (lToday <= pMarket->GetMarketDate()); // 计算至当前日期（包括今日）

	while (gl_ThreadStatus.IsBackGroundthreadsWorking()) Sleep(100); // 等待所有的工作线程结束

	if (!gl_systemStatus.IsExitingCalculatingRS()) { // 如果顺利完成了计算任务
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
		CString str;
		str = buffer;
		gl_systemMessage.PushInformationMessage(str);
	}
	else {
		gl_systemStatus.SetExitingCalculatingRS(false);// 如果是计算过程中止了，则重置中止标识。
		gl_systemMessage.PushInformationMessage(_T("中止了重新计算日线相对强度的过程"));
	}
	gl_ThreadStatus.SetCalculatingDayLineRS(false); // 本线程顺利退出，处于非运行状态

	return 11;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 计算给定日期的日线相对强度。
//
//
/////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadBuildDayLineRSOfDate(not_null<CChinaMarket*> pMarket, long lDate) {
	gl_BackGroundTaskThread.Wait();
	gl_ThreadStatus.IncreaseBackGroundWorkingthreads();     // 正在工作的线程数加一
	if (!gl_systemStatus.IsExitingSystem() && !gl_systemStatus.IsExitingCalculatingRS()) {
		pMarket->BuildDayLineRS(lDate);  // 调用实际执行函数
	}
	gl_ThreadStatus.DecreaseBackGroundWorkingthreads(); // 正在工作的线程数减一
	gl_BackGroundTaskThread.Signal();

	return 12;
}