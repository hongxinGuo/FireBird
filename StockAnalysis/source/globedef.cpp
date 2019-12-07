#include"globedef.h"

#include "Market.h"
#include "SystemTime.h"

// �����ñ���
CRTDataPtr gl_TESTpRTData = nullptr;

CSinaRTWebData gl_SinaRTWebData; // ����ʵʱ���ݲɼ�
CTengxunRTWebData gl_TengxunRTWebData; // ��Ѷʵʱ���ݲɼ�
CNeteaseRTWebData gl_NeteaseRTWebData; // ����ʵʱ���ݲɼ�
CNeteaseDayLineWebData gl_NeteaseDayLineWebData; // ����������ʷ����
CNeteaseDayLineWebData gl_NeteaseDayLineWebDataSecond; // ����������ʷ����
CNeteaseDayLineWebData gl_NeteaseDayLineWebDataThird; // ����������ʷ����
CNeteaseDayLineWebData gl_NeteaseDayLineWebDataFourth; // ����������ʷ����
CNeteaseDayLineWebData gl_NeteaseDayLineWebDataFive; // ����������ʷ����
CNeteaseDayLineWebData gl_NeteaseDayLineWebDataSix; // ����������ʷ����
CCrweberIndexWebData gl_CrweberIndexWebData; // crweber.com�ϵ�ÿ������ָ��

CCrweberIndex gl_CrweberIndex;
CCrweberIndex gl_CrweberIndexLast;

CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����
CSystemTime gl_systemTime; // ϵͳʱ����ܡ�
CPriorityQueueRTData gl_QueueSinaRTData; // ϵͳʵʱ���ݶ��С�
//CQueueRTData gl_QueueSinaRTDataForSave; // ���ڴ洢������ʵʱ���ݶ���
CPriorityQueueRTData gl_QueueTengxunRTData; // ϵͳʵʱ���ݶ��С�
CThreadStatus gl_ThreadStatus; // ϵͳ�еĸ���״̬�������������߳���ʹ��

atomic<bool> gl_ExitingSystem = false; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
bool gl_fExitingCalculatingRS = false; // ����֪ͨ�����߳��˳����ź�
bool gl_fResetSystem = false; // ϵͳ������ʶ

bool gl_fNormalMode = false; // ����ģʽ��ʶ��Ĭ��Ϊ�档ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�
bool gl_fTestMode = true; // �Ƿ�������gl_fTestMode��ʶ�������ж��Ƿ���ʵ��ϵͳ��ʹ����MySQLTest��������

const int gl_cMaxSavingOneDayLineThreads = 3;
constexpr int gl_cMaxCalculateDayLineRS = 8;
// �ź�����������Ϊȫ�ֱ�����Ϊ�˳�ʼ����
Semaphore gl_SaveOneStockDayLine(4);
Semaphore gl_ProcessSinaRTDataQueue(1);   // ����ʵʱ���ݴ���ͬʱֻ����һ���̴߳���
Semaphore gl_ProcessTengxunRTDataQueue(1);
Semaphore gl_ProcessNeteaseRTDataQueue(1);

Semaphore gl_SemaphoreCalculateDayLineRS(8);

CMarket gl_ChinaStockMarket;  // ���г�ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�