//////////////////////////////////////////////////////////////////////////////////////
//
// Ϊ�˱�֤����ĳ�ʼ�������е�ȫ�ֱ�����Ӧ���ڴ��ļ��С�
//
// Ĭ��״̬Ϊ��������״̬��װ�빤�����ݿ⣻����״̬�ɲ����������趨������װ��������ݿ⡣
//
///////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"
#include"NlohmannJsonHeader.h"
#include"ConCurrenCppHeader.h"
#include"ConcurrentQueueHeader.h"

import FireBird.DataSource.Finnhub;
import FireBird.DataSource.NeteaseDayLine;
import FireBird.DataSource.NeteaseRT;
import FireBird.DataSource.Quandl;
import FireBird.DataSource.SinaRT;
import FireBird.DataSource.TengxunRT;
import FireBird.DataSource.TengxunDayLine;
import FireBird.DataSource.Tiingo;

import FireBird.Market.China;
import FireBird.Market.Virtual;
import FireBird.Market.World;
import FireBird.Market.Task;

import FireBird.System.Message;
import FireBird.System.Data;

import FireBird.FinnhubInquiryType;
import FireBird.System.FinnhubInaccessibleExchange;
import FireBird.Tiingo.InaccessibleStock;

import FireBird.WebSocket.Finnhub;
import FireBird.WebSocket.TiingoIEX;
import FireBird.WebSocket.TiingoForex;
import FireBird.WebSocket.TiingoCrypto;

import FireBird.System.ThreadStatus;

import FireBird.Type.DayLineWebData;
import FireBird.Container.FinnhubStock;
import FireBird.Container.StockSymbol;

HANDLE gl_hFireBirdMutex{ nullptr };

shared_ptr<spdlog::logger> gl_dailyLogger = nullptr;
shared_ptr<spdlog::logger> gl_traceLogger; // ������־������ϵͳ����
shared_ptr<spdlog::logger> gl_warnLogger; // ���������־������ϵͳ����
shared_ptr<spdlog::logger> gl_dailyWebSocketLogger = nullptr;
shared_ptr<spdlog::logger> gl_SoftwareDevelopingLogger = nullptr;

// ���±�����ΪΨһʵ��
CSystemConfiguration gl_systemConfiguration; // ϵͳ���ò������ܻ�.��ȫ�ֱ���Ҫλ������ȫ�ֱ�������ǰ�棬�Ա�֤��һ����ʼ����
CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵڶ�λ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����������ʱ����
CThreadStatus gl_ThreadStatus; // ϵͳ�й����̵߳ĸ���״̬�������������߳���ʹ��
CSystemData gl_SystemData;

CFinnhubInquiryType gl_FinnhubInquiryType;
CFinnhubInaccessibleExchange gl_finnhubInaccessibleExchange; // finnhub��ֹ���ʽ���������������˻��޷����ʵĽ��������ݣ���Ψһʵ��
CTiingoInaccessibleStock gl_tiingoInaccessibleStock;

std::chrono::sys_seconds gl_tpNow; // ��ǰϵͳʱ�ӵ�ʱ����� ���е��г�ʹ��ͬһ��Э������ʱ��Coordinated Universal Time��

// Ϊ�����ȳ�ʼ�����ź�����������Ϊȫ�ֱ���
binary_semaphore gl_UpdateChinaMarketDB{ 1 }; // ���ڸ���ChinaMarket���ݿ⡣todo ���ڶ�MySQL���ݿⲻ̫�˽⣬ż������ִ洢���⣬�ҹ�����ͬ���йأ��ʶ����û������
binary_semaphore gl_UpdateWorldMarketDB{ 1 }; // ���ڸ���WorldMarket���ݿ⡣todo ���ڶ�MySQL���ݿⲻ̫�˽⣬ż������ִ洢���⣬�ҹ�����ͬ���йأ��ʶ����û������
binary_semaphore gl_ProcessChinaMarketRTData{ 1 }; // �������й��г���ʵʱ����ʱ��������ͬʱ�洢֮��
counting_semaphore<__MAX_BACKGROUND_WORKING_THREAD__> gl_BackgroundWorkingThread{ __MAX_BACKGROUND_WORKING_THREAD__ }; // ����̨�����߳�����������ʹ��ϵͳ�����趨Ϊʵ�ʴ�С

concurrencpp::runtime gl_runtime; // �����߳����е�����
concurrencpp::runtime gl_backgroundRuntime; // ��̨�����߳����е�����
int gl_concurrency_level = 4; // ���м����������������Ĭ��Ϊ�ĸ�Э��
array<timer, TASK_END> gl_aTimer; // timer���С����е��������ʹ�õ���timer�Ļ�������Ӧ��timer�洢�ڴ������С�

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
ConcurrentQueue<CWebRTDataPtr> gl_qChinaMarketRTData(10000); // �й��г�����ʵʱ���ݶ��С�
ConcurrentQueue<CDayLineWebDataPtr> gl_qDayLine(1000); // ��������

// ChinaMarket���������
CContainerChinaStock gl_dataContainerChinaStock;
CContainerStockSymbol gl_dataContainerChinaStockSymbol;

// World market���������
CContainerFinnhubForexSymbol gl_dataFinnhubForexSymbol;
CContainerFinnhubCrypto gl_dataFinnhubCryptoSymbol;
CContainerFinnhubCountry gl_dataContainerFinnhubCountry;
CContainerFinnhubEconomicCalendar gl_dataContainerFinnhubEconomicCalendar;

CContainerFinnhubStockExchange gl_dataContainerFinnhubStockExchange;
CContainerFinnhubForexExchange gl_dataContainerFinnhubForexExchange;
CContainerFinnhubCryptoExchange gl_dataContainerFinnhubCryptoExchange;

CContainerFinnhubStock gl_dataContainerFinnhubStock;
CContainerTiingoStock gl_dataContainerTiingoStock;
CContainerTiingoSymbol gl_dataContainerTiingoNewSymbol; // �洢ÿ�ղ�ѯ�������Ĺ�Ʊָ��
CContainerTiingoSymbol gl_dataContainerTiingoDelistedSymbol; // �洢�����еĹ�Ʊָ��
CContainerTiingoCryptoSymbol gl_dataContainerTiingoCryptoSymbol;
CContainerTiingoFundamentalDefinition gl_dataContainerTiingoFundamentalDefinition;

CContainerChosenStock gl_dataContainerChosenFinnhubStock;
CContainerChosenForex gl_dataContainerChosenWorldForex;
CContainerChosenCrypto gl_dataContainerChosenWorldCrypto;

// ���г�����ΪΨһʵ����Ϊ��������ı�����
CWorldMarketPtr gl_pWorldMarket = nullptr; //��ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�//
CChinaMarketPtr gl_pChinaMarket = nullptr; //��ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�//

vector<CVirtualMarketPtr> gl_vMarket{}; // ���г�ָ���������ֻ����ִ�и��г���ScheduleTask
