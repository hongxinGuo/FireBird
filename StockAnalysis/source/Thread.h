#pragma once

#include"stdafx.h"

// 各工作线程声明

// 读取新浪实时数据线程。
UINT ThreadReadingSinaRTDataProc(LPVOID pParam);    // 此工作线程返回值为1

// 读取网易日线历史数据线程
UINT ThreadReadingNeteaseDayLineProc(LPVOID pParam);      // 此工作线程返回值为2

// 计算实时数据线程
UINT ThreadCalculatingRTDataProc(LPVOID pParam);  // 此工作线程返回值为3

// 存储临时系统状态线程
UINT ThreadSaveTempRTDataProc(LPVOID pParam);     // 此工作线程返回值为4

// 计算和存储当日数据线程
UINT ThreadCompileTodayStocks(LPVOID pParam);     // 此工作线程返回值为5

// 存储日线历史数据线程
UINT ThreadSaveDayLineProc(LPVOID pParam);        // 此工作线程返回值为6

// 从数据库中读取日线历史数据线程
UINT ThreadLoadDayLineProc(LPVOID pParam);        // 此工作线程返回值为7

// 计算股票相对强度线程。此线程调用线程ThreadCalculateRealtiveStrongAtThisDayProc执行具体任务，最多生成8个工作线程。
UINT ThreadCalculateRelativeStrongProc(LPVOID pParam); // 此工作线程返回值为8

// 更新数据库线程（尚未实现）
UINT ThreadUpdatingDataBaseProc(LPVOID pParam);   // 此工作线程返回值为9

// 读取腾讯实时数据线程。
UINT ThreadReadingTengxunRTDataProc(LPVOID pParam);    // 此工作线程返回值为10

// 计算给定日期股票日线相对强度线程。此线程由ThreadCalculateRelativeStrongProcx线程调用，用于并发计算不同日期的日线相对强度，以提高效率
UINT ThreadCalculateRealtiveStrongAtThisDayProc(LPVOID pParam); // 此工作线程返回值为11
