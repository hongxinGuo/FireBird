#ifndef __GLOBE_DEFINE__
#define __GLOBE_DEFINE__

#include"stdafx.h"

enum {
  __STOCK_NOT_CHECKED__ = 128, // ��δ�����Ĺ�Ʊ����
  __STOCK_IPOED__ = 255, // ������Ʊ��ʶ
  __STOCK_NULL__ = 0, // ��Ч���루�˴���Ĺ�Ʊ�����ڣ�
  __STOCK_DELISTED__ = 1, // �����еĹ�Ʊ��ʶ
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

#include"RTData.h"

#include"SetDayLine.h"

#include"SinaRTWebData.h"
#include"TengxunRTWebData.h"
#include"NeteaseDayLineWebData.h"
#include"NeteaseRTWebData.h"
#include"CrweberIndexWebData.h"

#include"CrweberIndex.h"

#include"SystemMessage.h"
#include"QueueRTData.h"
#include"QueueRTWebData.h"
#include"PriorityQueueRTData.h"
#include"ThreadStatus.h"
#include"SystemTime.h"

#include"Semaphore.h"

using namespace std;
#include<deque>
#include<vector>
#include<map>
#include<deque>
#include<memory>
#include<atomic>

using namespace MyAccessory;

// �����ñ���
extern CRTDataPtr gl_TESTpRTData;

extern CSinaRTWebData gl_SinaRTWebData; // ����ʵʱ���ݲɼ�
extern CTengxunRTWebData gl_TengxunRTWebData; // ��Ѷʵʱ���ݲɼ�
extern CNeteaseRTWebData gl_NeteaseRTWebData; // ����ʵʱ���ݲɼ�
extern CNeteaseDayLineWebData gl_NeteaseDayLineWebData; // ����������ʷ����
extern CNeteaseDayLineWebData gl_NeteaseDayLineWebDataSecond; // ����������ʷ����
extern CNeteaseDayLineWebData gl_NeteaseDayLineWebDataThird; // ����������ʷ����
extern CNeteaseDayLineWebData gl_NeteaseDayLineWebDataFourth; // ����������ʷ����
extern CNeteaseDayLineWebData gl_NeteaseDayLineWebDataFive; // ����������ʷ����
extern CNeteaseDayLineWebData gl_NeteaseDayLineWebDataSix; // ����������ʷ����
extern CCrweberIndexWebData gl_CrweberIndexWebData;   // crweber.com�ϵ�ÿ������ָ��

extern CCrweberIndex gl_CrweberIndex;
extern CCrweberIndex gl_CrweberIndexLast;

extern CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����
extern CSystemTime gl_systemTime; // ϵͳʱ����ܡ�
extern CPriorityQueueRTData gl_QueueSinaRTData; // ϵͳʵʱ���ݶ��С�
extern CQueueRTData gl_QueueSinaRTDataForSave; // ���ڴ洢������ʵʱ���ݶ���
extern CPriorityQueueRTData gl_QueueTengxunRTData; // ϵͳʵʱ���ݶ��С�

extern CQueueRTWebData gl_QueueSinaRTWebData; // �������������ݴ����
extern CQueueRTWebData gl_QueueTengxunRTWebData; // ��Ѷ���������ݴ����

extern CThreadStatus gl_ThreadStatus; // ϵͳ�еĸ���״̬�������������߳���ʹ��

extern deque<CString> gl_dequeRTStockInquire; // ����ʵʱ��Ʊ��Ϣ�Ĺ�Ʊ����
extern deque<CString> gl_dequeDayLineStockInquire; // ����������ʷ���ݵĹ�Ʊ���С�

extern atomic<bool> gl_ExitingSystem; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
extern bool gl_fExitingCalculatingRS; // ����֪ͨ�����߳��˳����ź�
extern bool gl_fResetSystem; // ϵͳ������ʶ

extern bool gl_fNormalMode; // ����ģʽ��ʶ��Ĭ��Ϊ�١�ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�
extern bool gl_fTestMode; // �Ƿ�������gl_fTestMode��ʶ�������ж��Ƿ���ʵ��ϵͳ��ʹ����MySQLTest��������

extern const int gl_cMaxSavingOneDayLineThreads;
// �ź�����������Ϊȫ�ֱ�����Ϊ�˳�ʼ����
extern Semaphore gl_SaveOneStockDayLine;
extern Semaphore gl_SemaphoreCalculateDayLineRS;
extern Semaphore gl_ProcessSinaRTDataQueue;
extern Semaphore gl_ProcessTengxunRTDataQueue;
extern Semaphore gl_ProcessNeteaseRTDataQueue;

class CMarket;
// ���г���������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�
extern CMarket gl_ChinaStockMarket; // �г������л�Ծ�Ĺ�Ʊ��λ�����У���һʵ�����������������һ��ʵ����

#endif
