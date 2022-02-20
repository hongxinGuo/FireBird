#pragma once

#include"Semaphore.h"
using namespace MyLib;

// 信号量必须声明为全局变量（为了初始化）
extern Semaphore gl_UpdateWorldMarketDB;  // 此信号量用于生成日线历史数据库
extern Semaphore gl_SaveOneStockDayLine;  // 此信号量用于生成日线历史数据库
extern Semaphore gl_SemaphoreBackGroundTaskThreads; // 后台工作线程数。最大为8
extern Semaphore gl_MaintainCrweberDB; // Crweber数据库只允许同时一个线程操作之。
