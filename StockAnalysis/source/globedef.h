#pragma once

#include"stdafx.h"

// �й���Ʊ�г���ʼ��
#define __CHINA_MARKET_BEGIN_DAY__ 19900101

enum {
  __STOCK_NOT_CHECKED__ = 128, // ��δ�����Ĺ�Ʊ����
  __STOCK_IPOED__ = 255, // ������Ʊ��ʶ
  __STOCK_NULL__ = 0, // ��Ч���루�˴���Ĺ�Ʊ�����ڣ�
  __STOCK_DELISTED__ = 1, // �����У���ͣ�ƣ��Ĺ�Ʊ��ʶ
};

enum {
  __SHANGHAI_MARKET__ = 1, // �Ϻ��г�
  __SHENZHEN_MARKET__ = 2, // �����г�
  __SHANGHAI_INDEX__ = 3, // �Ϻ�ָ��
  __SHENZHEN_INDEX__ = 4, // ����ָ��
  __SHANGHAI_MAIN__ = 5, // �Ϻ�����
  __SHENZHEN_MAIN__ = 6, // ��������
  __SHANGHAI_3BAN__ = 7, // �Ϻ�3��
  __SHENZHEN_3BAN__ = 8, //������С��
  __SHANGHAI_B_SHARE__ = 9, //�Ϻ�B��
  __SHENZHEN_B_SHARE__ = 10, // ����B��
  __SHANGHAI_KECHUANG__ = 11, // �Ϻ��ƴ���
  __SHENZHEN_CHUANGYE__ = 12, // ���ڴ�ҵ��
};

#include"VirtualMarket.h"
#include"PotenDailyBriefMarket.h"

#include"RTData.h"

#include"SystemMessage.h"
#include"QueueRTData.h"
#include"QueueWebData.h"
#include"PriorityQueueRTData.h"
#include"ThreadStatus.h"
#include"SystemTime.h"

using namespace std;
#include<deque>
#include<atomic>
#include<vector>

// �����ñ���
extern CRTDataPtr gl_TESTpRTData;

extern CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����
extern CSystemTime gl_systemTime; // ϵͳʱ����ܡ�

extern CThreadStatus gl_ThreadStatus; // ϵͳ�еĸ���״̬�������������߳���ʹ��

extern deque<CString> gl_dequeRTStockInquire; // ����ʵʱ��Ʊ��Ϣ�Ĺ�Ʊ����
extern deque<CString> gl_dequeDayLineStockInquire; // ����������ʷ���ݵĹ�Ʊ���С�

extern atomic_bool gl_ExitingSystem; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
extern bool gl_fExitingCalculatingRS; // ����֪ͨ�����߳��˳����ź�
extern bool gl_fResetSystem; // ϵͳ������ʶ

extern bool gl_fNormalMode; // ����ģʽ��ʶ��Ĭ��Ϊ�١�ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�
extern bool gl_fTestMode; // �Ƿ�������gl_fTestMode��ʶ�������ж��Ƿ���ʵ��ϵͳ��ʹ����MySQLTest��������

extern const int gl_cMaxSavingOneDayLineThreads;

extern vector<CVirtualMarket*> gl_vMarket; // ���г�ָ���������ֻ����ִ�и��г���SchedulingTask

extern CPotenDailyBriefingMarket gl_PotenDailyBriefingMarket;

class CChinaMarket;
// ���г���������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�
extern CChinaMarket gl_ChinaStockMarket; // �г������л�Ծ�Ĺ�Ʊ��λ�����У���һʵ�����������������һ��ʵ����
