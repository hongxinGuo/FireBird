#include"pch.h"

#include"ConvertToString.h"
#include"ThreadStatus.h"
#include"TimeConvert.h"
#include"ChinaStockCodeConverter.h"
#include"InfoReport.h"

#include"ChinaMarket.h"
#include "ContainerChinaStock.h"

#include "CharSetTransfer.h"
#include "dataBaseConnector.h"
#include"Thread.h"

CContainerChinaStock::CContainerChinaStock() {
	CContainerChinaStock::Reset();
}

CContainerChinaStock::~CContainerChinaStock() {
	//for (const auto& pStock : m_vStock) {
	//pStock->SetUpdateProfileDB(true);
	//}
	//UpdateStockProfileDB();
}

void CContainerChinaStock::Reset() {
	CContainerVirtualStock::Reset();

	m_lLoadedStock = 0;
}

size_t CContainerChinaStock::GetActiveStockSize() const {
	size_t lTotalActiveStock = 0;
	for (const auto& pStock : m_vStock) {
		if (pStock->IsActive()) lTotalActiveStock++;
	}
	return lTotalActiveStock;
}

long CContainerChinaStock::LoadProfileDB2() {
	CSetChinaStockSymbol setChinaStockSymbol;
	long lDayLineNeedCheck = 0;

	Reset();
	setChinaStockSymbol.m_strSort = "[Symbol]";
	setChinaStockSymbol.Open();
	setChinaStockSymbol.m_pDatabase->BeginTrans();
	// 装入股票代码数据库
	while (!setChinaStockSymbol.IsEOF()) {
		const auto pStock = make_shared<CChinaStock>();
		if (!IsSymbol(T2Utf8(setChinaStockSymbol.m_Symbol))) {
			pStock->LoadStockCodeDB(setChinaStockSymbol);
			pStock->CheckNeedProcessRTData();
			pStock->CheckIPOStatus();
			pStock->CheckDayLineStatus();
			Add(pStock);
		}
		else {
			setChinaStockSymbol.Delete(); // 删除此重复代码
		}
		setChinaStockSymbol.MoveNext();
	}
	if (IsUpdateDayLine()) {
		lDayLineNeedCheck = GetDayLineNeedUpdateNumber();
		if (gl_pChinaMarket->GetDayOfWeek() == 1) gl_systemMessage.PushInformationMessage("每星期一复查退市股票日线");
		auto str = std::format("{:d}个股票需要检查日线数据", lDayLineNeedCheck);
		gl_systemMessage.PushInformationMessage(str);
	}
	setChinaStockSymbol.m_pDatabase->CommitTrans();
	setChinaStockSymbol.Close();
	m_lLoadedStock = m_vStock.size();
	Sort();

	return lDayLineNeedCheck;
}

long CContainerChinaStock::LoadProfileDB() {
	long lDayLineNeedCheck = 0;
	bool bDeleteDuplicatedCode = false;
	using namespace StockMarket;
	const auto& t = ChinaStockCode{};

	Reset();
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);
	auto result = db(select(all_of(t)).from(t).unconditionally().order_by(t.Symbol.asc()));
	auto rowCount = result.size();
	Reserve(rowCount + 100); // 预留一些空间，避免后续添加新股票时频繁扩容
	for (const auto& row : result) {
		// 装入股票代码数据库
		const auto pStock = make_shared<CChinaStock>();
		string str;
		str = row.Symbol;
		if (!IsSymbol(str)) {
			pStock->SetSymbol(str);
			pStock->SetDisplaySymbol(row.DisplaySymbol);
			pStock->SetDescription(row.Description);
			pStock->SetExchangeCode(row.Exchange);
			pStock->SetIPOStatus(row.IPOStatus);
			pStock->LoadUpdateDate(row.UpdateDate);

			pStock->CheckNeedProcessRTData();
			pStock->CheckIPOStatus();
			pStock->CheckDayLineStatus();
			Add(pStock);
		}
		else bDeleteDuplicatedCode = true;
	}
	tx.commit();

	if (IsUpdateDayLine()) {
		lDayLineNeedCheck = GetDayLineNeedUpdateNumber();
		if (gl_pChinaMarket->GetDayOfWeek() == 1) gl_systemMessage.PushInformationMessage("每星期一复查退市股票日线");
		auto str = std::format("{:d}个股票需要检查日线数据", lDayLineNeedCheck);
		gl_systemMessage.PushInformationMessage(str);
	}
	m_lLoadedStock = m_vStock.size();
	Sort();

	if (bDeleteDuplicatedCode) {
		DeleteDuplicatedStockDB();
	}
	return lDayLineNeedCheck;
}

void CContainerChinaStock::UpdateProfileDB() {
	if (IsUpdateProfileDB()) {
		try {
			int iCount = 0;
			int iStockCodeNeedUpdate = 0;
			CSetChinaStockSymbol setChinaStockSymbol;
			for (const auto& pStock2 : m_vStock) {
				if (pStock2->IsUpdateProfileDB()) iStockCodeNeedUpdate++;
			}
			setChinaStockSymbol.m_strSort = "[Symbol]";
			setChinaStockSymbol.Open();
			setChinaStockSymbol.m_pDatabase->BeginTrans();
			while (iCount < iStockCodeNeedUpdate) {	//更新原有的代码集状态
				if (setChinaStockSymbol.IsEOF()) break;
				if (IsSymbol(T2Utf8(setChinaStockSymbol.m_Symbol))) {
					const CChinaStockPtr pStock = GetStock(T2Utf8(setChinaStockSymbol.m_Symbol));
					if (pStock->IsUpdateProfileDB()) {
						pStock->SetUpdateProfileDB(false);
						//ASSERT(!pStock3->IsTodayNewStock());
						iCount++;
						pStock->UpdateSymbol(setChinaStockSymbol);
					}
				}
				else {
					setChinaStockSymbol.Delete(); // 删除未使用的代码
				}
				setChinaStockSymbol.MoveNext();
			}
			if (iCount < iStockCodeNeedUpdate) { // 添加新股票代码
				for (const auto& pStock3 : m_vStock) {
					if (pStock3->IsUpdateProfileDB()) {
						pStock3->SetUpdateProfileDB(false);
						ASSERT(pStock3->IsTodayNewStock());
						iCount++;
						pStock3->AppendSymbol(setChinaStockSymbol);
						pStock3->SetTodayNewStock(false);
					}
					if (iCount >= iStockCodeNeedUpdate) break;
				}
			}
			setChinaStockSymbol.m_pDatabase->CommitTrans();
			setChinaStockSymbol.Close();
			m_lLoadedStock = m_vStock.size();
			ASSERT(iCount == iStockCodeNeedUpdate);
		} catch (CException& e) {
			ReportInformation(e);
		}
	}
}

void CContainerChinaStock::DeleteDuplicatedStockDB() {
	auto db = gl_dbStockMarket.get();
	// Use execute(string) to run raw SQL text (operator() requires a sqlpp statement)
	db.execute("DELETE t1 FROM china_stock_code t1 INNER JOIN china_stock_code t2 ON t1.Symbol = t2.Symbol AND t1.ID > t2.ID");
	db.execute("COMMIT");
}

bool CContainerChinaStock::IsDayLineDBUpdated() noexcept {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return dynamic_pointer_cast<CChinaStock>(pStock)->IsDayLineDBUpdated(); });
}

void CContainerChinaStock::ClearDayLineDBUpdatedFlag() noexcept {
	for (long l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		pStock->SetDayLineDBUpdated(false);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 生成网易日线股票代码的字符串，用于查询此股票在当前市场是否处于活跃状态（或者是否存在此股票号码）
//
//  此函数是检查m_vStock股票池
//
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
string CContainerChinaStock::CreateNeteaseDayLineInquiringStr() {
	bool fFoundStock = false;
	string strTemp;
	string strReturn;
	size_t lIndex = 0;

	while (!fFoundStock && (lIndex < Size())) {
		const CChinaStockPtr pStock = GetStock(lIndex);
		if (!pStock->IsUpdateDayLine()) { // 日线数据不需要更新。在系统初始时，设置此m_fUpdateDayLine标识
			lIndex++;
		}
		else if (pStock->GetDayLineEndDate() >= gl_pChinaMarket->GetLastTradeDate()) {//上一交易日的日线数据已经存储？此时已经处理过一次日线数据了，无需再次处理。
			pStock->SetUpdateDayLine(false); // 此股票日线资料不需要更新了。
			lIndex++;
		}
		else {
			fFoundStock = true;
		}
	}

	if (lIndex >= Size()) {	//  没有找到需要申请日线的证券
		TRACE(_T("未找到需更新日线历史数据的股票\n"));
		return "";
	}

	// 找到了需申请日线历史数据的股票
	const CChinaStockPtr pStock = GetStock(lIndex);
	ASSERT(!pStock->IsUpdateDayLineDB());
	ASSERT(pStock->IsUpdateDayLine());
	pStock->SetUpdateDayLine(false);
	strReturn += XferStandardToNetease(pStock->GetSymbol());
	return strReturn;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 生成腾讯日线股票代码的字符串，用于查询此股票在当前市场是否处于活跃状态（或者是否存在此股票号码）
//
//  此函数是检查m_vStock股票池
//
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
string CContainerChinaStock::CreateTengxunDayLineInquiringStr() {
	string strTemp;
	string strReturn;
	size_t lIndex = 0;

	while (lIndex < Size()) {
		const CChinaStockPtr pStock = GetStock(lIndex);
		if (pStock->IsUpdateDayLine()) { // 日线数据不需要更新。在系统初始时，设置此m_fUpdateDayLine标识
			break;
		}
		if (pStock->GetDayLineEndDate() >= gl_pChinaMarket->GetLastTradeDate()) {//上一交易日的日线数据已经存储？此时已经处理过一次日线数据了，无需再次处理。
			break;
		}
		lIndex++;
	}

	if (lIndex >= Size()) {	//  没有找到需要申请日线的证券
		TRACE(_T("未找到需更新日线历史数据的股票\n"));
		return "";
	}

	// 找到了需申请日线历史数据的股票
	const CChinaStockPtr pStock = GetStock(lIndex);
	ASSERT(!pStock->IsUpdateDayLineDB());
	ASSERT(pStock->IsUpdateDayLine());
	pStock->SetUpdateDayLine(false);
	strReturn += XferStandardToTengxun(pStock->GetSymbol());
	return strReturn;
}

void CContainerChinaStock::ClearDayLineNeedUpdateStatus() const {
	for (const auto& pStock : m_vStock) {
		pStock->SetUpdateDayLine(false);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 得到股票的简称
//
//////////////////////////////////////////////////////////////////////////////////////////////
string CContainerChinaStock::GetStockName(const string& strStockCode) {
	try {
		return GetStock(m_mapSymbol.at(strStockCode))->GetDisplaySymbol();
	} catch (exception& e) {
		ReportErrorToSystemMessage("GetStockName " + strStockCode + " ", e);
		return "";
	}
}

void CContainerChinaStock::ProcessRTData() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (pStock->IsActive()) {
			pStock->ProcessRTData();
		}
	}
}

void CContainerChinaStock::UnloadDayLine() noexcept {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		pStock->UnloadDayLine();
	}
}

void CContainerChinaStock::SetDayLineNeedMaintain() const {
	SetUpdateDayLine();
	for (auto& pStock : m_vStock) {
		pStock->SetDayLineEndDate(CHINA_MARKET_BEGIN_DATE_);
	}
}

void CContainerChinaStock::SetUpdateDayLine() const {
	for (const auto& pStock : m_vStock) {
		pStock->SetUpdateDayLine(true);
	}
}

long CContainerChinaStock::GetDayLineNeedSaveNumber() const {
	long l = 0;
	for (const auto& pStock : m_vStock) {
		if (pStock->IsUpdateDayLineDB()) ++l;
	}
	return l;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将日线数据存入数据库．
//
// 无论是否执行了存储函数，都需要将下载的日线历史数据删除，这样能够节省内存的占用。由于实际存储功能使用线程模式实现，
// 故而其执行时间可能晚于主线程，导致主线程删除日线数据时出现同步问题。解决的方法是让工作线程独立删除存储后的日线数据，
// 主线程的删除函数只在不调用工作线程（无需存储日线数据）的情况下方才执行。
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CContainerChinaStock::TaskUpdateDayLineDB() {
	bool fSave = false;

	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (pStock->IsUpdateDayLineDB()) {
			pStock->SetUpdateDayLineDB(false);
			if (pStock->GetDayLineSize() > 0) {
				if (pStock->HaveNewDayLineData()) {
					gl_systemMessage.SetChinaMarketSavingFunction("update dayline");
					gl_runtime.thread_executor()->post([pStock] {
						gl_UpdateChinaMarketDB.acquire();
						if (!gl_systemConfiguration.IsExitingSystem()) {
							const bool fDataSaved = pStock->SaveDayLineBasicInfo();
							pStock->UpdateDayLineStartEndDate();
							if (fDataSaved) {
								string str = pStock->GetSymbol();
								str += "日线资料存储完成";
								gl_systemMessage.PushDayLineInfoMessage(str);
							}
							pStock->UnloadDayLine();// 为防止出现同步问题，卸载日线历史数据的任务也由本线程执行。
						}
						gl_UpdateChinaMarketDB.release();
					});
					fSave = true;
				}
				else pStock->UnloadDayLine(); // 当无需执行存储函数时，这里还要单独卸载日线数据。因存储日线数据线程稍后才执行，故而不能在此统一执行删除函数。
			}
			else {
				// 此种情况为有股票代码，但此代码尚未上市
				pStock->SetIPOStatus(_STOCK_NOT_YET_LIST_);
				pStock->SetUpdateProfileDB(true);
				string str1 = pStock->GetSymbol();
				str1 += " 为未上市股票代码";
				gl_systemMessage.PushDayLineInfoMessage(str1);
			}
		}
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
	}

	return fSave;
}

bool CContainerChinaStock::BuildWeekLine(long lStartDate) {
	gl_systemMessage.PushInformationMessage("重新生成周线历史数据");
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		//gl_BackgroundWorkingThread.acquire();
		//gl_runtime.thread_executor()->post([pStock, lStartDate] {
		gl_systemMessage.SetChinaMarketSavingFunction("rebuild week line");

		if (!gl_systemConfiguration.IsExitingSystem()) {
			pStock->BuildWeekLine(lStartDate);
		}
		//gl_BackgroundWorkingThread.release();
		//});
	}
	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) { Sleep(100); }
	gl_systemMessage.PushInformationMessage("周线历史数据生成完毕");

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
//
// 处理当前交易日的实时数据，生成日线各基本数据（相对强度、进攻性买卖盘）。
//
// 只有下载完日线历史数据后，方可执行处理实时数据，否则可能误判股票代码存在与否。
//
// long lCurrentTradeDay 当前交易日。由于存在周六和周日，故而此日期并不一定就是当前日期，而可能时周五
//
//////////////////////////////////////////////////////////////////////////////////
long CContainerChinaStock::BuildDayLine(long lCurrentTradeDay) {
	long iCount = 0;
	CSetChinaMarketDayLineInfo setDayLineBasicInfo;

	string s = "开始处理" + ConvertDateToChineseTimeStampString(lCurrentTradeDay) + "的实时数据";
	gl_systemMessage.PushInformationMessage(s);

	DeleteDayLineBasicInfo(lCurrentTradeDay);

	// 存储当前交易日的数据
	setDayLineBasicInfo.m_strFilter = "[ID] = 1";
	setDayLineBasicInfo.Open();
	setDayLineBasicInfo.m_pDatabase->BeginTrans();
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (!pStock->IsTodayDataActive()) {	// 此股票今天停牌,所有的数据皆为零,不需要存储.
			continue;
		}
		iCount++;
		pStock->SetDayLineEndDate(lCurrentTradeDay);
		pStock->SetIPOStatus(_STOCK_IPOED_); // 再设置一次。防止新股股票代码由于没有历史数据而被误判为不存在。
		pStock->SetUpdateProfileDB(true);
		pStock->AppendTodayBasicInfo(&setDayLineBasicInfo);
	}
	setDayLineBasicInfo.m_pDatabase->CommitTrans();
	setDayLineBasicInfo.Close();

	s = ConvertDateToChineseTimeStampString(lCurrentTradeDay) + "的日线数据已生成";
	gl_systemMessage.PushInformationMessage(s);

	s = fmt::format("今日处理了{:d}个股票", iCount);
	gl_systemMessage.PushInformationMessage(s);

	return iCount;
}

void CContainerChinaStock::DeleteDayLineBasicInfo(long lDate) {
	CSetChinaMarketDayLineInfo setDayLineBasicInfo;

	setDayLineBasicInfo.m_strFilter = fmt::format("[Date] = {:8Ld}", lDate).c_str();
	setDayLineBasicInfo.Open();
	setDayLineBasicInfo.m_pDatabase->BeginTrans();
	while (!setDayLineBasicInfo.IsEOF()) {
		setDayLineBasicInfo.Delete();
		setDayLineBasicInfo.MoveNext();
	}
	setDayLineBasicInfo.m_pDatabase->CommitTrans();
	setDayLineBasicInfo.Close();
}

double CContainerChinaStock::GetUpDownRate(const string& strClose, const string& strLastClose) noexcept {
	char* p;
	const double lastClose = strtod(strLastClose.c_str(), &p);
	if (lastClose < 0.001) return 0;
	double result = (strtod(strClose.c_str(), &p) - lastClose) / lastClose;
	if ((result > 0.11) || (result < -0.11)) result = 0;
	return result;
}
