#pragma once

#include"stdafx.h"

// 各工作线程声明

// 读取新浪实时数据线程。
UINT ClientThreadReadingSinaRTDataProc(LPVOID pParam);    // 此工作线程返回值为1

// 读取网易日线历史数据线程
UINT ClientThreadReadDayLineProc(LPVOID pParam);      // 此工作线程返回值为2

// 计算实时数据线程
UINT ClientThreadCalculatingRTDataProc(LPVOID pParam);  // 此工作线程返回值为3
// 存储临时系统状态线程
UINT ClientThreadSaveTempRTDataProc(LPVOID pParam);     // 此工作线程返回值为4

// 计算和存储当日数据线程
UINT ClientThreadCompileTodayStocks(LPVOID pParam);     // 此工作线程返回值为5
// 存储日线历史数据线程
UINT ClientThreadSaveDayLineProc(LPVOID pParam);        // 此工作线程返回值为6
// 从数据库中读取日线历史数据线程
UINT ClientthreadLoadDayLineProc(LPVOID pParam);        // 此工作线程返回值为7

// 计算股票相对强度线程
UINT ClientThreadCalculateRelativeStrongProc(LPVOID pParam); // 此工作线程返回值为8

// 更新数据库线程（尚未实现）
UINT ClientThreadUpdatingDataBaseProc(LPVOID pParam);   // 此工作线程返回值为9
