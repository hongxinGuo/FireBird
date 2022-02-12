#include"pch.h"
#include"globedef.h"

#include "ChinaMarket.h"
#include"WorldMarket.h"
#include"PotenDailyBriefingMarket.h"
#include"CrweberIndexMarket.h"

#include"SystemMessage.h"
#include"SystemData.h"
#include"WebInquirer.h"

CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����

CThreadStatus gl_ThreadStatus; // ϵͳ�еĸ���״̬�������������߳���ʹ��

deque<CString> gl_dequeRTStockInquire; // ����ʵʱ��Ʊ��Ϣ�Ĺ�Ʊ���У���δʹ�ã�
deque<CString> gl_dequeDayLineStockInquire; // ����������ʷ���ݵĹ�Ʊ���У���δʹ�ã�

atomic_bool gl_fExitingSystem = false; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
bool gl_fExitingCalculatingRS = false; // ����֪ͨ�����߳��˳����ź�

bool gl_fNormalMode = false; // ����ģʽ��ʶ��Ĭ��Ϊ�档ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�
bool gl_fTestMode = true; // �Ƿ�������gl_fTestMode��ʶ�������ж��Ƿ���ʵ��ϵͳ��ʹ����MySQLTest��������

vector<CVirtualMarketPtr> gl_vMarketPtr; // ���г�ָ���������ֻ����ִ�и��г���SchedulingTask

CWebInquirer gl_WebInquirer;
CSystemData gl_SystemData;

// ���г�����ΪΨһʵ��
CWorldMarketPtr gl_pWorldMarket = nullptr;
CPotenDailyBriefingMarketPtr gl_pPotenDailyBriefingMarket = nullptr;
CCrweberIndexMarketPtr gl_pCrweberIndexMarket = nullptr;
CChinaMarketPtr gl_pChinaMarket = nullptr;  // ���г�ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�