#pragma once

#include"stdafx.h"

// �������߳�����

// ��ȡ����ʵʱ�����̡߳�
UINT ClientThreadReadingSinaRTDataProc(LPVOID pParam);    // �˹����̷߳���ֵΪ1

// ��ȡ����������ʷ�����߳�
UINT ClientThreadReadingNeteaseDayLineProc(LPVOID pParam);      // �˹����̷߳���ֵΪ2

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

// �����Ʊ���ǿ���̡߳����̵߳����߳�ClientThreadCalculateRealtiveStrongAtThisDayProcִ�о��������������8�������̡߳�
UINT ClientThreadCalculateRelativeStrongProc(LPVOID pParam); // �˹����̷߳���ֵΪ8

// �������ݿ��̣߳���δʵ�֣�
UINT ClientThreadUpdatingDataBaseProc(LPVOID pParam);   // �˹����̷߳���ֵΪ9

// ��ȡ��Ѷʵʱ�����̡߳�
UINT ClientThreadReadingTengxunRTDataProc(LPVOID pParam);    // �˹����̷߳���ֵΪ10

// ����������ڹ�Ʊ�������ǿ���̡߳����߳���ClientThreadCalculateRelativeStrongProcx�̵߳��ã����ڲ������㲻ͬ���ڵ��������ǿ�ȣ������Ч��
UINT ClientThreadCalculateRealtiveStrongAtThisDayProc(LPVOID pParam); // �˹����̷߳���ֵΪ11
