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

#include"FinnhubWebSocket.h"
#include"TiingoCryptoWebSocket.h"
#include"TiingoForexWebSocket.h"
#include"TiingoIEXWebSocket.h"

#include"ThreadStatus.h"

// ���±�����ΪΨһʵ��
CSystemConfiguration gl_systemConfiguration; // ϵͳ���ò������ܻ�.��ȫ�ֱ���Ҫλ������ȫ�ֱ�������ǰ�棬�Ա�֤��һ����ʼ����
CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵڶ�λ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����������ʱ����
CThreadStatus gl_ThreadStatus; // ϵͳ�й����̵߳ĸ���״̬�������������߳���ʹ��

CFinnhubInquiryType gl_FinnhubInquiryType;

time_t gl_tUTCTime = 0; // ���е��г�ʹ��ͬһ��Э������ʱ��Coordinated Universal Time��

CFinnhubInaccessibleExchange gl_finnhubInaccessibleExchange; // finnhub��ֹ���ʽ���������������˻��޷����ʵĽ��������ݣ���Ψһʵ��

// Ϊ�����ȳ�ʼ�����ź�����������Ϊȫ�ֱ���
binary_semaphore gl_UpdateChinaMarketDB{1}; // ���ڸ���ChinaMarket���ݿ⡣todo �����Ҷ�MySQL���ݿⲻ̫�˽⣬ż������ִ洢���⣬�ҹ�����ͬ���йأ��ʶ����û������
binary_semaphore gl_UpdateWorldMarketDB{1}; // ���ڸ���WorldMarket���ݿ⡣todo �����Ҷ�MySQL���ݿⲻ̫�˽⣬ż������ִ洢���⣬�ҹ�����ͬ���йأ��ʶ����û������
binary_semaphore gl_ProcessChinaMarketRTData{1}; // �������й��г���ʵʱ����ʱ��������ͬʱ�洢֮��
counting_semaphore<3> gl_WebSourceParseAndStoreData{3};//���ڽ���WebSource�е����ݡ���ParseAndStoreData�߳����������3�����������ܱ�֤�㹻�ļ����ٶȣ�Ҳ���ᷢ��ϵͳ����������Ϊ4��ʱ�����ܹ۲쵽ϵͳ������
counting_semaphore<8> gl_BackgroundWorkingThread{8}; // ����̨�����߳���������

CSystemData gl_SystemData;

// Data source, ��ΪΨһʵ��
CSinaRTDataSourcePtr gl_pSinaRTDataSource = nullptr;
CTengxunRTDataSourcePtr gl_pTengxunRTDataSource = nullptr;
CNeteaseRTDataSourcePtr gl_pNeteaseRTDataSource = nullptr;
CNeteaseDayLineDataSourcePtr gl_pNeteaseDayLineDataSource = nullptr;
CTengxunDayLineDataSourcePtr gl_pTengxunDayLineDataSource = nullptr;
CFinnhubDataSourcePtr gl_pFinnhubDataSource = nullptr;
CTiingoDataSourcePtr gl_pTiingoDataSource = nullptr;
CQuandlDataSourcePtr gl_pQuandlDataSource = nullptr;

// Web socket�� ��ΪΨһʵ����
CFinnhubWebSocketPtr gl_pFinnhubWebSocket;
CTiingoIEXWebSocketPtr gl_pTiingoIEXWebSocket;
CTiingoForexWebSocketPtr gl_pTiingoForexWebSocket;
CTiingoCryptoWebSocketPtr gl_pTiingoCryptoWebSocket;

// ����������
CPriorityQueueWebRTData gl_qSinaRT; // �й��г�����ʵʱ���ݶ��С�
CPriorityQueueWebRTData gl_qNeteaseRT; // �й��г�����ʵʱ���ݶ��С�
CPriorityQueueWebRTData gl_qTengxunRT; // �й��г���Ѷʵʱ���ݶ��С�
CTemplateMutexAccessQueue<CDayLineWebData> gl_qDayLine; // ��������

CContainerFinnhubForexSymbol gl_dataFinnhubForexSymbol;
CContainerFinnhubCryptoSymbol gl_dataFinnhubCryptoSymbol;
CContainerFinnhubCountry gl_dataFinnhubCountry;
CContainerFinnhubEconomicCalendar gl_dataFinnhubEconomicCalendar;

CContainerTiingoStock gl_dataTiingoStock;
CContainerTiingoCryptoSymbol gl_dataTiingoCryptoSymbol;

CContainerChosenStock gl_containerChosenStock;
CContainerChosenForex gl_containerChosenForex;
CContainerChosenCrypto gl_containerChosenCrypto;

// ���г�����ΪΨһʵ����Ϊ��������ı�����
CWorldMarketPtr gl_pWorldMarket = nullptr;
CChinaMarketPtr gl_pChinaMarket = nullptr; //��ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�//

vector<CVirtualMarketPtr> gl_vMarketPtr{}; // ���г�ָ���������ֻ����ִ�и��г���SchedulingTask
