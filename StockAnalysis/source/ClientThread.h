#pragma once

#include"stdafx.h"


// �������߳�����
UINT ClientThreadCalculatingRTDataProc(LPVOID pParam);
UINT ClientThreadRTDataProc(LPVOID pParam);
UINT ClientThreadReadDayLineProc(LPVOID pParam);
UINT ClientThreadUpdatingDataBaseProc(LPVOID pParam);
UINT ClientThreadCompileTodayStocks(LPVOID pParam);
UINT ClientThreadSaveDayLineProc(LPVOID pParam);
UINT ClientthreadLoadDayLineProc(LPVOID pParam);
UINT ClientThreadCalculatingRelativeStrong(LPVOID pParam);
UINT ClientThreadCalculateRelativeStrongProc(LPVOID pParam);
UINT ClientthreadLoadDayLineProc(LPVOID pParam);
