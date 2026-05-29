#include"pch.h"

#include"ThreadStatus.h"
#include"TimeConvert.h"
#include"ChinaStockCodeConverter.h"
#include"InfoReport.h"

#include"ChinaMarket.h"
#include "ContainerChinaStock.h"

#include "dataBaseConnector.h"
#include"Thread.h"

CContainerChinaStock::CContainerChinaStock() {
	CContainerChinaStock::Reset();
}

CContainerChinaStock::~CContainerChinaStock() {
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

long CContainerChinaStock::LoadProfileDB() {
	long lDayLineNeedCheck = 0;
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
		else {
			db(sqlpp::remove_from(t).where(t.ID == row.ID));
		}
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

	return lDayLineNeedCheck;
}

void CContainerChinaStock::UpdateProfileDB() {
	if (IsUpdateProfileDB()) {
		try {
			using namespace StockMarket;
			const auto& t = ChinaStockCode{};
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);

			//auto multi_insert = insert_into(t).columns(t.Symbol, t.Description, t.Exchange,t.DisplaySymbol, t.IPOStatus, t.UpdateDate);

			for (size_t i = 0; i < m_vStock.size(); ++i) {
				const auto& pStock = m_vStock[i];
				if (pStock->IsUpdateProfileDB()) {
					pStock->UpdateJsonUpdateDate();
					if (pStock->IsNewStock()) {	// 插入新股票代码
						db(sqlpp::insert_into(t).set(
							t.Symbol = pStock->GetSymbol(),
							t.Description = pStock->GetDescription(),
							t.Exchange = pStock->GetExchangeCode(),
							t.DisplaySymbol = pStock->GetDisplaySymbol(),
							t.IPOStatus = pStock->GetIPOStatus(),
							t.UpdateDate = pStock->GetJsonUpdateDate().dump()
						));
						pStock->SetTodayNewStock(false);
					}
					else {// 更新现有股票代码
						db(sqlpp::update(t).set(
							t.Symbol = pStock->GetSymbol(),
							t.Description = pStock->GetDescription(),
							t.Exchange = pStock->GetExchangeCode(),
							t.DisplaySymbol = pStock->GetDisplaySymbol(),
							t.IPOStatus = pStock->GetIPOStatus(),
							t.UpdateDate = pStock->GetJsonUpdateDate().dump()
						).where(t.Symbol == pStock->GetSymbol()));
					}
					pStock->SetUpdateProfileDB(false);
				}
			}
			tx.commit();
			m_lLoadedStock = m_vStock.size();
		} catch (CException& e) {
			ReportInformation(e);
		}
	}
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
						pStock->UpdateDayLineDB();
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
///
/// 处理当前交易日的实时数据，生成日线各基本数据（相对强度、进攻性买卖盘）。
///
/// 只有下载完日线历史数据后，方可执行处理实时数据，否则可能误判股票代码存在与否。
///
/// long lCurrentTradeDay 当前交易日。由于存在周六和周日，故而此日期并不一定就是当前日期，而可能时周五
///
/// Note: 需要使用大宗插入模式，否则出错
//////////////////////////////////////////////////////////////////////////////////
long CContainerChinaStock::BuildDayLine(long lCurrentTradeDay) {
	long iCount = 0;
	int ratio = 1000; // 由于日线历史数据是以整数形式存储的，故而这里要除以一个比例因子才能得到正确的价格数据。
	string s = "开始处理" + ConvertDateToChineseTimeStampString(lCurrentTradeDay) + "的实时数据";
	gl_systemMessage.PushInformationMessage(s);

	DeleteDayLine(lCurrentTradeDay);

	// 存储当前交易日的数据
	using namespace StockMarket;
	const auto& t = ChinaStockDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto multi_insert = insert_into(t).columns(t.Date, t.Exchange, t.Symbol,
	                                           t.LastClose, t.Open, t.High, t.Low, t.Close,
	                                           t.Volume, t.Amount, t.UpAndDown, t.UpDownRate, t.ChangeHandRate, t.CurrentValue, t.TotalValue);
	size_t lSize = m_vStock.size();
	int nValue = 0;
	for (size_t l = 0; l < lSize; l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (pStock->IsTodayDataActive()) {	// 此股票今天停牌,所有的数据皆为零,不需要存储.
			iCount++;
			TRACE("sss \n");
			pStock->SetDayLineEndDate(lCurrentTradeDay);
			pStock->SetIPOStatus(_STOCK_IPOED_); // 再设置一次。防止新股股票代码由于没有历史数据而被误判为不存在。
			pStock->SetUpdateProfileDB(true);

			multi_insert.values.add(
				t.Date = lCurrentTradeDay,
				t.Exchange = pStock->GetExchangeCode(),
				t.Symbol = pStock->GetSymbol(),
				t.LastClose = static_cast<double>(pStock->GetLastClose()) / ratio,
				t.Open = static_cast<double>(pStock->GetOpen()) / ratio,
				t.High = static_cast<double>(pStock->GetHigh()) / ratio,
				t.Low = static_cast<double>(pStock->GetLow()) / ratio,
				t.Close = static_cast<double>(pStock->GetNew()) / ratio,
				t.Volume = static_cast<double>(pStock->GetVolume()),
				t.Amount = static_cast<double>(pStock->GetAmount()),
				t.UpAndDown = static_cast<double>(pStock->GetUpDown()) / ratio,
				t.UpDownRate = pStock->GetUpDownRate() / ratio,
				t.ChangeHandRate = 0.0,
				t.CurrentValue = static_cast<double>(pStock->GetCurrentValue()),
				t.TotalValue = static_cast<double>(pStock->GetTotalValue())
			);
			nValue++;
		}
	}
	if (nValue > 0) db(multi_insert);
	tx.commit();

	s = ConvertDateToChineseTimeStampString(lCurrentTradeDay) + "的日线数据已生成";
	gl_systemMessage.PushInformationMessage(s);

	s = fmt::format("今日处理了{:d}个股票", iCount);
	gl_systemMessage.PushInformationMessage(s);

	return iCount;
}

void CContainerChinaStock::DeleteDayLine(long lDate) {
	using namespace StockMarket;
	const auto& t = ChinaStockDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	db(sqlpp::remove_from(t).where(t.Date == lDate));
	tx.commit();
}

double CContainerChinaStock::GetUpDownRate(const string& strClose, const string& strLastClose) noexcept {
	char* p;
	const double lastClose = strtod(strLastClose.c_str(), &p);
	if (lastClose < 0.001) return 0;
	double result = (strtod(strClose.c_str(), &p) - lastClose) / lastClose;
	if ((result > 0.11) || (result < -0.11)) result = 0;
	return result;
}
