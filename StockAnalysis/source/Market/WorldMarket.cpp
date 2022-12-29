#include"pch.h"

#include "WorldMarket.h"
#include"thread.h"

#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"

#include"FinnhubWebInquiry.h"
#include"TiingoWebInquiry.h"
#include"QuandlWebInquiry.h"

#include"FinnhubWebSocket.h"
#include"TiingoIEXWebSocket.h"
#include"TiingoForexWebSocket.h"
#include"TiingoCryptoWebSocket.h"

#include"SetWorldStockDayLine.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>

#include<thread>
#include<memory>
using std::thread;
using std::make_shared;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CWorldMarket, CVirtualMarket)

CWorldMarket::CWorldMarket() {
	if (static int siInstance = 0; ++siInstance > 1) { TRACE("CWorldMarket�г�����ֻ�������һ��ʵ��\n"); }

	// ���裨Ҳ�޷���ÿ�ո��µı�����������
	m_lCurrentUpdateEPSSurprisePos = 0;
	m_lCurrentUpdateDayLinePos = 0; // ����֤ȯ���������ж�ʮ��֮�࣬�޷���һ��֮�ڸ����꣬�ʲ������ô�������
	m_bFinnhubWebSiteAccessible = true;

	m_strMarketId = _T("�����г�");
	m_lMarketTimeZone = 4 * 3600; // ��������ʹ��������׼ʱ��, GMT + 4
	CalculateTime();

	Reset();
}

CWorldMarket::~CWorldMarket() { PreparingExitMarket(); }

void CWorldMarket::Reset(void) {
	ResetFinnhub();
	ResetTiingo();
	ResetDataClass();
}

void CWorldMarket::ResetFinnhub(void) {
	m_dataFinnhubStockExchange.Reset();
	m_dataFinnhubForexExchange.Reset();

	m_fRebuildDayLine = false;
	SetSystemReady(false); // �г���ʼ״̬Ϊδ���úá�
}

void CWorldMarket::ResetQuandl(void) {}

void CWorldMarket::ResetTiingo(void) {}

void CWorldMarket::ResetDataClass(void) {
	m_dataFinnhubStockExchange.Reset();
	m_dataFinnhubForexExchange.Reset();
	m_dataFinnhubCryptoExchange.Reset();
	m_dataFinnhubForexSymbol.Reset();
	m_dataFinnhubCryptoSymbol.Reset();
	m_dataFinnhubCountry.Reset();
	m_dataFinnhubEconomicCalendar.Reset();

	m_dataWorldStock.Reset();
	m_dataTiingoStock.Reset();

	m_dataChosenStock.Reset();
	m_dataChosenForex.Reset();
	m_dataChosenCrypto.Reset();
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

	for (const auto& pDataSource : m_vDataSource) { pDataSource->Reset(); }

	CString str = _T("����World Market��������׼ʱ�䣺");
	str += GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(str);
}

bool CWorldMarket::PreparingExitMarket(void) {
	ASSERT(gl_systemStatus.IsExitingSystem());
	DisconnectAllWebSocket();

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Ĭ��ÿ100����ִ��һ�Ρ�
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::SchedulingTask(void) {
	CVirtualMarket::SchedulingTask();

	static time_t s_lastTimeSchedulingTask = 0;
	const long lCurrentTime = GetMarketTime();

	TaskCheckSystemReady();

	// ���ø�Web data source�������������ݵĽ��պʹ���
	InquireAndProcessDataSource(lCurrentTime);

	//����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
	if (GetUTCTime() > s_lastTimeSchedulingTask) {
		SchedulingTaskPerSecond(GetUTCTime() - s_lastTimeSchedulingTask, lCurrentTime);
		s_lastTimeSchedulingTask = GetUTCTime();
	}

	return true;
}

bool CWorldMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
	static int s_iCount1Hour = 3576; // �������ÿ�εĴ�11���ӣ���һ����ÿ�εĴ�22����
	static int s_iCount5Minute = 287; // ��һ����ÿ�εĴ�11����
	static int s_iCount1Minute = 58; // ��10��ÿ�εĴ�1����
	static int s_iCount10Second = 9;

	s_iCount10Second -= lSecond;
	s_iCount1Minute -= lSecond;
	s_iCount5Minute -= lSecond;
	s_iCount1Hour -= lSecond;
	if (s_iCount1Hour < 0) {
		s_iCount1Hour = 3599;
		SchedulingTaskPerHour(lCurrentTime);
	}
	if (s_iCount5Minute < 0) {
		s_iCount5Minute = 299;
		SchedulingTaskPer5Minute(lCurrentTime);
	}
	if (s_iCount1Minute < 0) {
		s_iCount1Minute = 59;
		SchedulingTaskPerMinute(lCurrentTime);
	}
	if (s_iCount10Second < 0) {
		s_iCount10Second = 9;
		SchedulingTaskPer10Seconds(lCurrentTime);
	}

	if (!IsTimeToResetSystem(lCurrentTime)) {
		// ������ʱ����ϵͳ�������ݿ���Ҫ����װ�룬�ʶ���ʱ��������¡�
		TaskProcessWebSocketData();
		TaskUpdateWorldStockFromWebSocket();
	}

	return true;
}

bool CWorldMarket::SchedulingTaskPer10Seconds(long lCurrentTime) {
	StopAllWebSocketIfOutOfTime();
	return true;
}

bool CWorldMarket::SchedulingTaskPerMinute(long lCurrentTime) {
	// ����WebSocket����
	StartAllWebSocket();

	TaskResetMarket(lCurrentTime);

	// ������������һ������������ִ����Ϻ󷵻��ˡ�
	if (!IsTimeToResetSystem(lCurrentTime)) {
		// ������ʱ����ϵͳ�������ݿ���Ҫ����װ�룬�ʶ���ʱ������������ݿ⡣
		if (m_dataFinnhubCountry.GetLastTotalCountry() < m_dataFinnhubCountry.GetTotalCountry()) { TaskUpdateCountryListDB(); }
		if (IsNeedUpdateForexExchangeDB()) TaskUpdateForexExchangeDB();
		if (IsNeedUpdateForexSymbolDB()) TaskUpdateForexSymbolDB();
		if (IsNeedUpdateCryptoExchangeDB()) TaskUpdateCryptoExchangeDB();
		if (IsNeedUpdateCryptoSymbolDB()) TaskUpdateFinnhubCryptoSymbolDB();
		if (IsNeedUpdateInsiderTransactionDB()) TaskUpdateInsiderTransactionDB();
		if (IsNeedUpdateInsiderSentimentDB()) TaskUpdateInsiderSentimentDB();
		TaskUpdateForexDayLineDB();
		TaskUpdateCryptoDayLineDB();
		if (IsNeedSaveStockDayLineDB()) TaskUpdateDayLineDB();
		TaskUpdateEPSSurpriseDB();
		if (IsNeedUpdateEconomicCalendarDB()) TaskUpdateEconomicCalendarDB();
		return true;
	}

	return false;
}

bool CWorldMarket::SchedulingTaskPer5Minute(long lCurrentTime) {
	if (IsCompanyNewsNeedUpdate()) TaskUpdateCompanyNewsDB();
	if (IsBasicFinancialNeedUpdate()) TaskUpdateBasicFinancialDB();
	if (IsNeedUpdateTiingoStock()) TaskUpdateTiingoStockDB();
	if (IsNeedUpdateTiingoCryptoSymbol()) TaskUpdateTiingoCryptoSymbolDB();

	// ���¹�Ʊ���������÷������
	if (gl_pFinnhubDataSource->IsSymbolUpdated() && IsStockProfileNeedUpdate()) { TaskUpdateStockProfileDB(); }

	return true;
}

bool CWorldMarket::SchedulingTaskPerHour(long lCurrentTime) { return true; }

/// <summary>
/// /////////////////////////////////////////////////////////////////////////////////
///
/// ��������������ÿ����ִ��һ�εĵ����С�
/// <returns></returns>
///
bool CWorldMarket::TaskResetMarket(long lCurrentTime) {
	// �г�ʱ��ʮ��ʱ����ϵͳ
	if (HaveResetMarketPermission()) {
		// �����������ϵͳ
		if ((lCurrentTime > 170000) && (lCurrentTime <= 170100)) {
			// ���г�ʱ���������ʱ(����ʱ��������ʱ�������г������������ڽ����������ݡ�
			SetResetMarket(true); // ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
			SetResetMarketPermission(false); // ���첻����������ϵͳ��
			SetSystemReady(false);
			return true;
		}
	}
	return false;
}

bool CWorldMarket::TaskUpdateTiingoIndustry(void) {
	if (gl_pFinnhubDataSource->IsStockProfileUpdated()) {
		thread thread1(ThreadUpdateTiingoIndustry, this);
		thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateSICIndustry(void) {
	if (gl_pFinnhubDataSource->IsStockProfileUpdated()) {
		thread thread1(ThreadUpdateSICIndustry, this);
		thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateNaicsIndustry(void) {
	if (gl_pFinnhubDataSource->IsStockProfileUpdated()) {
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

	for (int i = 0; i < m_dataFinnhubForexSymbol.GetForexSymbolSize(); i++) {
		if (gl_systemStatus.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
		pSymbol = m_dataFinnhubForexSymbol.GetForexSymbol(i);
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) {
			// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					thread thread1(ThreadUpdateForexDayLineDB, pSymbol.get());
					thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
					fUpdated = true;
					TRACE("����%s��������\n", pSymbol->GetSymbol().GetBuffer());
				}
				else pSymbol->UnloadDayLine(); // ������ִ�д洢����ʱ�����ﻹҪ����ж���������ݡ���洢���������߳��Ժ��ִ�У��ʶ������ڴ�ͳһִ��ɾ��������
			}
			else {
				// �������Ϊ�й�Ʊ���룬���˴�����δ����
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

	for (int i = 0; i < m_dataFinnhubCryptoSymbol.GetCryptoSymbolSize(); ++i) {
		if (gl_systemStatus.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
		pSymbol = m_dataFinnhubCryptoSymbol.GetCryptoSymbol(i);
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) {
			// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					thread thread1(ThreadUpdateCryptoDayLineDB, pSymbol.get());
					thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
					fUpdated = true;
					//TRACE("����%s��������\n", pSymbol->GetSymbol().GetBuffer());
				}
				else pSymbol->UnloadDayLine(); // ������ִ�д洢����ʱ�����ﻹҪ����ж���������ݡ���洢���������߳��Ժ��ִ�У��ʶ������ڴ�ͳһִ��ɾ��������
			}
			else {
				// �������Ϊ�й�Ʊ���룬���˴�����δ���У������������й�Ʊ
				if (pSymbol->GetDayLineEndDate() > 19800101) {
					// ������
					pSymbol->SetIPOStatus(_STOCK_DELISTED_);
					pSymbol->SetUpdateProfileDB(true);
				}
				else {
					// �������Ϊ�й�Ʊ���룬���˴�����δ����
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
	for (long l = 0; l < m_dataWorldStock.GetStockSize(); ++l) {
		pStock = m_dataWorldStock.GetStock(l);
		if (pStock->IsEPSSurpriseNeedSaveAndClearFlag()) {
			// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
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

bool CWorldMarket::TaskCheckSystemReady(void) {
	if (!IsSystemReady()) {
		if (gl_pFinnhubDataSource->IsSymbolUpdated() && gl_pFinnhubDataSource->IsForexExchangeUpdated() &&
			gl_pFinnhubDataSource->IsForexSymbolUpdated()
			&& gl_pFinnhubDataSource->IsCryptoExchangeUpdated() && gl_pFinnhubDataSource->IsCryptoSymbolUpdated()) {
			const CString str = "�����г���ʼ�����";
			gl_systemMessage.PushInformationMessage(str);
			SetSystemReady(true);
			return true;
		}
		return false;
	}
	return true;
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

bool CWorldMarket::TaskUpdateStockProfileDB(void) {
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

	if (gl_systemConfiguration.GetFinnhubToken().GetLength() > 5) { gl_pFinnhubWebInquiry->SetInquiryToken(gl_systemConfiguration.GetFinnhubToken()); }
	else { gl_systemMessage.PushInformationMessage(_T("Finnhub Token Needed")); }
	if (gl_systemConfiguration.GetTiingoToken().GetLength() > 5) { gl_pTiingoWebInquiry->SetInquiryToken(gl_systemConfiguration.GetTiingoToken()); }
	else { gl_systemMessage.PushInformationMessage(_T("Tiingo Token Needed")); }
	if (gl_systemConfiguration.GetQuandlToken().GetLength() > 5) { gl_pQuandlWebInquiry->SetInquiryToken(gl_systemConfiguration.GetQuandlToken()); }
	else { gl_systemMessage.PushInformationMessage(_T("Quandl Token Needed")); }

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
	for (long l = 0; l < m_dataWorldStock.GetStockSize(); l++) {
		const auto pStock = m_dataWorldStock.GetStock(l);
		if (pStock->IsUpdateCompanyNewsDBAndClearFlag()) {
			// �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			pStock->UpdateCompanyNewsDB();
			TRACE("����%s company news����\n", pStock->GetSymbol().GetBuffer());
		}
		if (gl_systemStatus.IsExitingSystem()) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}

	return (true);
}

bool CWorldMarket::UpdateInsiderTransactionDB(void) {
	CWorldStockPtr pStock = nullptr;

	for (long i = 0; i < GetStockSize(); i++) {
		pStock = GetStock(i);
		if (pStock->IsInsiderTransactionNeedSaveAndClearFlag()) {
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
		if (pStock->IsInsiderSentimentNeedSaveAndClearFlag()) {
			if (pStock->HaveInsiderSentiment()) {
				pStock->SaveInsiderSentiment();
				const CString str = pStock->GetSymbol() + _T("�ڲ������������ϸ������");
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
	m_dataWorldStock.ResetEPSSurprise();
	gl_pFinnhubDataSource->SetEPSSurpriseUpdated(false);

	return true;
}

bool CWorldMarket::RebuildPeer(void) {
	m_dataWorldStock.ResetPeer();
	gl_pFinnhubDataSource->SetPeerUpdated(false);

	return true;
}

bool CWorldMarket::RebuildBasicFinancial(void) {
	m_dataWorldStock.ResetBasicFinancial();
	gl_pFinnhubDataSource->SetStockBasicFinancialUpdated(false);

	return true;
}

bool CWorldMarket::RebuildStockDayLineDB(void) {
	m_dataWorldStock.ResetDayLine();
	gl_pFinnhubDataSource->SetStockProfileUpdated(false);

	return true;
}

bool CWorldMarket::UpdateStockDayLineStartEndDate(void) {
	const CString strFilterPrefix = _T("[Symbol] = '");
	CSetWorldStockDayLine setWorldStockDayLine;

	for (long l = 0; l < m_dataWorldStock.GetStockSize(); l++) {
		const CWorldStockPtr pStock = m_dataWorldStock.GetStock(l);
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
/// <param name=""></param>
/// <returns></returns>
vector<CString> CWorldMarket::GetFinnhubWebSocketSymbolVector(void) {
	vector<CString> vSymbol;

	CWorldStockPtr pStock = nullptr;
	for (long l = 0; l < m_dataChosenStock.GetSize(); l++) {
		pStock = m_dataChosenStock.GetStock(l);
		vSymbol.push_back(pStock->GetSymbol());
	}

	CFinnhubCryptoSymbolPtr pCrypto = nullptr;
	for (long l = 0; l < m_dataChosenCrypto.GetSize(); l++) {
		pCrypto = m_dataChosenCrypto.GetCrypto(l);
		vSymbol.push_back(pCrypto->GetSymbol());
	}

	CForexSymbolPtr pForex = nullptr;
	for (long l = 0; l < m_dataChosenForex.GetSize(); l++) {
		pForex = m_dataChosenForex.GetForex(l);
		vSymbol.push_back(pForex->GetSymbol());
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
vector<CString> CWorldMarket::GetTiingoIEXWebSocketSymbolVector(void) {
	vector<CString> vSymbol;
	for (long l = 0; l < m_dataChosenStock.GetSize(); l++) {
		const CWorldStockPtr pStock = m_dataChosenStock.GetStock(l);
		vSymbol.push_back(pStock->GetSymbol());
	}

	return vSymbol;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// �������е�Crypto����ʱ��ÿ��������������50-100K������
//
// thresholdlevel 2: Top-of-Book AND Last Trade updates.
// thresholdlevel 5: only Last Trade updates.
//
//////////////////////////////////////////////////////////////////////////////////////////////
vector<CString> CWorldMarket::GetTiingoCryptoWebSocketSymbolVector(void) {
	vector<CString> vSymbol;
	for (long l = 0; l < m_dataChosenCrypto.GetSize(); l++) {
		const CFinnhubCryptoSymbolPtr pCrypto = m_dataChosenCrypto.GetCrypto(l);
		vSymbol.push_back(pCrypto->GetSymbol());
	}

	return vSymbol;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 5״̬��ÿ�����100K���ҡ�
//
// threshlodlevel 5: ALL Top-of-Book updates
// thresholdlevel 7: A top-of-book update that is due to a change in either the bid/ask price or size.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
vector<CString> CWorldMarket::GetTiingoForexWebSocketSymbolVector(void) {
	vector<CString> vSymbol;
	for (long l = 0; l < m_dataChosenForex.GetSize(); l++) {
		const CForexSymbolPtr pForex = m_dataChosenForex.GetForex(l);
		vSymbol.push_back(pForex->GetSymbol());
	}

	return vSymbol;
}

bool CWorldMarket::StartAllWebSocket(void) {
	if (IsSystemReady()) {
		if (!gl_pFinnhubWebInquiry->IsWebError()) StartFinnhubWebSocket();
		if (!gl_pTiingoWebInquiry->IsWebError()) {
			StartTiingoIEXWebSocket();
			StartTiingoCryptoWebSocket();
			StartTiingoForexWebSocket();
		}
	}
	return true;
}

void CWorldMarket::StartFinnhubWebSocket(void) { if (gl_systemConfiguration.IsUsingFinnhubWebSocket() && !gl_pFinnhubWebInquiry->IsTimeout()) { if (gl_finnhubWebSocket.IsClosed()) { gl_finnhubWebSocket.CreateThreadConnectWebSocketAndSendMessage(GetFinnhubWebSocketSymbolVector()); } } }

void CWorldMarket::StartTiingoIEXWebSocket(void) { if (gl_systemConfiguration.IsUsingTiingoIEXWebSocket() && !gl_pTiingoWebInquiry->IsTimeout()) { if (gl_tiingoIEXWebSocket.IsClosed()) { gl_tiingoIEXWebSocket.CreatingThreadConnectWebSocketAndSendMessage(GetTiingoIEXWebSocketSymbolVector()); } } }

void CWorldMarket::StartTiingoCryptoWebSocket(void) { if (gl_systemConfiguration.IsUsingTiingoCryptoWebSocket() && !gl_pTiingoWebInquiry->IsTimeout()) { if (gl_tiingoCryptoWebSocket.IsClosed()) { gl_tiingoCryptoWebSocket.CreateThreadConnectWebSocketAndSendMessage(GetTiingoCryptoWebSocketSymbolVector()); } } }

void CWorldMarket::StartTiingoForexWebSocket(void) { if (gl_systemConfiguration.IsUsingTiingoForexWebSocket() && !gl_pTiingoWebInquiry->IsTimeout()) { if (gl_tiingoForexWebSocket.IsClosed()) { gl_tiingoForexWebSocket.CreatingThreadConnectWebSocketAndSendMessage(GetTiingoForexWebSocketSymbolVector()); } } }

/// <summary>
/// // ֹͣWebSocket���˺��������ɹ����߳���ֹͣWebSocket�����õȴ���ֹͣ�����ء�����ϵͳ�����е�ֹͣ������
/// </summary>
/// <param name=""></param>
void CWorldMarket::DisconnectAllWebSocket(void) {
	ASSERT(gl_systemStatus.IsExitingSystem()); //������ֻ��ϵͳ�˳�ʱ����
	if (gl_systemConfiguration.IsUsingFinnhubWebSocket()) gl_finnhubWebSocket.Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoIEXWebSocket()) gl_tiingoIEXWebSocket.Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoCryptoWebSocket()) gl_tiingoCryptoWebSocket.Disconnect();
	if (gl_systemConfiguration.IsUsingTiingoForexWebSocket()) gl_tiingoForexWebSocket.Disconnect();
}

/// <summary>
/// ֹͣWebSocket���˺��������ɹ����߳���ֹͣWebSocket�����õȴ���ֹͣ�����ء�����ϵͳ�����е�ֹͣ������
/// </summary>
/// <param name=""></param>
void CWorldMarket::StopAllWebSocketIfOutOfTime(void) {
	if (IsSystemReady()) {
		StopFinnhubWebSocketIfOutOfTime();
		StopTiingoIEXWebSocketIfOutOfTime();
		StopTiingoCryptoWebSocketIfOutOfTime();
		StopTiingoForexWebSocketIfOutOfTime();
	}
}

void CWorldMarket::StopFinnhubWebSocketIfOutOfTime(void) {
	if ((gl_pFinnhubWebInquiry->IsTimeout()) && !gl_finnhubWebSocket.IsClosed()) {
		gl_finnhubWebSocket.CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Finnhub web����12002���ر�Web socket����"));
	}
}

void CWorldMarket::StopTiingoIEXWebSocketIfOutOfTime(void) {
	if ((gl_pTiingoWebInquiry->IsTimeout()) && !gl_tiingoIEXWebSocket.IsClosed()) {
		gl_tiingoIEXWebSocket.CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo IEX web����12002���ر�Web socket����"));
	}
}

void CWorldMarket::StopTiingoCryptoWebSocketIfOutOfTime(void) {
	if ((gl_pTiingoWebInquiry->IsTimeout()) && !gl_tiingoCryptoWebSocket.IsClosed()) {
		gl_tiingoCryptoWebSocket.CreateThreadDisconnectWebSocket();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Crypto web����12002���ر�Web socket����"));
	}
}

void CWorldMarket::StopTiingoForexWebSocketIfOutOfTime(void) {
	if ((gl_pTiingoWebInquiry->IsTimeout()) && !gl_tiingoForexWebSocket.IsClosed()) {
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
	int iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		auto pString = gl_finnhubWebSocket.PopData();
		CString strMessage = "Finnhub: ";
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
		CString strMessage = "Tiingo IEX: ";
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

	int iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		auto pString = gl_tiingoCryptoWebSocket.PopData();
		CString strMessage = "Tiingo Crypto: ";
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
	int iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		auto pString = gl_tiingoForexWebSocket.PopData();
		CString strMessage = "Tiingo Forex: ";
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
/// <param name=""></param>
/// <returns></returns>
bool CWorldMarket::TaskUpdateWorldStockFromWebSocket(void) {
	//CWorldStockPtr pStock;

	auto total = gl_SystemData.GetTiingoIEXSocketSize();
	for (auto i = 0; i < total; i++) {
		const CTiingoIEXSocketPtr pIEXData = gl_SystemData.PopTiingoIEXSocket();
		UpdateWorldStockFromTiingoIEXSocket(pIEXData);
		gl_systemMessage.SetCurrentTiingoWebSocketIEX(pIEXData->m_strSymbol);
	}

	total = gl_SystemData.GetTiingoCryptoSocketSize();
	for (auto i = 0; i < total; i++) {
		const CTiingoCryptoSocketPtr pCryptoData = gl_SystemData.PopTiingoCryptoSocket();
		gl_systemMessage.SetCurrentTiingoWebSocketCrypto(pCryptoData->m_strSymbol);
	}

	total = gl_SystemData.GetTiingoForexSocketSize();
	for (auto i = 0; i < total; i++) {
		const CTiingoForexSocketPtr pForexData = gl_SystemData.PopTiingoForexSocket();
		gl_systemMessage.SetCurrentTiingoWebSocketForex(pForexData->m_strSymbol);
	}

	total = gl_SystemData.GetFinnhubSocketSize();
	for (auto i = 0; i < total; i++) {
		const CFinnhubSocketPtr pFinnhubData = gl_SystemData.PopFinnhubSocket();
		UpdateWorldStockFromFinnhubSocket(pFinnhubData);
		gl_systemMessage.SetCurrentFinnhubWebSocketStake(pFinnhubData->m_strSymbol);
	}

	return true;
}

bool CWorldMarket::UpdateWorldStockFromTiingoIEXSocket(CTiingoIEXSocketPtr pTiingoIEXbData) {
	CWorldStockPtr pStock = nullptr;

	if (IsStock(pTiingoIEXbData->m_strSymbol)) {
		pStock = GetStock(pTiingoIEXbData->m_strSymbol);
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
	if (IsStock(pFinnhubData->m_strSymbol)) {
		const CWorldStockPtr pStock = GetStock(pFinnhubData->m_strSymbol);
		pStock->SetActive(true);
		pStock->SetNew(pFinnhubData->m_dLastPrice * 1000);
	}

	return true;
}
