#include"pch.h"

#include "WorldMarket.h"
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
	ResetDataClass();
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

void CWorldMarket::ResetQuandl() { }

void CWorldMarket::ResetTiingo() { }

void CWorldMarket::ResetDataClass() {
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

void CWorldMarket::ResetMarket() {
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
		case WORLD_MARKET_UPDATE_STOCK_PROFILE_DB__:
			TaskUpdateStockProfileDB(lCurrentTime);
			break;
		case WORLD_MARKET_MONITORING_WEB_SOCKET_STATUS__:
			TaskMonitoringWebSocketStatus(lCurrentTime);
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

	AddTask(WORLD_MARKET_UPDATE_STOCK_PROFILE_DB__, lTimeMinute + 40);// ���¹�Ʊ������ݿ������

	AddTask(WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__, lCurrentTime);
	AddTask(WORLD_MARKET_MONITORING_WEB_SOCKET_STATUS__, GetNextTime(lTimeMinute, 0, 1, 0));

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

void CWorldMarket::TaskMonitoringWebSocketStatus(long lCurrentTime) {
	StartAllWebSocket();
	AddTask(WORLD_MARKET_MONITORING_WEB_SOCKET_STATUS__, GetNextTime(lCurrentTime, 0, 1, 0));
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
	const size_t symbolSize = m_dataFinnhubForexSymbol.Size();

	for (int i = 0; i < symbolSize; i++) {
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
		pSymbol = m_dataFinnhubForexSymbol.GetSymbol(i);
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) {	// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					CreateThreadUpdateForexDayLineDB(pSymbol);
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
bool CWorldMarket::UpdateCryptoDayLineDB() {
	CString str;
	bool fUpdated = false;
	CFinnhubCryptoSymbolPtr pSymbol = nullptr;
	const size_t symbolSize = m_dataFinnhubCryptoSymbol.Size();

	for (int i = 0; i < symbolSize; ++i) {
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
		pSymbol = m_dataFinnhubCryptoSymbol.GetSymbol(i);
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) {	// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					CreateThreadUpdateCryptoDayLineDB(pSymbol);
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
	const size_t stockSize = m_containerStock.Size();

	CWorldStockPtr pStock = nullptr;
	for (long l = 0; l < stockSize; ++l) {
		pStock = m_containerStock.GetStock(l);
		if (pStock->IsEPSSurpriseNeedSaveAndClearFlag()) {// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			CreateThreadUpdateEPSSurpriseDB(pStock);
			TRACE("����%s EPS surprise����\n", pStock->GetSymbol().GetBuffer());
		}
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}

	return (true);
}

void CWorldMarket::CreateThreadUpdateForexDayLineDB(CForexSymbolPtr pSymbol) {
	thread thread1(ThreadUpdateForexDayLineDB, pSymbol);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
}

void CWorldMarket::CreateThreadUpdateCryptoDayLineDB(CFinnhubCryptoSymbolPtr pSymbol) {
	thread thread1(ThreadUpdateCryptoDayLineDB, pSymbol);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
}

void CWorldMarket::CreateThreadUpdateEPSSurpriseDB(CWorldStockPtr pStock) {
	thread thread1(ThreadUpdateEPSSurpriseDB, pStock);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
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

void CWorldMarket::CreateThreadUpdateDayLineStartEndDate() {
	thread thread1(ThreadUpdateWorldStockDayLineStartEndDate, gl_pWorldMarket);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
}

void CWorldMarket::CreateThreadUpdateDayLineDB() {
	thread thread1(ThreadUpdateWorldStockDayLineDB, gl_pWorldMarket);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
}

void CWorldMarket::TaskUpdateStockProfileDB(long lCurrentTime) {
	if (m_dataFinnhubCountry.GetLastTotalCountry() < m_dataFinnhubCountry.GetTotalCountry()) {
		CreateThreadUpdateCountryListDB();
	}
	if (IsUpdateForexExchangeDB()) CreateThreadUpdateForexExchangeDB();
	if (IsUpdateForexSymbolDB()) CreateThreadUpdateForexSymbolDB();
	if (IsUpdateCryptoExchangeDB()) CreateThreadUpdateCryptoExchangeDB();
	if (IsUpdateCryptoSymbolDB()) CreateThreadUpdateFinnhubCryptoSymbolDB();
	if (IsUpdateInsiderTransactionDB()) CreateThreadUpdateInsiderTransactionDB();
	if (IsUpdateInsiderSentimentDB()) CreateThreadUpdateInsiderSentimentDB();
	if (IsSaveStockDayLineDB()) CreateThreadUpdateDayLineDB();
	if (IsUpdateEconomicCalendarDB()) CreateThreadUpdateEconomicCalendarDB();
	if (IsUpdateCompanyNewsDB()) CreateThreadUpdateCompanyNewsDB();
	if (IsUpdateBasicFinancialDB()) CreateThreadUpdateBasicFinancialDB();
	if (IsNeedUpdateTiingoStock()) CreateThreadUpdateTiingoStockDB();
	if (IsNeedUpdateTiingoCryptoSymbol()) CreateThreadUpdateTiingoCryptoSymbolDB();

	UpdateForexDayLineDB();
	UpdateCryptoDayLineDB();
	UpdateEPSSurpriseDB();

	if (!gl_pFinnhubDataSource->IsUpdateSymbol() && IsUpdateStockProfileDB()) {
		CreateThreadUpdateStockProfileDB();
	}

	if (gl_finnhubInaccessibleExchange.IsNeedUpdate()) { // ���½�ֹ����֤ȯ����������
		gl_finnhubInaccessibleExchange.UpdateFile();
	}

	long lNextTime = GetNextTime(lCurrentTime, 0, 5, 0);
	if (IsTimeToResetSystem(lNextTime)) lNextTime = 170510;
	ASSERT(!IsTimeToResetSystem(lNextTime));// ������ʱ����ϵͳ�������ݿ���Ҫ����װ�룬�ʶ���ʱ������������ݿ⡣
	AddTask(WORLD_MARKET_UPDATE_STOCK_PROFILE_DB__, lNextTime); // ÿ����Ӹ���һ��
}

void CWorldMarket::CreateThreadUpdateStockProfileDB() {
	thread thread1(ThreadUpdateWorldMarketStockProfileDB, gl_pWorldMarket);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
}

void CWorldMarket::CreateThreadUpdateCompanyNewsDB() {
	thread thread1(ThreadUpdateCompanyNewsDB, gl_pWorldMarket);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
}

void CWorldMarket::CreateThreadUpdateBasicFinancialDB() {
	thread thread1(ThreadUpdateBasicFinancialDB, gl_pWorldMarket);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
}

void CWorldMarket::CreateThreadUpdateCountryListDB() {
	thread thread1(ThreadUpdateCountryListDB, gl_pWorldMarket);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
}

void CWorldMarket::CreateThreadUpdateInsiderTransactionDB() {
	thread thread1(ThreadUpdateInsiderTransactionDB, gl_pWorldMarket);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
}

void CWorldMarket::CreateThreadUpdateInsiderSentimentDB() {
	thread thread1(ThreadUpdateInsiderSentimentDB, gl_pWorldMarket);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
}

void CWorldMarket::CreateThreadUpdateTiingoStockDB() {
	thread thread1(ThreadUpdateTiingoStockDB, gl_pWorldMarket);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
}

void CWorldMarket::CreateThreadUpdateTiingoCryptoSymbolDB() {
	thread thread1(ThreadUpdateTiingoCryptoSymbolDB, gl_pWorldMarket);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
}

void CWorldMarket::CreateThreadUpdateForexExchangeDB() {
	thread thread1(ThreadUpdateForexExchangeDB, gl_pWorldMarket);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
}

void CWorldMarket::CreateThreadUpdateForexSymbolDB() {
	thread thread1(ThreadUpdateForexSymbolDB, gl_pWorldMarket);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
}

void CWorldMarket::CreateThreadUpdateCryptoExchangeDB() {
	thread thread1(ThreadUpdateCryptoExchangeDB, gl_pWorldMarket);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
}

void CWorldMarket::CreateThreadUpdateFinnhubCryptoSymbolDB() {
	thread thread1(ThreadUpdateFinnhubCryptoSymbolDB, gl_pWorldMarket);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
}

void CWorldMarket::CreateThreadUpdateEconomicCalendarDB() {
	thread thread1(ThreadUpdateEconomicCalendarDB, gl_pWorldMarket);
	thread1.detach();
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
	for (long i = 0; i < GetStockSize(); i++) {
		const CWorldStockPtr pStock = GetStock(i);
		pStock->UpdateDayLineDB();
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}
	return true;
}

bool CWorldMarket::UpdateCompanyNewsDB() {
	for (long l = 0; l < m_containerStock.Size(); l++) {
		const auto pStock = m_containerStock.GetStock(l);
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
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}
	return true;
}

bool CWorldMarket::UpdateInsiderSentimentDB() {
	for (long i = 0; i < GetStockSize(); i++) {
		const CWorldStockPtr pStock = GetStock(i);
		if (pStock->IsSaveInsiderSentimentAndClearFlag()) {
			if (pStock->HaveInsiderSentiment()) {
				pStock->SaveInsiderSentiment();
				const CString str = pStock->GetSymbol() + _T("�ڲ������������ϸ������");
				gl_systemMessage.PushDayLineInfoMessage(str);
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
	m_containerStock.ResetEPSSurprise();
	gl_pFinnhubDataSource->SetUpdateEPSSurprise(true);
}

void CWorldMarket::RebuildPeer() {
	m_containerStock.ResetPeer();
	gl_pFinnhubDataSource->SetUpdatePeer(true);
}

void CWorldMarket::RebuildBasicFinancial() {
	m_containerStock.ResetBasicFinancial();
	gl_pFinnhubDataSource->SetUpdateStockBasicFinancial(true);
}

void CWorldMarket::RebuildStockDayLineDB() {
	m_containerStock.ResetDayLine();
	gl_pFinnhubDataSource->SetUpdateStockProfile(true);
}

void CWorldMarket::UpdateStockDayLineStartEndDate() {
	try {
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

	for (long l = 0; l < m_containerChosenStock.Size(); l++) {
		const CWorldStockPtr pStock = m_containerChosenStock.GetStock(l);
		vSymbol.push_back(pStock->GetSymbol().GetBuffer());
	}

	for (long l = 0; l < m_containerChosenForex.Size(); l++) {
		const CForexSymbolPtr pForex = m_containerChosenForex.GetForexSymbol(l);
		vSymbol.push_back(pForex->GetSymbol().GetBuffer());
	}

	for (long l = 0; l < m_containerChosenCrypto.Size(); l++) {
		const CFinnhubCryptoSymbolPtr pCrypto = m_containerChosenCrypto.GetCryptoSymbol(l);
		vSymbol.push_back(pCrypto->GetSymbol().GetBuffer());
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
// thresholdLevel 0�������е�IEX����ʱ��ÿ��������Ϊ1M-9M;thresholdLevel5��������IEX����ʱ��ÿ��������Ϊ10-50K��
//
// thresholdLevel 5��all Last Trade updates and only Quote updates that are deemed major updates by our system.
// thresholdLevel 0: ALL Top-of-Book AND Last Trade updates.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vectorString CWorldMarket::GetTiingoIEXWebSocketSymbolVector() {
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
vectorString CWorldMarket::GetTiingoCryptoWebSocketSymbolVector() {
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
vectorString CWorldMarket::GetTiingoForexWebSocketSymbolVector() {
	vectorString vSymbol;
	for (long l = 0; l < m_containerChosenForex.Size(); l++) {
		const CForexSymbolPtr pForex = m_containerChosenForex.GetForexSymbol(l);
		vSymbol.push_back(pForex->GetSymbol().GetBuffer());
	}

	return vSymbol;
}

void CWorldMarket::StartAllWebSocket() {
	if (IsSystemReady()) {
		if (!gl_pFinnhubDataSource->IsWebError() && gl_pFinnhubWebSocket->IsIdle()) StartFinnhubWebSocket();
		if (!gl_pTiingoDataSource->IsWebError()) {
			if (gl_pTiingoIEXWebSocket->IsIdle()) StartTiingoIEXWebSocket();
			if (gl_pTiingoCryptoWebSocket->IsIdle()) StartTiingoCryptoWebSocket();
			if (gl_pTiingoForexWebSocket->IsIdle()) StartTiingoForexWebSocket();
		}
	}
}

void CWorldMarket::StartFinnhubWebSocket() {
	if (gl_systemConfiguration.IsUsingFinnhubWebSocket() && !gl_pFinnhubDataSource->IsTimeout()) {
		gl_pFinnhubWebSocket->CreateThreadConnectWebSocketAndSendMessage(GetFinnhubWebSocketSymbolVector());
		gl_pFinnhubWebSocket->SetHeartbeatTime(GetUTCTime());
	}
}

void CWorldMarket::StartTiingoIEXWebSocket() {
	if (gl_systemConfiguration.IsUsingTiingoIEXWebSocket() && !gl_pTiingoDataSource->IsTimeout()) {
		gl_pTiingoIEXWebSocket->CreateThreadConnectWebSocketAndSendMessage(GetTiingoIEXWebSocketSymbolVector());
		gl_pTiingoIEXWebSocket->SetHeartbeatTime(GetUTCTime());
	}
}

void CWorldMarket::StartTiingoCryptoWebSocket() {
	if (gl_systemConfiguration.IsUsingTiingoCryptoWebSocket() && !gl_pTiingoDataSource->IsTimeout()) {
		gl_pTiingoCryptoWebSocket->CreateThreadConnectWebSocketAndSendMessage(GetTiingoCryptoWebSocketSymbolVector());
		gl_pTiingoCryptoWebSocket->SetHeartbeatTime(GetUTCTime());
	}
}

void CWorldMarket::StartTiingoForexWebSocket() {
	if (gl_systemConfiguration.IsUsingTiingoForexWebSocket() && !gl_pTiingoDataSource->IsTimeout()) {
		gl_pTiingoForexWebSocket->CreateThreadConnectWebSocketAndSendMessage(GetTiingoForexWebSocketSymbolVector());
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

void CWorldMarket::StopFinnhubWebSocketIfTimeOut() {
	if ((gl_pFinnhubDataSource->IsTimeout()) || gl_pFinnhubWebSocket->IsIdle()) {
		gl_pFinnhubWebSocket->CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Finnhub web����#12002���ر�Web socket����"));
	}
}

void CWorldMarket::StopTiingoIEXWebSocketIfTimeOut() {
	if ((gl_pTiingoDataSource->IsTimeout()) && !gl_pTiingoIEXWebSocket->IsClosed()) {
		gl_pTiingoIEXWebSocket->CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo IEX web����#12002���ر�Web socket����"));
	}
}

void CWorldMarket::StopTiingoCryptoWebSocketIfTimeOut() {
	if ((gl_pTiingoDataSource->IsTimeout()) && !gl_pTiingoCryptoWebSocket->IsClosed()) {
		gl_pTiingoCryptoWebSocket->CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Crypto web����#12002���ر�Web socket����"));
	}
}

void CWorldMarket::StopTiingoForexWebSocketIfTimeOut() {
	if ((gl_pTiingoDataSource->IsTimeout()) && !gl_pTiingoForexWebSocket->IsClosed()) {
		gl_pTiingoForexWebSocket->CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Forex web����#12002���ر�Web socket����"));
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
		auto pString = gl_pFinnhubWebSocket->PopData();
		CString strMessage = _T("Finnhub: ");
		strMessage += (*pString).c_str();
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
		auto pString = gl_pTiingoIEXWebSocket->PopData();
		CString strMessage = _T("Tiingo IEX: ");
		strMessage += (*pString).c_str();
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
		auto pString = gl_pTiingoCryptoWebSocket->PopData();
		CString strMessage = _T("Tiingo Crypto: ");
		strMessage += (*pString).c_str();
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
		auto pString = gl_pTiingoForexWebSocket->PopData();
		CString strMessage = _T("Tiingo Forex: ");
		strMessage += (*pString).c_str();
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
}

void CWorldMarket::UpdateWorldStockFromFinnhubSocket(const CFinnhubSocketPtr& pFinnhubData) {
	if (IsStock(pFinnhubData->m_sSymbol.c_str())) {
		const CWorldStockPtr pStock = GetStock(pFinnhubData->m_sSymbol.c_str());
		pStock->SetActive(true);
		pStock->SetNew(pFinnhubData->m_dLastPrice * 1000);
	}
}

void CWorldMarket::UpdateMarketStatus(CMarketStatusVectorPtr pv) {
	for (auto p : *pv) {
		m_pvMarketStatus->push_back(p);
	}
}

void CWorldMarket::UpdateMarketHoliday(CMarketHolidayVectorPtr pv) {
	for (auto p : *pv) {
		m_pvMarketHoliday->push_back(p);
	}
}
