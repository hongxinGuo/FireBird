#include "WorldMarket.h"
#include"pch.h"

#include"ThreadStatus.h"

#include "WorldMarket.h"
#include"FinnhubInquiryType.h"
#include"thread.h"
#include"Callablefunction.h"
#include"WebInquirer.h"

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
		TRACE("CWorldMarket市场变量只允许存在一个实例\n");
	}

	// 无需（也无法）每日更新的变量放在这里
	m_lCurrentUpdateEPSSurprisePos = 0;
	m_lCurrentUpdateDayLinePos = 0; // 由于证券代码总数有二十万之多，无法在一天之内更新完，故不再重置此索引。
	m_bFinnhubWebSiteAccessible = true;

	m_strMarketId = _T("美国市场");
	m_lMarketTimeZone = 4 * 3600; // 美国股市使用美东标准时间, GMT + 4
	CalculateTime();

	Reset();

	//thread thread1(ThreadWorldMarketBackground);
	//thread1.detach();
}

CWorldMarket::~CWorldMarket() {
	PreparingExitMarket();

	if (!gl_systemStatus.IsExitingSystem()) {
		gl_systemStatus.SetExitingSystem(true);
		while (gl_ThreadStatus.IsWorldMarketBackgroundThreadRunning()) Sleep(1);
		gl_systemStatus.SetExitingSystem(false);
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
	m_dataFinnhubStockExchange.Reset();
	m_dataFinnhubForexExchange.Reset();

	m_fRebulidDayLine = false;
	SetSystemReady(false); // 市场初始状态为未设置好。
}

void CWorldMarket::ResetQuandl(void) {
}

void CWorldMarket::ResetTiingo(void) {
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

	UpdateToken();

	LoadWorldExchangeDB(); // 装入世界交易所信息
	LoadCountryDB();
	LoadStockDB();
	LoadWorldChoicedStock();
	LoadForexExchange();
	LoadFinnhubForexSymbol();
	LoadWorldChoicedForex();
	LoadCryptoExchange();
	LoadFinnhubCryptoSymbol();
	LoadWorldChoicedCrypto();
	LoadEconomicCalendarDB();
	LoadTiingoStock();
	LoadTiingoCryptoSymbol();

	for (auto& pDataSource : m_vDataSource) {
		pDataSource->Reset();
	}

	CString str = _T("重置World Market于美东标准时间：");
	str += GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(str);
}

bool CWorldMarket::PreparingExitMarket(void)
{
	ASSERT(gl_systemStatus.IsExitingSystem());
	DeconnectingAllWebSocket();
	while (gl_ThreadStatus.IsWorldMarketBackgroundThreadRunning()) Sleep(1);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// 默认每100毫秒执行一次。
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::SchedulingTask(void) {
	CVirtualMarket::SchedulingTask();

	static time_t s_lastTimeSchedulingTask = 0;
	const long lCurrentTime = GetMarketTime();

	TaskCheckSystemReady();

	ProcessMessageAndReceivedData(lCurrentTime);

	//根据时间，调度各项定时任务.每秒调度一次
	if (GetUTCTime() > s_lastTimeSchedulingTask) {
		SchedulingTaskPerSecond(GetUTCTime() - s_lastTimeSchedulingTask, lCurrentTime);
		s_lastTimeSchedulingTask = GetUTCTime();
	}

	return true;
}

void CWorldMarket::ProcessMessageAndReceivedData(long lCurrentTime) {
	for (auto& pDataSource : m_vDataSource) {
		pDataSource->Run(lCurrentTime);
	}
}

bool CWorldMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
	static int s_iCount1Hour = 3576; // 与五分钟每次的错开11秒钟，与一分钟每次的错开22秒钟
	static int s_iCount5Minute = 287; // 与一分钟每次的错开11秒钟
	static int s_iCount1Minute = 58; // 与10秒每次的错开1秒钟
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

	if (!IsTimeToResetSystem(lCurrentTime)) { // 下午五时重启系统，各数据库需要重新装入，故而此时不允许更新。
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

	// 这个必须是最后一个任务。因其在执行完毕后返回了。
	if (!IsTimeToResetSystem(lCurrentTime)) { // 下午五时重启系统，各数据库需要重新装入，故而此时不允许更新数据库。
		if (m_dataFinnhubCountry.GetLastTotalCountry() < m_dataFinnhubCountry.GetTotalCountry()) {
			TaskUpdateCountryListDB();
		}
		if (IsNeedUpdateForexExchangeDB()) TaskUpdateForexExchangeDB();
		if (IsNeedUpdateForexSymbolDB()) TaskUpdateForexSymbolDB();
		if (IsNeedUpdateCryptoExchangeDB()) TaskUpdateCryptoExchangeDB();
		if (IsNeedUpdateCryptoSymbolDB())  TaskUpdateFinnhubCryptoSymbolDB();
		if (IsNeedUpdateInsiderTransactionDB()) TaskUpdateInsiderTransactionDB();
		if (IsNeedUpdateInsiderSentimentDB()) TaskUpdateInsiderSentimentDB();
		TaskUpdateForexDayLineDB();
		TaskUpdateCryptoDayLineDB();
		if (IsNeedSaveStockDayLineDB()) TaskUpdateDayLineDB();
		TaskUpdateEPSSurpriseDB();
		if (IsNeedUpdateExonomicCalendarDB()) TaskUpdateEconomicCalendarDB();
		return true;
	}

	return false;
}

bool CWorldMarket::SchedulingTaskPer5Minute(long lCurrentTime) {
	// 建立WebSocket连接
	StartWebSocket();

	if (gl_pFinnhubWebInquiry->IsWebError()) { // finnhub.io有时被屏蔽，故而出现错误时暂停一会儿。每五分钟清除标志一次
		gl_pFinnhubWebInquiry->SetWebError(false);
	}

	if (gl_pDataSourceFinnhub->IsSymbolUpdated() && IsStockProfileNeedUpdate()) {
		TaskUpdateStockProfileDB();
	}

	if (IsBasicFinancialNeedUpdate()) {
		TaskUpdateBasicFinancialDB();
	}

	if (IsNeedUpdateTiingoStock()) {
		TaskUpdateTiingoStockDB();
	}

	if (IsNeedUpdateTiingoCryptoSymbol()) {
		TaskUpdateTiingoCryptoSymbolDB();
	}

	return true;
}

bool CWorldMarket::SchedulingTaskPerHour(long lCurrentTime) {
	RestartWebSocket();

	return true;
}

/// <summary>
/// /////////////////////////////////////////////////////////////////////////////////
///
/// 此任务必须放置于每分钟执行一次的调度中。
/// <returns></returns>
///
bool CWorldMarket::TaskResetMarket(long lCurrentTime) {
	// 市场时间十七时重启系统
	if (IsPermitResetMarket()) { // 如果允许重置系统
		if ((lCurrentTime > 170000) && (lCurrentTime <= 170100)) { // 本市场时间的下午五时(北京时间上午五时重启本市场。这样有利于接收日线数据。
			SetResetMarket(true);// 只是设置重启标识，实际重启工作由CMainFrame的OnTimer函数完成。
			SetPermitResetMarket(false); // 今天不再允许重启系统。
			SetSystemReady(false);
			return true;
		}
	}
	return false;
}

bool CWorldMarket::TaskUpdateTiingoIndustry(void) {
	if (gl_pDataSourceFinnhub->IsStockProfileUpdated()) {
		thread thread1(ThreadUpdateTiingoIndustry, this);
		thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateSICIndustry(void) {
	if (gl_pDataSourceFinnhub->IsStockProfileUpdated()) {
		thread thread1(ThreadUpdateSICIndustry, this);
		thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
		return true;
	}
	return false;
}

bool CWorldMarket::TaskUpdateNaicsIndustry(void) {
	if (gl_pDataSourceFinnhub->IsStockProfileUpdated()) {
		thread thread1(ThreadUpdateNaicsIndustry, this);
		thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将Forex日线数据存入数据库．
//  此函数由工作线程ThreadForexDayLineSaveProc调用，尽量不要使用全局变量。(目前使用主线程调用之，以消除同步问题的出现）
//
// 无论是否执行了存储函数，都需要将下载的日线历史数据删除，这样能够节省内存的占用。由于实际存储功能使用线程模式实现，
// 故而其执行时间可能晚于主线程，导致主线程删除日线数据时出现同步问题。解决的方法是让工作线程独立删除存储后的日线数据，
// 主线程的删除函数只在不调用工作线程（无需存储日线数据）的情况下方才执行。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::TaskUpdateForexDayLineDB(void) {
	CString str;
	bool fUpdated = false;
	CForexSymbolPtr pSymbol = nullptr;

	for (int i = 0; i < m_dataFinnhubForexSymbol.GetForexSymbolSize(); i++) {
		if (gl_systemStatus.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
		pSymbol = m_dataFinnhubForexSymbol.GetForexSymbol(i);
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) { // 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					thread thread1(ThreadUpdateForexDayLineDB, pSymbol.get());
					thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
					fUpdated = true;
					TRACE("更新%s日线数据\n", pSymbol->GetSymbol().GetBuffer());
				}
				else pSymbol->UnloadDayLine(); // 当无需执行存储函数时，这里还要单独卸载日线数据。因存储日线数据线程稍后才执行，故而不能在此统一执行删除函数。
			}
			else { // 此种情况为有股票代码，但此代码尚未上市
				pSymbol->SetIPOStatus(__STOCK_NOT_YET_LIST__);
				CString str1 = pSymbol->GetSymbol();
				str1 += _T(" 为未上市股票代码");
				gl_systemMessage.PushDayLineInfoMessage(str1);
			}
		}
	}

	return(fUpdated);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将Crypto日线数据存入数据库．
//  此函数由工作线程ThreadCryptoDayLineSaveProc调用，尽量不要使用全局变量。(目前使用主线程调用之，以消除同步问题的出现）
//
// 无论是否执行了存储函数，都需要将下载的日线历史数据删除，这样能够节省内存的占用。由于实际存储功能使用线程模式实现，
// 故而其执行时间可能晚于主线程，导致主线程删除日线数据时出现同步问题。解决的方法是让工作线程独立删除存储后的日线数据，
// 主线程的删除函数只在不调用工作线程（无需存储日线数据）的情况下方才执行。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::TaskUpdateCryptoDayLineDB(void) {
	CString str;
	bool fUpdated = false;
	CFinnhubCryptoSymbolPtr pSymbol = nullptr;

	for (int i = 0; i < m_dataFinnhubCryptoSymbol.GetCryptoSymbolSize(); i++) {
		if (gl_systemStatus.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
		pSymbol = m_dataFinnhubCryptoSymbol.GetCryptoSymbol(i);
		if (pSymbol->IsDayLineNeedSavingAndClearFlag()) { // 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			if (pSymbol->GetDayLineSize() > 0) {
				if (pSymbol->HaveNewDayLineData()) {
					thread thread1(ThreadUpdateCryptoDayLineDB, pSymbol.get());
					thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
					fUpdated = true;
					//TRACE("更新%s日线数据\n", pSymbol->GetSymbol().GetBuffer());
				}
				else pSymbol->UnloadDayLine(); // 当无需执行存储函数时，这里还要单独卸载日线数据。因存储日线数据线程稍后才执行，故而不能在此统一执行删除函数。
			}
			else { // 此种情况为有股票代码，但此代码尚未上市；或者是已退市股票
				if (pSymbol->GetDayLineEndDate() > 19800101) { // 已退市
					pSymbol->SetIPOStatus(__STOCK_DELISTED__);
					pSymbol->SetUpdateProfileDB(true);
				}
				else { // 此种情况为有股票代码，但此代码尚未上市
					pSymbol->SetIPOStatus(__STOCK_NOT_YET_LIST__);
					CString str1 = pSymbol->GetSymbol();
					str1 += _T(" 为未上市股票代码");
					gl_systemMessage.PushDayLineInfoMessage(str1);
				}
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
		if (pStock->IsEPSSurpriseNeedSaveAndClearFlag()) { // 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			thread thread1(ThreadUpdateEPSSurpriseDB, pStock.get());
			thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
			TRACE("更新%s EPS surprise数据\n", pStock->GetSymbol().GetBuffer());
		}
		if (gl_systemStatus.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
	}

	return(true);
}

bool CWorldMarket::TaskCheckSystemReady(void) {
	CString str = _T("");

	if (!IsSystemReady()) {
		if (gl_pDataSourceFinnhub->IsSymbolUpdated() && gl_pDataSourceFinnhub->IsForexExchangeUpdated() && gl_pDataSourceFinnhub->IsForexSymbolUpdated()
			&& gl_pDataSourceFinnhub->IsCryptoExchangeUpdated() && gl_pDataSourceFinnhub->IsCryptoSymbolUpdated()) {
			str = _T("世界市场初始化完毕");
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
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateDayLineDB() {
	thread thread1(ThreadUpdateWorldStockDayLineDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateStockProfileDB(void) {
	thread thread1(ThreadUpdateWorldMarketStockProfileDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateBasicFinancialDB(void) {
	thread thread1(ThreadUpdateBasicFinancialDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateCountryListDB(void) {
	thread thread1(ThreadUpdateCountryListDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateInsiderTransactionDB(void) {
	thread thread1(ThreadUpdateInsiderTransactionDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateInsiderSentimentDB(void) {
	thread thread1(ThreadUpdateInsiderSentimentDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateTiingoStockDB(void) {
	thread thread1(ThreadUpdateTiingoStockDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateTiingoCryptoSymbolDB(void) {
	thread thread1(ThreadUpdateTiingoCryptoSymbolDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateForexExchangeDB(void) {
	thread thread1(ThreadUpdateForexExchangeDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateForexSymbolDB() {
	thread thread1(ThreadUpdateForexSymbolDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateCryptoExchangeDB(void) {
	thread thread1(ThreadUpdateCryptoExchangeDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateFinnhubCryptoSymbolDB() {
	thread thread1(ThreadUpdateFinnhubCryptoSymbolDB, this);
	thread1.detach();// 必须分离之，以实现并行操作，并保证由系统回收资源。
	return true;
}

bool CWorldMarket::TaskUpdateEconomicCalendarDB(void) {
	thread thread1(ThreadUpdateEconomicCalendarDB, this);
	thread1.detach();

	return true;
}

void CWorldMarket::StopReceivingWebSocket(void) {
	if (!gl_systemConfigeration.IsUsingFinnhubWebSocket()) m_finnhubWebSocket.Deconnecting();
	if (!gl_systemConfigeration.IsUsingTiingoIEXWebSocket()) m_tiingoIEXWebSocket.Deconnecting();
	if (!gl_systemConfigeration.IsUsingTiingoCryptoWebSocket()) m_tiingoCryptoWebSocket.Deconnecting();
	if (!gl_systemConfigeration.IsUsingTiingoForexWebSocket()) m_tiingoForexWebSocket.Deconnecting();
}

void CWorldMarket::DeconnectingAllWebSocket(void) {
	if (gl_systemConfigeration.IsUsingFinnhubWebSocket()) m_finnhubWebSocket.Deconnecting();
	if (gl_systemConfigeration.IsUsingTiingoIEXWebSocket()) m_tiingoIEXWebSocket.Deconnecting();
	if (gl_systemConfigeration.IsUsingTiingoCryptoWebSocket()) m_tiingoCryptoWebSocket.Deconnecting();
	if (gl_systemConfigeration.IsUsingTiingoForexWebSocket()) m_tiingoForexWebSocket.Deconnecting();
}

bool CWorldMarket::UpdateToken(void) {
	ASSERT(gl_systemConfigeration.IsInitialized());

	if (gl_systemConfigeration.GetFinnhubToken().GetLength() > 5) {
		gl_pFinnhubWebInquiry->SetInquiryingStringSuffix(gl_systemConfigeration.GetFinnhubToken());
	}
	else {
		gl_systemMessage.PushInformationMessage(_T("Finnhub Token Needed"));
	}
	if (gl_systemConfigeration.GetTiingoToken().GetLength() > 5) {
		gl_pTiingoWebInquiry->SetInquiryingStringSuffix(gl_systemConfigeration.GetTiingoToken());
	}
	else {
		gl_systemMessage.PushInformationMessage(_T("Tiingo Token Needed"));
	}
	if (gl_systemConfigeration.GetQuandlToken().GetLength() > 5) {
		gl_pQuandlWebInquiry->SetInquiryingStringSuffix(gl_systemConfigeration.GetQuandlToken());
	}
	else {
		gl_systemMessage.PushInformationMessage(_T("Quandl Token Needed"));
	}

	return true;
}

bool CWorldMarket::UpdateStockDayLineDB(void) {
	CWorldStockPtr pStock = nullptr;

	for (long i = 0; i < GetStockSize(); i++) {
		pStock = GetStock(i);
		pStock->UpdateDayLineDB();
		if (gl_systemStatus.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
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
				str = pStock->GetSymbol() + _T("内部交易资料更新完成");
				gl_systemMessage.PushDayLineInfoMessage(str);
				//TRACE("更新%s内部交易数据\n", pStock->GetSymbol().GetBuffer());
			}
		}
		if (gl_systemStatus.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
	}
	return true;
}

bool CWorldMarket::UpdateInsiderSentimentDB(void) {
	CString str;
	CWorldStockPtr pStock = nullptr;

	for (long i = 0; i < GetStockSize(); i++) {
		pStock = GetStock(i);
		if (pStock->IsInsiderSentimentNeedSaveAndClearFlag()) {
			if (pStock->HaveInsiderSentiment()) {
				pStock->SaveInsiderSentiment();
				str = pStock->GetSymbol() + _T("内部交易情绪资料更新完成");
				gl_systemMessage.PushDayLineInfoMessage(str);
				//TRACE("更新%s内部交易数据\n", pStock->GetSymbol().GetBuffer());
			}
		}
		if (gl_systemStatus.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
	}
	return true;
}

bool CWorldMarket::UpdateTiingoIndustry(void) {
	// 尚未实现
	return false;
}

bool CWorldMarket::UpdateSICIndustry(void) {
	// 尚未实现
	return false;
}

bool CWorldMarket::UpdateNaicsIndustry(void) {
	// 尚未实现
	return false;
}

bool CWorldMarket::RebuildEPSSurprise(void) {
	m_dataWorldStock.ResetEPSSurprise();
	gl_pDataSourceFinnhub->SetEPSSurpriseUpdated(false);

	return true;
}

bool CWorldMarket::RebuildPeer(void) {
	m_dataWorldStock.ResetPeer();
	gl_pDataSourceFinnhub->SetPeerUpdated(false);

	return true;
}

bool CWorldMarket::RebuildBasicFinancial(void) {
	m_dataWorldStock.ResetBasicFinancial();
	gl_pDataSourceFinnhub->SetStockBasicFinancialUpdated(false);

	return true;
}

bool CWorldMarket::RebuildStockDayLineDB(void) {
	m_dataWorldStock.ResetDayLine();
	gl_pDataSourceFinnhub->SetStockProfileUpdated(false);

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
/// Finnhub WebSocket的免费账户最多只能发送50个证券的数据
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
// Tiingo对免费账户的流量限制，为500次/小时， 20000次/天， 5GB/月。
//
// thresholdlevel 0接收所有的IEX数据时，每秒数据量为1M-9M;thresholdlevel5接收所有IEX数据时，每秒数据量为10-50K。
//
// thresholdlevel 5：all Last Trade updates and only Quote updates that are deemed major updates by our system.
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
// 接收所有的Crypto数据时，每秒数据量大致在50-100K附近。
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
// 5状态下每秒接收100K左右。
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

bool CWorldMarket::RestartWebSocket(void) {
	if (IsSystemReady()) {
		if (!gl_pFinnhubWebInquiry->IsWebError()) RestartFinnhubWebSocket();
		if (!gl_pTiingoWebInquiry->IsWebError()) RestartTiingoWebSocket();
	}
	return true;
}

void CWorldMarket::RestartFinnhubWebSocket(void) {
	if (gl_systemConfigeration.IsUsingFinnhubWebSocket()) {
		if (!m_finnhubWebSocket.IsReceivingData()) {
			m_finnhubWebSocket.DeconnectingWithoutWaitingSucceed();
			gl_systemMessage.PushInnerSystemInformationMessage(_T("停止Finnhub web socket服务"));
		}
		else {
			m_finnhubWebSocket.SetReceivingData(false);
		}
	}
}

void CWorldMarket::RestartTiingoWebSocket(void) {
	if (gl_systemConfigeration.IsUsingTiingoIEXWebSocket()) {
		if (!m_tiingoIEXWebSocket.IsReceivingData()) {
			m_tiingoIEXWebSocket.DeconnectingWithoutWaitingSucceed();
			gl_systemMessage.PushInnerSystemInformationMessage(_T("停止Tiingo IEX web socket服务"));
		}
		else {
			m_tiingoIEXWebSocket.SetReceivingData(false);
		}
	}
	if (gl_systemConfigeration.IsUsingTiingoCryptoWebSocket()) {
		if (!m_tiingoCryptoWebSocket.IsReceivingData()) {
			m_tiingoCryptoWebSocket.DeconnectingWithoutWaitingSucceed();
			gl_systemMessage.PushInnerSystemInformationMessage(_T("停止Tiingo Crypto web socket服务"));
		}
		else {
			m_tiingoCryptoWebSocket.SetReceivingData(false);
		}
	}
	if (gl_systemConfigeration.IsUsingTiingoForexWebSocket()) {
		if (!m_tiingoForexWebSocket.IsReceivingData()) {
			m_tiingoForexWebSocket.DeconnectingWithoutWaitingSucceed();
			gl_systemMessage.PushInnerSystemInformationMessage(_T("停止Tiingo Forex web socket服务"));
		}
		else {
			m_tiingoForexWebSocket.SetReceivingData(false);
		}
	}
}

bool CWorldMarket::StartWebSocket(void) {
	if (IsSystemReady()) {
		if (!gl_pFinnhubWebInquiry->IsWebError()) StartFinnhubWebSocket();
		if (!gl_pTiingoWebInquiry->IsWebError()) StartTiingoWebSocket();
	}
	return true;
}

void CWorldMarket::StartFinnhubWebSocket(void) {
	if (gl_systemConfigeration.IsUsingFinnhubWebSocket()) {
		if (m_finnhubWebSocket.IsClosed()) {
			m_finnhubWebSocket.CreatingThreadConnectWebSocketAndSendMessage(GetFinnhubWebSocketSymbolVector());
			gl_systemMessage.PushInnerSystemInformationMessage(_T("开启Finnhub web socket服务"));
		}
	}
}

void CWorldMarket::StartTiingoWebSocket(void) {
	if (gl_systemConfigeration.IsUsingTiingoIEXWebSocket()) {
		if (m_tiingoIEXWebSocket.IsClosed()) {
			m_tiingoIEXWebSocket.CreatingThreadConnectWebSocketAndSendMessage(GetTiingoIEXWebSocketSymbolVector());
			gl_systemMessage.PushInnerSystemInformationMessage(_T("开启Tiingo IEX web socket服务"));
		}
	}
	if (gl_systemConfigeration.IsUsingTiingoCryptoWebSocket()) {
		if (m_tiingoCryptoWebSocket.IsClosed()) {
			m_tiingoCryptoWebSocket.CreatingThreadConnectWebSocketAndSendMessage(GetTiingoCryptoWebSocketSymbolVector());
			gl_systemMessage.PushInnerSystemInformationMessage(_T("开启Tiingo Crypto web socket服务"));
		}
	}
	if (gl_systemConfigeration.IsUsingTiingoForexWebSocket()) {
		if (m_tiingoForexWebSocket.IsClosed()) {
			m_tiingoForexWebSocket.CreatingThreadConnectWebSocketAndSendMessage(GetTiingoForexWebSocketSymbolVector());
			gl_systemMessage.PushInnerSystemInformationMessage(_T("开启Tiingo Forex web socket服务"));
		}
	}
}

bool CWorldMarket::TaskProcessWebSocketData(void) {
	if (gl_WebInquirer.FinnhubWebSocketDataSize() > 0) ProcessFinnhubWebSocketData();
	if (gl_WebInquirer.TiingoIEXWebSocketDataSize() > 0) ProcessTiingoIEXWebSocketData();
	if (gl_WebInquirer.TiingoCryptoWebSocketDataSize() > 0) ProcessTiingoCryptoWebSocketData();
	if (gl_WebInquirer.TiingoForexWebSocketDataSize() > 0) ProcessTiingoForexWebSocketData();

	return true;
}

bool CWorldMarket::ProcessFinnhubWebSocketData() {
	auto total = gl_WebInquirer.FinnhubWebSocketDataSize();
	CString strMessage;
	shared_ptr<std::string> pString;
	int iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopFinnhubWebSocketData();
		strMessage = _T("Finnhub: ");
		strMessage += (*pString).c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		m_finnhubWebSocket.ParseFinnhubWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedFinnhubWebSocket(iTotalDataSize);

	return true;
}

bool CWorldMarket::ProcessTiingoIEXWebSocketData() {
	auto total = gl_WebInquirer.TiingoIEXWebSocketDataSize();
	CString strMessage;
	shared_ptr<std::string> pString;
	size_t iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopTiingoIEXWebSocketData();
		strMessage = _T("Tiingo IEX: ");
		strMessage += (*pString).c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		m_tiingoIEXWebSocket.ParseTiingoIEXWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoIEXWebSocket(iTotalDataSize);
	return true;
}

bool CWorldMarket::ProcessTiingoCryptoWebSocketData() {
	auto total = gl_WebInquirer.TiingoCryptoWebSocketDataSize();
	CString strMessage;

	shared_ptr<std::string> pString;
	int iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopTiingoCryptoWebSocketData();
		strMessage = _T("Tiingo Crypto: ");
		strMessage += (*pString).c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		m_tiingoCryptoWebSocket.ParseTiingoCryptoWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoCryptoWebSocket(iTotalDataSize);
	return true;
}

bool CWorldMarket::ProcessTiingoForexWebSocketData() {
	auto total = gl_WebInquirer.TiingoForexWebSocketDataSize();
	CString strMessage;
	shared_ptr<std::string> pString;
	int iTotalDataSize = 0;
	for (auto i = 0; i < total; i++) {
		pString = gl_WebInquirer.PopTiingoForexWebSocketData();
		strMessage = _T("Tiingo Forex: ");
		strMessage += (*pString).c_str();
		gl_systemMessage.PushWebSocketInfoMessage(strMessage);
		iTotalDataSize += pString->size();
		m_tiingoForexWebSocket.ParseTiingoForexWebSocketData(pString);
	}
	gl_systemMessage.SetProcessedTiingoForexWebSocket(iTotalDataSize);
	return true;
}

/// <summary>
/// 更新证券的实时信息
/// 目前只使用IEX和Crypto的数据，其他数据抛弃掉
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