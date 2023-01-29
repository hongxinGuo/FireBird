//////////////////////////////////////////////////////////////////////////////////////
//
// Ϊ�˱�֤����ĳ�ʼ�������е�ȫ�ֱ�����Ӧ���ڴ��ļ��С�
//
///////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"FinnhubInquiryType.h"
#include"FinnhubInaccessibleExchange.h"

#include"FinnhubDataSource.h"
#include"NeteaseDayLineDataSource.h"
#include"NeteaseRTDataSource.h"
#include"QuandlDataSource.h"
#include"SinaRTDataSource.h"
#include"TengxunRTDataSource.h"
#include"TengxunDayLineDataSource.h"
#include"TiingoDataSource.h"

#include"FinnhubWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"TengxunDayLineWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"QuandlWebInquiry.h"
#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"TiingoWebInquiry.h"

#include"FinnhubWebSocket.h"
#include"TiingoCryptoWebSocket.h"
#include"TiingoForexWebSocket.h"
#include"TiingoIEXWebSocket.h"

#include"ThreadStatus.h"

#include "ChinaMarket.h"

bool gl_bStartInitializeGlobeVariable = false;

CSystemConfiguration gl_systemConfiguration; // ϵͳ���ò������ܻ�.��ȫ�ֱ���Ҫλ������ȫ�ֱ�������ǰ�棬�Ա�֤��һ����ʼ����
CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵڶ�λ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����������ʱ����
CSystemStatus gl_systemStatus; // ϵͳ״̬��
CThreadStatus gl_ThreadStatus; // ϵͳ�й����̵߳ĸ���״̬�������������߳���ʹ��

CFinnhubInquiryType gl_FinnhubInquiryType;

CHighPerformanceCounter gl_counter;

CFinnhubInaccessibleExchange gl_finnhubInaccessibleExchange; // finnhub��ֹ���ʽ�����������˻��޷����ʵĽ��������ݣ�

// Ϊ�����ȳ�ʼ�����ź�����������Ϊȫ�ֱ���
binary_semaphore gl_UpdateWorldMarketDB{1}; // ���ź������ڸ���WorldMarket���ݿ�
counting_semaphore<4> gl_SaveDayLineThreadPermitted{1}; // �����ݿ���û����������ʱ�����Ӵ��ź������ֵ��4
counting_semaphore<8> gl_BackGroundTaskThread{8}; // ��̨�����߳��������Ϊ8
binary_semaphore gl_ProcessChinaMarketRTData{1}; // �����й��г���ʵʱ����ʱ��������ͬʱ�洢֮��
// �ź���gl_WebSourceParseAndStoreData���ڽ���WebSource�е����ݡ�
// ��ParseAndStoreData�߳����������3�����������ܱ�֤�㹻�ļ����ٶȣ�Ҳ���ᷢ��ϵͳ����������Ϊ4��ʱ�����ܹ۲쵽ϵͳ������
counting_semaphore<1> gl_WebSourceParseAndStoreData{1};

CSystemData gl_SystemData;

// ����������ȡ��
CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry = nullptr; // ����ʵʱ���ݲɼ�
CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry = nullptr; // ��Ѷʵʱ���ݲɼ�
CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry = nullptr; // ����ʵʱ���ݲɼ�
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // ����������ʷ����
CTengxunDayLineWebInquiryPtr gl_pTengxunDayLineWebInquiry = nullptr; // ����������ʷ����

CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry = nullptr; // Finnhub.io֤ȯ��Ϣ
CQuandlWebInquiryPtr gl_pQuandlWebInquiry = nullptr; // Quandl.com��Ʊ����
CTiingoWebInquiryPtr gl_pTiingoWebInquiry = nullptr; // Tiingo.com��Ʊ����

// Data source
CSinaRTDataSourcePtr gl_pSinaRTDataSource = nullptr;
CTengxunRTDataSourcePtr gl_pTengxunRTDataSource = nullptr;
CNeteaseRTDataSourcePtr gl_pNeteaseRTDataSource = nullptr;
CNeteaseDayLineDataSourcePtr gl_pNeteaseDayLineDataSource = nullptr;
CTengxunDayLineDataSourcePtr gl_pTengxunDayLineDataSource = nullptr;
CFinnhubDataSourcePtr gl_pFinnhubDataSource = nullptr;
CTiingoDataSourcePtr gl_pTiingoDataSource = nullptr;

CQuandlDataSourcePtr gl_pQuandlDataSource = nullptr;

// Web socket
CFinnhubWebSocket gl_finnhubWebSocket;
CTiingoIEXWebSocket gl_tiingoIEXWebSocket;
CTiingoForexWebSocket gl_tiingoForexWebSocket;
CTiingoCryptoWebSocket gl_tiingoCryptoWebSocket;

vector<CVirtualMarketPtr> gl_vMarketPtr{}; // ���г�ָ���������ֻ����ִ�и��г���SchedulingTask

// ���г�����ΪΨһʵ����Ϊ��������ı�����
CWorldMarketPtr gl_pWorldMarket = nullptr;
CChinaMarketPtr gl_pChinaMarket = nullptr; //��ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�//
