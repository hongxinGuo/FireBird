#pragma once

#include"stdafx.h"


// �������߳�����
UINT ClientThreadReadingRTDataProc(LPVOID pParam);
UINT ClientThreadReadDayLineProc(LPVOID pParam);

UINT ClientThreadUpdatingDataBaseProc(LPVOID pParam);
UINT ClientThreadCompileTodayStocks(LPVOID pParam);
UINT ClientThreadSaveDayLineProc(LPVOID pParam);
UINT ClientthreadLoadDayLineProc(LPVOID pParam);

UINT ClientThreadCalculatingRTDataProc(LPVOID pParam);
UINT ClientThreadCalculateRelativeStrongProc(LPVOID pParam);

UINT ClientThreadSaveTempRTDataProc(LPVOID pParam);

UINT ClientThreadSavingStockCodeDataProc(LPVOID pParam);

