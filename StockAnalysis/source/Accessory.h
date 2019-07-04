#pragma once

#include"stdafx.h"
#include"globedef.h"

long ChangeDay(long lSourceDay, int nDay);

bool CalculateOneDayRelativeStrong(long lDay);
bool CalculateRelativeStrong(CTime ctStart);
bool UpdateStockCodeDataBase(void);




// 各工作线程声明
UINT ClientThreadCalculatingRTDataProc(LPVOID pParam);
UINT ClientThreadRTDataProc(LPVOID pParam);
UINT ClientThreadReadDayLineProc(LPVOID pParam);
UINT ClientThreadUpdatingDataBaseProc(LPVOID pParam);
UINT ClientThreadCompileTodayStocks(LPVOID pParam);
UINT ClientThreadSaveDayLineProc(LPVOID pParam);
UINT ClientthreadLoadDayLineProc(LPVOID pParam);

