#pragma once

#include"stdafx.h"

// �������߳�����

// ��ȡ����ʵʱ�����̡߳�
UINT ThreadReadingSinaRTDataProc(LPVOID pParam);    // �˹����̷߳���ֵΪ1

// ��ȡ����������ʷ�����߳�
UINT ThreadReadingNeteaseDayLineProc(LPVOID pParam);      // �˹����̷߳���ֵΪ2

// ����ʵʱ�����߳�
UINT ThreadCalculatingRTDataProc(LPVOID pParam);  // �˹����̷߳���ֵΪ3

// �洢��ʱϵͳ״̬�߳�
UINT ThreadSaveTempRTDataProc(LPVOID pParam);     // �˹����̷߳���ֵΪ4

// ����ʹ洢���������߳�
UINT ThreadCompileTodayStocks(LPVOID pParam);     // �˹����̷߳���ֵΪ5

// �洢������ʷ�����߳�
UINT ThreadSaveDayLineProc(LPVOID pParam);        // �˹����̷߳���ֵΪ6

// �����ݿ��ж�ȡ������ʷ�����߳�
UINT ThreadLoadDayLineProc(LPVOID pParam);        // �˹����̷߳���ֵΪ7

// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRealtiveStrongAtThisDayProcִ�о��������������8�������̡߳�
UINT ThreadCalculateRelativeStrongProc(LPVOID pParam); // �˹����̷߳���ֵΪ8

// �������ݿ��̣߳���δʵ�֣�
UINT ThreadUpdatingDataBaseProc(LPVOID pParam);   // �˹����̷߳���ֵΪ9

// ��ȡ��Ѷʵʱ�����̡߳�
UINT ThreadReadingTengxunRTDataProc(LPVOID pParam);    // �˹����̷߳���ֵΪ10

// ����������ڹ�Ʊ�������ǿ���̡߳����߳���ThreadCalculateRelativeStrongProcx�̵߳��ã����ڲ������㲻ͬ���ڵ��������ǿ�ȣ������Ч��
UINT ThreadCalculateRealtiveStrongAtThisDayProc(LPVOID pParam); // �˹����̷߳���ֵΪ11
