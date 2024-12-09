/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����Ⱥ�����
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
module;
export module FireBird.System.ScheduleTask;

export {
	void TaskSchedulePer100ms(); // ��������Ⱥ������ɶ�ʱtimer gl_timerMainSchedule��������������ɴ�����
	void TaskSchedulePerSecond(); // ÿ��ִ��һ�εĸ���������
	void TaskExitSystem();

	void TaskCheckWorldMarketReady();// world market task

	void InitializeMarkets();
	void AssignDataSourceAndWebInquiryToMarket();

	void ResetMarkets();
	bool IsMarketResetting(); // 
	void ScheduleMarketTask();

	void SetMaxCurrencyLevel();

	void SystemInitialization();
}
