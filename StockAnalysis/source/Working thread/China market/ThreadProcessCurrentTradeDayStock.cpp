#include"pch.h"

#include"ThreadStatus.h"
#include"TimeConvert.h"

#include"DayLine.h"
#include"ChinaMarket.h"
#include"Thread.h"

using namespace std;
#include<thread>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 此线程由系统在收市后于15:05自动唤醒，每日只执行一次
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadProcessTodayStock(not_null<CChinaMarket*> pMarket) {
	gl_ThreadStatus.IncreaseSavingThread();

	pMarket->ProcessTodayStock();

	gl_ThreadStatus.DecreaseSavingThread();

	return 14;
}