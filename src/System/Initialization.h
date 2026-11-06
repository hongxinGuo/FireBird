#pragma once

void DeleteAllFinnhubInaccessibleUSExchange();

void SystemInitialization();
void SystemShutdown();

void InitializeMarkets();
void InitializeLogSystem();

void ResetMarkets();
void AssignDataSourceAndWebInquiryToMarket();
void SetMaxCurrencyLevel();


void TaskCheckWorldMarketReady();// world market task

bool IsMarketResetting(); // 
void ScheduleMarketTask();

void CloseAllBackgroundThread();
void CloseDataSourceBackgroundThread();
