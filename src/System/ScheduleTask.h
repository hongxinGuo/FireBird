/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
void InitializeMarkets();
void AssignDataSourceAndWebInquiryToMarket();

void ResetMarkets();
bool IsMarketResetting(); // 
void ScheduleMarketTask();

void ScheduleTask(); // ��������Ⱥ������ɶ�ʱtimer gl_timerMainSchedule��������������ɴ�����
void SchedulePerSecondTask(); // ÿ��ִ��һ�εĸ���������
void InitializeSystem();
