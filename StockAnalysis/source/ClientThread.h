#pragma once

#include"stdafx.h"


// �������߳�����
UINT ClientThreadReadingRTDataProc(LPVOID pParam);    // �˹����̷߳���ֵΪ1
UINT ClientThreadReadDayLineProc(LPVOID pParam);      // �˹����̷߳���ֵΪ2

UINT ClientThreadCalculatingRTDataProc(LPVOID pParam);  // �˹����̷߳���ֵΪ3
UINT ClientThreadSaveTempRTDataProc(LPVOID pParam);     // �˹����̷߳���ֵΪ4

UINT ClientThreadCompileTodayStocks(LPVOID pParam);     // �˹����̷߳���ֵΪ5
UINT ClientThreadSaveDayLineProc(LPVOID pParam);        // �˹����̷߳���ֵΪ6
UINT ClientthreadLoadDayLineProc(LPVOID pParam);        // �˹����̷߳���ֵΪ7

UINT ClientThreadCalculateRelativeStrongProc(LPVOID pParam); // �˹����̷߳���ֵΪ8

UINT ClientThreadUpdatingDataBaseProc(LPVOID pParam);   // �˹����̷߳���ֵΪ9



