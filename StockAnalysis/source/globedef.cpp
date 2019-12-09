#include"globedef.h"

#include "Market.h"
#include "SystemTime.h"

// �����ñ���
CRTDataPtr gl_TESTpRTData = nullptr;

CSinaWebRTData gl_SinaWebRTData; // ����ʵʱ���ݲɼ�
CTengxunWebRTData gl_TengxunWebRTData; // ��Ѷʵʱ���ݲɼ�
CNeteaseWebRTData gl_NeteaseWebRTData; // ����ʵʱ���ݲɼ�
CNeteaseWebDayLineData gl_NeteaseWebDayLineData; // ����������ʷ����
CNeteaseWebDayLineData gl_NeteaseWebDayLineDataSecond; // ����������ʷ����
CNeteaseWebDayLineData gl_NeteaseWebDayLineDataThird; // ����������ʷ����
CNeteaseWebDayLineData gl_NeteaseWebDayLineDataFourth; // ����������ʷ����
CNeteaseWebDayLineData gl_NeteaseWebDayLineDataFive; // ����������ʷ����
CNeteaseWebDayLineData gl_NeteaseWebDayLineDataSix; // ����������ʷ����
CCrweberIndexWebData gl_CrweberIndexWebData; // crweber.com�ϵ�ÿ������ָ��

CCrweberIndex gl_CrweberIndex;
CCrweberIndex gl_CrweberIndexLast;

CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����
CSystemTime gl_systemTime; // ϵͳʱ����ܡ�

CPriorityQueueRTData gl_QueueSinaRTData; // ϵͳʵʱ���ݶ��С�
//CQueueRTData gl_QueueSinaRTDataForSave; // ���ڴ洢������ʵʱ���ݶ���
CPriorityQueueRTData gl_QueueTengxunRTData; // ϵͳʵʱ���ݶ��С�
CPriorityQueueRTData gl_QueueNeteaseRTData; // ϵͳʵʱ���ݶ��С�

CQueueWebRTData gl_QueueSinaWebRTData; // �������������ݴ����
CQueueWebRTData gl_QueueTengxunWebRTData; // ��Ѷ���������ݴ����
CQueueWebRTData gl_QueueNeteaseWebRTData; // �������������ݴ����

CThreadStatus gl_ThreadStatus; // ϵͳ�еĸ���״̬�������������߳���ʹ��

atomic<bool> gl_ExitingSystem = false; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
bool gl_fExitingCalculatingRS = false; // ����֪ͨ�����߳��˳����ź�
bool gl_fResetSystem = false; // ϵͳ������ʶ

bool gl_fNormalMode = false; // ����ģʽ��ʶ��Ĭ��Ϊ�档ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�
bool gl_fTestMode = true; // �Ƿ�������gl_fTestMode��ʶ�������ж��Ƿ���ʵ��ϵͳ��ʹ����MySQLTest��������

const int gl_cMaxSavingOneDayLineThreads = 4;
// �ź�����������Ϊȫ�ֱ�����Ϊ�˳�ʼ����
Semaphore gl_SaveOneStockDayLine(4);  // ���ź�����������������ʷ���ݿ�
Semaphore gl_ProcessSinaRTDataQueue(1);   // ����ʵʱ���ݴ���ͬʱֻ����һ���̴߳���
Semaphore gl_ProcessTengxunRTDataQueue(1);
Semaphore gl_ProcessNeteaseRTDataQueue(1);

Semaphore gl_SemaphoreCalculateDayLineRS(8);

CMarket gl_ChinaStockMarket;  // ���г�ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�