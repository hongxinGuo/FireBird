#include"globedef.h"

#include "ChinaMarket.h"
#include "SystemTime.h"

// �����ñ���
CRTDataPtr gl_TESTpRTData = nullptr;

CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����
CSystemTime gl_systemTime; // ϵͳʱ����ܡ�

CThreadStatus gl_ThreadStatus; // ϵͳ�еĸ���״̬�������������߳���ʹ��

atomic_bool gl_ExitingSystem = false; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
bool gl_fExitingCalculatingRS = false; // ����֪ͨ�����߳��˳����ź�

bool gl_fNormalMode = false; // ����ģʽ��ʶ��Ĭ��Ϊ�档ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�
bool gl_fTestMode = true; // �Ƿ�������gl_fTestMode��ʶ�������ж��Ƿ���ʵ��ϵͳ��ʹ����MySQLTest��������

const int gl_cMaxSavingOneDayLineThreads = 4;

vector<CVirtualMarket*> gl_vMarket; // ���г�ָ���������ֻ����ִ�и��г���SchedulingTask

// ���г�����ΪΨһʵ��
CPotenDailyBriefingMarket gl_PotenDailyBriefingMarket;
CCrweberIndexMarket gl_CrweberIndexmarket;
CChinaMarket gl_ChinaStockMarket;  // ���г�ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�