/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 各调度函数。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

void TaskSchedulePer100ms(); // 主任务调度函数，由定时timer gl_timerMainSchedule发起，其他任务皆由此生成
void TaskSchedulePerSecond(); // 每秒执行一次的各辅助任务。
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
