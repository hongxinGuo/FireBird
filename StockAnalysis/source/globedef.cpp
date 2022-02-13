//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���е�ȫ�ֱ����Ķ����λ�ڴ˴�
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include"globedef.h"
#include"SemaphoreDef.h"

#include "ChinaMarket.h"
#include"WorldMarket.h"
#include"PotenDailyBriefingMarket.h"
#include"CrweberIndexMarket.h"

#include"SystemMessage.h"
#include"SystemData.h"
#include"WebInquirer.h"

#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"CrweberIndexWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"FinnhubWebInquiry.h"
#include"TiingoWebInquiry.h"
#include"QuandlWebInquiry.h"

CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����

CThreadStatus gl_ThreadStatus; // ϵͳ�еĸ���״̬�������������߳���ʹ��

deque<CString> gl_dequeRTStockInquire; // ����ʵʱ��Ʊ��Ϣ�Ĺ�Ʊ���У���δʹ�ã�
deque<CString> gl_dequeDayLineStockInquire; // ����������ʷ���ݵĹ�Ʊ���У���δʹ�ã�

atomic_bool gl_fExitingSystem = false; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
bool gl_fExitingCalculatingRS = false; // ����֪ͨ�����߳��˳����ź�

bool gl_fNormalMode = false; // ����ģʽ��ʶ��Ĭ��Ϊ�档ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�
bool gl_fTestMode = true; // �Ƿ�������gl_fTestMode��ʶ�������ж��Ƿ���ʵ��ϵͳ��ʹ����MySQLTest��������

CWebRTDataContainer gl_WebRTDataContainer; // ����ʵʱ���ݴ洢����

// �ź�����������Ϊȫ�ֱ�����Ϊ�˳�ʼ����
Semaphore gl_UpdateWorldMarketDB(1);  // ���ź�����������������Ʊ������ʷ���ݿ�
Semaphore gl_SaveOneStockDayLine(4);  // ���ź�����������������ʷ���ݿ�
Semaphore gl_ProcessSinaRTDataQueue(1);   // ����ʵʱ���ݴ���ͬʱֻ����һ���̴߳���
Semaphore gl_ProcessTengxunRTDataQueue(1);
Semaphore gl_ProcessNeteaseRTDataQueue(1);
Semaphore gl_SemaphoreBackGroundTaskThreads(cMaxBackGroundTaskThreads); // ��̨�����߳��������Ĭ��Ϊ8
Semaphore gl_MaintainCrweberDB(1); // Crweber���ݿ�ֻ����ͬʱһ���̲߳���֮��

vector<CVirtualMarketPtr> gl_vMarketPtr; // ���г�ָ���������ֻ����ִ�и��г���SchedulingTask

CWebInquirer gl_WebInquirer;
CSystemData gl_SystemData;

int gl_iMaxSavingOneDayLineThreads = 4; // ����洢�������ݵ��߳������˱���ȡֵ��ΧΪ1-4
// ����������ȡ��
CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry = nullptr; // ����ʵʱ���ݲɼ�
CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry = nullptr; // ��Ѷʵʱ���ݲɼ�
CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry = nullptr; // ����ʵʱ���ݲɼ�
CCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry = nullptr; // crweber.com�ϵ�ÿ������ָ��
CPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry = nullptr; // Poten.com�ϵ��������ݡ�
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // ����������ʷ����
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2 = nullptr; // ����������ʷ����
CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry; // Finnhub.io֤ȯ��Ϣ
CQuandlWebInquiryPtr gl_pQuandlWebInquiry;
CTiingoWebInquiryPtr gl_pTiingoWebInquiry;

// ���г�����ΪΨһʵ��
CWorldMarketPtr gl_pWorldMarket = nullptr;
CPotenDailyBriefingMarketPtr gl_pPotenDailyBriefingMarket = nullptr;
CCrweberIndexMarketPtr gl_pCrweberIndexMarket = nullptr;
CChinaMarketPtr gl_pChinaMarket = nullptr;  // ���г�ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�