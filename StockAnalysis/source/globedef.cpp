//////////////////////////////////////////////////////////////////////////////////////
//
// Ϊ�˱�֤����ĳ�ʼ�������е�ȫ�ֱ�����Ӧ���ڴ��ļ��С�
//
///////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

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

#include "ChinaMarket.h"
#include"WorldMarket.h"

CSystemConfigeration gl_systemConfigeration; // ϵͳ���ò������ܻ�.��ȫ�ֱ���Ҫλ������ȫ�ֱ�������ǰ�棬�Ա�֤��һ����ʼ����
CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵڶ�λ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����������ʱ����
CSystemStatus gl_systemStatus; // ϵͳ״̬��
CThreadStatus gl_ThreadStatus; // ϵͳ�й����̵߳ĸ���״̬�������������߳���ʹ��
CHighPerformanceCounter g_highPerformanceCounter;

CWebRTDataContainer gl_WebRTDataContainer; // ����ʵʱ���ݴ洢����

// Ϊ�����ȳ�ʼ�����ź�����������Ϊȫ�ֱ���
Semaphore gl_UpdateWorldMarketDB(1);  // ���ź������ڸ���WorldMarket���ݿ�
counting_semaphore<4> gl_UpdateWorldMarketDB2{ 4 };  // ���ź������ڸ���WorldMarket���ݿ�
Semaphore gl_SaveThreadPermitted(4);  // ���ź�����������������ʷ���ݿ�
Semaphore gl_BackGroundTaskThread(8); // ��̨�����߳��������Ĭ��Ϊ8

CWebInquirer gl_WebInquirer;
CSystemData gl_SystemData;

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