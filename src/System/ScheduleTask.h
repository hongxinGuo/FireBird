/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����Ⱥ�����
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

void TaskSchedulePer100ms(); // ��������Ⱥ������ɶ�ʱtimer gl_timerMainSchedule��������������ɴ�����
void TaskSchedulePerSecond(); // ÿ��ִ��һ�εĸ���������
void TaskExitSystem();

void TaskCheckWorldMarketReady();// world market task

void InitializeLogSystem();
void InitializeMarkets();
void AssignDataSourceAndWebInquiryToMarket();

void ResetMarkets();
bool IsMarketResetting(); // 
void ScheduleMarketTask();

void SetMaxCurrencyLevel();

void SystemInitialization();
