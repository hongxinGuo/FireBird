#include"globedef.h"

#include "ChinaMarket.h"

CRTDataPtr gl_TESTpRTData = nullptr;// �����ñ���

CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����

CThreadStatus gl_ThreadStatus; // ϵͳ�еĸ���״̬�������������߳���ʹ��

atomic_bool gl_ExitingSystem = false; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
bool gl_fExitingCalculatingRS = false; // ����֪ͨ�����߳��˳����ź�

bool gl_fNormalMode = false; // ����ģʽ��ʶ��Ĭ��Ϊ�档ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�
bool gl_fTestMode = true; // �Ƿ�������gl_fTestMode��ʶ�������ж��Ƿ���ʵ��ϵͳ��ʹ����MySQLTest��������

vector<CVirtualMarketPtr> gl_vMarketPtr; // ���г�ָ���������ֻ����ִ�и��г���SchedulingTask

#ifdef __GOOGLEMOCK__
#include"MockChinaMarket.h"
using namespace Testing;

// ���г�����ΪΨһʵ��
CPotenDailyBriefingMarketPtr gl_pPotenDailyBriefingMarket = nullptr;
CCrweberIndexMarketPtr gl_pCrweberIndexMarket = nullptr;
CMockChinaMarketPtr gl_pChinaStockMarket = nullptr;  // ���г�ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�

#else
// ���г�����ΪΨһʵ��
CPotenDailyBriefingMarketPtr gl_pPotenDailyBriefingMarket = nullptr;
CCrweberIndexMarketPtr gl_pCrweberIndexMarket = nullptr;
CChinaMarketPtr gl_pChinaStockMarket = nullptr;  // ���г�ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�

#endif