//////////////////////////////////////////////////////////////////////////////////////
//
// ���е�ȫ�ֱ�����Ӧ���ڴ��ļ��С�
//
///////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include "ChinaMarket.h"
#include"WorldMarket.h"

#include"ThreadStatus.h"

#include"WebInquirer.h"

#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"FinnhubWebInquiry.h"
#include"TiingoWebInquiry.h"
#include"QuandlWebInquiry.h"

#include"WebRTDataContainer.h"
#include"HighPerformanceCounter.h"

#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"

CSystemConfigeration gl_systemConfigeration; // ϵͳ�����ܻ�.��ȫ�ֱ���Ҫλ������ȫ�ֱ�������ǰ��
CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����
CSystemStatus gl_systemStatus;
CThreadStatus gl_ThreadStatus; // ϵͳ�еĸ���״̬�������������߳���ʹ��
CHighPerformanceCounter g_highPerformanceCounter;

CWebRTDataContainer gl_WebRTDataContainer; // ����ʵʱ���ݴ洢����

// �ź�����������Ϊȫ�ֱ�����Ϊ�˳�ʼ����
Semaphore gl_UpdateWorldMarketDB(1);  // ���ź�����������������Ʊ������ʷ���ݿ�
Semaphore gl_SaveThreadPermitted(4);  // ���ź�����������������ʷ���ݿ�
Semaphore gl_SemaphoreBackGroundTaskThreads(8); // ��̨�����߳��������Ĭ��Ϊ8

CWebInquirer gl_WebInquirer;
CSystemData gl_SystemData;

int gl_iMaxSavingOneDayLineThreads = 4; // ����洢�������ݵ��߳������˱���ȡֵ��ΧΪ1-4

// ����������ȡ��
CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry = nullptr; // ����ʵʱ���ݲɼ�
CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry = nullptr; // ��Ѷʵʱ���ݲɼ�
CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry = nullptr; // ����ʵʱ���ݲɼ�
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // ����������ʷ����
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2 = nullptr; // ����������ʷ����
CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry = nullptr; // Finnhub.io֤ȯ��Ϣ
CQuandlWebInquiryPtr gl_pQuandlWebInquiry = nullptr; // Quandl.com��Ʊ����
CTiingoWebInquiryPtr gl_pTiingoWebInquiry = nullptr; // Tiingo.com��Ʊ����

vector<CDataSourcePtr> gl_vDataSource;
CFinnhubDataSourcePtr gl_pDataSourceFinnhub = nullptr;
CTiingoDataSourcePtr gl_pDataSourceTiingo = nullptr;
CDataSourcePtr gl_pDataSourceQuandl = nullptr;

vector<CVirtualMarketPtr> gl_vMarketPtr{}; // ���г�ָ���������ֻ����ִ�и��г���SchedulingTask

// ���г�����ΪΨһʵ��
CWorldMarketPtr gl_pWorldMarket = nullptr;
CChinaMarketPtr gl_pChinaMarket = nullptr; //���г�ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�//