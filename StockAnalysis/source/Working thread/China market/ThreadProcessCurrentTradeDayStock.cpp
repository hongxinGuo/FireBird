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
// ���߳���ϵͳ�����к���15:05�Զ����ѣ�ÿ��ִֻ��һ��
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