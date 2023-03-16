// ReSharper disable All
#include"pch.h"

#include "WorldMarket.h"
#include"thread.h"

#include"FinnhubInaccessibleExchange.h"

#include"FinnhubDataSource.h"
//#include"TiingoDataSource.h"

#include"FinnhubWebSocket.h"
#include"TiingoIEXWebSocket.h"
#include"TiingoForexWebSocket.h"
#include"TiingoCryptoWebSocket.h"

#include"SetWorldStockDayLine.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXUserAgent.h>

#include<thread>
#include<memory>

#include "QuandlDataSource.h"
#include "TiingoDataSource.h"
#include "TimeConvert.h"
using std::thread;
using std::make_shared;

CWorldMarket::CWorldMarket() {
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

	Reset();
}

CWorldMarket::~CWorldMarket() {
	PreparingExitMarket();
}

void CWorldMarket::Reset(void) {
	m_iCount1Hour = 3576; // �������ÿ�εĴ�11���ӣ���һ����ÿ�εĴ�22����
	m_iCount5Minute = 287; // ��һ����ÿ�εĴ�11����
	m_iCount1Minute = 58; // ��10��ÿ�εĴ�1����
	m_iCount10Second = 9;

	ResetFinnhub();
	ResetTiingo();
	ResetDataClass();
}

void CWorldMarket::ResetFinnhub(void) {
	SetSystemReady(false); // �г���ʼ״̬Ϊδ���úá�
	if (IsEarlyThen(gl_finnhubInaccessibleExchange.GetUpdateDate(), GetMarketDate(), 7)) {
		gl_finnhubInaccessibleExchange.Clear(); // ��ʹ�ø���ʱ������һ�ܵ����ݡ����֮����ϵͳ�Զ������µ�״̬��
		gl_finnhubInaccessibleExchange.SetUpdateDate(GetMarketDate());
	}
}

void CWorldMarket::ResetQuandl(void) { }

void CWorldMarket::ResetTiingo(void) { }

void CWorldMarket::ResetDataClass(void) {
	m_dataFinnhubStockExchange.Reset();
	m_dataFinnhubForexExchange.Reset();
	m_dataFinnhubCryptoExchange.Reset();
	m_dataFinnhubForexSymbol.Reset();
	m_dataFinnhubCryptoSymbol.Reset();
	m_dataFinnhubCountry.Reset();
	m_dataFinnhubEconomicCalendar.Reset();

	m_containerStock.Reset();

	m_dataTiingoStock.Reset();
	m_dataTiingoCryptoSymbol.Reset();

	m_containerChosenStock.Reset();
	m_containerChosenForex.Reset();
	m_containerChosenCrypto.Reset();
}

void CWorldMarket::ResetMarket(void) {
	Reset();

	UpdateToken();

	LoadWorldExchangeDB(); // װ�����罻������Ϣ
	LoadCountryDB();
	LoadStockDB();
	LoadWorldChosenStock();
	LoadForexExchange();
	LoadFinnhubForexSymbol();
	LoadWorldChosenForex();
	LoadCryptoExchange();
	LoadFinnhubCryptoSymbol();
	LoadWorldChosenCrypto();
	LoadEconomicCalendarDB();
	LoadTiingoStock();
	LoadTiingoCryptoSymbol();

	for (const auto& pDataSource : m_vDataSource) {
		pDataSource->Reset();
	}

	CString str = _T("����World Market��������׼ʱ�䣺");
	str += GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(str);
}

bool CWorldMarket::PreparingExitMarket(void) {
	//ASSERT(gl_systemStatus.IsExitingSystem());
	DisconnectAllWebSocket();

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Ĭ��ÿ100����ִ��һ�Ρ�
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::SchedulingTask(void) {
	CalculateTime();

	const long lCurrentTime = GetMarketTime();
	const time_t tUTC = GetUTCTime();
	const long lTimeDiffer = tUTC > m_lastTimeSchedulingTask;

	// ���ø�Web data source�������������ݵĽ��պʹ���
	RunDataSource(lCurrentTime);

	// ִ�б��г����ʱ����
	ProcessEveryDayTask(lCurrentTime);

	//����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
	if (lTimeDiffer > 0) {
		SchedulingTaskPerSecond(lTimeDiffer, lCurrentTime);
		m_lastTimeSchedulingTask = tUTC;
	}

	return true;
}

void CWorldMarket::CreateTaskOfReset() {
	if (HaveResetMarketPermission()) { // �����������ϵͳ
		const auto pTask = make_shared<CMarketTask>();
		pTask->SetType(WORLD_MARKET_CREATE_TASK__);
		pTask->SetTime(1000); // ��ʱʮ������ϵͳ
		StoreMarketTask(pTask);
		SetResetMarketPermission(false);
	}
}

bool CWorldMarket::ProcessEveryDayTask(long lCurrentTime) {
	if (IsMarketTaskEmpty()) return false;
	const auto pTask = GetMarketTask();
	if (lCurrentTime >= pTask->GetTime()) {
		DiscardCurrentMarketTask();
		switch (pTask->GetType()) {
		case WORLD_MARKET_CREATE_TASK__: // ������������
			TaskCreateTask(lCurrentTime);
			break;
		case WORLD_MARKET_CHECK_SYSTEM__: // 170000����ϵͳ
			TaskCheckMarketReady(lCurrentTime);
			break;
		case WORLD_MARKET_RESET__: // 170000����ϵͳ
			TaskResetMarket(lCurrentTime);
			break;
		case WORLD_MARKET_UPDATE_STOCK_PROFILE_DB__:
			//TaskUpdateStockProfileDB(lCurrentTime);
			break;
		default:
			ASSERT(0); // �Ƿ�����
			break;
		}
		return true;
	}
	return false;
}

bool CWorldMarket::TaskCreateTask(long lCurrentTime) {
	CMarketTaskPtr pTask;
	const long lTimeMinute = (lCurrentTime / 100) * 100; // ��ǰСʱ�ͷ���

	while (!IsMarketTaskEmpty()) DiscardCurrentMarketTask();

	// ��ʹ��ʼ�����
	AddTask(WORLD_MARKET_CHECK_SYSTEM__, 1);

	// ����ϵͳ
	if (lCurrentTime < 170000) {
		AddTask(WORLD_MARKET_RESET__, 170000); // ִ��ʱ��Ϊ��170000
	}
	return true;
}

bool CWorldMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
	ResetMarketFlagAtMidnight(lCurrentTime);
	CreateTaskOfReset();

	m_iCount10Second -= lSecond;
	m_iCount1Minute -= lSecond;
	m_iCount5Minute -= lSecond;

	if (m_iCount5Minute < 0) {
		m_iCount5Minute = 300 - lSecond;
		SchedulingTaskPer5Minute(lCurrentTime);
	}
	if (m_iCount1Minute < 0) {
		m_iCount1Minute = 60 - lSecond;
		SchedulingTaskPerMinute(lCurrentTime);
	}
	if (m_iCount10Second < 0) {
		m_iCount10Second = 10 - lSecond;
		SchedulingTaskPer10Seconds(lCurrentTime);
	}

	if (!IsTimeToResetSystem(lCurrentTime)) {	// ������ʱ����ϵͳ�������ݿ���Ҫ����װ�룬�ʶ���ʱ��������¡�
		TaskProcessWebSocketData();
		TaskUpdateWorldStockFromWebSocket();
	}

	return true;
}

bool CWorldMarket::SchedulingTaskPer10Seconds(long lCurrentTime) {
	StopWebSocketsIfTimeOut();
	return true;
}

bool CWorldMarket::SchedulingTaskPerMinute(long lCurrentTime) {
	// ����WebSocket����
	StartAllWebSocket();

	// ������������һ������������ִ����Ϻ󷵻��ˡ�
	if (!IsTimeToResetSystem(lCurrentTime)) {	// ������ʱ����ϵͳ�������ݿ���Ҫ����װ�룬�ʶ���ʱ������������ݿ⡣
		if (m_dataFinnhubCountry.GetLastTotalCountry() < m_dataFinnhubCountry.GetTotalCountry()) {
			TaskUpdateCountryListDB();
		}
		if (IsUpdateForexExchangeDB()) TaskUpdateForexExchangeDB();
		if (IsUpdateForexSymbolDB()) TaskUpdateForexSymbolDB();
		if (IsUpdateCryptoExchangeDB()) TaskUpdateCryptoExchangeDB();
		if (IsUpdateCryptoSymbolDB()) TaskUpdateFinnhubCryptoSymbolDB();
		if (IsUpdateInsiderTransactionDB()) TaskUpdateInsiderTransactionDB();
		if (IsUpdateInsiderSentimentDB()) TaskUpdateInsiderSentimentDB();
		TaskUpdateForexDayLineDB();
		TaskUpdateCryptoDayLineDB();
		if (IsSaveStockDayLineDB()) TaskUpdateDayLineDB();
		TaskUpdateEPSSurpriseDB();
		if (IsUpdateEconomicCalendarDB()) TaskUpdateEconomicCalendarDB();
		return true;
	}

	return false;
}

bool CWorldMarket::SchedulingTaskPer5Minute(long lCurrentTime) {
	if (IsUpdateCompanyNewsDB()) TaskUpdateCompanyNewsDB();
	if (IsUpdateBasicFinancialDB()) TaskUpdateBasicFinancialDB();
	if (IsNeedUpdateTiingoStock()) TaskUpdateTiingoStockDB();
	if (IsNeedUpdateTiingoCryptoSymbol()) TaskUpdateTiingoCryptoSymbolDB();

	// ���¹�Ʊ���������÷������
	if (!gl_pFinnhubDataSource->IsUpdateSymbol() && IsUpdateStockProfileDB()) {
		TaskUpdateStockProfileDB(lCurrentTime);
	}

	return true;
}

bool CWorldMarket::TaskResetMarket(long lCurrentTime) {
	// �г�ʱ��ʮ��ʱ����ϵͳ
	SetResetMarket(true); // ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
	SetSystemReady(false);

	return true;
}

bool CWorldMarket::TaskUpdateTiingoIndustry(void) {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) { // ����tiingo stock profile��finnhub����stock profile����
		thread thread1(ThreadUpdateTiingoIndustry, this);
		thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateSICIndustry(void) {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) {// ����tiingo stock profile��finnhub����stock profile����
		thread thread1(ThreadUpdateSICIndustry, this);
		thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateNaicsIndustry(void) {
	if (!gl_pFinnhubDataSource->IsUpdateStockProfile()) {// ����tiingo stock profile��finnhub����stock profile����
		thread thread1(ThreadUpdateNaicsIndustry, this);
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
bool CWorldMarket::TaskUpdateForexDayLineDB(void) {
	CString str;
	bool fUpdated = false;
	CForexSymbolPtr pSymbol = nullptr;

	for (int i = 0; i < m_dataFinnhubForexSymbol.Size(); i++) {
		if (gl_systemStatus.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
		pSymbol = m_dataFinnhubForexSymbol.GetSymbol(i);
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) {	// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					thread thread1(ThreadUpdateForexDayLineDB, pSymbol.get());
					thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
					fUpdated = true;
					TRACE("����%s��������\n", pSymbol->GetSymbol().GetBuffer());
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
bool CWorldMarket::TaskUpdateCryptoDayLineDB(void) {
	CString str;
	bool fUpdated = false;
	CFinnhubCryptoSymbolPtr pSymbol = nullptr;

	for (int i = 0; i < m_dataFinnhubCryptoSymbol.Size(); ++i) {
		if (gl_systemStatus.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
		pSymbol = m_dataFinnhubCryptoSymbol.GetSymbol(i);
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) {	// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					thread thread1(ThreadUpdateCryptoDayLineDB, pSymbol.get());
					thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
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

bool CWorldMarket::TaskUpdateEPSSurpriseDB(void) {
	CString str;

	CWorldStockPtr pStock = nullptr;
	for (long l = 0; l < m_containerStock.Size(); ++l) {
		pStock = m_containerStock.GetStock(l);
		if (pStock->IsEPSSurpriseNeedSaveAndClearFlag()) {// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			thread thread1(ThreadUpdateEPSSurpriseDB, pStock.get());
			thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
			TRACE("����%s EPS surprise����\n", pStock->GetSymbol().GetBuffer());
		}
		if (gl_systemStatus.IsExitingSystem()) {
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
	if (!IsSystemReady()) AddTask(WORLD_MARKET_CHECK_SYSTEM__, GetNextSecond(lCurrentTime));
	return IsSystemReady();
}

bool CWorldMarket::TaskUpdateDayLineStartEndDate(void) {
	thread thread1(ThreadUpdateWorldStockDayLineStartEndDate, this);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateDayLineDB() {
	thread thread1(ThreadUpdateWorldStockDayLineDB, this);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateStockProfileDB(long lCurrentTime) {
	thread thread1(ThreadUpdateWorldMarketStockProfileDB, this);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateCompanyNewsDB(void) {
	thread thread1(ThreadUpdateCompanyNewsDB, this);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateBasicFinancialDB(void) {
	thread thread1(ThreadUpdateBasicFinancialDB, this);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateCountryListDB(void) {
	thread thread1(ThreadUpdateCountryListDB, this);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateInsiderTransactionDB(void) {
	thread thread1(ThreadUpdateInsiderTransactionDB, this);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateInsiderSentimentDB(void) {
	thread thread1(ThreadUpdateInsiderSentimentDB, this);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateTiingoStockDB(void) {
	thread thread1(ThreadUpdateTiingoStockDB, this);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateTiingoCryptoSymbolDB(void) {
	thread thread1(ThreadUpdateTiingoCryptoSymbolDB, this);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateForexExchangeDB(void) {
	thread thread1(ThreadUpdateForexExchangeDB, this);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateForexSymbolDB() {
	thread thread1(ThreadUpdateForexSymbolDB, this);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateCryptoExchangeDB(void) {
	thread thread1(ThreadUpdateCryptoExchangeDB, this);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateFinnhubCryptoSymbolDB() {
	thread thread1(ThreadUpdateFinnhubCryptoSymbolDB, this);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateEconomicCalendarDB(void) {
	thread thread1(ThreadUpdateEconomicCalendarDB, this);
	thread1.detach();

	return true;
}

bool CWorldMarket::UpdateToken(void) {
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

bool CWorldMarket::UpdateStockDayLineDB(void) {
	for (long i = 0; i < GetStockSize(); i++) {
		const CWorldStockPtr pStock = GetStock(i);
		pStock->UpdateDayLineDB();
		if (gl_systemStatus.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}
	return true;
}

bool CWorldMarket::UpdateCompanyNewsDB(void) {
	for (long l = 0; l < m_containerStock.Size(); l++) {
		const auto pStock = m_containerStock.GetStock(l);
		if (pStock->IsUpdateCompanyNewsDBAndClearFlag()) {// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			pStock->UpdateCompanyNewsDB();
		}
		if (gl_systemStatus.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}

	return (true);
}

bool CWorldMarket::UpdateInsiderTransactionDB(void) {
	for (long i = 0; i < GetStockSize(); i++) {
		const CWorldStockPtr pStock = GetStock(i);
		if (pStock->IsSaveInsiderTransactionAndClearFlag()) {
			if (pStock->HaveInsiderTransaction()) {
				pStock->SaveInsiderTransaction();
				const CString str = pStock->GetSymbol() + _T("�ڲ��������ϸ������");
				gl_systemMessage.PushDayLineInfoMessage(str);
				//TRACE("����%s�ڲ���������\n", pStock->GetSymbol().GetBuffer());
			}
		}
		if (gl_systemStatus.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}
	return true;
}

bool CWorldMarket::UpdateInsiderSentimentDB(void) {
	for (long i = 0; i < GetStockSize(); i++) {
		const CWorldStockPtr pStock = GetStock(i);
		if (pStock->IsSaveInsiderSentimentAndClearFlag()) {
			if (pStock->HaveInsiderSentiment()) {
				pStock->SaveInsiderSentiment();
				const CString str = pStock->GetSymbol() + _T("�ڲ������������ϸ������");
				gl_systemMessage.PushDayLineInfoMessage(str);
			}
		}
		if (gl_systemStatus.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}
	return true;
}

bool CWorldMarket::UpdateTiingoIndustry(void) {
	// ��δʵ��
	return false;
}

bool CWorldMarket::UpdateSICIndustry(void) {
	// ��δʵ��
	return false;
}

bool CWorldMarket::UpdateNaicsIndustry(void) {
	// ��δʵ��
	return false;
}

bool CWorldMarket::RebuildEPSSurprise(void) {
	m_containerStock.ResetEPSSurprise();
	gl_pFinnhubDataSource->SetUpdateEPSSurprise(true);

	return true;
}

bool CWorldMarket::RebuildPeer(void) {
	m_containerStock.ResetPeer();
	gl_pFinnhubDataSource->SetUpdatePeer(true);

	return true;
}

bool CWorldMarket::RebuildBasicFinancial(void) {
	m_containerStock.ResetBasicFinancial();
	gl_pFinnhubDataSource->SetUpdateStockBasicFinancial(true);

	return true;
}

bool CWorldMarket::RebuildStockDayLineDB(void) {
	m_containerStock.ResetDayLine();
	gl_pFinnhubDataSource->SetUpdateStockProfile(true);

	return true;
}

bool CWorldMarket::UpdateStockDayLineStartEndDate(void) {
	const CString strFilterPrefix = _T("[Symbol] = '");
	CSetWorldStockDayLine setWorldStockDayLine;

	for (long l = 0; l < m_containerStock.Size(); l++) {
		const auto pStock = m_containerStock.GetStock(l);
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

	return true;
}

/// <summary>
/// Finnhub WebSocket������˻����ֻ�ܷ���50��֤ȯ������
/// </summary>
vectorString CWorldMarket::GetFinnhubWebSocketSymbolVector(void) {
	vectorString vSymbol;

	for (long l = 0; l < m_containerChosenStock.Size(); l++) {
		const CWorldStockPtr pStock = m_containerChosenStock.GetStock(l);
		vSymbol.push_back(pStock->GetSymbol().GetBuffer());
	}

	for (long l = 0; l < m_containerChosenCrypto.Size(); l++) {
		const CFinnhubCryptoSymbolPtr pCrypto = m_containerChosenCrypto.GetCryptoSymbol(l);
		vSymbol.push_back(pCrypto->GetSymbol().GetBuffer());
	}

	for (long l = 0; l < m_containerChosenForex.Size(); l++) {
		const CForexSymbolPtr pForex = m_containerChosenForex.GetForexSymbol(l);
		vSymbol.push_back(pForex->GetSymbol().GetBuffer());
	}

	// Send a message to the server (default to TEXT mode)
	//m_FinnhubWebSocket.Send("{\"type\":\"subscribe\",\"symbol\":\"BINANCE:BTCUSDT\"}"); //{"type":"subscribe","symbol":"BINANCE:BTCUSDT"}
	//m_FinnhubWebSocket.Send("{\"type\":\"subscribe\",\"symbol\":\"BINANCE:LTCBTC\"}"); //{"type":"subscribe","symbol":"BINANCE:LTCBTC"}
	//m_FinnhubWebSocket.Send("{\"type\":\"subscribe\",\"symbol\":\"IC MARKETS:1\"}"); //
	//m_FinnhubWebSocket.Send("{\"type\":\"subscribe\",\"symbol\":\"OANDA:AUD_SGD\"}"); // OANDA:AUD_SGD
	//m_FinnhubWebSocket.Send("{\"type\":\"subscribe\",\"symbol\":\"FXCM:USD/JPY\"}"); // FXCM:USD/JPY

	return vSymbol;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo������˻����������ƣ�Ϊ500��/Сʱ�� 20000��/�죬 5GB/�¡�
//
// thresholdlevel 0�������е�IEX����ʱ��ÿ��������Ϊ1M-9M;thresholdlevel5��������IEX����ʱ��ÿ��������Ϊ10-50K��
//
// thresholdlevel 5��all Last Trade updates and only Quote updates that are deemed major updates by our system.
// thresholdlevel 0: ALL Top-of-Book AND Last Trade updates.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vectorString CWorldMarket::GetTiingoIEXWebSocketSymbolVector(void) {
	vectorString vSymbol;
	for (long l = 0; l < m_containerChosenStock.Size(); l++) {
		const CWorldStockPtr pStock = m_containerChosenStock.GetStock(l);
		vSymbol.push_back(pStock->GetSymbol().GetBuffer());
	}

	return vSymbol;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// �������е�Crypto����ʱ��ÿ��������������50-100K������
//
// thresholdLevel 2: Top-of-Book AND Last Trade updates.
// thresholdLevel 5: only Last Trade updates.
//
//////////////////////////////////////////////////////////////////////////////////////////////
vectorString CWorldMarket::GetTiingoCryptoWebSocketSymbolVector(void) {
	vectorString vSymbol;
	for (long l = 0; l < m_containerChosenCrypto.Size(); l++) {
		const CFinnhubCryptoSymbolPtr pCrypto = m_containerChosenCrypto.GetCryptoSymbol(l);
		vSymbol.push_back(pCrypto->GetSymbol().GetBuffer());
	}

	return vSymbol;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 5״̬��ÿ�����100K���ҡ�
//
// thresholdLevel 5: ALL Top-of-Book updates
// thresholdLevel 7: A top-of-book update that is due to a change in either the bid/ask price or size.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
vectorString CWorldMarket::GetTiingoForexWebSocketSymbolVector(void) {
	vectorString vSymbol;
	for (long l = 0; l < m_containerChosenForex.Size(); l++) {
		const CForexSymbolPtr pForex = m_containerChosenForex.GetForexSymbol(l);
		vSymbol.push_back(pForex->GetSymbol().GetBuffer());
	}

	return vSymbol;
}

bool CWorldMarket::StartAllWebSocket(void) {
	if (IsSystemReady()) {
		if (!gl_pFinnhubDataSource->IsWebError()) StartFinnhubWebSocket();
		if (!gl_pTiingoDataSource->IsWebError()) {
			StartTiingoIEXWebSocket();
			StartTiingoCryptoWebSocket();
			StartTiingoForexWebSocket();
		}
	}
	return true;
}

void CWorldMarket::StartFinnhubWebSocket(void) {
	if (gl_systemConfiguration.IsUsingFinnhubWebSocket() && !gl_pFinnhubDataSource->IsTimeout()) {
		if (gl_finnhubWebSocket.IsClosed()) {
			gl_finnhubWebSocket.CreateThreadConnectWebSocketAndSendMessage(GetFinnhubWebSocketSymbolVector());
		}
	}
}

void CWorldMarket::StartTiingoIEXWebSocket(void) {
	if (gl_systemConfiguration.IsUsingTiingoIEXWebSocket() && !gl_pTiingoDataSource->IsTimeout()) {
		if (gl_tiingoIEXWebSocket.IsClosed()) {
			gl_tiingoIEXWebSocket.CreateThreadConnectWebSocketAndSendMessage(GetTiingoIEXWebSocketSymbolVector());
		}
	}
}

void CWorldMarket::StartTiingoCryptoWebSocket(void) {
	if (gl_systemConfiguration.IsUsingTiingoCryptoWebSocket() && !gl_pTiingoDataSource->IsTimeout()) {
		if (gl_tiingoCryptoWebSocket.IsClosed()) {
			gl_tiingoCryptoWebSocket.CreateThreadConnectWebSocketAndSendMessage(GetTiingoCryptoWebSocketSymbolVector());
		}
	}
}

void CWorldMarket::StartTiingoForexWebSocket(void) {
	if (gl_systemConfiguration.IsUsingTiingoForexWebSocket() && !gl_pTiingoDataSource->IsTimeout()) {
		if (gl_tiingoForexWebSocket.IsClosed()) {
			gl_tiingoForexWebSocket.CreateThreadConnectWebSocketAndSendMessage(GetTiingoForexWebSocketSymbolVector());
		}
	}
}

/// <summary>
/// // ֹͣWebSocket���˺��������ɹ����߳���ֹͣWebSocket�����õȴ���ֹͣ�����ء�����ϵͳ�����е�ֹͣ������
/// </summary>
void CWorldMarket::DisconnectAllWebSocket(void) {
	//ASSERT(gl_systemStatus.IsExitingSystem()); //������ֻ��ϵͳ�˳�ʱ����
	if (gl_systemConfiguration.IsUsingFinnhubWebSocket()) gl_finnhubWebSocket.Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoIEXWebSocket()) gl_tiingoIEXWebSocket.Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoCryptoWebSocket()) gl_tiingoCryptoWebSocket.Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoForexWebSocket()) gl_tiingoForexWebSocket.Disconnect();
}

/// <summary>
/// ֹͣWebSocket���˺��������ɹ����߳���ֹͣWebSocket�����õȴ���ֹͣ�����ء�����ϵͳ�����е�ֹͣ������
/// </summary>
void CWorldMarket::StopWebSocketsIfTimeOut(void) {
	if (IsSystemReady()) {
		StopFinnhubWebSocketIfTimeOut();
		StopTiingoIEXWebSocketIfTimeOut();
		StopTiingoCryptoWebSocketIfTimeOut();
		StopTiingoForexWebSocketIfTimeOut();
	}
}

void CWorldMarket::StopFinnhubWebSocketIfTimeOut(void) {
	if ((gl_pFinnhubDataSource->IsTimeout()) && !gl_finnhubWebSocket.IsClosed()) {
		gl_finnhubWebSocket.CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Finnhub web����12002���ر�Web socket����"));
	}
}

void CWorldMarket::StopTiingoIEXWebSocketIfTimeOut(void) {
	if ((gl_pTiingoDataSource->IsTimeout()) && !gl_tiingoIEXWebSocket.IsClosed()) {
		gl_tiingoIEXWebSocket.CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo IEX web����12002���ر�Web socket����"));
	}
}

void CWorldMarket::StopTiingoCryptoWebSocketIfTimeOut(void) {
	if ((gl_pTiingoDataSource->IsTimeout()) && !gl_tiingoCryptoWebSocket.IsClosed()) {
		gl_tiingoCryptoWebSocket.CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Crypto web����12002���ر�Web socket����"));
	}
}

void CWorldMarket::StopTiingoForexWebSocketIfTimeOut(void) {
	if ((gl_pTiingoDataSource->IsTimeout()) && !gl_tiingoForexWebSocket.IsClosed()) {
		gl_tiingoForexWebSocket.CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Forex web����12002���ر�Web socket����"));
	}
}

bool CWorldMarket::TaskProcessWebSocketData(void) {
	if (gl_finnhubWebSocket.DataSize() > 0) ProcessFinnhubWebSocketData();
	if (gl_tiingoIEXWebSocket.DataSize() > 0) ProcessTiingoIEXWebSocketData();
	if (gl_tiingoCryptoWebSocket.DataSize() > 0) ProcessTiingoCryptoWebSocketData();
	if (gl_tiingoForexWebSocket.DataSize() > 0) ProcessTiingoForexWebSocketData();

	return true;
}

bool CWorldMarket::ProcessFinnhubWebSocketData() {
	const auto total = gl_finnhubWebSocket.DataSize();
	size_t iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		auto pString = gl_finnhubWebSocket.PopData();
		CString strMessage = _T("Finnhub: ");
		strMessage += (*pString).c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_finnhubWebSocket.ParseFinnhubWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedFinnhubWebSocket(iTotalDataSize);

	return true;
}

bool CWorldMarket::ProcessTiingoIEXWebSocketData() {
	const auto total = gl_tiingoIEXWebSocket.DataSize();
	size_t iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		auto pString = gl_tiingoIEXWebSocket.PopData();
		CString strMessage = _T("Tiingo IEX: ");
		strMessage += (*pString).c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_tiingoIEXWebSocket.ParseTiingoIEXWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoIEXWebSocket(iTotalDataSize);
	return true;
}

bool CWorldMarket::ProcessTiingoCryptoWebSocketData() {
	const auto total = gl_tiingoCryptoWebSocket.DataSize();

	size_t iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		auto pString = gl_tiingoCryptoWebSocket.PopData();
		CString strMessage = _T("Tiingo Crypto: ");
		strMessage += (*pString).c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_tiingoCryptoWebSocket.ParseTiingoCryptoWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoCryptoWebSocket(iTotalDataSize);
	return true;
}

bool CWorldMarket::ProcessTiingoForexWebSocketData() {
	const auto total = gl_tiingoForexWebSocket.DataSize();
	size_t iTotalDataSize = 0;
	for (size_t i = 0; i < total; i++) {
		auto pString = gl_tiingoForexWebSocket.PopData();
		CString strMessage = _T("Tiingo Forex: ");
		strMessage += (*pString).c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		gl_tiingoForexWebSocket.ParseTiingoForexWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoForexWebSocket(iTotalDataSize);
	return true;
}

/// <summary>
/// ����֤ȯ��ʵʱ��Ϣ
/// Ŀǰֻʹ��IEX��Crypto�����ݣ���������������
/// </summary>
/// <returns></returns>
bool CWorldMarket::TaskUpdateWorldStockFromWebSocket(void) {
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

	return true;
}

bool CWorldMarket::UpdateWorldStockFromTiingoIEXSocket(CTiingoIEXSocketPtr pTiingoIEXbData) {
	if (IsStock(pTiingoIEXbData->m_sSymbol.c_str())) {
		const CWorldStockPtr pStock = GetStock(pTiingoIEXbData->m_sSymbol.c_str());
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

	return true;
}

bool CWorldMarket::UpdateWorldStockFromFinnhubSocket(CFinnhubSocketPtr pFinnhubData) {
	if (IsStock(pFinnhubData->m_sSymbol.c_str())) {
		const CWorldStockPtr pStock = GetStock(pFinnhubData->m_sSymbol.c_str());
		pStock->SetActive(true);
		pStock->SetNew(pFinnhubData->m_dLastPrice * 1000);
	}

	return true;
}
