#include "WorldMarket.h"
#include"pch.h"

#include"systemData.h"

#include "WorldMarket.h"

#include"thread.h"

#include"FinnhubInaccessibleExchange.h"

#include"FinnhubDataSource.h"

#include"FinnhubWebSocket.h"
#include"TiingoIEXWebSocket.h"
#include"TiingoForexWebSocket.h"
#include"TiingoCryptoWebSocket.h"

#include"SetFinnhubStockDayLine.h"

#include "ChinaMarket.h"
#include "InfoReport.h"
#include "QuandlDataSource.h"
#include "ThreadStatus.h"
#include "TiingoDataSource.h"
#include "TiingoInaccessibleStock.h"
#include "TimeConvert.h"

CWorldMarket::CWorldMarket() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	if (static int siInstance = 0; ++siInstance > 1) {
		TRACE("CWorldMarket�г�����ֻ�������һ��ʵ��\n");
	}

	m_strMarketId = _T("�����г�");

	//m_lMarketTimeZone = GetMarketLocalTimeOffset(_T("America/New_York")); // ��������ʹ��������׼ʱ��
	m_lMarketTimeZone = 4 * 3600;
	m_lOpenMarketTime = 9 * 3600 + 1800; // �������п���ʱ��Ϊ�ŵ���ʮ��

	// ���裨Ҳ�޷���ÿ�ո��µı�����������
	m_lCurrentUpdateEPSSurprisePos = 0;
	m_lCurrentUpdateDayLinePos = 0; // ����֤ȯ���������ж�ʮ��֮�࣬�޷���һ��֮�ڸ����꣬�ʲ������ô�������
	m_bFinnhubWebSiteAccessible = true;

	CalculateTime();

	m_pvMarketStatus = make_shared<vector<CMarketStatusPtr>>();
	m_pvMarketHoliday = make_shared<vector<CMarketHolidayPtr>>();

	Reset();

	AddTask(WORLD_MARKET_CREATE_TASK__, 1);
}

CWorldMarket::~CWorldMarket() {
	PrepareToCloseMarket();
}

void CWorldMarket::Reset() {
	ResetFinnhub();
	ResetTiingo();
	ResetDataContainer();
}

void CWorldMarket::ResetFinnhub() {
	SetSystemReady(false); // �г���ʼ״̬Ϊδ���úá�
	if (IsEarlyThen(gl_finnhubInaccessibleExchange.GetUpdateDate(), GetMarketDate(), 7)) {
		gl_finnhubInaccessibleExchange.Clear(); // ��ʹ�ø���ʱ������һ�ܵ����ݡ����֮����ϵͳ�Զ������µ�״̬��
		gl_finnhubInaccessibleExchange.SetUpdateDate(GetMarketDate());
	}
	m_pvMarketStatus->clear();
	m_pvMarketHoliday->clear();
}

void CWorldMarket::ResetQuandl() {
}

void CWorldMarket::ResetTiingo() const {
	if (IsEarlyThen(gl_tiingoInaccessibleStock.GetUpdateDate(), GetMarketDate(), 7)) {
		gl_tiingoInaccessibleStock.Clear(); // ��ʹ�ø���ʱ������һ�ܵ����ݡ����֮����ϵͳ�Զ������µ�״̬��
		gl_tiingoInaccessibleStock.SetUpdateDate(GetMarketDate());
	}
}

void CWorldMarket::ResetDataContainer() {
	gl_dataContainerFinnhubStockExchange.Reset();
	gl_dataContainerFinnhubForexExchange.Reset();
	gl_dataContainerFinnhubCryptoExchange.Reset();
	gl_dataFinnhubForexSymbol.Reset();
	gl_dataFinnhubCryptoSymbol.Reset();
	gl_dataContainerFinnhubCountry.Reset();
	gl_dataContainerFinnhubEconomicCalendar.Reset();

	gl_dataContainerFinnhubStock.Reset();

	gl_dataContainerTiingoStock.Reset();
	gl_dataContainerTiingoNewSymbol.Reset();
	gl_dataContainerTiingoCryptoSymbol.Reset();

	gl_dataContainerChosenFinnhubStock.Reset();
	gl_dataContainerChosenWorldForex.Reset();
	gl_dataContainerChosenWorldCrypto.Reset();
}

void CWorldMarket::ResetMarket() {
	m_fResettingMarket = true;

	Reset();

	UpdateToken();

	gl_dataContainerFinnhubStockExchange.LoadDB(); // װ�����罻������Ϣ
	gl_dataContainerFinnhubCountry.LoadDB();
	gl_dataContainerFinnhubStock.LoadDB();
	gl_dataContainerChosenFinnhubStock.LoadDB();
	gl_dataContainerFinnhubForexExchange.LoadDB();
	gl_dataFinnhubForexSymbol.LoadDB();
	gl_dataContainerChosenWorldForex.LoadDB();
	gl_dataContainerFinnhubCryptoExchange.LoadDB();
	gl_dataFinnhubCryptoSymbol.LoadDB();
	gl_dataContainerChosenWorldCrypto.LoadDB();
	gl_dataContainerFinnhubEconomicCalendar.LoadDB();

	gl_dataContainerTiingoStock.LoadDB();
	gl_dataContainerTiingoCryptoSymbol.LoadDB();
	gl_dataContainerTiingoFundamentalDefinition.LoadDB();

	for (const auto& pDataSource : m_vDataSource) {
		pDataSource->Reset();
	}

	CString str = _T("����World Market��������׼ʱ�䣺");
	str += GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(str);

	m_fResettingMarket = false;
}

void CWorldMarket::PrepareToCloseMarket() {
	//ASSERT(gl_systemConfiguration.IsExitingSystem());
	DisconnectAllWebSocket();
}

//////////////////////////////////////////////////////////////////////////////////////
//
// ��������Ⱥ���
//
// todo ����concurrencpp::timer�󣬿��Խ��˺����ĸ����������Ϊ��ʱ���������Ĺ����߳�����
//
//////////////////////////////////////////////////////////////////////////////////////
int CWorldMarket::ProcessTask(long lCurrentTime) {
	if (IsMarketTaskEmpty()) return false;
	const auto pTask = GetMarketTask();
	if (lCurrentTime >= pTask->GetTime()) {
		DiscardCurrentMarketTask();
		switch (pTask->GetType()) {
		case WORLD_MARKET_CREATE_TASK__: // ������������
			TaskCreateTask(lCurrentTime);
			break;
		case WORLD_MARKET_CHECK_SYSTEM_READY__: // 170000����ϵͳ
			TaskCheckMarketReady(lCurrentTime);
			break;
		case WORLD_MARKET_RESET__: // 170000����ϵͳ
			TaskResetMarket(lCurrentTime);
			break;
		case WORLD_MARKET_UPDATE_DB__:
			ASSERT(!IsTimeToResetSystem(lCurrentTime));// ������ʱ����ϵͳ�������ݿ���Ҫ����װ�룬�ʶ���ʱ������������ݿ⡣
			TaskUpdateWorldMarketDB(lCurrentTime);
			break;
		case WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__:
			TaskMonitorWebSocket(lCurrentTime);
			break;
		case WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__:
			TaskProcessWebSocketData(lCurrentTime);
			TaskPerSecond(lCurrentTime);
			break;
		case WORLD_MARKET_CONNECT_FINNHUB_WEB_SOCKET__:
			ASSERT(!gl_systemConfiguration.IsUsingFinnhubWebSocket());
			gl_systemConfiguration.SetUsingFinnhubWebSocket(true); // ֻ���ñ�ʶ��ʵ������������������ɡ�
			break;
		case WORLD_MARKET_TIINGO_INQUIRE_IEX_TOP_OF_BOOK__:
			gl_pTiingoDataSource->SetUpdateIEXTopOfBook(true); // 
			break;
		case WORLD_MARKET_TIINGO_BUILD_TODAY_STOCK_DAYLINE__:
			gl_pWorldMarket->TaskCreateTiingoTradeDayDayLine(lCurrentTime);
			break;
		case WORLD_MARKET_TIINGO_PROCESS_DAYLINE__:
			gl_pWorldMarket->TaskProcessTiingoDayLine(lCurrentTime);
			break;
		case WORLD_MARKET_TIINGO_UPDATE_52WEEK_HIGH_LOW__:
			gl_pWorldMarket->TaskUpdateTiingoStock52WeekHighLowDB(lCurrentTime);
			break;
		case WORLD_MARKET_TIINGO_CALCULATE__:
			gl_pWorldMarket->TaskTiingoCalculate(lCurrentTime);
			break;
		default:
			break;
		}
		return pTask->GetType();
	}
	return 0;
}

int CWorldMarket::ProcessCurrentImmediateTask(long lMarketTime) {
	ASSERT(!m_marketImmediateTask.Empty());

	auto pTask = m_marketImmediateTask.GetTask();
	auto taskType = pTask->GetType();
	m_marketImmediateTask.DiscardCurrentTask();

	return taskType;
}

int CWorldMarket::XferMarketTimeToIndex() {
	return XferChinaMarketTimeToIndex(&m_tmMarket);
}

void CWorldMarket::TaskCreateTask(long lCurrentTime) {
	const long lTimeMinute = (lCurrentTime / 100) * 100; // ��ǰСʱ�ͷ���

	while (!IsMarketTaskEmpty()) DiscardCurrentMarketTask();

	// ϵͳ��ʼ�����
	AddTask(WORLD_MARKET_CHECK_SYSTEM_READY__, 1);

	// �г�����
	if (lCurrentTime < gl_systemConfiguration.GetWorldMarketResettingTime()) {
		AddTask(WORLD_MARKET_RESET__, gl_systemConfiguration.GetWorldMarketResettingTime()); // Ĭ��ִ��ʱ��Ϊ��170000
	}

	AddTask(WORLD_MARKET_UPDATE_DB__, lTimeMinute + 40);// ���¹�Ʊ������ݿ������

	AddTask(WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__, lCurrentTime);

	AddTask(WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__, GetNextTime(lTimeMinute + 60, 0, 1, 0)); // �����Ӻ�ʼ���WebSocket

	AddTask(WORLD_MARKET_TIINGO_PROCESS_DAYLINE__, GetNextTime(lTimeMinute, 0, 5, 0)); // ����Ӻ�����������

	AddTask(WORLD_MARKET_CREATE_TASK__, 240000); // �����г������������ÿ����ʱִ��
}

void CWorldMarket::TaskProcessWebSocketData(long lCurrentTime) {
	ASSERT(!IsTimeToResetSystem(lCurrentTime));	// ������ʱ����ϵͳ�������ݿ���Ҫ����װ�룬�ʶ���ʱ��������¡�

	ProcessWebSocketData();
	UpdateFinnhubStockFromWebSocket();

	long lNextTime = GetNextSecond(lCurrentTime);
	if (IsTimeToResetSystem(lNextTime)) lNextTime = GetResetTime() + 501;
	AddTask(WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__, lNextTime);
}

void CWorldMarket::TaskMonitorWebSocket(long lCurrentTime) {
	AddTask(WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__, GetNextTime(lCurrentTime, 0, 1, 0));
	if (!IsSystemReady()) return;

	gl_pFinnhubWebSocket->MonitorWebSocket(GetFinnhubWebSocketSymbols());
	gl_pTiingoCryptoWebSocket->MonitorWebSocket(gl_dataContainerChosenWorldCrypto.GetSymbols());
	gl_pTiingoIEXWebSocket->MonitorWebSocket(gl_dataContainerChosenFinnhubStock.GetSymbols());
	gl_pTiingoForexWebSocket->MonitorWebSocket(gl_dataContainerChosenWorldForex.GetSymbols());
}

void CWorldMarket::TaskResetMarket(long lCurrentTime) {
	// �г�ʱ��ʮ��ʱ����ϵͳ
	ASSERT(!m_fResettingMarket);
	ResetMarket();
	SetSystemReady(false);
	ASSERT(!m_fResettingMarket);

	AddTask(WORLD_MARKET_CHECK_SYSTEM_READY__, lCurrentTime); // ÿ������ϵͳʱ����������ϵͳ״̬�������
}

bool CWorldMarket::TaskUpdateTiingoIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) { // ����tiingo stock profile��finnhub����stock profile����
		gl_runtime.thread_executor()->post([this] {
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateTiingoIndustry();
			gl_UpdateWorldMarketDB.release();
		});
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateSicIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) {// ����tiingo stock profile��finnhub����stock profile����
		gl_runtime.thread_executor()->post([this] {
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateSicIndustry();
			gl_UpdateWorldMarketDB.release();
		});
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateNaicsIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) {// ����tiingo stock profile��finnhub����stock profile����
		gl_runtime.thread_executor()->post([this] {
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateNaicsIndustry();
			gl_UpdateWorldMarketDB.release();
		});
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	��Tiingo�������ݴ������ݿ⣮
//
// �����Ƿ�ִ���˴洢����������Ҫ�����ص�������ʷ����ɾ���������ܹ���ʡ�ڴ��ռ�á�����ʵ�ʴ洢����ʹ���߳�ģʽʵ�֣�
// �ʶ���ִ��ʱ������������̣߳��������߳�ɾ����������ʱ����ͬ�����⡣����ķ������ù����̶߳���ɾ���洢����������ݣ�
// ���̵߳�ɾ������ֻ�ڲ����ù����̣߳�����洢�������ݣ�������·���ִ�С�
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::TaskUpdateTiingoStockDayLineDB() {
	bool fUpdated = false;
	CTiingoStockPtr pTiingoStock = nullptr;
	const size_t symbolSize = gl_dataContainerTiingoStock.Size();

	for (int i = 0; i < symbolSize; i++) {
		pTiingoStock = gl_dataContainerTiingoStock.GetStock(i);
		if (pTiingoStock->IsUpdateDayLineDBAndClearFlag()) {	// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			if (pTiingoStock->GetDayLineSize() > 0) {
				if (pTiingoStock->HaveNewDayLineData()) {
					gl_BackgroundWorkingThread.acquire();
					gl_runtime.thread_executor()->post([pTiingoStock] {
						gl_ThreadStatus.IncreaseBackGroundWorkingThread();
						if (gl_systemConfiguration.IsExitingSystem()) return;// ������������˳�����ֹͣ�洢��
						pTiingoStock->UpdateDayLineDB();
						pTiingoStock->UpdateDayLineStartEndDate();
						pTiingoStock->SetUpdateProfileDB(true);
						pTiingoStock->UnloadDayLine();
						const CString str = pTiingoStock->GetSymbol() + _T("�������ϴ洢���");
						gl_systemMessage.PushDayLineInfoMessage(str);
						gl_ThreadStatus.DecreaseBackGroundWorkingThread();
						gl_BackgroundWorkingThread.release();
					});
					fUpdated = true;
				}
				else pTiingoStock->UnloadDayLine(); // ������ִ�д洢����ʱ�����ﻹҪ����ж���������ݡ���洢���������߳��Ժ��ִ�У��ʶ������ڴ�ͳһִ��ɾ��������
			}
		}
	}

	return (fUpdated);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	��Forex�������ݴ������ݿ⣮
//
// �����Ƿ�ִ���˴洢����������Ҫ�����ص�������ʷ����ɾ���������ܹ���ʡ�ڴ��ռ�á�����ʵ�ʴ洢����ʹ���߳�ģʽʵ�֣�
// �ʶ���ִ��ʱ������������̣߳��������߳�ɾ����������ʱ����ͬ�����⡣����ķ������ù����̶߳���ɾ���洢����������ݣ�
// ���̵߳�ɾ������ֻ�ڲ����ù����̣߳�����洢�������ݣ�������·���ִ�С�
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::TaskUpdateForexDayLineDB() {
	bool fUpdated = false;
	CForexSymbolPtr pSymbol = nullptr;
	const size_t symbolSize = gl_dataFinnhubForexSymbol.Size();

	for (int i = 0; i < symbolSize; i++) {
		pSymbol = gl_dataFinnhubForexSymbol.GetSymbol(i);
		if (pSymbol->IsUpdateDayLineDBAndClearFlag()) {	// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					gl_runtime.thread_executor()->post([pSymbol] {
						gl_ThreadStatus.IncreaseBackGroundWorkingThread();
						gl_UpdateWorldMarketDB.acquire();
						if (!gl_systemConfiguration.IsExitingSystem()) {// ������������˳�����ֹͣ�洢��
							pSymbol->UpdateDayLineDB();
							pSymbol->UpdateDayLineStartEndDate();
							pSymbol->SetUpdateProfileDB(true);
							pSymbol->UnloadDayLine();
							const CString str = pSymbol->GetSymbol() + _T("�������ϴ洢���");
							gl_systemMessage.PushDayLineInfoMessage(str);
						}
						gl_UpdateWorldMarketDB.release();
						gl_ThreadStatus.DecreaseBackGroundWorkingThread();
					});
					fUpdated = true;
				}
				else pSymbol->UnloadDayLine(); // ������ִ�д洢����ʱ�����ﻹҪ����ж���������ݡ���洢���������߳��Ժ��ִ�У��ʶ������ڴ�ͳһִ��ɾ��������
			}
			else { // �������Ϊ�й�Ʊ���룬���˴�����δ����
				pSymbol->SetIPOStatus(_STOCK_NOT_YET_LIST_);
				CString str1 = pSymbol->GetSymbol();
				str1 += _T(" Ϊδ���й�Ʊ����");
				gl_systemMessage.PushDayLineInfoMessage(str1);
			}
		}
	}

	return (fUpdated);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//
// �����Ƿ�ִ���˴洢����������Ҫ�����ص�������ʷ����ɾ���������ܹ���ʡ�ڴ��ռ�á�����ʵ�ʴ洢����ʹ���߳�ģʽʵ�֣�
// �ʶ���ִ��ʱ������������̣߳��������߳�ɾ����������ʱ����ͬ�����⡣����ķ������ù����̶߳���ɾ���洢����������ݣ�
// ���̵߳�ɾ������ֻ�ڲ����ù����̣߳�����洢�������ݣ�������·���ִ�С�
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::TaskUpdateCryptoDayLineDB() {
	CString str;
	bool fUpdated = false;
	CFinnhubCryptoPtr pSymbol = nullptr;
	const size_t symbolSize = gl_dataFinnhubCryptoSymbol.Size();

	for (int i = 0; i < symbolSize; ++i) {
		pSymbol = gl_dataFinnhubCryptoSymbol.GetSymbol(i);
		if (pSymbol->IsUpdateDayLineDBAndClearFlag()) {	// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					gl_runtime.thread_executor()->post([pSymbol] {
						gl_ThreadStatus.IncreaseBackGroundWorkingThread();
						gl_UpdateWorldMarketDB.acquire();
						if (!gl_systemConfiguration.IsExitingSystem()) { // ������������˳�����ֹͣ�洢��
							pSymbol->UpdateDayLineDB();
							pSymbol->UpdateDayLineStartEndDate();
							pSymbol->SetUpdateProfileDB(true);
							pSymbol->UnloadDayLine();
							const CString str2 = pSymbol->GetSymbol() + _T("�������ϴ洢���");
							gl_systemMessage.PushDayLineInfoMessage(str2);
						}
						gl_UpdateWorldMarketDB.release();
						gl_ThreadStatus.DecreaseBackGroundWorkingThread();
					});
					fUpdated = true;
				}
				else pSymbol->UnloadDayLine(); // ������ִ�д洢����ʱ�����ﻹҪ����ж���������ݡ���洢���������߳��Ժ��ִ�У��ʶ������ڴ�ͳһִ��ɾ��������
			}
			else {// �������Ϊ�й�Ʊ���룬���˴�����δ���У������������й�Ʊ
				if (pSymbol->GetDayLineEndDate() > 19800101) {// ������
					pSymbol->SetIPOStatus(_STOCK_DELISTED_);
					pSymbol->SetUpdateProfileDB(true);
				}
				else {// �������Ϊ�й�Ʊ���룬���˴�����δ����
					pSymbol->SetIPOStatus(_STOCK_NOT_YET_LIST_);
					CString str1 = pSymbol->GetSymbol();
					str1 += _T(" Ϊδ���й�Ʊ����");
					gl_systemMessage.PushDayLineInfoMessage(str1);
				}
			}
		}
	}
	return fUpdated;
}

void CWorldMarket::TaskCreateTiingoTradeDayDayLine(long lCurrentTime) {
	if (gl_systemConfiguration.GetTiingoIEXTopOfBookUpdateDate() < gl_pWorldMarket->GetCurrentTradeDate()
		&& !gl_pTiingoDataSource->IsUpdateIEXTopOfBook()
		&& lCurrentTime > 180500) { // ��ǰ������δ�����IEX���Ѿ����յ��������ѹ�����ʱ�䣿
		gl_runtime.thread_executor()->post([] {
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerTiingoStock.BuildDayLine(gl_pWorldMarket->GetCurrentTradeDate());
			gl_UpdateWorldMarketDB.release();
		});
	}
	else {
		AddTask(WORLD_MARKET_TIINGO_BUILD_TODAY_STOCK_DAYLINE__, GetNextTime(lCurrentTime, 0, 1, 0)); // һ���Ӻ�ִ����һ��
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����tiingo�������ݡ�
// Ŀǰ���õķ�����δ�Ż������´������й�Ʊ��ʱ��ܳ����ʶ�������ʱ��ʹ�ô˺�����
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CWorldMarket::TaskProcessTiingoDayLine(long lCurrentTime) {
	if (gl_systemConfiguration.GetTiingoStockDayLineProcessedDate() >= GetCurrentTradeDate()) return; // �Ѹ�����52WeekHighLow,�����Զ����¡�
	if (gl_systemConfiguration.IsPaidTypeTiingoAccount()) {
		if (!gl_pTiingoDataSource->IsUpdateDayLine()) { // �������������ݺ󷽿ɴ���
			gl_runtime.thread_executor()->post([] {
				gl_dataContainerTiingoStock.TaskProcessDayLine();
			});
		}
		else {
			AddTask(WORLD_MARKET_TIINGO_PROCESS_DAYLINE__, GetNextTime(lCurrentTime, 0, 1, 0)); // һ���Ӻ�ִ����һ��
		}
	}
	else {
		if (!gl_pTiingoDataSource->IsUpdateIEXTopOfBook()) { // ������IEX�������ݺ󷽿ɴ���
			gl_runtime.thread_executor()->post([] {
				gl_dataContainerTiingoStock.TaskProcessDayLine();
			});
		}
		else {
			AddTask(WORLD_MARKET_TIINGO_PROCESS_DAYLINE__, GetNextTime(lCurrentTime, 0, 1, 0)); // һ���Ӻ�ִ����һ��
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CWorldMarket::TaskTiingoCalculate(long lCurrentTime) {
	gl_runtime.thread_executor()->post([] {
		gl_systemMessage.PushInnerSystemInformationMessage("calculateing 52 week low");
		gl_dataContainerTiingoStock.TaskCalculate();
		gl_systemMessage.PushInnerSystemInformationMessage("52 week low Calculated");
	});
}

void CWorldMarket::TaskDeleteDelistedStock() {
	gl_runtime.thread_executor()->post([] {
		DeleteTiingoDelistedStock();
	});
}

void CWorldMarket::TaskPerSecond(long lCurrentTime) {
	static int m_iCountDownPerMinute = 59;
	static int m_iCountDownPerHour = 3599;
	static int m_iCountDownPerDay = 3500 * 24 - 1;

	if (--m_iCountDownPerMinute < 0) { // ÿ����һ��
		m_iCountDownPerMinute = 59;
	}
	if (--m_iCountDownPerHour < 0) { // ÿСʱһ��
		ASSERT(gl_systemConfiguration.GetTiingoHourLyRequestLimit() > 100);
		m_iCountDownPerHour = 3599;
	}
	if (--m_iCountDownPerDay < 0) { // ÿ��һ��
		ASSERT(gl_systemConfiguration.GetTiingoDailyRequestLimit() > 10000);
		m_iCountDownPerDay = 3600 * 24 - 1;
	}
}

bool CWorldMarket::UpdateEPSSurpriseDB() {
	CString str;
	const size_t stockSize = gl_dataContainerFinnhubStock.Size();

	CFinnhubStockPtr pStock = nullptr;
	for (long l = 0; l < stockSize; ++l) {
		pStock = gl_dataContainerFinnhubStock.GetStock(l);
		if (pStock->IsUpdateEPSSurpriseDBAndClearFlag()) {// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			pStock->UpdateEPSSurpriseDB();
		}
		if (gl_systemConfiguration.IsExitingSystem()) break; // ������������˳�����ֹͣ�洢��
	}

	return (true);
}

void CWorldMarket::UpdateSECFilingsDB() {
	CString str;
	const size_t stockSize = gl_dataContainerFinnhubStock.Size();

	CFinnhubStockPtr pStock = nullptr;
	for (long l = 0; l < stockSize; ++l) {
		pStock = gl_dataContainerFinnhubStock.GetStock(l);
		if (pStock->IsUpdateSECFilingsDBAndClearFlag()) {// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			ASSERT(pStock->UpdateSECFilingsDB());
		}
		if (gl_systemConfiguration.IsExitingSystem()) break; // ������������˳�����ֹͣ�洢��
	}
}

bool CWorldMarket::TaskCheckMarketReady(long lCurrentTime) {
	if (!IsSystemReady()) {
		if (!gl_pFinnhubDataSource->IsUpdateSymbol() && !gl_pFinnhubDataSource->IsUpdateForexExchange() && !gl_pFinnhubDataSource->IsUpdateForexSymbol()
			&& !gl_pFinnhubDataSource->IsUpdateCryptoExchange() && !gl_pFinnhubDataSource->IsUpdateCryptoSymbol()) {
			const CString str = "�����г���ʼ�����";
			gl_systemMessage.PushInformationMessage(str);
			SetSystemReady(true);
		}
	}
	if (!IsSystemReady()) {
		AddTask(WORLD_MARKET_CHECK_SYSTEM_READY__, GetNextSecond(lCurrentTime));
	}
	return IsSystemReady();
}

void CWorldMarket::TaskUpdateWorldMarketDB(long lCurrentTime) {
	if (gl_dataContainerFinnhubCountry.GetLastTotalCountry() < gl_dataContainerFinnhubCountry.GetTotalCountry()) { // ��������
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerFinnhubCountry.UpdateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubForexExchange.IsNeedUpdate()) { // ForexExchange
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerFinnhubForexExchange.UpdateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataFinnhubForexSymbol.IsUpdateProfileDB()) { // Forex symbol
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataFinnhubForexSymbol.UpdateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubCryptoExchange.IsNeedUpdate()) { // Crypto Exchange
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerFinnhubCryptoExchange.UpdateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataFinnhubCryptoSymbol.IsUpdateProfileDB()) { // crypto symbol
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataFinnhubCryptoSymbol.UpdateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateInsiderTransactionDB()) { // Insider Transaction
		gl_runtime.thread_executor()->post([this] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateInsiderTransactionDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateInsiderSentimentDB()) { // Insider Sentiment
		gl_runtime.thread_executor()->post([this] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateInsiderSentimentDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateDayLineDB()) { // stock dayLine
		gl_runtime.thread_executor()->post([this] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateFinnhubStockDayLineDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubEconomicCalendar.IsUpdateDB()) { // Economic Calendar
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerFinnhubEconomicCalendar.UpdateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateCompanyNewsDB()) { // Company News
		gl_runtime.thread_executor()->post([this] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateCompanyNewsDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateBasicFinancialDB()) { // Basic financial
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerFinnhubStock.UpdateBasicFinancialDB(); // ������ܷ�ʱ��ԭ����顣Ŀǰ�Ȳ�ʹ�ô˸�����
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateEPSSurpriseDB()) { // stock EPS surprise
		gl_runtime.thread_executor()->post([this] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateEPSSurpriseDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateSECFilingsDB()) { // stock EPS surprise
		gl_runtime.thread_executor()->post([this] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateSECFilingsDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}

	// Tiingo����
	if (gl_dataContainerTiingoStock.IsUpdateProfileDB()) { // Tiingo Stock
		static int s_counter = 0;
		if (s_counter > 30) {
			gl_runtime.thread_executor()->post([] {
				gl_ThreadStatus.IncreaseBackGroundWorkingThread();
				gl_UpdateWorldMarketDB.acquire();
				gl_dataContainerTiingoStock.UpdateDB();
				gl_UpdateWorldMarketDB.release();
				gl_ThreadStatus.DecreaseBackGroundWorkingThread();
			});
			s_counter = 0;
		}
		s_counter++;
	}
	if (gl_dataContainerTiingoCryptoSymbol.IsUpdateProfileDB()) { // Tiingo crypto symbol
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerTiingoCryptoSymbol.UpdateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerTiingoFundamentalDefinition.IsUpdateDB()) { // Tiingo crypto symbol
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerTiingoFundamentalDefinition.UpdateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
	if (gl_dataContainerTiingoStock.IsUpdateFinancialStateDB()) {
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerTiingoStock.UpdateFinancialStateDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}

	if (gl_dataContainerTiingoStock.IsUpdateDayLineDB()) {
		gl_runtime.thread_executor()->post([this] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			this->TaskUpdateTiingoStockDayLineDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}

	TaskUpdateForexDayLineDB(); // ��������ڲ��������ɹ����߳�
	TaskUpdateCryptoDayLineDB(); // ��������ڲ��������ɹ����߳�

	if (!gl_pFinnhubDataSource->IsUpdateSymbol() && gl_dataContainerFinnhubStock.IsUpdateProfileDB()) { // stock profile
		static int s_counter2 = 0;
		if (s_counter2 > 30) {
			gl_runtime.thread_executor()->post([] {
				gl_ThreadStatus.IncreaseBackGroundWorkingThread();
				gl_UpdateWorldMarketDB.acquire();
				gl_dataContainerFinnhubStock.UpdateProfileDB();
				gl_UpdateWorldMarketDB.release();
				gl_ThreadStatus.DecreaseBackGroundWorkingThread();
			});
			s_counter2 = 0;
		}
		s_counter2++;
	}

	if (gl_finnhubInaccessibleExchange.IsUpdateDB()) { // ���½�ֹ����֤ȯ����������
		gl_ThreadStatus.IncreaseBackGroundWorkingThread();
		gl_runtime.thread_executor()->post([] {
			gl_finnhubInaccessibleExchange.UpdateDB();
			gl_finnhubInaccessibleExchange.SetUpdateDB(false);
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}

	if (gl_tiingoInaccessibleStock.IsUpdateDB()) { // ���½�ֹ����֤ȯ����������
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_tiingoInaccessibleStock.UpdateDB();
			gl_tiingoInaccessibleStock.SetUpdateDB(false);
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}

	long lNextTime = GetNextTime(lCurrentTime, 0, 1, 0);
	if (IsTimeToResetSystem(lNextTime)) lNextTime = GetResetTime() + 510;
	ASSERT(!IsTimeToResetSystem(lNextTime));// ����ϵͳʱ�����ݿ���Ҫ����װ�룬�ʶ���ʱ������������ݿ⡣
	AddTask(WORLD_MARKET_UPDATE_DB__, lNextTime); // ÿ����Ӹ���һ��
}

void CWorldMarket::TaskUpdateTiingoStock52WeekHighLowDB(long lCurrentTime) {
	if (gl_dataContainerTiingoStock.IsUpdate52WeekHighLowDB()) { // stock dayLine
		gl_runtime.thread_executor()->post([] {
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerTiingoStock.TaskUpdate52WeekHighLowDB();
			gl_UpdateWorldMarketDB.release();
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		});
	}
}

bool CWorldMarket::UpdateToken() {
	ASSERT(gl_systemConfiguration.IsInitialized());

	if (gl_systemConfiguration.GetFinnhubToken().GetLength() > 5) {
		gl_pFinnhubDataSource->SetInquiryToken(gl_systemConfiguration.GetFinnhubToken());
	}
	else {
		gl_systemMessage.PushInformationMessage(_T("Finnhub Token Needed"));
	}
	if (gl_systemConfiguration.GetTiingoToken().GetLength() > 5) {
		gl_pTiingoDataSource->SetInquiryToken(gl_systemConfiguration.GetTiingoToken());
	}
	else {
		gl_systemMessage.PushInformationMessage(_T("Tiingo Token Needed"));
	}
	if (gl_systemConfiguration.GetQuandlToken().GetLength() > 5) {
		gl_pQuandlDataSource->SetInquiryToken(gl_systemConfiguration.GetQuandlToken());
	}
	else {
		gl_systemMessage.PushInformationMessage(_T("Quandl Token Needed"));
	}

	return true;
}

bool CWorldMarket::UpdateFinnhubStockDayLineDB() {
	for (long i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(i);
		pStock->UpdateDayLineDB();
		if (gl_systemConfiguration.IsExitingSystem()) break; // ������������˳�����ֹͣ�洢��
	}
	return true;
}

bool CWorldMarket::UpdateCompanyNewsDB() {
	for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
		const auto pStock = gl_dataContainerFinnhubStock.GetStock(l);
		if (pStock->IsUpdateCompanyNewsDBAndClearFlag()) {// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			pStock->UpdateCompanyNewsDB();
		}
		if (gl_systemConfiguration.IsExitingSystem()) break; // ������������˳�����ֹͣ�洢��
	}

	return (true);
}

bool CWorldMarket::UpdateInsiderTransactionDB() {
	int iCounter = 0;
	for (long i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(i);
		if (pStock->IsUpdateInsiderTransactionDBAndClearFlag()) {
			if (pStock->HaveInsiderTransaction()) {
				iCounter++;
				pStock->UpdateInsiderTransactionDB();
			}
		}
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}
	return true;
}

bool CWorldMarket::UpdateInsiderSentimentDB() {
	for (long i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(i);
		if (pStock->IsUpdateInsiderSentimentDBAndClearFlag()) {
			if (pStock->HaveInsiderSentiment()) {
				pStock->UpdateInsiderSentimentDB();
			}
		}
		if (gl_systemConfiguration.IsExitingSystem()) break; // ������������˳�����ֹͣ�洢��
	}
	return true;
}

bool CWorldMarket::UpdateTiingoIndustry() {
	// not implemented
	return false;
}

bool CWorldMarket::UpdateSicIndustry() {
	// not implemented
	return false;
}

bool CWorldMarket::UpdateNaicsIndustry() {
	// not implemented
	return false;
}

void CWorldMarket::RebuildEPSSurprise() {
	gl_dataContainerFinnhubStock.ResetEPSSurprise();
	gl_pFinnhubDataSource->SetUpdateEPSSurprise(true);
}

void CWorldMarket::RebuildPeer() {
	gl_dataContainerFinnhubStock.ResetPeer();
	gl_pFinnhubDataSource->SetUpdatePeer(true);
}

void CWorldMarket::RebuildBasicFinancial() {
	gl_dataContainerFinnhubStock.ResetBasicFinancial();
	gl_pFinnhubDataSource->SetUpdateStockBasicFinancial(true);
}

void CWorldMarket::RebuildStockDayLineDB() {
	gl_dataContainerFinnhubStock.ResetDayLine();
	gl_pFinnhubDataSource->SetUpdateStockProfile(true);
}

void CWorldMarket::UpdateStockDayLineStartEndDate() {
	try {
		const CString strFilterPrefix = _T("[Symbol] = '");
		CSetFinnhubStockDayLine setFinnhubStockDayLine;

		for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
			const auto pStock = gl_dataContainerFinnhubStock.GetStock(l);
			setFinnhubStockDayLine.m_strFilter = strFilterPrefix + pStock->GetSymbol() + _T("'");
			setFinnhubStockDayLine.m_strSort = _T("[Date]");
			setFinnhubStockDayLine.Open();
			if (!setFinnhubStockDayLine.IsEOF()) {
				if (setFinnhubStockDayLine.m_Date < pStock->GetDayLineStartDate()) {
					pStock->SetDayLineStartDate(setFinnhubStockDayLine.m_Date);
					pStock->SetUpdateProfileDB(true);
				}
				setFinnhubStockDayLine.MoveLast();
				if (setFinnhubStockDayLine.m_Date > pStock->GetDayLineEndDate()) {
					pStock->SetDayLineEndDate(setFinnhubStockDayLine.m_Date);
					pStock->SetUpdateProfileDB(true);
				}
			}
			setFinnhubStockDayLine.Close();
		}
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

/// <summary>
/// Finnhub WebSocket������˻����ֻ�ܷ���50��֤ȯ������
/// </summary>
vectorString CWorldMarket::GetFinnhubWebSocketSymbols() {
	vectorString vSymbol;

	vectorString vSymbolTemp = gl_dataContainerChosenFinnhubStock.GetSymbols();
	for (auto symbol : vSymbolTemp) {
		vSymbol.push_back(symbol);
	}

	vSymbolTemp = gl_dataContainerChosenWorldForex.GetSymbols();
	for (auto symbol : vSymbolTemp) {
		vSymbol.push_back(symbol);
	}

	vSymbolTemp = gl_dataContainerChosenWorldCrypto.GetSymbols();
	for (auto symbol : vSymbolTemp) {
		vSymbol.push_back(symbol);
	}

	return vSymbol;
}

/// <summary>
/// // ֹͣWebSocket���˺����ȴ���ֹͣ�󷽷��ء���ϵͳ�˳�ǰ��׼��������
/// ����ֱ�ӵ���Disconnect(),
/// </summary>
void CWorldMarket::DisconnectAllWebSocket() {
	//������ֻ��ϵͳ�˳�ʱ����
	//ASSERT(gl_systemConfiguration.IsExitingSystem()); 
	if (gl_systemConfiguration.IsUsingFinnhubWebSocket()) gl_pFinnhubWebSocket->Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoIEXWebSocket()) gl_pTiingoIEXWebSocket->Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoCryptoWebSocket()) gl_pTiingoCryptoWebSocket->Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoForexWebSocket()) gl_pTiingoForexWebSocket->Disconnect();
}

void CWorldMarket::ProcessWebSocketData() {
	if (gl_pFinnhubWebSocket->DataSize() > 0) ProcessFinnhubWebSocketData();
	if (gl_pTiingoIEXWebSocket->DataSize() > 0) ProcessTiingoIEXWebSocketData();
	if (gl_pTiingoCryptoWebSocket->DataSize() > 0) ProcessTiingoCryptoWebSocketData();
	if (gl_pTiingoForexWebSocket->DataSize() > 0) ProcessTiingoForexWebSocketData();
}

void CWorldMarket::ProcessFinnhubWebSocketData() {
	const auto total = gl_pFinnhubWebSocket->DataSize();
	size_t iTotalDataSize = 0;
	for (size_t i = 0; i < total; i++) {
		const auto pString = gl_pFinnhubWebSocket->PopData();
		CString strMessage = _T("Finnhub: ");
		strMessage += pString->c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_pFinnhubWebSocket->ParseFinnhubWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedFinnhubWebSocket(iTotalDataSize);
}

void CWorldMarket::ProcessTiingoIEXWebSocketData() {
	const auto total = gl_pTiingoIEXWebSocket->DataSize();
	size_t iTotalDataSize = 0;
	for (size_t i = 0; i < total; i++) {
		const auto pString = gl_pTiingoIEXWebSocket->PopData();
		CString strMessage = _T("Tiingo IEX: ");
		strMessage += pString->c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_pTiingoIEXWebSocket->ParseTiingoIEXWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoIEXWebSocket(iTotalDataSize);
}

void CWorldMarket::ProcessTiingoCryptoWebSocketData() {
	const auto total = gl_pTiingoCryptoWebSocket->DataSize();

	size_t iTotalDataSize = 0;
	for (size_t i = 0; i < total; i++) {
		const auto pString = gl_pTiingoCryptoWebSocket->PopData();
		CString strMessage = _T("Tiingo Crypto: ");
		strMessage += pString->c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_pTiingoCryptoWebSocket->ParseTiingoCryptoWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoCryptoWebSocket(iTotalDataSize);
}

void CWorldMarket::ProcessTiingoForexWebSocketData() {
	const auto total = gl_pTiingoForexWebSocket->DataSize();
	size_t iTotalDataSize = 0;
	for (size_t i = 0; i < total; i++) {
		const auto pString = gl_pTiingoForexWebSocket->PopData();
		CString strMessage = _T("Tiingo Forex: ");
		strMessage += pString->c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_pTiingoForexWebSocket->ParseTiingoForexWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoForexWebSocket(iTotalDataSize);
}

/// <summary>
/// ����֤ȯ��ʵʱ��Ϣ
/// Ŀǰֻʹ��IEX��Crypto�����ݣ���������������
/// </summary>
/// <returns></returns>
void CWorldMarket::UpdateFinnhubStockFromWebSocket() {
	auto total = gl_SystemData.GetTiingoIEXSocketSize();
	for (size_t i = 0; i < total; i++) {
		const CTiingoIEXSocketPtr pIEXData = gl_SystemData.PopTiingoIEXSocket();
		UpdateFinnhubStockFromTiingoIEXSocket(pIEXData);
		gl_systemMessage.SetCurrentTiingoWebSocketIEX(pIEXData->m_sSymbol.c_str());
	}

	total = gl_SystemData.GetTiingoCryptoSocketSize();
	for (size_t i = 0; i < total; i++) {
		const CTiingoCryptoSocketPtr pCryptoData = gl_SystemData.PopTiingoCryptoSocket();
		gl_systemMessage.SetCurrentTiingoWebSocketCrypto(pCryptoData->m_sSymbol.c_str());
	}

	total = gl_SystemData.GetTiingoForexSocketSize();
	for (size_t i = 0; i < total; i++) {
		const CTiingoForexSocketPtr pForexData = gl_SystemData.PopTiingoForexSocket();
		gl_systemMessage.SetCurrentTiingoWebSocketForex(pForexData->m_sSymbol.c_str());
	}

	total = gl_SystemData.GetFinnhubSocketSize();
	for (size_t i = 0; i < total; i++) {
		const CFinnhubSocketPtr pFinnhubData = gl_SystemData.PopFinnhubSocket();
		UpdateFinnhubStockFromFinnhubSocket(pFinnhubData);
		gl_systemMessage.SetCurrentFinnhubWebSocketStake(pFinnhubData->m_sSymbol.c_str());
	}
}

void CWorldMarket::UpdateFinnhubStockFromTiingoIEXSocket(const CTiingoIEXSocketPtr& pTiingoIEXbData) {
	if (gl_dataContainerFinnhubStock.IsSymbol(pTiingoIEXbData->m_sSymbol.c_str())) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(pTiingoIEXbData->m_sSymbol.c_str());
		pStock->SetActive(true);
		switch (pTiingoIEXbData->m_chMessageType) {
		case 'T':
			pStock->SetNew(pTiingoIEXbData->m_dLastPrice * 1000);
			break;
		case 'Q':
			// do nothing
			break;
		default:
			// do nothing
			break;
		}
	}
}

void CWorldMarket::UpdateFinnhubStockFromFinnhubSocket(const CFinnhubSocketPtr& pFinnhubData) {
	if (gl_dataContainerFinnhubStock.IsSymbol(pFinnhubData->m_sSymbol.c_str())) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(pFinnhubData->m_sSymbol.c_str());
		pStock->SetActive(true);
		pStock->SetNew(pFinnhubData->m_dLastPrice * 1000);
	}
}

void CWorldMarket::UpdateMarketStatus(const CMarketStatussPtr& pv) const {
	for (auto p : *pv) {
		m_pvMarketStatus->push_back(p);
	}
}

void CWorldMarket::UpdateMarketHoliday(const CMarketHolidaysPtr& pv) const {
	for (auto p : *pv) {
		m_pvMarketHoliday->push_back(p);
	}
}

void CWorldMarket::DeleteTiingoDelistedStock() {
	while (gl_dataContainerTiingoDelistedSymbol.Size() > 0) {
		auto pTiingoDelistedStock = gl_dataContainerTiingoDelistedSymbol.GetStock(0);
		DeleteTiingoDayLine(pTiingoDelistedStock);
		DeleteTiingoFinancialStatement(pTiingoDelistedStock);
		ASSERT(gl_dataContainerTiingoStock.IsSymbol(pTiingoDelistedStock));
		gl_dataContainerTiingoStock.Delete(pTiingoDelistedStock->GetSymbol());
		gl_dataContainerTiingoDelistedSymbol.Delete(pTiingoDelistedStock);
	}
}

void CWorldMarket::DeleteTiingoDayLine(const CTiingoStockPtr& pStock) {
	CSetTiingoStockDayLine setDayLine;
	setDayLine.m_strFilter = _T("[Symbol] = '");
	setDayLine.m_strFilter += pStock->GetSymbol() + _T("'");

	setDayLine.Open();
	setDayLine.m_pDatabase->BeginTrans();
	while (!setDayLine.IsEOF()) {
		setDayLine.Delete();
		setDayLine.MoveNext();
	}
	setDayLine.m_pDatabase->CommitTrans();
	setDayLine.Close();
}

void CWorldMarket::DeleteTiingoFinancialStatement(const CTiingoStockPtr& pStock) {
	CSetTiingoCompanyFinancialState setDayLine;
	setDayLine.m_strFilter = _T("[Symbol] = '");
	setDayLine.m_strFilter += pStock->GetSymbol() + _T("'");

	setDayLine.Open();
	setDayLine.m_pDatabase->BeginTrans();
	while (!setDayLine.IsEOF()) {
		setDayLine.Delete();
		setDayLine.MoveNext();
	}
	setDayLine.m_pDatabase->CommitTrans();
	setDayLine.Close();
}
