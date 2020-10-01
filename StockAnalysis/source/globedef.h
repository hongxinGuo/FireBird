#pragma once

// �й���Ʊ�г���ʼ��
#define __CHINA_MARKET_BEGIN_DATE__ 19900101

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

#include"stdafx.h"

#include"VirtualMarket.h"
#include"PotenDailyBriefingMarket.h"
#include"CrweberIndexMarket.h"
#include"ChinaMarket.h"

#include"WebRTData.h"

#include"SystemMessage.h"
#include"WebRTDataContainer.h"
#include"QueueWebData.h"
#include"PriorityQueueWebRTData.h"
#include"ThreadStatus.h"

using namespace std;
#include<deque>
#include<atomic>
#include<vector>

extern CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����

extern CThreadStatus gl_ThreadStatus; // ϵͳ�еĸ���״̬�������������߳���ʹ��

extern deque<CString> gl_dequeRTStockInquire; // ����ʵʱ��Ʊ��Ϣ�Ĺ�Ʊ���У���δʹ�ã�
extern deque<CString> gl_dequeDLStockInquire; // ����������ʷ���ݵĹ�Ʊ���У���δʹ�ã�

extern atomic_bool gl_fExitingSystem; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
extern bool gl_fExitingCalculatingRS; // ����֪ͨ�����߳��˳����ź�

extern bool gl_fNormalMode; // ����ģʽ��ʶ��Ĭ��Ϊ�١�ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�
extern bool gl_fTestMode; // �Ƿ�������gl_fTestMode��ʶ��Ĭ��Ϊ�档ϵͳ������ʱ��Ҫ���ô˱�ʶΪ�١��������ж��Ƿ���ʵ��ϵͳ��ʹ����MySQLTest��������

extern vector<CVirtualMarketPtr> gl_vMarketPtr; // ���г�ָ���������ֻ����ִ�и��г���SchedulingTask

// ���г�Ψһ��ʵ��
extern CPotenDailyBriefingMarketPtr gl_pPotenDailyBriefingMarket;
extern CCrweberIndexMarketPtr gl_pCrweberIndexMarket;
// ���г���������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�
extern CChinaMarketPtr gl_pChinaStockMarket; // �г������л�Ծ�Ĺ�Ʊ��λ�����У���һʵ�����������������һ��ʵ����
