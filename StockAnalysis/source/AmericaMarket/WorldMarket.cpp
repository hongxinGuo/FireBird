#include"pch.h"

#include "WorldMarket.h"
#include"thread.h"
#include"Callablefunction.h"

#include"ProductFinnhubStockPriceQuote.h"
#include"ProductFinnhubStockEstimatesEPSSurprise.h"
#include"ProductFinnhubForexSymbol.h"
#include"ProductFinnhubCryptoSymbol.h"

#include"WebInquirer.h"
#include"EPSSurprise.h"

#include"SetWorldMarketOption.h"
#include"SetFinnhubForexExchange.h"
#include"SetFinnhubCryptoExchange.h"
#include"SetWorldStock.h"
#include"SetWorldChoicedStock.h"
#include"SetWorldChoicedForex.h"
#include"SetWorldChoicedCrypto.h"
#include"SetWorldStockDayLine.h"
#include"SetCountry.h"
#include"SetEconomicCalendar.h"
#include"SetEPSSurprise.h"
#include"SetTiingoStock.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include<ixwebsocket/IXWebSocketMessage.h>

using namespace std;
#include<algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Semaphore gl_UpdateWorldMarketDB(1);  // ���ź�����������������Ʊ������ʷ���ݿ�

IMPLEMENT_DYNCREATE(CWorldMarket, CVirtualMarket)

CWorldMarket::CWorldMarket() {
	static int siInstance = 0;

	if (++siInstance > 1) {
		TRACE("CWorldMarket�г�����ֻ�������һ��ʵ��\n");
	}

	// ����ÿ�ո��µı�����������
	m_fFinnhubEPSSurpriseUpdated = true;
	m_lCurrentUpdateEPSSurprisePos = 0;
	m_lCurrentUpdateDayLinePos = 0; // ����֤ȯ���������ж�ʮ��֮�࣬�޷���һ��֮�ڸ����꣬�ʲ������ô�������

	m_strMarketId = _T("�����г�");
	m_lMarketTimeZone = 4 * 3600; // ��������ʹ��������׼ʱ�䡣
	CalculateTime();

	Reset();
}

CWorldMarket::~CWorldMarket() {
	PreparingExitMarket();
}

void CWorldMarket::Reset(void) {
	ResetFinnhub();
	ResetTiingo();
	ResetDataClass();
}

void CWorldMarket::ResetFinnhub(void) {
	m_lCurrentProfilePos = 0;
	m_lCurrentRTDataQuotePos = 0;
	m_lCurrentForexExchangePos = 0;
	m_lCurrentForexSymbolPos = 0;
	m_lCurrentCryptoExchangePos = 0;
	m_lCurrentCryptoSymbolPos = 0;

	m_pCurrentFinnhubProduct = nullptr;

	m_dataFinnhubStockExchange.Reset();
	m_dataFinnhubForexExchange.Reset();

	// Finnhub�������������ݱ�ʶ��ÿ����Ҫ���á�
	m_fFinnhubSymbolUpdated = false; // ÿ����Ҫ���´���
	m_fFinnhubStockProfileUpdated = false;
	m_fFinnhubDayLineUpdated = false;
	m_fFinnhubForexExchangeUpdated = false;
	m_fFinnhubForexSymbolUpdated = false;
	m_fFinnhubForexDayLineUpdated = false;
	m_fFinnhubCryptoExchangeUpdated = false;
	m_fFinnhubCryptoSymbolUpdated = false;
	m_fFinnhubCryptoDayLineUpdated = false;
	m_fCountryListUpdated = false;
	m_fFinnhubEconomicCalendarUpdated = false;

	m_fInquiringFinnhubStockSymbol = false;
	m_fInquiringFinnhubStockProfile = false;
	m_fInquiringFinnhubStockPeer = false;
	m_fInquiringFinnhubStockInsiderTransaction = false;
	m_fInquiringFinnhubForexDayLine = false;
	m_fInquiringFinnhubCryptoDayLine = false;
	m_fInquiringFinnhubStockDayLine = false;

	m_fFinnhubPeerUpdated = false;
	m_fFinnhubInsiderTransactionUpdated = false;
	m_lCurrentUpdatePeerPos = 0;
	m_lCurrentUpdateInsiderTransactionPos = 0;

	m_fFinnhubInquiring = false;
	m_fFinnhubDataReceived = true;

	m_fRebulidDayLine = false;
	SetSystemReady(false); // �г���ʼ״̬Ϊδ���úá�

	m_lCurrentUpdateForexDayLinePos = 0;
	m_lCurrentUpdateCryptoDayLinePos = 0;

	m_iProcessedFinnhubWebSocket = 0;
	m_iProcessedTiingoIEXWebSocket = 0;
	m_iProcessedTiingoCryptoWebSocket = 0;
	m_iProcessedTiingoForexWebSocket = 0;

	if (GetDayOfWeek() == 6) { // ÿ�ܵ�����������һ��EPSSurprise
		m_lCurrentUpdateEPSSurprisePos = 0;
		SetFinnhubEPSSurpriseUpdated(false);
	}
}

void CWorldMarket::ResetQuandl(void) {
	m_pCurrentQuandlProduct = nullptr;
}

void CWorldMarket::ResetTiingo(void) {
	m_pCurrentTiingoProduct = nullptr;

	m_fTiingoInquiring = false;
	m_fTiingoDataReceived = true;

	m_fTiingoStockSymbolUpdated = false;
	m_fTiingoCryptoSymbolUpdated = false;
	m_fTiingoDayLineUpdated = false;
}

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

	m_dataChoicedStock.Reset();
	m_dataChoicedForex.Reset();
	m_dataChoicedCrypto.Reset();
}

void CWorldMarket::ResetMarket(void) {
	Reset();

	LoadOption();
	LoadWorldExchangeDB(); // װ�����罻������Ϣ
	LoadCountryDB();
	LoadStockDB();
	LoadWorldChoicedStock();
	LoadForexExchange();
	LoadForexSymbol();
	LoadWorldChoicedForex();
	LoadCryptoExchange();
	LoadCryptoSymbol();
	LoadWorldChoicedCrypto();
	LoadEconomicCalendarDB();
	LoadTiingoStock();

	CString str = _T("����World Market��������׼ʱ�䣺");
	str += GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(str);
}

bool CWorldMarket::PreparingExitMarket(void)
{
	StopReceivingWebSocket();

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Ĭ��ÿ100����ִ��һ�Ρ�
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::SchedulingTask(void) {
	CVirtualMarket::SchedulingTask();

	static time_t s_timeLast = 0;
	static int s_iCountfinnhubLimit = 12; // Finnhub.ioÿ1.5����������һ�Σ��Է�ֹ����Ƶ�ʹ��ߵ������
	static int s_iCountTiingoLimit = 80; // ��֤ÿ80��ִ��һ�Σ���8��ÿ�Σ�.Tiingo����˻��ٶ�����ΪÿСʱ500�Σ� ÿ����9�Σ���ÿ��8�뼴�ɡ�
	const long lCurrentTime = GetMarketTime();

	TaskCheckSystemReady();

	if (--s_iCountfinnhubLimit < 0) {
		TaskInquiryFinnhub(lCurrentTime);
		if (IsFinnhubInquiring()) {
			s_iCountfinnhubLimit = 12; // ����������������ݣ������ü��������Ա�������һ�Ρ�
		}
	}
	ProcessFinnhubWebDataReceived(); // Ҫ�ȴ����յ���Finnhub��������
	ProcessFinnhubInquiringMessage(); // Ȼ�������봦����һ��

	if (--s_iCountTiingoLimit < 0) {
		s_iCountTiingoLimit = 80;
		TaskInquiryTiingo();
	}
	ProcessTiingoWebDataReceived(); // Ҫ�ȴ����յ���Tiingo��������
	ProcessTiingoInquiringMessage(); // Ȼ�������봦����һ��

//����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
	if (GetUTCTime() > s_timeLast) {
		SchedulingTaskPerSecond(GetUTCTime() - s_timeLast, lCurrentTime);
		s_timeLast = GetUTCTime();
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// finnhub��ȡ������������ͽ����ֻ�ִ�з�ʽ���ʶ����ٵ������β����һ���ֻء�
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::ProcessFinnhubInquiringMessage(void) {
	CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
	CString strTemp;
	CWorldStockPtr pStock = nullptr;
	CForexSymbolPtr pForexSymbol = nullptr;
	CFinnhubCryptoSymbolPtr pCryptoSymbol = nullptr;
	bool fDone = false;

	if (m_qFinnhubProduct.size() > 0) { // ������ȴ���
		ASSERT(IsFinnhubInquiring());
		if (IsFinnhubDataReceived()) { //�Ѿ�����������������Finnhub�����Ѿ����յ��ˣ�
			m_pCurrentFinnhubProduct = m_qFinnhubProduct.front();
			m_qFinnhubProduct.pop();

			gl_pFinnhubWebInquiry->SetInquiryingStringPrefix(m_pCurrentFinnhubProduct->CreatMessage()); // ����ǰ׺

			SetFinnhubDataReceived(false); // ���ô˱�ʶ��Ҫ�������������̣߳�GetWebData��֮ǰ���������߳��еĶ������׳���
			gl_pFinnhubWebInquiry->GetWebData();
			fDone = true;
		}
	}

	return fDone;
}

//////////////////////////////////////////////
//
// �������߳̽��յ���Finnhub������Ϣ��
//
// Ŀǰֻ����ͬʱ����һ����Ϣ������Ϣ��������������֮ǰ������������һ����Ϣ�������ܹ���֤ͬһ�ԡ�������Finnhub��վ���ٶ����ƣ�
// ÿ����ֻ����60�����룬�ʶ�û�б�Ҫǿ�������ٶȡ�
//
//////////////////////////////////////////////
bool CWorldMarket::ProcessFinnhubWebDataReceived(void) {
	CWebDataPtr pWebData = nullptr;
	bool fDone = false;

	if (m_pCurrentFinnhubProduct == nullptr) return false;

	if (IsFinnhubDataReceived()) { // ����������ݽ������
		if (gl_WebInquirer.GetFinnhubDataSize() > 0) {  // ����ǰ��������
			ASSERT(IsFinnhubInquiring());
			pWebData = gl_WebInquirer.PopFinnhubData();

			m_pCurrentFinnhubProduct->ProcessWebData(pWebData);

			gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(_T("")); // ��Щ��������û���õ��м��ַ��Σ���������֮ǰ���м��ַ��Σ�����еĻ���������������ַ����Ĵ���
			SetFinnhubInquiring(false);
			fDone = true;
			m_pCurrentFinnhubProduct = nullptr;
		}
	}

	return fDone;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����Tiingo���������ݣ�ʹ���̶߳�ȡTiingo��������
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CWorldMarket::ProcessTiingoInquiringMessage(void) {
	bool fDone = false;
	CString strMessage;

	if (m_qTiingoProduct.size() > 0) { // ������ȴ���
		ASSERT(IsTiingoInquiring());
		if (IsTiingoDataReceived()) { //�Ѿ�����������������Tiingo�����Ѿ����յ��ˣ�
			m_pCurrentTiingoProduct = m_qTiingoProduct.front();
			m_qTiingoProduct.pop();

			strMessage = m_pCurrentTiingoProduct->CreatMessage();
			gl_pTiingoWebInquiry->SetInquiryingStringPrefix(strMessage); // ���ô�����ʽʱ��ֻ����StringPrefix���ɣ����������ַ�����Ϊ�ա�

			SetTiingoDataReceived(false); // ���ô˱�ʶ��Ҫ�������������̣߳�GetWebData��֮ǰ���������߳��еĶ������׳���
			gl_pTiingoWebInquiry->GetWebData(); // ���ö�ȡ�������ݵ��߳�
			fDone = true;
		}
	}

	return fDone;
}

bool CWorldMarket::ProcessTiingoWebDataReceived(void) {
	CWebDataPtr pWebData = nullptr;
	bool fDone = false;

	if (m_pCurrentTiingoProduct == nullptr) return false;

	if (IsTiingoDataReceived()) { // ����������ݽ������
		if (gl_WebInquirer.GetTiingoDataSize() > 0) {  // ����ǰ��������
			ASSERT(IsTiingoInquiring());
			pWebData = gl_WebInquirer.PopTiingoData();

			m_pCurrentTiingoProduct->ProcessWebData(pWebData);

			SetTiingoInquiring(false);
			fDone = true;
			m_pCurrentTiingoProduct = nullptr;
		}
	}

	return fDone;
}

bool CWorldMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
	static int s_iCount1Hour = 3599;
	static int s_iCount5Minute = 299;
	static int s_iCount1Minute = 59;
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

	if (!IsTimeToResetSystem(lCurrentTime)) { // ������ʱ����ϵͳ�������ݿ���Ҫ����װ�룬�ʶ���ʱ��������¡�
		TaskProcessWebSocketData();
		TaskUpdateWorldStockFromWebSocket();
	}

	return true;
}

bool CWorldMarket::SchedulingTaskPer10Seconds(long lCurrentTime) {
	// ����WebSocket����
	if (IsSystemReady()) {
		if (m_finnhubWebSocket.IsClosed()) {
			m_finnhubWebSocket.CreatingThreadConnectingWebSocketAndSendMessage(GetFinnhubWebSocketSymbolVector());
		}

		if (m_tiingoIEXWebSocket.IsClosed()) {
			m_tiingoIEXWebSocket.CreatingThreadConnectingWebSocketAndSendMessage(GetTiingoIEXWebSocketSymbolVector());
		}

		if (m_tiingoCryptoWebSocket.IsClosed()) {
			m_tiingoCryptoWebSocket.CreatingThreadConnectingWebSocketAndSendMessage(GetTiingoCryptoWebSocketSymbolVector());
		}

		if (m_tiingoForexWebSocket.IsClosed()) {
			m_tiingoForexWebSocket.CreatingThreadConnectingWebSocketAndSendMessage(GetTiingoForexWebSocketSymbolVector());
		}
	}

	return true;
}

bool CWorldMarket::SchedulingTaskPerMinute(long lCurrentTime) {
	TaskResetMarket(lCurrentTime);

	if (!IsTimeToResetSystem(lCurrentTime)) { // ������ʱ����ϵͳ�������ݿ���Ҫ����װ�룬�ʶ���ʱ������������ݿ⡣
		if (m_dataFinnhubCountry.GetLastTotalCountry() < m_dataFinnhubCountry.GetTotalCountry()) {
			TaskUpdateCountryListDB();
		}
		TaskUpdateForexExchangeDB();
		TaskUpdateForexSymbolDB();
		TaskUpdateCryptoExchangeDB();
		TaskUpdateCryptoSymbolDB();
		TaskUpdateInsiderTransactionDB();
		TaskUpdateForexDayLineDB();
		TaskUpdateCryptoDayLineDB();
		TaskUpdateDayLineDB();
		TaskUpdateEPSSurpriseDB();
		TaskUpdateEconomicCalendarDB();
		return true;
	}

	return false;
}

bool CWorldMarket::SchedulingTaskPer5Minute(long lCurrentTime) {
	if (IsFinnhubSymbolUpdated() && IsStockProfileNeedUpdate()) {
		TaskUpdateStockProfileDB();
	}

	TaskUpdateTiingoStockDB();
	TaskUpdateTiingoCryptoDB();

	return true;
}

bool CWorldMarket::SchedulingTaskPerHour(long lCurrentTime) {
	return true;
}

/// <summary>
/// /////////////////////////////////////////////////////////////////////////////////
///
/// ��������������ÿ����ִ��һ�εĵ����С�
/// <returns></returns>
///
bool CWorldMarket::TaskResetMarket(long lCurrentTime) {
	// �г�ʱ��ʮ��ʱ����ϵͳ
	if (IsPermitResetMarket()) { // �����������ϵͳ
		if ((lCurrentTime > 170000) && (lCurrentTime <= 170100)) { // ���г�ʱ���������ʱ(����ʱ��������ʱ�������г������������ڽ����������ݡ�
			SetResetMarket(true);// ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
			SetPermitResetMarket(false); // ���첻����������ϵͳ��
			SetSystemReady(false);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// �˺�����SchedulingTask���ȣ�ÿ1500��������ִ��һ�Ρ�
//
//////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::TaskInquiryFinnhub(long lCurrentTime) {
	if (((lCurrentTime < 165700) || (lCurrentTime > 170500))) { // ������ʱ����ϵͳ���ʶ���ʱ���������������Ϣ��
		TaskInquiryFinnhubCountryList();
		TaskInquiryFinnhubForexExchange();
		TaskInquiryFinnhubCryptoExchange();
		TaskInquiryFinnhubCompanySymbol(); // ��һ���������������뵱��֤ȯ����
		TaskInquiryFinnhubForexSymbol();
		TaskInquiryFinnhubCryptoSymbol();
		//TaskInquiryFinnhubEconomicCalendar();

		// ����Finnhub������Ϣ�����񣬽�Ҫ����������Ա�֤���г�ʱ���賿ʮ���Ӻ�ִ�С������ܹ���֤�������г�ʱû��ִ�в�ѯ����
		if (IsSystemReady()) {
			TaskInquiryFinnhubCompanyProfileConcise();
			TaskInquiryFinnhubPeer();
			TaskInquiryFinnhubInsiderTransaction();
			//TaskInquiryFinnhubEPSSurprise(); // �������ûʲô�ã���ʱֹͣ���¡�
			TaskInquiryFinnhubForexDayLine();
			TaskInquiryFinnhubCryptoDayLine();
			TaskInquiryFinnhubDayLine();
			if (!IsFinnhubDayLineUpdated()) {
				//TaskInquiryFinnhubRTQuote();
			}
		}
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubCountryList(void) {
	if (!IsCountryListUpdated() && !IsFinnhubInquiring()) {
		CWebSourceDataProductPtr p = m_FinnhubFactory.CreateProduct(this, __ECONOMIC_COUNTRY_LIST__);
		m_qFinnhubProduct.push(p);
		SetFinnhubInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Finnhub economic country List�Ѹ���"));
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubCompanySymbol(void) {
	bool fFound = false;
	CFinnhubStockExchangePtr pExchange;
	CString str = _T("");
	long lExchangeSize = m_dataFinnhubStockExchange.GetExchangeSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;
	long lCurrentStockExchangePos = 0;

	if (!IsFinnhubSymbolUpdated() && !IsFinnhubInquiring()) {
		if (!m_fInquiringFinnhubStockSymbol) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock symbol..."));
			m_fInquiringFinnhubStockSymbol = true;
		}
		for (lCurrentStockExchangePos = 0; lCurrentStockExchangePos < lExchangeSize; lCurrentStockExchangePos++) {
			if (!m_dataFinnhubStockExchange.GetExchange(lCurrentStockExchangePos)->IsUpdated()) {
				pExchange = m_dataFinnhubStockExchange.GetExchange(lCurrentStockExchangePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(this, __STOCK_SYMBOLS__);
			p->SetIndex(lCurrentStockExchangePos);
			m_qFinnhubProduct.push(p);
			SetFinnhubInquiring(true);
			pExchange->SetUpdated(true);
			TRACE("����%s������֤ȯ����\n", pExchange->m_strCode.GetBuffer());
		}
		else {
			m_fInquiringFinnhubStockSymbol = false;
			fHaveInquiry = false;
			SetFinnhubSymbolUpdated(true);
			TRACE("Finnhub�������������ݲ�ѯ���\n");
			str = _T("Finnhub�������������ݲ�ѯ���");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskUpdateForexSymbolDB(void) {
	return CreatingThreadUpdateForexSymbolDB();
}

bool CWorldMarket::TaskUpdateCryptoSymbolDB(void) {
	return CreatingThreadUpdateCryptoSymbolDB();
}

bool CWorldMarket::TaskInquiryFinnhubCompanyProfileConcise(void) {
	bool fFound = false;
	long lStockSetSize = GetStockSize();
	CString str = _T("");
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;

	ASSERT(IsSystemReady());
	if (!IsFinnhubStockProfileUpdated() && !IsFinnhubInquiring()) {
		if (!m_fInquiringFinnhubStockProfile) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock profile..."));
			m_fInquiringFinnhubStockProfile = true;
		}
		for (m_lCurrentProfilePos = 0; m_lCurrentProfilePos < lStockSetSize; m_lCurrentProfilePos++) {
			if (!GetStock(m_lCurrentProfilePos)->IsProfileUpdated()) {
				fFound = true;
				break;
			}
		}
		if (fFound) {
			p = m_FinnhubFactory.CreateProduct(this, __COMPANY_PROFILE_CONCISE__);
			p->SetIndex(m_lCurrentProfilePos);
			m_qFinnhubProduct.push(p);
			//TRACE("����%s���\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
			SetFinnhubInquiring(true);
			fHaveInquiry = true;
		}
		else {
			m_fInquiringFinnhubStockProfile = false;
			SetFinnhubStockProfileUpdated(true);
			TRACE("Finnhub��Ʊ���������\n");
			str = _T("Finnhub��Ʊ���������");
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryFinnhubDayLine(void) {
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = GetStockSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;

	ASSERT(IsSystemReady());
	if (!IsFinnhubDayLineUpdated() && !IsFinnhubInquiring()) {
		if (!m_fInquiringFinnhubStockDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock day line..."));
			m_fInquiringFinnhubStockDayLine = true;
		}
		for (m_lCurrentUpdateDayLinePos = 0; m_lCurrentUpdateDayLinePos < lStockSetSize; m_lCurrentUpdateDayLinePos++) {
			pStock = GetStock(m_lCurrentUpdateDayLinePos);
			if (pStock->IsUSMarket() && pStock->IsDayLineNeedUpdate()) { // Ŀǰ����˻�ֻ�����������г��Ĺ�Ʊ���ߡ�
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(this, __STOCK_PRICE_CANDLES__);
			p->SetIndex(m_lCurrentUpdateDayLinePos);
			m_qFinnhubProduct.push(p);
			SetFinnhubInquiring(true);
			pStock->SetDayLineNeedUpdate(false);
			TRACE("����%s��������\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			m_fInquiringFinnhubStockDayLine = false;
			fHaveInquiry = false;
			SetFinnhubDayLineUpdated(true);
			m_lCurrentUpdateDayLinePos = 0; // ���ô����������е��������ݸ���һ������ʱ��Ҫ����24Сʱ���ʱ��ָ��¼��ɡ�
			TRACE("Finnhub���߸�����ϣ����¿�ʼ����\n");
			str = _T("US Market������ʷ���ݸ������");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryFinnhubInsiderTransaction(void) {
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = GetStockSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;

	ASSERT(IsSystemReady());
	if (!IsFinnhubInsiderTransactionUpdated() && !IsFinnhubInquiring()) {
		if (!m_fInquiringFinnhubStockInsiderTransaction) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock insider transaction..."));
			m_fInquiringFinnhubStockInsiderTransaction = true;
		}
		for (m_lCurrentUpdateInsiderTransactionPos = 0; m_lCurrentUpdateInsiderTransactionPos < lStockSetSize; m_lCurrentUpdateInsiderTransactionPos++) {
			pStock = GetStock(m_lCurrentUpdateInsiderTransactionPos);
			if (pStock->IsUSMarket()) {
				if (pStock->IsInsiderTransactionNeedUpdate()) { // Ŀǰ����˻�ֻ�����������г��Ĺ�Ʊ���ߡ�
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(this, __INSIDER_TRANSACTION__);
			p->SetIndex(m_lCurrentUpdateInsiderTransactionPos);
			m_qFinnhubProduct.push(p);
			SetFinnhubInquiring(true);
			TRACE("����%s �ڲ���������\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			m_fInquiringFinnhubStockInsiderTransaction = false;
			fHaveInquiry = false;
			SetFinnhubInsiderTransactionUpdated(true);
			TRACE("FinnhubInsider Transaction�������\n");
			str = _T("US Market Insider Transaction���ݸ������");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryFinnhubRTQuote(void) {
	CWebSourceDataProductPtr p = nullptr;
	ASSERT(IsSystemReady());
	if (!IsFinnhubInquiring()) {
		m_lCurrentRTDataQuotePos++;
		if (m_lCurrentRTDataQuotePos == GetStockSize()) m_lCurrentRTDataQuotePos = 0;
		p = make_shared<CProductFinnhubStockPriceQuote>();
		p->SetIndex(m_lCurrentRTDataQuotePos);
		m_qFinnhubProduct.push(p);
		SetFinnhubInquiring(true);
		TRACE("����%sʵʱ����\n", GetStock(m_lCurrentRTDataQuotePos)->GetSymbol().GetBuffer());
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubPeer(void) {
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = GetStockSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;

	ASSERT(IsSystemReady());
	if (!IsFinnhubPeerUpdated() && !IsFinnhubInquiring()) {
		if (!m_fInquiringFinnhubStockPeer) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock peer..."));
			m_fInquiringFinnhubStockPeer = true;
		}
		for (m_lCurrentUpdatePeerPos = 0; m_lCurrentUpdatePeerPos < lStockSetSize; m_lCurrentUpdatePeerPos++) {
			if (!GetStock(m_lCurrentUpdatePeerPos)->IsPeerUpdated()) {
				pStock = GetStock(m_lCurrentUpdatePeerPos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(this, __PEERS__);
			p->SetIndex(m_lCurrentUpdatePeerPos);
			m_qFinnhubProduct.push(p);
			SetFinnhubInquiring(true);
			TRACE("����%s Peer����\n", GetStock(m_lCurrentUpdatePeerPos)->GetSymbol().GetBuffer());
		}
		else {
			m_fInquiringFinnhubStockPeer = false;
			fHaveInquiry = false;
			SetFinnhubPeerUpdated(true);
			TRACE("Finnhub Peers�������\n");
			str = _T("Finnhub Peer Updated");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryFinnhubEconomicCalendar(void) {
	CWebSourceDataProductPtr p = nullptr;

	if (!IsFinnhubEconomicCalendarUpdated() && !IsFinnhubInquiring()) {
		p = m_FinnhubFactory.CreateProduct(this, __ECONOMIC_CALENDAR__);
		m_qFinnhubProduct.push(p);
		SetFinnhubInquiring(true);
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubEPSSurprise(void) {
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = GetStockSize();
	bool fHaveInquiry = false;

	ASSERT(IsSystemReady());
	if (!IsFinnhubEPSSurpriseUpdated() && !IsFinnhubInquiring()) {
		for (m_lCurrentUpdateEPSSurprisePos = 0; m_lCurrentUpdateEPSSurprisePos < lStockSetSize; m_lCurrentUpdateEPSSurprisePos++) {
			if (!GetStock(m_lCurrentUpdateEPSSurprisePos)->IsEPSSurpriseUpdated()) {
				pStock = GetStock(m_lCurrentUpdateEPSSurprisePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			CWebSourceDataProductPtr p = make_shared<CProductFinnhubStockEstimatesEPSSurprise>();
			p->SetIndex(m_lCurrentUpdateEPSSurprisePos);
			m_qFinnhubProduct.push(p);
			SetFinnhubInquiring(true);
			GetStock(m_lCurrentUpdateEPSSurprisePos)->SetEPSSurpriseUpdated(true);
			TRACE("����%s EPS Surprise����\n", GetStock(m_lCurrentUpdateEPSSurprisePos)->GetSymbol().GetBuffer());
		}
		else {
			SetFinnhubEPSSurpriseUpdated(true);
			TRACE("Finnhub EPS Surprise�������\n");
			str = _T("Finnhub EPS Surprise Updated");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryFinnhubForexExchange(void) {
	if (!IsFinnhubForexExchangeUpdated() && !IsFinnhubInquiring()) {
		m_qFinnhubProduct.push(m_FinnhubFactory.CreateProduct(this, __FOREX_EXCHANGE__));
		SetFinnhubInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Finnhub forex exchange�Ѹ���"));
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubForexSymbol(void) {
	CWebSourceDataProductPtr p = nullptr;

	if (!IsFinnhubForexSymbolUpdated() && !IsFinnhubInquiring()) {
		p = m_FinnhubFactory.CreateProduct(this, __FOREX_SYMBOLS__);
		p->SetIndex(m_lCurrentForexExchangePos++);
		m_qFinnhubProduct.push(p);
		SetFinnhubInquiring(true);
		if (m_lCurrentForexExchangePos >= m_dataFinnhubForexExchange.GetForexExchangeSize()) {
			SetFinnhubForexSymbolUpdated(true);
			m_lCurrentForexExchangePos = 0;
			gl_systemMessage.PushInformationMessage(_T("Finnhub Forex sysbols�Ѹ���"));
		}
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubForexDayLine(void) {
	bool fFound = false;
	CForexSymbolPtr pForexSymbol;
	CString str = _T("");
	const long lStockSetSize = m_dataFinnhubForexSymbol.GetForexSymbolSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;

	ASSERT(IsSystemReady());
	if (!IsFinnhubForexDayLineUpdated() && !IsFinnhubInquiring()) {
		if (!m_fInquiringFinnhubForexDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub forex day line..."));
			m_fInquiringFinnhubForexDayLine = true;
		}
		for (m_lCurrentUpdateForexDayLinePos = 0; m_lCurrentUpdateForexDayLinePos < lStockSetSize; m_lCurrentUpdateForexDayLinePos++) {
			if (GetForexSymbol(m_lCurrentUpdateForexDayLinePos)->IsDayLineNeedUpdate()) {
				pForexSymbol = GetForexSymbol(m_lCurrentUpdateForexDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(this, __FOREX_CANDLES__);
			p->SetIndex(m_lCurrentUpdateForexDayLinePos);
			m_qFinnhubProduct.push(p);
			SetFinnhubInquiring(true);
			pForexSymbol->SetDayLineNeedUpdate(false);
			TRACE("����Forex%s��������\n", pForexSymbol->GetSymbol().GetBuffer());
		}
		else {
			m_fInquiringFinnhubForexDayLine = false;
			SetFinnhubForexDayLineUpdated(true);
			TRACE("Finnhub Forex���߸������\n");
			str = _T("Forex DayLine Updated");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryFinnhubCryptoExchange(void) {
	if (!IsFinnhubCryptoExchangeUpdated() && !IsFinnhubInquiring()) {
		m_qFinnhubProduct.push(m_FinnhubFactory.CreateProduct(this, __CRYPTO_EXCHANGE__));
		SetFinnhubInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Finnhub crypto exchange�Ѹ���"));
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubCryptoSymbol(void) {
	CWebSourceDataProductPtr p = nullptr;

	if (!IsFinnhubCryptoSymbolUpdated() && !IsFinnhubInquiring()) {
		p = m_FinnhubFactory.CreateProduct(this, __CRYPTO_SYMBOLS__);
		p->SetIndex(m_lCurrentCryptoExchangePos++);
		m_qFinnhubProduct.push(p);
		SetFinnhubInquiring(true);
		if (m_lCurrentCryptoExchangePos >= m_dataFinnhubCryptoExchange.GetCryptoExchangeSize()) {
			SetFinnhubCryptoSymbolUpdated(true);
			gl_systemMessage.PushInformationMessage(_T("Finnhub Crypto sysbols�Ѹ���"));
			m_lCurrentCryptoExchangePos = 0;
		}
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubCryptoDayLine(void) {
	bool fFound = false;
	CFinnhubCryptoSymbolPtr pCryptoSymbol;
	CString str = _T("");
	const long lStockSetSize = m_dataFinnhubCryptoSymbol.GetCryptoSymbolSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;

	ASSERT(IsSystemReady());
	if (!IsFinnhubCryptoDayLineUpdated() && !IsFinnhubInquiring()) {
		if (!m_fInquiringFinnhubCryptoDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub forex day line..."));
			m_fInquiringFinnhubCryptoDayLine = true;
		}
		for (m_lCurrentUpdateCryptoDayLinePos = 0; m_lCurrentUpdateCryptoDayLinePos < lStockSetSize; m_lCurrentUpdateCryptoDayLinePos++) {
			if (GetCryptoSymbol(m_lCurrentUpdateCryptoDayLinePos)->IsDayLineNeedUpdate()) {
				pCryptoSymbol = GetCryptoSymbol(m_lCurrentUpdateCryptoDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(this, __CRYPTO_CANDLES__);
			p->SetIndex(m_lCurrentUpdateCryptoDayLinePos);
			m_qFinnhubProduct.push(p);
			SetFinnhubInquiring(true);
			pCryptoSymbol->SetDayLineNeedUpdate(false);
			TRACE("����Crypto %s��������\n", pCryptoSymbol->GetSymbol().GetBuffer());
		}
		else {
			m_fInquiringFinnhubCryptoDayLine = false;
			SetFinnhubCryptoDayLineUpdated(true);
			TRACE("Finnhub Crypto���߸������\n");
			str = _T("Crypto DayLine Updated");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryTiingo(void) {
	if (IsSystemReady()) {
		TaskInquiryTiingoCompanySymbol();
		TaskInquiryTiingoCryptoSymbol();
		// ����Tiingo�涨ÿ��ֻ�ܲ�ѯ500�����룬�ʲ��Գɹ�����ʱ��ʹ�á�
		TaskInquiryTiingoDayLine(); // ����������ϡ�
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryTiingoCompanySymbol(void) {
	if (!IsTiingoStockSymbolUpdated() && !IsTiingoInquiring()) {
		CWebSourceDataProductPtr p = m_TiingoFactory.CreateProduct(this, __STOCK_SYMBOLS__);
		m_qTiingoProduct.push(p);
		SetTiingoInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Tiingo stock symbol�Ѹ���"));

		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryTiingoCryptoSymbol(void) {
	if (!IsTiingoCryptoSymbolUpdated() && !IsTiingoInquiring()) {
		CWebSourceDataProductPtr p = m_TiingoFactory.CreateProduct(this, __CRYPTO_SYMBOLS__);
		m_qTiingoProduct.push(p);
		SetTiingoInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Tiingo crypto symbol�Ѹ���"));

		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo����������������Finnhub���������غ�����ֻ����ͬʱ��������֮һ��
// Ŀǰ�˹���ֻ����������ѡ��Ʊ���������ݡ�
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::TaskInquiryTiingoDayLine(void) {
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = m_dataChoicedStock.GetSize();
	bool fHaveInquiry = false;

	ASSERT(IsSystemReady());
	if (!IsTiingoDayLineUpdated() && !IsTiingoInquiring()) {
		for (long lCurrentUpdateDayLinePos = 0; lCurrentUpdateDayLinePos < lStockSetSize; lCurrentUpdateDayLinePos++) {
			if (m_dataChoicedStock.GetStock(lCurrentUpdateDayLinePos)->IsDayLineNeedUpdate()) {
				pStock = m_dataChoicedStock.GetStock(lCurrentUpdateDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			CWebSourceDataProductPtr p = m_TiingoFactory.CreateProduct(this, __STOCK_PRICE_CANDLES__);
			p->SetIndex(m_dataWorldStock.GetIndex(pStock->GetSymbol()));
			m_qTiingoProduct.push(p);
			SetTiingoInquiring(true);
			pStock->SetDayLineNeedUpdate(false);
			TRACE("����Tiingo %s��������\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			SetTiingoDayLineUpdated(true);
			TRACE("Tiingo���߸������\n");
			str = _T("�����г���ѡ��Ʊ������ʷ���ݸ������");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskUpdateTiingoIndustry(void) {
	if (IsFinnhubStockProfileUpdated()) {
		CreatingThreadUpdateTiingoIndustry();
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateSICIndustry(void) {
	if (IsFinnhubStockProfileUpdated()) {
		CreatingThreadUpdateSICIndustry();
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateNaicsIndustry(void) {
	if (IsFinnhubStockProfileUpdated()) {
		CreatingThreadUpdateNaicsIndustry();
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
		if (gl_fExitingSystem) {
			break; // ������������˳�����ֹͣ�洢��
		}
		pSymbol = m_dataFinnhubForexSymbol.GetForexSymbol(i);
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) { // �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					CreatingThreadUpdateForexDayLineDB(pSymbol.get());
					fUpdated = true;
					TRACE("����%s��������\n", pSymbol->GetSymbol().GetBuffer());
				}
				else pSymbol->UnloadDayLine(); // ������ִ�д洢����ʱ�����ﻹҪ����ж���������ݡ���洢���������߳��Ժ��ִ�У��ʶ������ڴ�ͳһִ��ɾ��������
			}
			else { // �������Ϊ�й�Ʊ���룬���˴�����δ����
				CString str1 = pSymbol->GetSymbol();
				str1 += _T(" Ϊδ���й�Ʊ����");
				gl_systemMessage.PushDayLineInfoMessage(str1);
			}
		}
	}

	return(fUpdated);
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

	for (int i = 0; i < m_dataFinnhubCryptoSymbol.GetCryptoSymbolSize(); i++) {
		if (gl_fExitingSystem) {
			break; // ������������˳�����ֹͣ�洢��
		}
		pSymbol = m_dataFinnhubCryptoSymbol.GetCryptoSymbol(i);
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) { // �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					CreatingThreadUpdateCryptoDayLineDB(pSymbol.get());
					fUpdated = true;
					TRACE("����%s��������\n", pSymbol->GetSymbol().GetBuffer());
				}
				else pSymbol->UnloadDayLine(); // ������ִ�д洢����ʱ�����ﻹҪ����ж���������ݡ���洢���������߳��Ժ��ִ�У��ʶ������ڴ�ͳһִ��ɾ��������
			}
			else { // �������Ϊ�й�Ʊ���룬���˴�����δ����
				CString str1 = pSymbol->GetSymbol();
				str1 += _T(" Ϊδ���й�Ʊ����");
				gl_systemMessage.PushDayLineInfoMessage(str1);
			}
		}
	}

	return(fUpdated);
}

bool CWorldMarket::TaskUpdateEPSSurpriseDB(void) {
	CString str;

	CWorldStockPtr pStock = nullptr;
	for (long l = 0; l < m_dataWorldStock.GetStockSize(); l++) {
		pStock = m_dataWorldStock.GetStock(l);
		if (pStock->IsEPSSurpriseNeedSaveAndClearFlag()) { // �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			CreatingThreadUpdateEPSSurpriseDB(pStock.get());
			TRACE("����%s EPS surprise����\n", pStock->GetSymbol().GetBuffer());
		}
		if (gl_fExitingSystem) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}

	return(true);
}

bool CWorldMarket::TaskCheckSystemReady(void) {
	CString str = _T("");

	if (!IsSystemReady()) {
		if (IsFinnhubSymbolUpdated() && IsFinnhubForexExchangeUpdated() && IsFinnhubForexSymbolUpdated() && IsFinnhubCryptoExchangeUpdated() && IsFinnhubCryptoSymbolUpdated()) {
			str = _T("�����г���ʼ�����");
			gl_systemMessage.PushInformationMessage(str);
			SetSystemReady(true);
			return true;
		}
		return false;
	}
	return true;
}

bool CWorldMarket::CreatingthreadUpdateDayLneStartEndDate(void) {
	thread thread1(ThreadUpdateWorldStockDayLineStartEndDate, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateDayLineDB() {
	thread thread1(ThreadUpdateWorldStockDayLineDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateStockProfileDB(void) {
	thread thread1(ThreadUpdateStockProfileDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateCountryListDB(void) {
	thread thread1(ThreadUpdateCountryListDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateEPSSurpriseDB(CWorldStock* pStock) {
	thread thread1(ThreadUpdateEPSSurpriseDB, pStock);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateInsiderTransactionDB(void) {
	thread thread1(ThreadUpdateInsiderTransactionDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateTiingoStockDB(void) {
	thread thread1(ThreadUpdateTiingoStockDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateTiingoCryptoDB(void) {
	thread thread1(ThreadUpdateTiingoCryptoDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateTiingoIndustry(void) {
	thread thread1(ThreadUpdateTiingoIndustry, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateSICIndustry(void) {
	thread thread1(ThreadUpdateSICIndustry, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateNaicsIndustry(void) {
	thread thread1(ThreadUpdateNaicsIndustry, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateForexExchangeDB(void) {
	thread thread1(ThreadUpdateForexExchangeDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateForexSymbolDB() {
	thread thread1(ThreadUpdateForexSymbolDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateForexDayLineDB(CForexSymbol* pSymbol) {
	thread thread1(ThreadUpdateForexDayLineDB, pSymbol);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateCryptoExchangeDB(void) {
	thread thread1(ThreadUpdateCryptoExchangeDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateCryptoSymbolDB() {
	thread thread1(ThreadUpdateCryptoSymbolDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateCryptoDayLineDB(CFinnhubCryptoSymbol* pSymbol) {
	thread thread1(ThreadUpdateCryptoDayLineDB, pSymbol);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::CreatingThreadUpdateEconomicCalendarDB(void) {
	thread thread1(ThreadUpdateEconomicCalendarDB, this);
	thread1.detach();

	return true;
}

void CWorldMarket::StopReceivingWebSocket(void) {
	m_finnhubWebSocket.Deconnecting();
	m_tiingoIEXWebSocket.Deconnecting();
	m_tiingoCryptoWebSocket.Deconnecting();
	m_tiingoForexWebSocket.Deconnecting();
}

bool CWorldMarket::LoadOption(void) {
	CSetWorldMarketOption setOption;

	setOption.Open();
	if (setOption.m_FinnhubToken.GetLength() > 5) {
		gl_pFinnhubWebInquiry->SetInquiryingStringSuffix(setOption.m_FinnhubToken);
	}
	if (setOption.m_TiingoToken.GetLength() > 5) {
		gl_pTiingoWebInquiry->SetInquiryingStringSuffix(setOption.m_TiingoToken);
	}
	if (setOption.m_QuandlToken.GetLength() > 5) {
		gl_pQuandlWebInquiry->SetInquiryingStringSuffix(setOption.m_QuandlToken);
	}
	setOption.Close();

	return true;
}

bool CWorldMarket::UpdateStockDayLineDB(void) {
	CWorldStockPtr pStock = nullptr;

	for (long i = 0; i < GetStockSize(); i++) {
		pStock = GetStock(i);
		pStock->UpdateDayLineDB();
		if (gl_fExitingSystem) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}
	return true;
}

bool CWorldMarket::UpdateInsiderTransactionDB(void) {
	CString str;
	CWorldStockPtr pStock = nullptr;

	for (long i = 0; i < GetStockSize(); i++) {
		pStock = GetStock(i);
		if (pStock->IsInsiderTransactionNeedSaveAndClearFlag()) {
			if (pStock->HaveInsiderTransaction()) {
				pStock->SaveInsiderTransaction();
				str = pStock->GetSymbol() + _T("�ڲ��������ϸ������");
				gl_systemMessage.PushDayLineInfoMessage(str);
				TRACE("����%s�ڲ���������\n", pStock->GetSymbol().GetBuffer());
			}
		}
		if (gl_fExitingSystem) {
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
	SetFinnhubEPSSurpriseUpdated(false);
	m_lCurrentUpdateEPSSurprisePos = 0;

	return true;
}

bool CWorldMarket::RebuildPeer(void) {
	m_dataWorldStock.ResetPeer();
	SetFinnhubPeerUpdated(false);
	m_lCurrentUpdatePeerPos = 0;

	return true;
}

bool CWorldMarket::RebuildStockDayLineDB(void) {
	m_dataWorldStock.ResetDayLine();
	SetFinnhubStockProfileUpdated(false);

	return true;
}

bool CWorldMarket::UpdateStockDayLineStartEndDate(void) {
	CString strFilterPrefix = _T("[Symbol] = '");
	CString strFilter, str;
	CSetWorldStockDayLine setWorldStockDayLine;
	CWorldStockPtr pStock2 = nullptr;

	CWorldStockPtr pStock = nullptr;
	for (long l = 0; l < m_dataWorldStock.GetStockSize(); l++) {
		pStock = m_dataWorldStock.GetStock(l);
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
	for (long l = 0; l < m_dataChoicedStock.GetSize(); l++) {
		pStock = m_dataChoicedStock.GetStock(l);
		vSymbol.push_back(pStock->GetSymbol());
	}

	CFinnhubCryptoSymbolPtr pCrypto = nullptr;
	for (long l = 0; l < m_dataChoicedCrypto.GetSize(); l++) {
		pCrypto = m_dataChoicedCrypto.GetCrypto(l);
		vSymbol.push_back(pCrypto->GetSymbol());
	}

	CForexSymbolPtr pForex = nullptr;
	for (long l = 0; l < m_dataChoicedForex.GetSize(); l++) {
		pForex = m_dataChoicedForex.GetForex(l);
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
	CWorldStockPtr pStock = nullptr;
	vector<CString> vSymbol;
	for (long l = 0; l < m_dataChoicedStock.GetSize(); l++) {
		pStock = m_dataChoicedStock.GetStock(l);
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
	CFinnhubCryptoSymbolPtr pCrypto = nullptr;
	vector<CString> vSymbol;
	for (long l = 0; l < m_dataChoicedCrypto.GetSize(); l++) {
		pCrypto = m_dataChoicedCrypto.GetCrypto(l);
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
	CForexSymbolPtr pForex = nullptr;
	vector<CString> vSymbol;
	for (long l = 0; l < m_dataChoicedForex.GetSize(); l++) {
		pForex = m_dataChoicedForex.GetForex(l);
		vSymbol.push_back(pForex->GetSymbol());
	}

	return vSymbol;
}

bool CWorldMarket::TaskProcessWebSocketData(void) {
	m_iWebSocketReceivedNumberPerSecond = gl_WebInquirer.GetFinnhubWebSocketDataSize()
		+ gl_WebInquirer.GetTiingoIEXWebSocketDataSize()
		+ gl_WebInquirer.GetTiingoCryptoWebSocketDataSize()
		+ gl_WebInquirer.GetTiingoForexWebSocketDataSize();

	ProcessFinnhubWebSocketData();
	ProcessTiingoIEXWebSocketData();
	ProcessTiingoCryptoWebSocketData();
	ProcessTiingoForexWebSocketData();

	return true;
}

bool CWorldMarket::ProcessFinnhubWebSocketData() {
	auto total = gl_WebInquirer.GetFinnhubWebSocketDataSize();
	CString strMessage;
	shared_ptr<string> pString;
	int iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopFinnhubWebSocketData();
		strMessage = _T("Finnhub: ");
		strMessage += (*pString).c_str();
		//gl_systemMessage.PushCancelBuyMessage(strMessage);
		iTotalDataSize += pString->size();
		m_finnhubWebSocket.ParseFinnhubWebSocketData(pString);
	}
	m_iProcessedFinnhubWebSocket = iTotalDataSize;

	return true;
}

bool CWorldMarket::ProcessTiingoIEXWebSocketData() {
	auto total = gl_WebInquirer.GetTiingoIEXWebSocketDataSize();
	CString strMessage;
	shared_ptr<string> pString;
	size_t iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopTiingoIEXWebSocketData();
		strMessage = _T("Tiingo IEX: ");
		strMessage += (*pString).c_str();
		gl_systemMessage.PushCancelBuyMessage(strMessage);
		iTotalDataSize += pString->size();
		m_tiingoIEXWebSocket.ParseTiingoIEXWebSocketData(pString);
	}
	m_iProcessedTiingoIEXWebSocket = iTotalDataSize;
	return true;
}

bool CWorldMarket::ProcessTiingoCryptoWebSocketData() {
	auto total = gl_WebInquirer.GetTiingoCryptoWebSocketDataSize();
	CString strMessage;

	shared_ptr<string> pString;
	int iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopTiingoCryptoWebSocketData();
		strMessage = _T("Tiingo Crypto: ");
		strMessage += (*pString).c_str();
		gl_systemMessage.PushCancelBuyMessage(strMessage);
		iTotalDataSize += pString->size();
		m_tiingoCryptoWebSocket.ParseTiingoCryptoWebSocketData(pString);
	}
	m_iProcessedTiingoCryptoWebSocket = iTotalDataSize;
	return true;
}

bool CWorldMarket::ProcessTiingoForexWebSocketData() {
	auto total = gl_WebInquirer.GetTiingoForexWebSocketDataSize();
	CString strMessage;
	shared_ptr<string> pString;
	int iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopTiingoForexWebSocketData();
		strMessage = _T("Tiingo Forex: ");
		strMessage += (*pString).c_str();
		//gl_systemMessage.PushCancelBuyMessage(strMessage);
		iTotalDataSize += pString->size();
		m_tiingoForexWebSocket.ParseTiingoForexWebSocketData(pString);
	}
	m_iProcessedTiingoForexWebSocket = iTotalDataSize;
	return true;
}

/// <summary>
/// ����֤ȯ��ʵʱ��Ϣ
/// Ŀǰֻʹ��IEX��Finnhub�����ݣ���������������
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CWorldMarket::TaskUpdateWorldStockFromWebSocket(void) {
	CTiingoIEXSocketPtr pIEXData;
	CTiingoCryptoSocketPtr pCryptoData;
	CTiingoForexSocketPtr pForexData;
	CFinnhubSocketPtr pFinnhubData;
	CWorldStockPtr pStock = nullptr;

	auto total = gl_SystemData.GetTiingoIEXSocketSize();
	for (auto i = 0; i < total; i++) {
		pIEXData = gl_SystemData.PopTiingoIEXSocket();
		UpdateWorldStockFromTiingoIEXSocket(pIEXData);
		SetCurrentTiingoWebSocketIEX(pIEXData->m_strSymbol);
	}

	total = gl_SystemData.GetTiingoCryptoSocketSize();
	for (auto i = 0; i < total; i++) {
		pCryptoData = gl_SystemData.PopTiingoCryptoSocket();
		SetCurrentTiingoWebSocketCrypto(pCryptoData->m_strSymbol);
	}

	total = gl_SystemData.GetTiingoForexSocketSize();
	for (auto i = 0; i < total; i++) {
		pForexData = gl_SystemData.PopTiingoForexSocket();
		SetCurrentTiingoWebSocketForex(pForexData->m_strSymbol);
	}

	total = gl_SystemData.GetFinnhubSocketSize();
	for (auto i = 0; i < total; i++) {
		pFinnhubData = gl_SystemData.PopFinnhubSocket();
		UpdateWorldStockFromFinnhubSocket(pFinnhubData);
		SetCurrentFinnhubWebSocketStake(pFinnhubData->m_strSymbol);
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
			break;
		}
	}

	return true;
}

bool CWorldMarket::UpdateWorldStockFromFinnhubSocket(CFinnhubSocketPtr pFinnhubData) {
	CWorldStockPtr pStock = nullptr;

	if (IsStock(pFinnhubData->m_strSymbol)) {
		pStock = GetStock(pFinnhubData->m_strSymbol);
		pStock->SetActive(true);
		pStock->SetNew(pFinnhubData->m_dLastPrice * 1000);
	}

	return true;
}