/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

void TaskSchedulePer100ms(); // ��������Ⱥ������ɶ�ʱtimer gl_timerMainSchedule��������������ɴ�����
void TaskSchedulePerSecond(); // ÿ��ִ��һ�εĸ���������
void TaskExitSystem();

// world market task
void TaskCheckWorldMarketReady();

void InitializeMarkets();
void AssignDataSourceAndWebInquiryToMarket();

void ResetMarkets();
bool IsMarketResetting(); // 
void ScheduleMarketTask();

void SetMaxCurrencyLevel();

void SystemInitialization();
