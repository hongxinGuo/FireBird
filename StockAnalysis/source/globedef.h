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

#include"SinaWebRTData.h"
#include"TengxunWebRTData.h"
#include"NeteaseWebDayLineData.h"
#include"NeteaseWebRTData.h"
#include"CrweberIndexWebData.h"

#include"CrweberIndex.h"

#include"SystemMessage.h"
#include"QueueRTData.h"
#include"QueueWebRTData.h"
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

extern CSinaWebRTData gl_SinaWebRTData; // ����ʵʱ���ݲɼ�
extern CTengxunWebRTData gl_TengxunWebRTData; // ��Ѷʵʱ���ݲɼ�
extern CNeteaseWebRTData gl_NeteaseWebRTData; // ����ʵʱ���ݲɼ�
extern CNeteaseWebDayLineData gl_NeteaseWebDayLineData; // ����������ʷ����
extern CNeteaseWebDayLineData gl_NeteaseWebDayLineDataSecond; // ����������ʷ����
extern CNeteaseWebDayLineData gl_NeteaseWebDayLineDataThird; // ����������ʷ����
extern CNeteaseWebDayLineData gl_NeteaseWebDayLineDataFourth; // ����������ʷ����
extern CNeteaseWebDayLineData gl_NeteaseWebDayLineDataFive; // ����������ʷ����
extern CNeteaseWebDayLineData gl_NeteaseWebDayLineDataSix; // ����������ʷ����
extern CCrweberIndexWebData gl_CrweberIndexWebData;   // crweber.com�ϵ�ÿ������ָ��

extern CCrweberIndex gl_CrweberIndex;
extern CCrweberIndex gl_CrweberIndexLast;

extern CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����
extern CSystemTime gl_systemTime; // ϵͳʱ����ܡ�

extern CPriorityQueueRTData gl_QueueSinaRTData; // ϵͳʵʱ���ݶ��С�
extern CQueueRTData gl_QueueSinaRTDataForSave; // ���ڴ洢������ʵʱ���ݶ���
extern CPriorityQueueRTData gl_QueueTengxunRTData; // ϵͳʵʱ���ݶ��С�
extern CPriorityQueueRTData gl_QueueNeteaseRTData; // ϵͳʵʱ���ݶ��С�

extern CQueueWebRTData gl_QueueSinaWebRTData; // �������������ݴ����
extern CQueueWebRTData gl_QueueTengxunWebRTData; // ��Ѷ���������ݴ����
extern CQueueWebRTData gl_QueueNeteaseWebRTData; // �������������ݴ����
extern CQueueWebRTData gl_QueueCrweberdotcomWebData; // crweber.com���������ݴ����

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
extern Semaphore gl_SaveOneStockDayLine;  // ���ź�����������������ʷ���ݿ�
extern Semaphore gl_SemaphoreCalculateDayLineRS;
extern Semaphore gl_ProcessSinaRTDataQueue;
extern Semaphore gl_ProcessTengxunRTDataQueue;
extern Semaphore gl_ProcessNeteaseRTDataQueue;

class CMarket;
// ���г���������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�
extern CMarket gl_ChinaStockMarket; // �г������л�Ծ�Ĺ�Ʊ��λ�����У���һʵ�����������������һ��ʵ����

#endif
