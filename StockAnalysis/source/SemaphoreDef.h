#pragma once

#include"Semaphore.h"
using namespace MyLib;

// �ź�����������Ϊȫ�ֱ�����Ϊ�˳�ʼ����
extern Semaphore gl_UpdateWorldMarketDB;  // ���ź�����������������ʷ���ݿ�
extern Semaphore gl_SaveThreadPermitted;  // ���ź�����������������ʷ���ݿ�
extern Semaphore gl_SemaphoreBackGroundTaskThreads; // ��̨�����߳��������Ϊ8
