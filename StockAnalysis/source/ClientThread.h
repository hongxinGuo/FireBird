#pragma once

#include"stdafx.h"

// �������߳�����

// ��ȡ����ʵʱ�����̡߳�
UINT ClientThreadReadingSinaRTDataProc(LPVOID pParam);    // �˹����̷߳���ֵΪ1

// ��ȡ����������ʷ�����߳�
UINT ClientThreadReadDayLineProc(LPVOID pParam);      // �˹����̷߳���ֵΪ2

// ����ʵʱ�����߳�
UINT ClientThreadCalculatingRTDataProc(LPVOID pParam);  // �˹����̷߳���ֵΪ3
// �洢��ʱϵͳ״̬�߳�
UINT ClientThreadSaveTempRTDataProc(LPVOID pParam);     // �˹����̷߳���ֵΪ4

// ����ʹ洢���������߳�
UINT ClientThreadCompileTodayStocks(LPVOID pParam);     // �˹����̷߳���ֵΪ5
// �洢������ʷ�����߳�
UINT ClientThreadSaveDayLineProc(LPVOID pParam);        // �˹����̷߳���ֵΪ6
// �����ݿ��ж�ȡ������ʷ�����߳�
UINT ClientthreadLoadDayLineProc(LPVOID pParam);        // �˹����̷߳���ֵΪ7

// �����Ʊ���ǿ���߳�
UINT ClientThreadCalculateRelativeStrongProc(LPVOID pParam); // �˹����̷߳���ֵΪ8

// �������ݿ��̣߳���δʵ�֣�
UINT ClientThreadUpdatingDataBaseProc(LPVOID pParam);   // �˹����̷߳���ֵΪ9
