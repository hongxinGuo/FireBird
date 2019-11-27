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
CQueueRTData gl_QueueSinaRTData; // ϵͳʵʱ���ݶ��С�
CQueueRTData gl_QueueTengxunRTData; // ϵͳʵʱ���ݶ��С�
CThreadStatus gl_ThreadStatus; // ϵͳ�еĸ���״̬�������������߳���ʹ��

atomic<bool> gl_ExitingSystem = false; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
bool gl_fExitingCalculatingRS = false; // ����֪ͨ�����߳��˳����ź�
bool gl_fResetSystem = false; // ϵͳ������ʶ

bool gl_fNormalMode = false; // ����ģʽ��ʶ��Ĭ��Ϊ�档ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�
bool gl_fTestMode = true; // �Ƿ�������gl_fTestMode��ʶ�������ж��Ƿ���ʵ��ϵͳ��ʹ����MySQLTest��������

const int gl_cMaxSavingOneDayLineThreads = 3;
const int gl_cMaxCalculateDayLineRS = 8;
// �ź�����������Ϊȫ�ֱ�����Ϊ�˳�ʼ����
CSemaphore gl_SaveOneStockDayLine(gl_cMaxSavingOneDayLineThreads, gl_cMaxSavingOneDayLineThreads);
CSemaphore gl_CalculateDayLineRelativeStrong(gl_cMaxCalculateDayLineRS, gl_cMaxCalculateDayLineRS); // �����������ǿ�ȵ��߳��������8��ͬʱ����
CSemaphore gl_ProcessSinaRTDataQueue(1, 1);   // ����ʵʱ���ݴ���ͬʱֻ����һ���̴߳���
CSemaphore gl_ProcessTengxunRTDataQueue(1, 1);
CSemaphore gl_ProcessNeteaseRTDataQueue(1, 1);

CMarket gl_ChinaStockMarket;  // ���г�ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�