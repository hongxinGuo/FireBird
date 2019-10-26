#include"globedef.h"

#include"StockID.h"
#include "Market.h"
#include "SystemTime.h"

CSinaRTWebData     gl_SinaRTWebData;      // ����ʵʱ���ݲɼ�
CTengxunRTWebData  gl_TengxunRTWebData;   // ��Ѷʵʱ���ݲɼ�
CNeteaseDayLineWebData gl_NeteaseDayLineWebData; // ����������ʷ����
CCrweberIndexWebData gl_CrweberIndexWebData;   // crweber.com�ϵ�ÿ������ָ��

CCrweberIndex gl_CrweberIndex;

CSystemMessage    gl_systemMessage;       // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����
CSystemTime       gl_systemTime;          // ϵͳʱ����ܡ�
CQueueRTData      gl_QueueRTData;         // ϵͳʵʱ���ݶ��С�
CThreadStatus     gl_ThreadStatus;        // ϵͳ�еĸ���״̬�������������߳���ʹ��

CCriticalSectionBool gl_ExitingSystem;                 //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
bool gl_fExitingCalculatingRS = false;    // ����֪ͨ�����߳��˳����ź�
bool gl_fResetSystem = false;             // ϵͳ������ʶ

bool gl_fNormalMode = false;              // ����ģʽ��ʶ��Ĭ��Ϊ�档ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�
bool gl_fTestMode = true;                 // �Ƿ�������gl_fTestMode��ʶ�������ж��Ƿ���ʵ��ϵͳ��ʹ����MySQLTest��������

CMarket gl_ChinaStockMarket;              // ���г�ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�