#include"pch.h"

#include "WorldMarket.h"
#include"SystemData.h"

#include "WorldMarket.h"
#include"FinnhubInquiryType.h"
#include"thread.h"
#include"Callablefunction.h"
#include"WebInquirer.h"

#include"FinnhubInquiryType.h"

#include"ProductFinnhubStockPriceQuote.h"
#include"ProductFinnhubStockEstimatesEPSSurprise.h"

#include"SetWorldMarketOption.h"
#include"SetWorldStockDayLine.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include<ixwebsocket/IXWebSocketMessage.h>

using namespace std;
#include<algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CWorldMarket, CVirtualMarket)

CWorldMarket::CWorldMarket() {
	static int siInstance = 0;

	if (++siInstance > 1) {
		TRACE("CWorldMarket�г�����ֻ�������һ��ʵ��\n");
	}

	// ���裨Ҳ�޷���ÿ�ո��µı�����������
	m_fFinnhubEPSSurpriseUpdated = true;
	m_lCurrentUpdateEPSSurprisePos = 0;
	m_lCurrentUpdateDayLinePos = 0; // ����֤ȯ���������ж�ʮ��֮�࣬�޷���һ��֮�ڸ����꣬�ʲ������ô�������

	m_strMarketId = _T("�����г�");
	m_lMarketTimeZone = 4 * 3600; // ��������ʹ��������׼ʱ�䡣
	CalculateTime();

	Reset();

	m_fRecordFinnhubWebSocket = true;
	m_fRecordTiingoCryptoWebSocket = true;
	m_fRecordTiingoForexWebSocket = true;
	m_fRecordTiingoIEXWebSocket = true;

	thread thread1(ThreadWorldMarketBackground);
	thread1.detach();
}

CWorldMarket::~CWorldMarket() {
	PreparingExitMarket();

	if (!gl_fExitingSystem) {
		gl_fExitingSystem = true;
		while (gl_ThreadStatus.IsWorldMarketBackgroundThreadRunning()) Sleep(1);
		gl_fExitingSystem = false;
	}
	else {
		while (gl_ThreadStatus.IsWorldMarketBackgroundThreadRunning()) Sleep(1);
	}
}

void CWorldMarket::Reset(void) {
	ResetFinnhub();
	ResetTiingo();
	ResetDataClass();
}

void CWorldMarket::ResetFinnhub(void) {
	m_pCurrentFinnhubProduct = nullptr;

	m_dataFinnhubStockExchange.Reset();
	m_dataFinnhubForexExchange.Reset();

	// Finnhub�������������ݱ�ʶ��ÿ����Ҫ���á�
	m_fFinnhubSymbolUpdated = false; // ÿ����Ҫ���´���
	m_fFinnhubStockProfileUpdated = false;
	m_fFinnhubStockBasicFinancialUpdated = false;
	m_fFinnhubDayLineUpdated = false;
	m_fFinnhubForexExchangeUpdated = false;
	m_fFinnhubForexSymbolUpdated = false;
	m_fFinnhubForexDayLineUpdated = false;
	m_fFinnhubCryptoExchangeUpdated = false;
	m_fFinnhubCryptoSymbolUpdated = false;
	m_fFinnhubCryptoDayLineUpdated = false;
	m_fCountryListUpdated = false;
	m_fFinnhubEconomicCalendarUpdated = false;

	m_fFinnhubPeerUpdated = false;
	m_fFinnhubInsiderTransactionUpdated = false;

	m_fFinnhubInquiring = false;
	m_fFinnhubDataReceived = true;

	m_fRebulidDayLine = false;
	SetSystemReady(false); // �г���ʼ״̬Ϊδ���úá�

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
	LoadFinnhubCryptoSymbol();
	LoadWorldChoicedCrypto();
	LoadEconomicCalendarDB();
	LoadTiingoStock();
	LoadTiingoCryptoSymbol();

	CString str = _T("����World Market��������׼ʱ�䣺");
	str += GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(str);
}

bool CWorldMarket::PreparingExitMarket(void)
{
	ASSERT(gl_fExitingSystem);
	StopReceivingWebSocket();
	while (gl_ThreadStatus.IsWorldMarketBackgroundThreadRunning()) Sleep(1);

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
	static int s_iCountfinnhubLimit = 12; // Finnhub.ioÿ1.5����������һ�Σ��Է�ֹ����Ƶ�ʹ��ߵ������
	static int s_iCountTiingoLimit = 80; // ��֤ÿ80��ִ��һ�Σ���8��ÿ�Σ�.Tiingo����˻��ٶ�����ΪÿСʱ500�Σ� ÿ����9�Σ���ÿ��8�뼴�ɡ�
	const long lCurrentTime = GetMarketTime();

	TaskCheckSystemReady();

	if (--s_iCountfinnhubLimit < 0) {
		TaskInquiryFinnhub(lCurrentTime);
		if (IsFinnhubInquiring()) {
			ASSERT(gl_pFinnhubWebInquiry->GetShortestInquiringInterval() > 100);
			s_iCountfinnhubLimit = gl_pFinnhubWebInquiry->GetShortestInquiringInterval() / 100; // ����������������ݣ������ü��������Ա�������һ�Ρ�
		}
		if (gl_pFinnhubWebInquiry->IsWebError()) { // finnhub.io��ʱ�����Σ��ʶ����ִ���ʱ��ͣһ�����
			gl_pFinnhubWebInquiry->SetWebError(false);
			s_iCountfinnhubLimit = 6000; // ������ִ�����ÿ10������������һ�Ρ�
		}
	}
	ProcessFinnhubWebDataReceived(); // Ҫ�ȴ����յ���Finnhub��������
	ProcessFinnhubInquiringMessage(); // Ȼ�������봦����һ��

	if (--s_iCountTiingoLimit < 0) {
		ASSERT(gl_pTiingoWebInquiry->GetShortestInquiringInterval() > 100);
		s_iCountTiingoLimit = gl_pTiingoWebInquiry->GetShortestInquiringInterval() / 100;
		TaskInquiryTiingo();
		if (gl_pTiingoWebInquiry->IsWebError()) {
			gl_pTiingoWebInquiry->SetWebError(false);
			s_iCountTiingoLimit = 6000; // ������ִ�����ÿ10������������һ�Ρ�
		}
	}
	ProcessTiingoWebDataReceived(); // Ҫ�ȴ����յ���Tiingo��������
	ProcessTiingoInquiringMessage(); // Ȼ�������봦����һ��

//����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
	if (GetUTCTime() > s_lastTimeSchedulingTask) {
		SchedulingTaskPerSecond(GetUTCTime() - s_lastTimeSchedulingTask, lCurrentTime);
		s_lastTimeSchedulingTask = GetUTCTime();
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

	if (!IsTimeToResetSystem(lCurrentTime)) { // ������ʱ����ϵͳ�������ݿ���Ҫ����װ�룬�ʶ���ʱ��������¡�
		TaskProcessWebSocketData();
		TaskUpdateWorldStockFromWebSocket();
	}

	return true;
}

bool CWorldMarket::SchedulingTaskPer10Seconds(long lCurrentTime) {
	return true;
}

bool CWorldMarket::SchedulingTaskPerMinute(long lCurrentTime) {
	TaskResetMarket(lCurrentTime);
	// ����WebSocket����
	TaskActivateWebSocket();

	// ������������һ������������ִ����Ϻ󷵻��ˡ�
	if (!IsTimeToResetSystem(lCurrentTime)) { // ������ʱ����ϵͳ�������ݿ���Ҫ����װ�룬�ʶ���ʱ������������ݿ⡣
		if (m_dataFinnhubCountry.GetLastTotalCountry() < m_dataFinnhubCountry.GetTotalCountry()) {
			TaskUpdateCountryListDB();
		}
		TaskUpdateForexExchangeDB();
		TaskUpdateForexSymbolDB();
		TaskUpdateCryptoExchangeDB();
		TaskUpdateFinnhubCryptoSymbolDB();
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

	if (IsBasicFinancialNeedUpdate()) {
		TaskUpdateBasicFinancialDB();
	}

	TaskUpdateTiingoStockDB();
	TaskUpdateTiingoCryptoSymbolDB();

	return true;
}

bool CWorldMarket::SchedulingTaskPerHour(long lCurrentTime) {
	TaskReActivateWebSocket();

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

		// ����Finnhub������Ϣ�����񣬽�Ҫ����������Ա�֤���г�ʱ���賿ʮ���Ӻ�ִ�С������ܹ���֤�������г�ʱ����ִ�в�ѯ����
		if (IsSystemReady()) {
			TaskInquiryFinnhubCompanyProfileConcise();
			TaskInquiryFinnhubCompanyBasicFinancial();
			TaskInquiryFinnhubPeer();
			TaskInquiryFinnhubInsiderTransaction();
			TaskInquiryFinnhubCryptoDayLine();
			TaskInquiryFinnhubStockDayLine();
			//TaskInquiryFinnhubForexDayLine(); // Forex daylineĿǰֻ���ڸ����û�ʹ��
			//TaskInquiryFinnhubEPSSurprise(); // �������ûʲô�ã���ʱֹͣ���¡�
			if (!IsFinnhubStockDayLineUpdated()) {
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
		SetCurrentFunction(_T("Finnhub country List"));
		SetFinnhubInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub economic country List"));
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubCompanySymbol(void) {
	static bool s_fInquiringFinnhubStockSymbol = false;
	bool fFound = false;
	CFinnhubStockExchangePtr pExchange;
	CString str = _T("");
	long lExchangeSize = m_dataFinnhubStockExchange.GetExchangeSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;
	long lCurrentStockExchangePos = 0;

	if (!IsFinnhubSymbolUpdated() && !IsFinnhubInquiring()) {
		if (!s_fInquiringFinnhubStockSymbol) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock symbol..."));
			s_fInquiringFinnhubStockSymbol = true;
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
			SetCurrentFunction(_T("Finnhub����������:") + pExchange->m_strCode);
			SetFinnhubInquiring(true);
			pExchange->SetUpdated(true);
			//TRACE("����%s������֤ȯ����\n", pExchange->m_strCode.GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockSymbol = false;
			fHaveInquiry = false;
			SetFinnhubSymbolUpdated(true);
			TRACE("Finnhub�������������ݲ�ѯ���\n");
			str = _T("Finnhub�������������ݲ�ѯ���");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryFinnhubCompanyProfileConcise(void) {
	static bool s_fInquiringFinnhubStockProfile = false;
	bool fFound = false;
	long lStockSetSize = GetStockSize();
	CString str = _T("");
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;
	long lCurrentProfilePos;

	ASSERT(IsSystemReady());
	if (!IsFinnhubStockProfileUpdated() && !IsFinnhubInquiring()) {
		if (!s_fInquiringFinnhubStockProfile) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock profile..."));
			s_fInquiringFinnhubStockProfile = true;
		}
		for (lCurrentProfilePos = 0; lCurrentProfilePos < lStockSetSize; lCurrentProfilePos++) {
			if (!GetStock(lCurrentProfilePos)->IsProfileUpdated()) {
				fFound = true;
				break;
			}
		}
		if (fFound) {
			p = m_FinnhubFactory.CreateProduct(this, __COMPANY_PROFILE_CONCISE__);
			p->SetIndex(lCurrentProfilePos);
			m_qFinnhubProduct.push(p);
			SetCurrentFunction(_T("���:") + m_dataWorldStock.GetStock(lCurrentProfilePos)->GetSymbol());
			//TRACE("����%s���\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
			SetFinnhubInquiring(true);
			fHaveInquiry = true;
		}
		else {
			s_fInquiringFinnhubStockProfile = false;
			SetFinnhubStockProfileUpdated(true);
			TRACE("Finnhub��Ʊ���������\n");
			str = _T("Finnhub��Ʊ���������");
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryFinnhubCompanyBasicFinancial(void) {
	static bool s_fInquiringFinnhubCompanyBasicFinancial = false;
	bool fFound = false;
	long lStockSetSize = GetStockSize();
	CString str = _T("");
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;
	long lCurrentBasicFinncialsPos;

	ASSERT(IsSystemReady());
	if (!IsFinnhubStockBasicFinancialUpdated() && !IsFinnhubInquiring()) {
		if (!s_fInquiringFinnhubCompanyBasicFinancial) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock basic financials..."));
			s_fInquiringFinnhubCompanyBasicFinancial = true;
		}
		for (lCurrentBasicFinncialsPos = 0; lCurrentBasicFinncialsPos < lStockSetSize; lCurrentBasicFinncialsPos++) {
			if (!GetStock(lCurrentBasicFinncialsPos)->IsBasicFinancialUpdated()) {
				fFound = true;
				break;
			}
		}
		if (fFound) {
			p = m_FinnhubFactory.CreateProduct(this, __BASIC_FINANCIALS__);
			p->SetIndex(lCurrentBasicFinncialsPos);
			m_qFinnhubProduct.push(p);
			SetCurrentFunction(_T("��������:") + m_dataWorldStock.GetStock(lCurrentBasicFinncialsPos)->GetSymbol());
			//TRACE("����%s���\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
			SetFinnhubInquiring(true);
			fHaveInquiry = true;
		}
		else {
			s_fInquiringFinnhubCompanyBasicFinancial = false;
			SetFinnhubStockBasicFinancialUpdated(true);
			TRACE("Finnhub��Ʊ������������������\n");
			str = _T("Finnhub��Ʊ������������������");
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

bool CWorldMarket::TaskInquiryFinnhubStockDayLine(void) {
	static bool s_fInquiringFinnhubStockDayLine = false;
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = GetStockSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;

	ASSERT(IsSystemReady());
	if (!IsFinnhubStockDayLineUpdated() && !IsFinnhubInquiring()) {
		if (!s_fInquiringFinnhubStockDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock day line..."));
			s_fInquiringFinnhubStockDayLine = true;
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
			SetCurrentFunction(_T("����:") + pStock->GetSymbol());
			//TRACE("����%s��������\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockDayLine = false;
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
	static bool s_fInquiringFinnhubStockInsiderTransaction = false;
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = GetStockSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;
	long lCurrentUpdateInsiderTransactionPos = 0;

	ASSERT(IsSystemReady());
	if (!IsFinnhubInsiderTransactionUpdated() && !IsFinnhubInquiring()) {
		if (!s_fInquiringFinnhubStockInsiderTransaction) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock insider transaction..."));
			s_fInquiringFinnhubStockInsiderTransaction = true;
		}
		for (lCurrentUpdateInsiderTransactionPos = 0; lCurrentUpdateInsiderTransactionPos < lStockSetSize; lCurrentUpdateInsiderTransactionPos++) {
			pStock = GetStock(lCurrentUpdateInsiderTransactionPos);
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
			p->SetIndex(lCurrentUpdateInsiderTransactionPos);
			m_qFinnhubProduct.push(p);
			SetFinnhubInquiring(true);
			SetCurrentFunction(_T("�ڲ�����:") + pStock->GetSymbol());
			//TRACE("����%s �ڲ���������\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockInsiderTransaction = false;
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
	static long s_lCurrentRTDataQuotePos = 0;
	CWebSourceDataProductPtr p = nullptr;

	ASSERT(IsSystemReady());
	if (!IsFinnhubInquiring()) {
		p = make_shared<CProductFinnhubStockPriceQuote>();
		p->SetIndex(s_lCurrentRTDataQuotePos);
		m_qFinnhubProduct.push(p);
		SetFinnhubInquiring(true);
		s_lCurrentRTDataQuotePos++;
		if (s_lCurrentRTDataQuotePos == GetStockSize()) s_lCurrentRTDataQuotePos = 0;
		SetCurrentFunction(_T("Finnhub stock RT: ") + GetStock(s_lCurrentRTDataQuotePos)->GetSymbol());
		TRACE("����%sʵʱ����\n", GetStock(s_lCurrentRTDataQuotePos)->GetSymbol().GetBuffer());
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubPeer(void) {
	static bool s_fInquiringFinnhubStockPeer = false;
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = GetStockSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;
	long lCurrentUpdatePeerPos = 0;

	ASSERT(IsSystemReady());
	if (!IsFinnhubPeerUpdated() && !IsFinnhubInquiring()) {
		if (!s_fInquiringFinnhubStockPeer) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock peer..."));
			s_fInquiringFinnhubStockPeer = true;
		}
		for (lCurrentUpdatePeerPos = 0; lCurrentUpdatePeerPos < lStockSetSize; lCurrentUpdatePeerPos++) {
			if (!GetStock(lCurrentUpdatePeerPos)->IsPeerUpdated()) {
				pStock = GetStock(lCurrentUpdatePeerPos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(this, __PEERS__);
			p->SetIndex(lCurrentUpdatePeerPos);
			m_qFinnhubProduct.push(p);
			SetFinnhubInquiring(true);
			SetCurrentFunction(_T("Peer:") + GetStock(lCurrentUpdatePeerPos)->GetSymbol());
			//TRACE("����%s Peer����\n", GetStock(lCurrentUpdatePeerPos)->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockPeer = false;
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
		SetCurrentFunction(_T("Finnhub ecomomic calendar"));
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
		SetCurrentFunction(_T("Finnhub forex exchange"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub forex exchange"));
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubForexSymbol(void) {
	static long s_lCurrentForexExchangePos = 0;
	CWebSourceDataProductPtr p = nullptr;

	if (!IsFinnhubForexSymbolUpdated() && !IsFinnhubInquiring()) {
		p = m_FinnhubFactory.CreateProduct(this, __FOREX_SYMBOLS__);
		p->SetIndex(s_lCurrentForexExchangePos++);
		m_qFinnhubProduct.push(p);
		SetFinnhubInquiring(true);
		if (s_lCurrentForexExchangePos >= m_dataFinnhubForexExchange.GetForexExchangeSize()) {
			SetFinnhubForexSymbolUpdated(true);
			s_lCurrentForexExchangePos = 0;
			SetCurrentFunction(_T("Finnhub forex synmbol"));
			gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub Forex sysbols"));
		}
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubForexDayLine(void) {
	static bool s_fInquiringFinnhubForexDayLine = false;
	bool fFound = false;
	CForexSymbolPtr pForexSymbol;
	CString str = _T("");
	const long lStockSetSize = m_dataFinnhubForexSymbol.GetForexSymbolSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;
	long lCurrentUpdateForexDayLinePos = 0;

	ASSERT(IsSystemReady());
	if (!IsFinnhubForexDayLineUpdated() && !IsFinnhubInquiring()) {
		if (!s_fInquiringFinnhubForexDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub forex day line..."));
			s_fInquiringFinnhubForexDayLine = true;
		}
		for (lCurrentUpdateForexDayLinePos = 0; lCurrentUpdateForexDayLinePos < lStockSetSize; lCurrentUpdateForexDayLinePos++) {
			if (GetForexSymbol(lCurrentUpdateForexDayLinePos)->IsDayLineNeedUpdate()) {
				pForexSymbol = GetForexSymbol(lCurrentUpdateForexDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(this, __FOREX_CANDLES__);
			p->SetIndex(lCurrentUpdateForexDayLinePos);
			m_qFinnhubProduct.push(p);
			SetCurrentFunction(_T("Finnhub Forex���ߣ�") + pForexSymbol->GetSymbol());
			SetFinnhubInquiring(true);
			pForexSymbol->SetDayLineNeedUpdate(false);
			SetCurrentFunction(_T("����:") + pForexSymbol->GetSymbol());
			TRACE("����%s��������\n", pForexSymbol->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubForexDayLine = false;
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
		SetCurrentFunction(_T("Finnhub crypto exchange"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub crypto exchange"));
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubCryptoSymbol(void) {
	static long s_lCurrentCryptoExchangePos = 0;
	CWebSourceDataProductPtr p = nullptr;

	if (!IsFinnhubCryptoSymbolUpdated() && !IsFinnhubInquiring()) {
		p = m_FinnhubFactory.CreateProduct(this, __CRYPTO_SYMBOLS__);
		p->SetIndex(s_lCurrentCryptoExchangePos++);
		m_qFinnhubProduct.push(p);
		SetFinnhubInquiring(true);
		if (s_lCurrentCryptoExchangePos >= m_dataFinnhubCryptoExchange.GetCryptoExchangeSize()) {
			SetFinnhubCryptoSymbolUpdated(true);
			SetCurrentFunction(_T("Finnhub crypto synmbol"));
			gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub Crypto sysbols"));
			s_lCurrentCryptoExchangePos = 0;
		}
		return true;
	}
	return false;
}

bool CWorldMarket::TaskInquiryFinnhubCryptoDayLine(void) {
	static bool s_fInquiringFinnhubCryptoDayLine = false;
	bool fFound = false;
	CFinnhubCryptoSymbolPtr pCryptoSymbol;
	CString str = _T("");
	const long lStockSetSize = m_dataFinnhubCryptoSymbol.GetCryptoSymbolSize();
	bool fHaveInquiry = false;
	CWebSourceDataProductPtr p = nullptr;
	long lCurrentUpdateCryptoDayLinePos = 0;

	ASSERT(IsSystemReady());
	if (!IsFinnhubCryptoDayLineUpdated() && !IsFinnhubInquiring()) {
		if (!s_fInquiringFinnhubCryptoDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub forex day line..."));
			s_fInquiringFinnhubCryptoDayLine = true;
		}
		for (lCurrentUpdateCryptoDayLinePos = 0; lCurrentUpdateCryptoDayLinePos < lStockSetSize; lCurrentUpdateCryptoDayLinePos++) {
			if (GetFinnhubCryptoSymbol(lCurrentUpdateCryptoDayLinePos)->IsDayLineNeedUpdate()) {
				pCryptoSymbol = GetFinnhubCryptoSymbol(lCurrentUpdateCryptoDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(this, __CRYPTO_CANDLES__);
			p->SetIndex(lCurrentUpdateCryptoDayLinePos);
			m_qFinnhubProduct.push(p);
			SetCurrentFunction(_T("Finnhub Crypto���ߣ�") + pCryptoSymbol->GetSymbol());
			SetFinnhubInquiring(true);
			pCryptoSymbol->SetDayLineNeedUpdate(false);
			SetCurrentFunction(_T("����:") + pCryptoSymbol->GetSymbol());
			//TRACE("����%s��������\n", pCryptoSymbol->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubCryptoDayLine = false;
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
		SetCurrentFunction(_T("Tiingo stock synmbol"));
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
		SetCurrentFunction(_T("Tiingo crypto synmbol"));
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
			SetCurrentFunction(_T("Tiingo Stock���ߣ�") + pStock->GetSymbol());
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
		thread thread1(ThreadUpdateTiingoIndustry, this);
		thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateSICIndustry(void) {
	if (IsFinnhubStockProfileUpdated()) {
		thread thread1(ThreadUpdateSICIndustry, this);
		thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateNaicsIndustry(void) {
	if (IsFinnhubStockProfileUpdated()) {
		thread thread1(ThreadUpdateNaicsIndustry, this);
		thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
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
					thread thread1(ThreadUpdateForexDayLineDB, pSymbol.get());
					thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
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
					thread thread1(ThreadUpdateCryptoDayLineDB, pSymbol.get());
					thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
					fUpdated = true;
					//TRACE("����%s��������\n", pSymbol->GetSymbol().GetBuffer());
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
			thread thread1(ThreadUpdateEPSSurpriseDB, pStock.get());
			thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
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

bool CWorldMarket::TaskUpdateDayLineStartEndDate(void) {
	thread thread1(ThreadUpdateWorldStockDayLineStartEndDate, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateDayLineDB() {
	thread thread1(ThreadUpdateWorldStockDayLineDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateStockProfileDB(void) {
	thread thread1(ThreadUpdateStockProfileDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateBasicFinancialDB(void) {
	thread thread1(ThreadUpdateBasicFinancialDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateCountryListDB(void) {
	thread thread1(ThreadUpdateCountryListDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateInsiderTransactionDB(void) {
	thread thread1(ThreadUpdateInsiderTransactionDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateTiingoStockDB(void) {
	thread thread1(ThreadUpdateTiingoStockDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateTiingoCryptoSymbolDB(void) {
	thread thread1(ThreadUpdateTiingoCryptoSymbolDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateForexExchangeDB(void) {
	thread thread1(ThreadUpdateForexExchangeDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateForexSymbolDB() {
	thread thread1(ThreadUpdateForexSymbolDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateCryptoExchangeDB(void) {
	thread thread1(ThreadUpdateCryptoExchangeDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateFinnhubCryptoSymbolDB() {
	thread thread1(ThreadUpdateFinnhubCryptoSymbolDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CWorldMarket::TaskUpdateEconomicCalendarDB(void) {
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
		if (setOption.m_FinnhubMaxPerHour > 0) {
			gl_pFinnhubWebInquiry->SetShortestINquiringInterval(3600000 / setOption.m_FinnhubMaxPerHour);
		}
	}
	else {
		gl_systemMessage.PushInformationMessage(_T("Finnhub Token Needed"));
	}
	if (setOption.m_TiingoToken.GetLength() > 5) {
		gl_pTiingoWebInquiry->SetInquiryingStringSuffix(setOption.m_TiingoToken);
		if (setOption.m_TiingoMaxPerHour > 0) {
			gl_pTiingoWebInquiry->SetShortestINquiringInterval(3600000 / setOption.m_TiingoMaxPerHour);
		}
	}
	else {
		gl_systemMessage.PushInformationMessage(_T("Tiingo Token Needed"));
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
				//TRACE("����%s�ڲ���������\n", pStock->GetSymbol().GetBuffer());
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

	return true;
}

bool CWorldMarket::RebuildPeer(void) {
	m_dataWorldStock.ResetPeer();
	SetFinnhubPeerUpdated(false);

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

bool CWorldMarket::TaskReActivateWebSocket(void) {
	if (IsSystemReady()) {
		if (!m_finnhubWebSocket.IsReceivingData()) {
			m_finnhubWebSocket.DeconnectingWithoutWaitingSucceed();
			gl_systemMessage.PushInnerSystemInformationMessage(_T("ֹͣFinnhub web socket����"));
		}
		else {
			m_finnhubWebSocket.SetReceivingData(false);
		}
		if (!m_tiingoIEXWebSocket.IsReceivingData()) {
			m_tiingoIEXWebSocket.DeconnectingWithoutWaitingSucceed();
			gl_systemMessage.PushInnerSystemInformationMessage(_T("ֹͣTiingo IEX web socket����"));
		}
		else {
			m_tiingoIEXWebSocket.SetReceivingData(false);
		}
		if (!m_tiingoCryptoWebSocket.IsReceivingData()) {
			m_tiingoCryptoWebSocket.DeconnectingWithoutWaitingSucceed();
			gl_systemMessage.PushInnerSystemInformationMessage(_T("ֹͣTiingo Crypto web socket����"));
		}
		else {
			m_tiingoCryptoWebSocket.SetReceivingData(false);
		}
		if (!m_tiingoForexWebSocket.IsReceivingData()) {
			m_tiingoForexWebSocket.DeconnectingWithoutWaitingSucceed();
			gl_systemMessage.PushInnerSystemInformationMessage(_T("ֹͣTiingo Forex web socket����"));
		}
		else {
			m_tiingoForexWebSocket.SetReceivingData(false);
		}
	}
	return true;
}

bool CWorldMarket::TaskActivateWebSocket(void) {
	if (IsSystemReady()) {
		if (m_finnhubWebSocket.IsClosed()) {
			m_finnhubWebSocket.CreatingThreadConnectWebSocketAndSendMessage(GetFinnhubWebSocketSymbolVector());
			gl_systemMessage.PushInnerSystemInformationMessage(_T("����Finnhub web socket����"));
		}
		if (m_tiingoIEXWebSocket.IsClosed()) {
			m_tiingoIEXWebSocket.CreatingThreadConnectWebSocketAndSendMessage(GetTiingoIEXWebSocketSymbolVector());
			gl_systemMessage.PushInnerSystemInformationMessage(_T("����Tiingo IEX web socket����"));
		}
		if (m_tiingoCryptoWebSocket.IsClosed()) {
			m_tiingoCryptoWebSocket.CreatingThreadConnectWebSocketAndSendMessage(GetTiingoCryptoWebSocketSymbolVector());
			gl_systemMessage.PushInnerSystemInformationMessage(_T("����Tiingo Crypto web socket����"));
		}
		if (m_tiingoForexWebSocket.IsClosed()) {
			m_tiingoForexWebSocket.CreatingThreadConnectWebSocketAndSendMessage(GetTiingoForexWebSocketSymbolVector());
			gl_systemMessage.PushInnerSystemInformationMessage(_T("����Tiingo Forex web socket����"));
		}
	}
	return true;
}

bool CWorldMarket::TaskProcessWebSocketData(void) {
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
		if (m_fRecordFinnhubWebSocket) {
			strMessage = _T("Finnhub: ");
			strMessage += (*pString).c_str();
			gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		}
		iTotalDataSize += pString->size();
		m_finnhubWebSocket.ParseFinnhubWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedFinnhubWebSocket(iTotalDataSize);

	return true;
}

bool CWorldMarket::ProcessTiingoIEXWebSocketData() {
	auto total = gl_WebInquirer.GetTiingoIEXWebSocketDataSize();
	CString strMessage;
	shared_ptr<string> pString;
	size_t iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopTiingoIEXWebSocketData();
		if (m_fRecordTiingoIEXWebSocket) {
			strMessage = _T("Tiingo IEX: ");
			strMessage += (*pString).c_str();
			gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		}
		iTotalDataSize += pString->size();
		m_tiingoIEXWebSocket.ParseTiingoIEXWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoIEXWebSocket(iTotalDataSize);
	return true;
}

bool CWorldMarket::ProcessTiingoCryptoWebSocketData() {
	auto total = gl_WebInquirer.GetTiingoCryptoWebSocketDataSize();
	CString strMessage;

	shared_ptr<string> pString;
	int iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopTiingoCryptoWebSocketData();
		if (m_fRecordTiingoCryptoWebSocket) {
			strMessage = _T("Tiingo Crypto: ");
			strMessage += (*pString).c_str();
			gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		}
		iTotalDataSize += pString->size();
		m_tiingoCryptoWebSocket.ParseTiingoCryptoWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoCryptoWebSocket(iTotalDataSize);
	return true;
}

bool CWorldMarket::ProcessTiingoForexWebSocketData() {
	auto total = gl_WebInquirer.GetTiingoForexWebSocketDataSize();
	CString strMessage;
	shared_ptr<string> pString;
	int iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopTiingoForexWebSocketData();
		if (m_fRecordTiingoForexWebSocket) {
			strMessage = _T("Tiingo Forex: ");
			strMessage += (*pString).c_str();
			gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		}
		iTotalDataSize += pString->size();
		m_tiingoForexWebSocket.ParseTiingoForexWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoForexWebSocket(iTotalDataSize);
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
		gl_systemMessage.SetCurrentTiingoWebSocketIEX(pIEXData->m_strSymbol);
	}

	total = gl_SystemData.GetTiingoCryptoSocketSize();
	for (auto i = 0; i < total; i++) {
		pCryptoData = gl_SystemData.PopTiingoCryptoSocket();
		gl_systemMessage.SetCurrentTiingoWebSocketCrypto(pCryptoData->m_strSymbol);
	}

	total = gl_SystemData.GetTiingoForexSocketSize();
	for (auto i = 0; i < total; i++) {
		pForexData = gl_SystemData.PopTiingoForexSocket();
		gl_systemMessage.SetCurrentTiingoWebSocketForex(pForexData->m_strSymbol);
	}

	total = gl_SystemData.GetFinnhubSocketSize();
	for (auto i = 0; i < total; i++) {
		pFinnhubData = gl_SystemData.PopFinnhubSocket();
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