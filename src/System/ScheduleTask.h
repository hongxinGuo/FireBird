/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

void TaskSchedulePer100ms(); // 主任务调度函数，由定时timer gl_timerMainSchedule发起，其他任务借由此生成
void TaskSchedulePerSecond(); // 每秒执行一次的各辅助任务。
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
