#pragma once

#include"Semaphore.h"
using namespace MyLib;

// �ź�����������Ϊȫ�ֱ�����Ϊ�˳�ʼ����
extern Semaphore gl_UpdateWorldMarketDB;  // ���ź�����������������ʷ���ݿ�
extern Semaphore gl_SaveOneStockDayLine;  // ���ź�����������������ʷ���ݿ�
extern Semaphore gl_SemaphoreBackGroundTaskThreads; // ��̨�����߳��������Ϊ8
extern Semaphore gl_MaintainCrweberDB; // Crweber���ݿ�ֻ����ͬʱһ���̲߳���֮��
