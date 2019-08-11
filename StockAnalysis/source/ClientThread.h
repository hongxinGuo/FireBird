#pragma once

#include"stdafx.h"


// 各工作线程声明
UINT ClientThreadReadingRTDataProc(LPVOID pParam);    // 此工作线程返回值为1
UINT ClientThreadReadDayLineProc(LPVOID pParam);      // 此工作线程返回值为2

UINT ClientThreadCalculatingRTDataProc(LPVOID pParam);  // 此工作线程返回值为3
UINT ClientThreadSaveTempRTDataProc(LPVOID pParam);     // 此工作线程返回值为4

UINT ClientThreadCompileTodayStocks(LPVOID pParam);     // 此工作线程返回值为5
UINT ClientThreadSaveDayLineProc(LPVOID pParam);        // 此工作线程返回值为6
UINT ClientthreadLoadDayLineProc(LPVOID pParam);        // 此工作线程返回值为7

UINT ClientThreadCalculateRelativeStrongProc(LPVOID pParam); // 此工作线程返回值为8

UINT ClientThreadUpdatingDataBaseProc(LPVOID pParam);   // 此工作线程返回值为9



