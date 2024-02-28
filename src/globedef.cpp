//////////////////////////////////////////////////////////////////////////////////////
//
// Ϊ�˱�֤����ĳ�ʼ�������е�ȫ�ֱ�����Ӧ���ڴ��ļ��С�
//
// Ĭ��״̬��ȫ�ֱ�����װ�ز������ݿ⡣��ϵͳ��ʼ����������MainFrame����ResetMarket����������װ�أ���ʱװ�ص��ǹ������ݿ⡣
// todo ��Ĭ��״̬��Ϊ��������״̬��װ�빤�����ݿ⣻����״̬�ɲ����������趨������װ��������ݿ⡣
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

#include "ChinaMarket.h"
#include "WorldMarket.h"

#include"thread.h"
#include"threadStatus.h"

#undef max
#include"concurrencpp/concurrencpp.h"
using namespace concurrencpp;

// ���±�����ΪΨһʵ��
CSystemConfiguration gl_systemConfiguration; // ϵͳ���ò������ܻ�.��ȫ�ֱ���Ҫλ������ȫ�ֱ�������ǰ�棬�Ա�֤��һ����ʼ����
CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵڶ�λ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����������ʱ����

CThreadStatus gl_ThreadStatus; // ϵͳ�й����̵߳ĸ���״̬�������������߳���ʹ��
concurrencpp::runtime gl_runtime; // �����߳����е�����

CFinnhubInquiryType gl_FinnhubInquiryType;

time_t gl_tUTCTime = 0; // ���е��г�ʹ��ͬһ��Э������ʱ��Coordinated Universal Time��

CFinnhubInaccessibleExchange gl_finnhubInaccessibleExchange; // finnhub��ֹ���ʽ���������������˻��޷����ʵĽ��������ݣ���Ψһʵ��

// Ϊ�����ȳ�ʼ�����ź�����������Ϊȫ�ֱ���
binary_semaphore gl_UpdateChinaMarketDB{1}; // ���ڸ���ChinaMarket���ݿ⡣todo �����Ҷ�MySQL���ݿⲻ̫�˽⣬ż������ִ洢���⣬�ҹ�����ͬ���йأ��ʶ����û������
binary_semaphore gl_UpdateWorldMarketDB{1}; // ���ڸ���WorldMarket���ݿ⡣todo �����Ҷ�MySQL���ݿⲻ̫�˽⣬ż������ִ洢���⣬�ҹ�����ͬ���йأ��ʶ����û������
binary_semaphore gl_ProcessChinaMarketRTData{1}; // �������й��г���ʵʱ����ʱ��������ͬʱ�洢֮��
counting_semaphore<8> gl_BackgroundWorkingThread{8}; // ����̨�����߳���������

CSystemData gl_SystemData;
concurrencpp::timer gl_timer;

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

// ϵͳ״̬
bool gl_bChinaMarketResetting = false; // �й��г�������
bool gl_bWorldMarketResetting = false; // �����г�������

// ����������
ConcurrentQueue<CWebRTDataPtr> gl_qChinaMarketRTData(10000); // �й��г�����ʵʱ���ݶ��С�
ConcurrentQueue<CDayLineWebDataPtr> gl_qDayLine(1000); // ��������

// ChinaMarket���������
CContainerChinaStock gl_dataContainerChinaStock;
CContainerStockSymbol gl_dataContainerChinaStockSymbol;

// World market���������
CContainerFinnhubForexSymbol gl_dataFinnhubForexSymbol;
CContainerFinnhubCryptoSymbol gl_dataFinnhubCryptoSymbol;
CContainerFinnhubCountry gl_dataContainerFinnhubCountry;
CContainerFinnhubEconomicCalendar gl_dataContainerFinnhubEconomicCalendar;

CContainerFinnhubStockExchange gl_dataContainerFinnhubStockExchange;
CContainerFinnhubForexExchange gl_dataContainerFinnhubForexExchange;
CContainerFinnhubCryptoExchange gl_dataContainerFinnhubCryptoExchange;

CContainerWorldStock gl_dataContainerFinnhubStock;
CContainerTiingoStock gl_dataContainerTiingoStock;
CContainerTiingoCryptoSymbol gl_dataContainerTiingoCryptoSymbol;

CContainerChosenStock gl_dataContainerChosenWorldStock;
CContainerChosenForex gl_dataContainerChosenWorldForex;
CContainerChosenCrypto gl_dataContainerChosenWorldCrypto;

// ���г�����ΪΨһʵ����Ϊ��������ı�����
CWorldMarketPtr gl_pWorldMarket = nullptr; //��ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�//
CChinaMarketPtr gl_pChinaMarket = nullptr; //��ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�//

vector<CVirtualDataSourcePtr> gl_vDataSource{}; // ����������Դ����������ִ�и�����Ԫ����������ʹ���
vector<CVirtualMarketPtr> gl_vMarketPtr{}; // ���г�ָ���������ֻ����ִ�и��г���ScheduleTask
