#include"pch.h"

#include"systemData.h"

#include "WorldMarket.h"

#include <concurrencpp/executors/thread_pool_executor.h>

#include"thread.h"

#include"FinnhubInaccessibleExchange.h"

#include"FinnhubDataSource.h"

#include"FinnhubWebSocket.h"
#include"TiingoIEXWebSocket.h"
#include"TiingoForexWebSocket.h"
#include"TiingoCryptoWebSocket.h"

#include"SetWorldStockDayLine.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXUserAgent.h>

#include "InfoReport.h"
#include "QuandlDataSource.h"
#include "TiingoDataSource.h"
#include "TimeConvert.h"
using std::thread;
using std::make_shared;

CWorldMarket::CWorldMarket() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	if (static int siInstance = 0; ++siInstance > 1) {
		TRACE("CWorldMarket�г�����ֻ�������һ��ʵ��\n");
	}

	// ���裨Ҳ�޷���ÿ�ո��µı�����������
	m_lCurrentUpdateEPSSurprisePos = 0;
	m_lCurrentUpdateDayLinePos = 0; // ����֤ȯ���������ж�ʮ��֮�࣬�޷���һ��֮�ڸ����꣬�ʲ������ô�������
	m_bFinnhubWebSiteAccessible = true;

	m_strMarketId = _T("�����г�");
	m_lMarketTimeZone = 4 * 3600; // ��������ʹ��������׼ʱ��, GMT + 4
	CalculateTime();

	m_pvMarketStatus = make_shared<vector<CMarketStatusPtr>>();
	m_pvMarketHoliday = make_shared<vector<CMarketHolidayPtr>>();

	Reset();

	AddTask(CREATE_TASK__, 1);
}

CWorldMarket::~CWorldMarket() {
	PreparingExitMarket();
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

void CWorldMarket::ResetTiingo() {
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
	gl_dataContainerTiingoCryptoSymbol.Reset();

	gl_dataContainerChosenWorldStock.Reset();
	gl_dataContainerChosenWorldForex.Reset();
	gl_dataContainerChosenWorldCrypto.Reset();
}

void CWorldMarket::ResetMarket() {
	gl_bWorldMarketResetting = true;
	Reset();

	UpdateToken();

	gl_dataContainerFinnhubStockExchange.LoadDB(); // װ�����罻������Ϣ
	gl_dataContainerFinnhubCountry.LoadDB();
	gl_dataContainerFinnhubStock.LoadDB();
	gl_dataContainerChosenWorldStock.LoadDB();
	gl_dataContainerFinnhubForexExchange.LoadDB();
	gl_dataFinnhubForexSymbol.LoadDB();
	gl_dataContainerChosenWorldForex.LoadDB();
	gl_dataContainerFinnhubCryptoExchange.LoadDB();
	gl_dataFinnhubCryptoSymbol.LoadDB();
	gl_dataContainerChosenWorldCrypto.LoadDB();
	gl_dataContainerFinnhubEconomicCalendar.LoadDB();

	gl_dataContainerTiingoStock.LoadDB();
	gl_dataContainerTiingoCryptoSymbol.LoadDB();

	for (const auto& pDataSource : m_vDataSource) {
		pDataSource->Reset();
	}

	CString str = _T("����World Market��������׼ʱ�䣺");
	str += GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(str);

	gl_bWorldMarketResetting = false;
}

void CWorldMarket::PreparingExitMarket() {
	//ASSERT(gl_systemConfiguration.IsExitingSystem());
	DisconnectAllWebSocket();
}

bool CWorldMarket::ProcessTask(long lCurrentTime) {
	if (IsMarketTaskEmpty()) return false;
	const auto pTask = GetMarketTask();
	if (lCurrentTime >= pTask->GetTime()) {
		DiscardCurrentMarketTask();
		switch (pTask->GetType()) {
		case CREATE_TASK__: // ������������
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
		case WORLD_MARKET_START_ALL_WEB_SOCKET__:
			TaskStartAllWebSocket(lCurrentTime);
			break;
		case WORLD_MARKET_STOP_ALL_WEB_SOCKET__:
			TaskStopAllWebSocket(lCurrentTime);
			break;
		case WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__:
			TaskProcessWebSocketData(lCurrentTime);
			break;
		default:
			ASSERT(0); // �Ƿ�����
			break;
		}
		return true;
	}
	return false;
}

void CWorldMarket::TaskCreateTask(long lCurrentTime) {
	const long lTimeMinute = (lCurrentTime / 100) * 100; // ��ǰСʱ�ͷ���

	while (!IsMarketTaskEmpty()) DiscardCurrentMarketTask();

	// ϵͳ��ʼ�����
	AddTask(WORLD_MARKET_CHECK_SYSTEM_READY__, 1);

	// �г�����
	if (lCurrentTime < 170000) {
		AddTask(WORLD_MARKET_RESET__, 170000); // ִ��ʱ��Ϊ��170000
	}

	AddTask(WORLD_MARKET_UPDATE_DB__, lTimeMinute + 40);// ���¹�Ʊ������ݿ������

	AddTask(WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__, lCurrentTime);

	AddTask(WORLD_MARKET_STOP_ALL_WEB_SOCKET__, GetNextTime(lTimeMinute + 30, 0, 1, 0)); // ֹͣWebSocket����Ҫ��������WebSocket����
	AddTask(WORLD_MARKET_START_ALL_WEB_SOCKET__, GetNextTime(lTimeMinute + 60, 0, 1, 0));

	AddTask(CREATE_TASK__, 240000); // �����г������������ÿ����ʱִ��
}

void CWorldMarket::TaskProcessWebSocketData(long lCurrentTime) {
	ASSERT(!IsTimeToResetSystem(lCurrentTime));	// ������ʱ����ϵͳ�������ݿ���Ҫ����װ�룬�ʶ���ʱ��������¡�

	ProcessWebSocketData();
	UpdateWorldStockFromWebSocket();

	long lNextTime = GetNextSecond(lCurrentTime);
	if (IsTimeToResetSystem(lNextTime)) lNextTime = 170501;
	AddTask(WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__, lNextTime);
}

void CWorldMarket::TaskStartAllWebSocket(long lCurrentTime) {
	StartAllWebSocket();
	AddTask(WORLD_MARKET_START_ALL_WEB_SOCKET__, GetNextTime(lCurrentTime, 0, 1, 0));
}

void CWorldMarket::TaskStopAllWebSocket(long lCurrentTime) {
	StopAllWebSocketIfTimeOut();
	AddTask(WORLD_MARKET_STOP_ALL_WEB_SOCKET__, GetNextTime(lCurrentTime, 0, 1, 0));
}

void CWorldMarket::TaskResetMarket(long lCurrentTime) {
	// �г�ʱ��ʮ��ʱ����ϵͳ
	SetResetMarket(true); // ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
	SetSystemReady(false);

	AddTask(WORLD_MARKET_CHECK_SYSTEM_READY__, lCurrentTime); // ÿ������ϵͳʱ����������ϵͳ״̬�������
}

bool CWorldMarket::CreateThreadUpdateTiingoIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) { // ����tiingo stock profile��finnhub����stock profile����
		thread thread1(ThreadUpdateTiingoIndustry, gl_pWorldMarket);
		thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
		return true;
	}
	return false;
}

bool CWorldMarket::CreateThreadUpdateSICIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) {// ����tiingo stock profile��finnhub����stock profile����
		thread thread1(ThreadUpdateSICIndustry, gl_pWorldMarket);
		thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
		return true;
	}
	return false;
}

bool CWorldMarket::CreateThreadUpdateNaicsIndustry() {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) {// ����tiingo stock profile��finnhub����stock profile����
		thread thread1(ThreadUpdateNaicsIndustry, gl_pWorldMarket);
		thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	��Forex�������ݴ������ݿ⣮
//  �˺����ɹ����߳�ThreadForexDayLineSaveProc���ã�������Ҫʹ��ȫ�ֱ�����(Ŀǰʹ�����̵߳���֮��������ͬ������ĳ��֣�
//
// �����Ƿ�ִ���˴洢����������Ҫ�����ص�������ʷ����ɾ���������ܹ���ʡ�ڴ��ռ�á�����ʵ�ʴ洢����ʹ���߳�ģʽʵ�֣�
// �ʶ���ִ��ʱ������������̣߳��������߳�ɾ����������ʱ����ͬ�����⡣����ķ������ù����̶߳���ɾ���洢����������ݣ�
// ���̵߳�ɾ������ֻ�ڲ����ù����̣߳�����洢�������ݣ�������·���ִ�С�
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::UpdateForexDayLineDB() {
	CString str;
	bool fUpdated = false;
	CForexSymbolPtr pSymbol = nullptr;
	const size_t symbolSize = gl_dataFinnhubForexSymbol.Size();

	for (int i = 0; i < symbolSize; i++) {
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
		pSymbol = gl_dataFinnhubForexSymbol.GetSymbol(i);
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) {	// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					gl_runtime.background_executor()->post([pSymbol] {
						gl_UpdateWorldMarketDB.acquire();
						pSymbol->UpdateDayLineDB();
						pSymbol->UpdateDayLineStartEndDate();
						pSymbol->SetUpdateProfileDB(true);
						pSymbol->UnloadDayLine();
						const CString str = pSymbol->GetSymbol() + _T("�������ϴ洢���");
						gl_systemMessage.PushDayLineInfoMessage(str);
						gl_UpdateWorldMarketDB.release();
					});
					fUpdated = true;
					//TRACE("����%s��������\n", pSymbol->GetSymbol().GetBuffer());
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
//	��Crypto�������ݴ������ݿ⣮
//  �˺����ɹ����߳�ThreadCryptoDayLineSaveProc���ã�������Ҫʹ��ȫ�ֱ�����(Ŀǰʹ�����̵߳���֮��������ͬ������ĳ��֣�
//
// �����Ƿ�ִ���˴洢����������Ҫ�����ص�������ʷ����ɾ���������ܹ���ʡ�ڴ��ռ�á�����ʵ�ʴ洢����ʹ���߳�ģʽʵ�֣�
// �ʶ���ִ��ʱ������������̣߳��������߳�ɾ����������ʱ����ͬ�����⡣����ķ������ù����̶߳���ɾ���洢����������ݣ�
// ���̵߳�ɾ������ֻ�ڲ����ù����̣߳�����洢�������ݣ�������·���ִ�С�
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::UpdateCryptoDayLineDB() {
	CString str;
	bool fUpdated = false;
	CFinnhubCryptoSymbolPtr pSymbol = nullptr;
	const size_t symbolSize = gl_dataFinnhubCryptoSymbol.Size();

	for (int i = 0; i < symbolSize; ++i) {
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
		pSymbol = gl_dataFinnhubCryptoSymbol.GetSymbol(i);
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) {	// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					gl_runtime.background_executor()->post([pSymbol] {
						gl_UpdateWorldMarketDB.acquire();
						pSymbol->UpdateDayLineDB();
						pSymbol->UpdateDayLineStartEndDate();
						pSymbol->SetUpdateProfileDB(true);
						pSymbol->UnloadDayLine();
						const CString str = pSymbol->GetSymbol() + _T("�������ϴ洢���");
						gl_systemMessage.PushDayLineInfoMessage(str);
						gl_UpdateWorldMarketDB.release();
					});
					fUpdated = true;
					//TRACE("����%s��������\n", pSymbol->GetSymbol().GetBuffer());
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

	return (fUpdated);
}

bool CWorldMarket::UpdateEPSSurpriseDB() {
	CString str;
	const size_t stockSize = gl_dataContainerFinnhubStock.Size();

	CWorldStockPtr pStock = nullptr;
	for (long l = 0; l < stockSize; ++l) {
		pStock = gl_dataContainerFinnhubStock.GetStock(l);
		if (pStock->IsEPSSurpriseNeedSaveAndClearFlag()) {// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			//CreateThreadUpdateEPSSurpriseDB(pStock);
			pStock->UpdateEPSSurpriseDB();
			//TRACE("����%s EPS surprise����\n", pStock->GetSymbol().GetBuffer());
		}
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}

	return (true);
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
	//SetSystemReady(true);
	if (!IsSystemReady()) {
		AddTask(WORLD_MARKET_CHECK_SYSTEM_READY__, GetNextSecond(lCurrentTime));
	}
	return IsSystemReady();
}

void CWorldMarket::TaskUpdateWorldMarketDB(long lCurrentTime) {
	if (gl_dataContainerFinnhubCountry.GetLastTotalCountry() < gl_dataContainerFinnhubCountry.GetTotalCountry()) {
		gl_runtime.background_executor()->post([] {
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerFinnhubCountry.UpdateDB();
			gl_UpdateWorldMarketDB.release();
		});
	}
	if (gl_dataContainerFinnhubForexExchange.IsNeedUpdate()) {
		gl_runtime.background_executor()->post([] {
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerFinnhubForexExchange.UpdateDB();
			gl_UpdateWorldMarketDB.release();
		});
	}
	if (gl_dataFinnhubForexSymbol.IsUpdateProfileDB()) {
		gl_runtime.background_executor()->post([] {
			gl_UpdateWorldMarketDB.acquire();
			gl_dataFinnhubForexSymbol.UpdateDB();
			gl_UpdateWorldMarketDB.release();
		});
	}
	if (gl_dataContainerFinnhubCryptoExchange.IsNeedUpdate()) {
		gl_runtime.background_executor()->post([] {
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerFinnhubCryptoExchange.UpdateDB();
			gl_UpdateWorldMarketDB.release();
		});
	}
	if (gl_dataFinnhubCryptoSymbol.IsUpdateProfileDB()) {
		gl_runtime.background_executor()->post([] {
			gl_UpdateWorldMarketDB.acquire();
			gl_dataFinnhubCryptoSymbol.UpdateDB();
			gl_UpdateWorldMarketDB.release();
		});
	}
	if (gl_dataContainerFinnhubStock.IsSaveInsiderTransaction()) {
		gl_runtime.background_executor()->post([this] {
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateInsiderTransactionDB();
			gl_UpdateWorldMarketDB.release();
		});
	}
	if (gl_dataContainerFinnhubStock.IsSaveInsiderSentiment()) {
		gl_runtime.background_executor()->post([this] {
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateInsiderSentimentDB();
			gl_UpdateWorldMarketDB.release();
		});
	}
	if (gl_dataContainerFinnhubStock.IsDayLineNeedSaving()) {
		gl_runtime.background_executor()->post([this] {
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateStockDayLineDB();
			gl_UpdateWorldMarketDB.release();
		});
	}
	if (gl_dataContainerFinnhubEconomicCalendar.IsNeedUpdate()) {
		gl_runtime.background_executor()->post([] {
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerFinnhubEconomicCalendar.UpdateDB();
			gl_UpdateWorldMarketDB.release();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateCompanyNewsDB()) {
		gl_runtime.background_executor()->post([this] {
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateCompanyNewsDB();
			gl_UpdateWorldMarketDB.release();
		});
	}
	if (gl_dataContainerFinnhubStock.IsUpdateBasicFinancialDB()) {
		gl_runtime.background_executor()->post([] {
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerFinnhubStock.UpdateBasicFinancialDB();
			gl_UpdateWorldMarketDB.release();
		});
	}
	if (gl_dataContainerTiingoStock.IsNeedUpdate()) {
		gl_runtime.background_executor()->post([] {
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerTiingoStock.UpdateDB();
			gl_UpdateWorldMarketDB.release();
		});
	}
	if (gl_dataContainerTiingoCryptoSymbol.IsNeedUpdate()) {
		gl_runtime.background_executor()->post([] {
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerTiingoCryptoSymbol.UpdateDB();
			gl_UpdateWorldMarketDB.release();
		});
	}
	if (gl_dataContainerFinnhubStock.IsSaveEPSSurpriseDB()) {
		gl_runtime.background_executor()->post([this] {
			gl_UpdateWorldMarketDB.acquire();
			this->UpdateEPSSurpriseDB();
			gl_UpdateWorldMarketDB.release();
		});
	}

	gl_runtime.background_executor()->post([this] {
		gl_UpdateWorldMarketDB.acquire();
		this->UpdateForexDayLineDB();
		gl_UpdateWorldMarketDB.release();
	});

	gl_runtime.background_executor()->post([this] {
		gl_UpdateWorldMarketDB.acquire();
		this->UpdateCryptoDayLineDB();
		gl_UpdateWorldMarketDB.release();
	});

	if (!gl_pFinnhubDataSource->IsUpdateSymbol() && gl_dataContainerFinnhubStock.IsUpdateProfileDB()) {
		gl_runtime.background_executor()->post([] {
			gl_UpdateWorldMarketDB.acquire();
			gl_dataContainerFinnhubStock.UpdateProfileDB();
			gl_UpdateWorldMarketDB.release();
		});
	}

	if (gl_finnhubInaccessibleExchange.IsNeedUpdate()) { // ���½�ֹ����֤ȯ����������
		gl_runtime.background_executor()->post([] {
			gl_finnhubInaccessibleExchange.UpdateDB();
			gl_finnhubInaccessibleExchange.SetUpdate(false);
		});
	}

	long lNextTime = GetNextTime(lCurrentTime, 0, 5, 0);
	if (IsTimeToResetSystem(lNextTime)) lNextTime = 170510;
	ASSERT(!IsTimeToResetSystem(lNextTime));// ������ʱ����ϵͳ�������ݿ���Ҫ����װ�룬�ʶ���ʱ������������ݿ⡣
	AddTask(WORLD_MARKET_UPDATE_DB__, lNextTime); // ÿ����Ӹ���һ��
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

bool CWorldMarket::UpdateStockDayLineDB() {
	for (long i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
		const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(i);
		pStock->UpdateDayLineDB();
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}
	return true;
}

bool CWorldMarket::UpdateCompanyNewsDB() {
	for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
		const auto pStock = gl_dataContainerFinnhubStock.GetStock(l);
		if (pStock->IsUpdateCompanyNewsDBAndClearFlag()) {// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			pStock->UpdateCompanyNewsDB();
		}
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}

	return (true);
}

bool CWorldMarket::UpdateInsiderTransactionDB() {
	int iCounter = 0;
	for (long i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
		const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(i);
		if (pStock->IsSaveInsiderTransactionAndClearFlag()) {
			if (pStock->HaveInsiderTransaction()) {
				iCounter++;
				pStock->UpdateInsiderTransactionDB();
			}
		}
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}
	if (iCounter > 0) {
		char buffer[30];
		sprintf_s(buffer, _T("%i"), iCounter);
		const CString str2 = buffer;
		gl_systemMessage.PushDayLineInfoMessage(_T("���θ�����") + str2 + _T("����Ʊ���ڲ���������"));
	}
	return true;
}

bool CWorldMarket::UpdateInsiderSentimentDB() {
	for (long i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
		const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(i);
		if (pStock->IsSaveInsiderSentimentAndClearFlag()) {
			if (pStock->HaveInsiderSentiment()) {
				pStock->UpdateInsiderSentimentDB();
			}
		}
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}
	return true;
}

bool CWorldMarket::UpdateTiingoIndustry() {
	// not implemented
	return false;
}

bool CWorldMarket::UpdateSICIndustry() {
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
		CSetWorldStockDayLine setWorldStockDayLine;

		for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
			const auto pStock = gl_dataContainerFinnhubStock.GetStock(l);
			setWorldStockDayLine.m_strFilter = strFilterPrefix + pStock->GetSymbol() + _T("'");
			setWorldStockDayLine.m_strSort = _T("[Date]");
			setWorldStockDayLine.Open();
			if (!setWorldStockDayLine.IsEOF()) {
				if (setWorldStockDayLine.m_Date < pStock->GetDayLineStartDate()) {
					pStock->SetDayLineStartDate(setWorldStockDayLine.m_Date);
					pStock->SetUpdateProfileDB(true);
				}
				setWorldStockDayLine.MoveLast();
				if (setWorldStockDayLine.m_Date > pStock->GetDayLineEndDate()) {
					pStock->SetDayLineEndDate(setWorldStockDayLine.m_Date);
					pStock->SetUpdateProfileDB(true);
				}
			}
			setWorldStockDayLine.Close();
		}
	}
	catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

/// <summary>
/// Finnhub WebSocket������˻����ֻ�ܷ���50��֤ȯ������
/// </summary>
vectorString CWorldMarket::GetFinnhubWebSocketSymbolVector() {
	vectorString vSymbol;

	vectorString vSymbolTemp = gl_dataContainerChosenWorldStock.GetSymbolVector();
	for (auto symbol : vSymbolTemp) {
		vSymbol.push_back(symbol);
	}

	vSymbolTemp = gl_dataContainerChosenWorldForex.GetSymbolVector();
	for (auto symbol : vSymbolTemp) {
		vSymbol.push_back(symbol);
	}

	vSymbolTemp = gl_dataContainerChosenWorldCrypto.GetSymbolVector();
	for (auto symbol : vSymbolTemp) {
		vSymbol.push_back(symbol);
	}

	return vSymbol;
}

void CWorldMarket::StartAllWebSocket() {
	if (IsSystemReady()) {
		if (!gl_pFinnhubDataSource->IsWebError()) {
			if (gl_pFinnhubWebSocket->IsError() || gl_pFinnhubWebSocket->IsIdle()) StartFinnhubWebSocket();
		}
		if (!gl_pTiingoDataSource->IsWebError()) {
			if (gl_pTiingoIEXWebSocket->IsError() || gl_pTiingoIEXWebSocket->IsIdle()) StartTiingoIEXWebSocket();
			if (gl_pTiingoCryptoWebSocket->IsError() || gl_pTiingoCryptoWebSocket->IsIdle()) StartTiingoCryptoWebSocket();
			if (gl_pTiingoForexWebSocket->IsError() || gl_pTiingoForexWebSocket->IsIdle()) StartTiingoForexWebSocket();
		}
	}
}

void CWorldMarket::StartFinnhubWebSocket() {
	if (gl_systemConfiguration.IsUsingFinnhubWebSocket() && !gl_pFinnhubDataSource->IsTimeout()) {
		gl_pFinnhubWebSocket->CreateThreadConnectAndSendMessage(GetFinnhubWebSocketSymbolVector());
		gl_pFinnhubWebSocket->SetHeartbeatTime(GetUTCTime());
	}
}

void CWorldMarket::StartTiingoIEXWebSocket() {
	if (gl_systemConfiguration.IsUsingTiingoIEXWebSocket() && !gl_pTiingoDataSource->IsTimeout()) {
		gl_pTiingoIEXWebSocket->CreateThreadConnectAndSendMessage(gl_dataContainerChosenWorldStock.GetSymbolVector());
		gl_pTiingoIEXWebSocket->SetHeartbeatTime(GetUTCTime());
	}
}

void CWorldMarket::StartTiingoCryptoWebSocket() {
	if (gl_systemConfiguration.IsUsingTiingoCryptoWebSocket() && !gl_pTiingoDataSource->IsTimeout()) {
		gl_pTiingoCryptoWebSocket->CreateThreadConnectAndSendMessage(gl_dataContainerChosenWorldCrypto.GetSymbolVector());
		gl_pTiingoCryptoWebSocket->SetHeartbeatTime(GetUTCTime());
	}
}

void CWorldMarket::StartTiingoForexWebSocket() {
	if (gl_systemConfiguration.IsUsingTiingoForexWebSocket() && !gl_pTiingoDataSource->IsTimeout()) {
		gl_pTiingoForexWebSocket->CreateThreadConnectAndSendMessage(gl_dataContainerChosenWorldForex.GetSymbolVector());
		gl_pTiingoForexWebSocket->SetHeartbeatTime(GetUTCTime());
	}
}

/// <summary>
/// // ֹͣWebSocket���˺����ȴ���ֹͣ�󷽷��ء���ϵͳ�˳�ǰ��׼��������
/// </summary>
void CWorldMarket::DisconnectAllWebSocket() {
	//������ֻ��ϵͳ�˳�ʱ����
	//ASSERT(gl_systemConfiguration.IsExitingSystem()); 
	if (gl_systemConfiguration.IsUsingFinnhubWebSocket()) gl_pFinnhubWebSocket->Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoIEXWebSocket()) gl_pTiingoIEXWebSocket->Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoCryptoWebSocket()) gl_pTiingoCryptoWebSocket->Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoForexWebSocket()) gl_pTiingoForexWebSocket->Disconnect();
}

void CWorldMarket::StopAllWebSocketIfTimeOut() const {
	if (IsSystemReady()) {
		if (gl_systemConfiguration.IsUsingFinnhubWebSocket()) {
			StopFinnhubWebSocketIfTimeOut();
		}
		if (gl_systemConfiguration.IsUsingTiingoCryptoWebSocket()) {
			StopTiingoCryptoWebSocketIfTimeOut();
		}
		if (gl_systemConfiguration.IsUsingTiingoIEXWebSocket()) {
			StopTiingoIEXWebSocketIfTimeOut();
		}
		if (gl_systemConfiguration.IsUsingTiingoForexWebSocket()) {
			StopTiingoForexWebSocketIfTimeOut();
		}
	}
}

void CWorldMarket::StopFinnhubWebSocketIfTimeOut() {
	if (gl_pFinnhubDataSource->IsTimeout() || gl_pFinnhubWebSocket->IsIdle()) {
		gl_pFinnhubWebSocket->CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Finnhub web timeout���ر�Web socket����"));
	}
}

void CWorldMarket::StopTiingoIEXWebSocketIfTimeOut() {
	if (gl_pTiingoDataSource->IsTimeout() && !gl_pTiingoIEXWebSocket->IsIdle()) {
		gl_pTiingoIEXWebSocket->CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo IEX web timeout���ر�Web socket����"));
	}
}

void CWorldMarket::StopTiingoCryptoWebSocketIfTimeOut() {
	if (gl_pTiingoDataSource->IsTimeout() || gl_pTiingoCryptoWebSocket->IsIdle()) {
		gl_pTiingoCryptoWebSocket->CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Crypto web timeout���ر�Web socket����"));
	}
}

void CWorldMarket::StopTiingoForexWebSocketIfTimeOut() {
	if (gl_pTiingoDataSource->IsTimeout() && !gl_pTiingoForexWebSocket->IsIdle()) {
		gl_pTiingoForexWebSocket->CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Forex web timeout���ر�Web socket����"));
	}
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
	for (auto i = 0; i < total; i++) {
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
	for (auto i = 0; i < total; i++) {
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
	for (auto i = 0; i < total; i++) {
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
void CWorldMarket::UpdateWorldStockFromWebSocket() {
	auto total = gl_SystemData.GetTiingoIEXSocketSize();
	for (auto i = 0; i < total; i++) {
		const CTiingoIEXSocketPtr pIEXData = gl_SystemData.PopTiingoIEXSocket();
		UpdateWorldStockFromTiingoIEXSocket(pIEXData);
		gl_systemMessage.SetCurrentTiingoWebSocketIEX(pIEXData->m_sSymbol.c_str());
	}

	total = gl_SystemData.GetTiingoCryptoSocketSize();
	for (auto i = 0; i < total; i++) {
		const CTiingoCryptoSocketPtr pCryptoData = gl_SystemData.PopTiingoCryptoSocket();
		gl_systemMessage.SetCurrentTiingoWebSocketCrypto(pCryptoData->m_sSymbol.c_str());
	}

	total = gl_SystemData.GetTiingoForexSocketSize();
	for (auto i = 0; i < total; i++) {
		const CTiingoForexSocketPtr pForexData = gl_SystemData.PopTiingoForexSocket();
		gl_systemMessage.SetCurrentTiingoWebSocketForex(pForexData->m_sSymbol.c_str());
	}

	total = gl_SystemData.GetFinnhubSocketSize();
	for (auto i = 0; i < total; i++) {
		const CFinnhubSocketPtr pFinnhubData = gl_SystemData.PopFinnhubSocket();
		UpdateWorldStockFromFinnhubSocket(pFinnhubData);
		gl_systemMessage.SetCurrentFinnhubWebSocketStake(pFinnhubData->m_sSymbol.c_str());
	}
}

void CWorldMarket::UpdateWorldStockFromTiingoIEXSocket(const CTiingoIEXSocketPtr& pTiingoIEXbData) {
	if (gl_dataContainerFinnhubStock.IsSymbol(pTiingoIEXbData->m_sSymbol.c_str())) {
		const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(pTiingoIEXbData->m_sSymbol.c_str());
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

void CWorldMarket::UpdateWorldStockFromFinnhubSocket(const CFinnhubSocketPtr& pFinnhubData) {
	if (gl_dataContainerFinnhubStock.IsSymbol(pFinnhubData->m_sSymbol.c_str())) {
		const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(pFinnhubData->m_sSymbol.c_str());
		pStock->SetActive(true);
		pStock->SetNew(pFinnhubData->m_dLastPrice * 1000);
	}
}

void CWorldMarket::UpdateMarketStatus(const CMarketStatusVectorPtr& pv) const {
	for (auto p : *pv) {
		m_pvMarketStatus->push_back(p);
	}
}

void CWorldMarket::UpdateMarketHoliday(const CMarketHolidayVectorPtr& pv) const {
	for (auto p : *pv) {
		m_pvMarketHoliday->push_back(p);
	}
}
