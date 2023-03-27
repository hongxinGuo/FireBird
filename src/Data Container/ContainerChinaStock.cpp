#include"pch.h"

#include"ConvertToString.h"
#include"ThreadStatus.h"
#include"TimeConvert.h"
#include"ChinaStockCodeConverter.h"
#include"InfoReport.h"

#include"ChinaMarket.h"
#include "ContainerChinaStock.h"
#include"RSReference.h"
#include"Thread.h"

#include"SetRSStrongStock.h"
#include"SetRSStrong1Stock.h"
#include"SetRSStrong2Stock.h"

using namespace std;

CContainerChinaStock::CContainerChinaStock() {
	CContainerChinaStock::Reset();
}

void CContainerChinaStock::Reset() {
	CContainerVirtualStock::Reset();

	m_lLoadedStock = 0;

	m_lNeteaseRTDataInquiringIndex = 0;
	m_lSinaRTDataInquiringIndex = 0;
	m_lTengxunRTDataInquiringIndex = 0;
}

long CContainerChinaStock::GetActiveStockSize() const {
	long lTotalActiveStock = 0;
	for (const auto& pStock : m_vStock) {
		if (pStock->IsActive()) lTotalActiveStock++;
	}
	return lTotalActiveStock;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//		判断strStockCode是否为沪深A股主板的股票代码。
//		沪市A股代码以600或601开头，深市A股代码以000或001开头。
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CContainerChinaStock::IsAStock(const CString& strStockCode) {
	const CString strSymbol = GetStockSymbol(strStockCode);
	if (IsShanghaiExchange(strStockCode)) {
		if ((strSymbol[0] == '6') && (strSymbol[1] == '0')) {
			if ((strSymbol[2] == '0') || (strSymbol[2] == '1')) {
				return true;
			}
		}
	}
	else if (IsShenzhenExchange(strStockCode)) {
		if ((strSymbol[0] == '0') && (strSymbol[1] == '0')) {
			if ((strSymbol[2] == '0') || (strSymbol[2] == '2')) {
				return true;
			}
		}
	}
	return (false);
}

long CContainerChinaStock::LoadStockProfileDB() {
	CSetChinaStockSymbol setChinaStockSymbol;
	char buffer[30]{0, 0, 0};
	CString str;
	long lDayLineNeedCheck = 0;

	setChinaStockSymbol.m_strSort = _T("[Symbol]");
	setChinaStockSymbol.Open();
	setChinaStockSymbol.m_pDatabase->BeginTrans();
	// 装入股票代码数据库
	while (!setChinaStockSymbol.IsEOF()) {
		const auto pStock = make_shared<CChinaStock>();
		if (!IsSymbol(setChinaStockSymbol.m_Symbol)) {
			pStock->LoadStockCodeDB(setChinaStockSymbol);
			Add(pStock);
		}
		else {
			str = _T("发现重复代码：");
			str += setChinaStockSymbol.m_Symbol;
			gl_systemMessage.PushInnerSystemInformationMessage(str);
			setChinaStockSymbol.Delete(); // 删除此重复代码
		}
		setChinaStockSymbol.MoveNext();
	}
	if (IsDayLineNeedUpdate()) {
		lDayLineNeedCheck = GetDayLineNeedUpdateNumber();
		if (gl_pChinaMarket->GetDayOfWeek() == 1) gl_systemMessage.PushInformationMessage(_T("每星期一复查退市股票日线"));
		TRACE("尚余%d个股票需要检查日线数据\n", lDayLineNeedCheck);
		_itoa_s(lDayLineNeedCheck, buffer, 10);
		str = buffer;
		str += _T("个股票需要检查日线数据");
		gl_systemMessage.PushInformationMessage(str);
	}
	setChinaStockSymbol.m_pDatabase->CommitTrans();
	setChinaStockSymbol.Close();
	m_lLoadedStock = m_vStock.size();
	Sort();

	return lDayLineNeedCheck;
}

bool CContainerChinaStock::UpdateStockProfileDB() {
	int iStockCodeNeedUpdate = 0;
	int iCount = 0;

	//更新原有的代码集状态
	if (IsUpdateProfileDB()) {
		CSetChinaStockSymbol setChinaStockSymbol;
		for (const auto& pStock2 : m_vStock) {
			if (pStock2->IsUpdateProfileDB()) iStockCodeNeedUpdate++;
		}
		setChinaStockSymbol.m_strSort = _T("[Symbol]");
		setChinaStockSymbol.Open();
		setChinaStockSymbol.m_pDatabase->BeginTrans();
		while (iCount < iStockCodeNeedUpdate) {
			if (setChinaStockSymbol.IsEOF()) break;
			const CChinaStockPtr pStock = GetStock(setChinaStockSymbol.m_Symbol);
			if (pStock->IsUpdateProfileDBAndClearFlag()) {
				//ASSERT(!pStock3->IsTodayNewStock());
				iCount++;
				pStock->UpdateSymbol(setChinaStockSymbol);
			}
			setChinaStockSymbol.MoveNext();
		}
		if (iCount < iStockCodeNeedUpdate) {
			for (const auto& pStock3 : m_vStock) {
				if (pStock3->IsUpdateProfileDBAndClearFlag()) {
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
	}
	ASSERT(iCount == iStockCodeNeedUpdate);
	return true;
}

bool CContainerChinaStock::IsDayLineDBUpdated() noexcept {
	return ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return dynamic_pointer_cast<CChinaStock>(pStock)->IsDayLineDBUpdated(); });
}

void CContainerChinaStock::ClearDayLineDBUpdatedFlag() noexcept {
	for (long l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		pStock->SetDayLineDBUpdated(false);
	}
}

INT64 CContainerChinaStock::GetTotalAttackBuyAmount() {
	INT64 lAmount = 0;
	for (long l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (pStock->IsActive() && IsAStock(pStock)) {
			lAmount += pStock->GetAttackBuyAmount();
		}
	}
	return (lAmount);
}

INT64 CContainerChinaStock::GetTotalAttackSellAmount() {
	INT64 lAmount = 0;
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (pStock->IsActive() && IsAStock(pStock)) {
			lAmount += pStock->GetAttackSellAmount();
		}
	}
	return (lAmount);
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
CString CContainerChinaStock::CreateNeteaseDayLineInquiringStr() {
	bool fFound = false;
	int iCount = 0;
	CString strTemp;
	CString strReturn = _T("");
	long lIndex = 0;

	while (!fFound && (iCount++ < Size())) {
		const CChinaStockPtr pStock = GetStock(lIndex);
		if (!pStock->IsDayLineNeedUpdate()) { // 日线数据不需要更新。在系统初始时，设置此m_fDayLineNeedUpdate标识
			// TRACE("%S 日线数据无需更新\n", static_cast<LPCWSTR>(pStock->m_strSymbol));
			lIndex = GetNextIndex(lIndex);
		}
		else if (pStock->GetDayLineEndDate() >= gl_pChinaMarket->GetLastTradeDate()) {//上一交易日的日线数据已经存储？此时已经处理过一次日线数据了，无需再次处理。
			pStock->SetDayLineNeedUpdate(false); // 此股票日线资料不需要更新了。
			// TRACE("%S 日线数据本日已更新\n", static_cast<LPCWSTR>(pStock->m_strSymbol));
			lIndex = GetNextIndex(lIndex);
		}
		else { fFound = true; }
	}

	if (iCount >= Size()) {
		//  没有找到需要申请日线的证券
		TRACE("未找到需更新日线历史数据的股票\n");
		return _T("");
	}

	// 找到了需申请日线历史数据的股票
	const CChinaStockPtr pStock = GetStock(lIndex);
	ASSERT(!pStock->IsDayLineNeedSaving());
	ASSERT(pStock->IsDayLineNeedUpdate());
	pStock->SetDayLineNeedUpdate(false);
	strReturn += XferStandardToNetease(pStock->GetSymbol());
	return strReturn;
}

CString CContainerChinaStock::GetNextStockInquiringMiddleStr(long& iStockIndex, CString strPostfix, long lTotalNumber) {
	if (m_vStock.empty()) return _T("sh600000"); // 当没有证券可查询时，返回一个有效字符串
	CString strReturn = XferStandardToSina(GetStock(iStockIndex)->GetSymbol()); // 得到第一个股票代码
	iStockIndex = GetNextIndex(iStockIndex);
	int iCount = 1; // 从1开始计数，因为第一个数据前不需要添加postfix。
	while (iCount < lTotalNumber) {
		// 每次最大查询量为lTotalNumber个股票
		if (GetStock(iStockIndex)->IsActive() || GetStock(iStockIndex)->IsIPOed()) {
			iCount++;
			strReturn += strPostfix;
			strReturn += XferStandardToSina(GetStock(iStockIndex)->GetSymbol()); // 得到第一个股票代码
		}
		iStockIndex = GetNextIndex(iStockIndex);
	}

	return strReturn;
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
CString CContainerChinaStock::CreateTengxunDayLineInquiringStr() {
	bool fFound = false;
	int iCount = 0;
	CString strTemp;
	CString strReturn = _T("");
	long lIndex = 0;

	while (!fFound && (iCount++ < Size())) {
		const CChinaStockPtr pStock = GetStock(lIndex);
		if (!pStock->IsDayLineNeedUpdate()) { // 日线数据不需要更新。在系统初始时，设置此m_fDayLineNeedUpdate标识
			// TRACE("%S 日线数据无需更新\n", static_cast<LPCWSTR>(pStock->m_strSymbol));
			lIndex = GetNextIndex(lIndex);
		}
		else if (pStock->GetDayLineEndDate() >= gl_pChinaMarket->GetLastTradeDate()) {//上一交易日的日线数据已经存储？此时已经处理过一次日线数据了，无需再次处理。
			pStock->SetDayLineNeedUpdate(false); // 此股票日线资料不需要更新了。
			// TRACE("%S 日线数据本日已更新\n", static_cast<LPCWSTR>(pStock->m_strSymbol));
			lIndex = GetNextIndex(lIndex);
		}
		else { fFound = true; }
	}

	if (iCount >= Size()) {
		//  没有找到需要申请日线的证券
		TRACE("未找到需更新日线历史数据的股票\n");
		return _T("");
	}

	// 找到了需申请日线历史数据的股票
	const CChinaStockPtr pStock = GetStock(lIndex);
	ASSERT(!pStock->IsDayLineNeedSaving());
	ASSERT(pStock->IsDayLineNeedUpdate());
	pStock->SetDayLineNeedUpdate(false);
	strReturn += XferStandardToTengxun(pStock->GetSymbol());
	return strReturn;
}

CString CContainerChinaStock::GetNextNeteaseStockInquiringMiddleStr(const long lTotalNumber) {
	CString strNeteaseRTDataInquiringStr = XferStandardToNetease(GetStock(m_lNeteaseRTDataInquiringIndex)->GetSymbol());
	m_lNeteaseRTDataInquiringIndex = GetNextIndex(m_lNeteaseRTDataInquiringIndex);
	int iCount = 1; // 从1开始计数，因为第一个数据前不需要添加postfix。
	while (iCount < lTotalNumber) {	// 每次最大查询量为lTotalNumber个股票
		if (GetStock(m_lNeteaseRTDataInquiringIndex)->IsActive() || GetStock(m_lNeteaseRTDataInquiringIndex)->IsIPOed()) {//if (GetStock(m_lNeteaseRTDataInquiringIndex)->IsActive()) {
			iCount++;
			strNeteaseRTDataInquiringStr += _T(",");
			strNeteaseRTDataInquiringStr += XferStandardToNetease(GetStock(m_lNeteaseRTDataInquiringIndex)->GetSymbol());
		}
		m_lNeteaseRTDataInquiringIndex = GetNextIndex(m_lNeteaseRTDataInquiringIndex);
	}

	return strNeteaseRTDataInquiringStr;
}

long CContainerChinaStock::GetNextIndex(long lIndex) const {
	if (++lIndex >= m_vStock.size()) { lIndex = 0; }
	return lIndex;
}

bool CContainerChinaStock::ProcessRTData() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (pStock->IsActive()) {
			pStock->ProcessRTData();
		}
	}
	return true;
}

void CContainerChinaStock::ClearDayLineNeedUpdateStatus() const {
	for (const auto& pStock : m_vStock) {
		pStock->SetDayLineNeedUpdate(false);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 得到股票的简称
//
//////////////////////////////////////////////////////////////////////////////////////////////
CString CContainerChinaStock::GetStockName(const CString& strStockCode) {
	try { return (GetStock(m_mapSymbol.at(strStockCode))->GetDisplaySymbol()); }
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("GetStockName ") + strStockCode + _T(" "), e);
		return _T("");
	}
}

void CContainerChinaStock::UnloadDayLine() noexcept {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		pStock->UnloadDayLine();
	}
}

void CContainerChinaStock::SetDayLineNeedMaintain() const {
	SetDayLineNeedUpdate();
	for (auto& pStock : m_vStock) {
		pStock->SetDayLineEndDate(19900101);
	}
}

void CContainerChinaStock::SetDayLineNeedUpdate() const {
	for (const auto& pStock : m_vStock) {
		pStock->SetDayLineNeedUpdate(true);
	}
}

long CContainerChinaStock::GetDayLineNeedUpdateNumber() const {
	long l = 0;
	for (const auto& pStock : m_vStock) {
		if (pStock->IsDayLineNeedUpdate()) l++;
	}
	return l;
}

long CContainerChinaStock::GetDayLineNeedSaveNumber() const {
	long l = 0;
	for (const auto& pStock : m_vStock) {
		if (pStock->IsDayLineNeedSaving()) ++l;
	}
	return l;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将日线数据存入数据库．
//  此函数由工作线程ThreadDayLineSaveProc调用，尽量不要使用全局变量。(目前使用主线程调用之，以消除同步问题的出现）
//
// 无论是否执行了存储函数，都需要将下载的日线历史数据删除，这样能够节省内存的占用。由于实际存储功能使用线程模式实现，
// 故而其执行时间可能晚于主线程，导致主线程删除日线数据时出现同步问题。解决的方法是让工作线程独立删除存储后的日线数据，
// 主线程的删除函数只在不调用工作线程（无需存储日线数据）的情况下方才执行。
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CContainerChinaStock::SaveDayLineData() {
	CString str;
	bool fSave = false;

	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (pStock->IsDayLineNeedSavingAndClearFlag()) {
			// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
			if (pStock->GetDayLineSize() > 0) {
				if (pStock->HaveNewDayLineData()) {
					CreateThreadSaveDayLineBasicInfo(pStock);
					fSave = true;
				}
				else pStock->UnloadDayLine(); // 当无需执行存储函数时，这里还要单独卸载日线数据。因存储日线数据线程稍后才执行，故而不能在此统一执行删除函数。
			}
			else {
				// 此种情况为有股票代码，但此代码尚未上市
				pStock->SetIPOStatus(_STOCK_NOT_YET_LIST_);
				CString str1 = pStock->GetSymbol();
				str1 += _T(" 为未上市股票代码");
				gl_systemMessage.PushDayLineInfoMessage(str1);
			}
		}
		if (gl_systemStatus.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
	}

	return fSave;
}

void CContainerChinaStock::CreateThreadSaveDayLineBasicInfo(CChinaStockPtr pStock) {
	thread thread1(ThreadSaveDayLineBasicInfo, pStock.get());
	thread1.detach(); // 必须分离之，以实现并行操作，并保证由系统回收资源。
}

bool CContainerChinaStock::BuildWeekLine(long lStartDate) {
	gl_systemMessage.PushInformationMessage(_T("重新生成周线历史数据"));
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		thread thread1(ThreadBuildWeekLineOfStock, pStock.get(), lStartDate);
		thread1.detach();
	}
	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) { Sleep(100); }
	gl_systemMessage.PushInformationMessage(_T("周线历史数据生成完毕"));

	return true;
}

bool CContainerChinaStock::Choice10RSStrong2StockSet() {
	vector<CChinaStockPtr> v10RSStrongStock;

	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		thread thread1(ThreadCalculate10RSStrong2Stock, &v10RSStrongStock, pStock);
		thread1.detach();
	}
	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) {
		if (gl_systemStatus.IsExitingSystem()) return false;
		Sleep(100); // 等待工作线程完成所有任务
	}

	CSetRSStrong2Stock setRSStrong2;

	setRSStrong2.Open();
	setRSStrong2.m_pDatabase->BeginTrans();
	while (!setRSStrong2.IsEOF()) {
		setRSStrong2.Delete();
		setRSStrong2.MoveNext();
	}
	setRSStrong2.m_pDatabase->CommitTrans();
	setRSStrong2.m_pDatabase->BeginTrans();
	for (const auto& pStock : v10RSStrongStock) {
		setRSStrong2.AddNew();
		setRSStrong2.m_Symbol = pStock->GetSymbol();
		setRSStrong2.Update();
	}
	setRSStrong2.m_pDatabase->CommitTrans();
	setRSStrong2.Close();

	return true;
}

bool CContainerChinaStock::Choice10RSStrong1StockSet() {
	vector<CChinaStockPtr> v10RSStrongStock;

	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		thread thread1(ThreadCalculate10RSStrong1Stock, &v10RSStrongStock, pStock);
		thread1.detach();
	}
	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) {
		if (gl_systemStatus.IsExitingSystem()) return false;
		Sleep(100); // 等待工作线程完成所有任务
	}

	CSetRSStrong1Stock setRSStrong1;

	setRSStrong1.Open();
	setRSStrong1.m_pDatabase->BeginTrans();
	while (!setRSStrong1.IsEOF()) {
		setRSStrong1.Delete();
		setRSStrong1.MoveNext();
	}
	setRSStrong1.m_pDatabase->CommitTrans();
	setRSStrong1.m_pDatabase->BeginTrans();
	for (const auto& pStock : v10RSStrongStock) {
		setRSStrong1.AddNew();
		setRSStrong1.m_Symbol = pStock->GetSymbol();
		setRSStrong1.Update();
	}
	setRSStrong1.m_pDatabase->CommitTrans();
	setRSStrong1.Close();

	return true;
}

bool CContainerChinaStock::Choice10RSStrongStockSet(CRSReference* pRef, int iIndex) {
	vector<CChinaStockPtr> v10RSStrongStock;

	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		thread thread1(ThreadCalculate10RSStrongStock, &v10RSStrongStock, pRef, pStock);
		thread1.detach();
	}

	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) {
		if (gl_systemStatus.IsExitingSystem()) return false;
		Sleep(100); // 等待工作线程完成所有任务
	}

	gl_pChinaMarket->SetCurrentRSStrongIndex(iIndex); // CSetRSStrongStock需要此m_lCurrentRSStrongIndex来选择正确的数据表。
	CSetRSStrongStock setRSStrong(iIndex);

	setRSStrong.Open();
	setRSStrong.m_pDatabase->BeginTrans();
	while (!setRSStrong.IsEOF()) {
		setRSStrong.Delete();
		setRSStrong.MoveNext();
	}
	setRSStrong.m_pDatabase->CommitTrans();
	setRSStrong.m_pDatabase->BeginTrans();
	for (const auto& pStock : v10RSStrongStock) {
		setRSStrong.AddNew();
		setRSStrong.m_Symbol = pStock->GetSymbol();
		setRSStrong.Update();
	}
	setRSStrong.m_pDatabase->CommitTrans();
	setRSStrong.Close();

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
	char buffer[20]{0};
	CSetDayLineBasicInfo setDayLineBasicInfo;
	CSetDayLineExtendInfo setDayLineExtendInfo;
	long iCount = 0;

	CString str = "开始处理";
	str += ConvertDateToChineseTimeStampString(lCurrentTradeDay);
	str += _T("的实时数据");
	gl_systemMessage.PushInformationMessage(str);

	DeleteDayLineBasicInfo(lCurrentTradeDay);
	DeleteDayLineExtendInfo(lCurrentTradeDay);

	// 存储当前交易日的数据
	_ltoa_s(lCurrentTradeDay, buffer, 10);
	CString strDate = buffer;
	setDayLineBasicInfo.m_strFilter = _T("[ID] = 1");
	setDayLineBasicInfo.Open();
	setDayLineBasicInfo.m_pDatabase->BeginTrans();
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (!pStock->IsTodayDataActive()) {
			// 此股票今天停牌,所有的数据皆为零,不需要存储.
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

	// 存储今日生成的数据于DayLineExtendInfo表中。
	setDayLineExtendInfo.m_strFilter = _T("[ID] = 1");
	setDayLineExtendInfo.Open();
	setDayLineExtendInfo.m_pDatabase->BeginTrans();
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (!pStock->IsTodayDataActive()) {
			// 此股票今天停牌,所有的数据皆为零,不需要存储.
			continue;
		}
		pStock->AppendTodayExtendInfo(&setDayLineExtendInfo);
	}
	setDayLineExtendInfo.m_pDatabase->CommitTrans();
	setDayLineExtendInfo.Close();

	str = ConvertDateToChineseTimeStampString(lCurrentTradeDay);
	str += _T("的日线数据已生成");
	gl_systemMessage.PushInformationMessage(str);

	sprintf_s(buffer, _T("%d"), iCount);
	str = _T("今日处理了");
	str += buffer;
	str += _T("个股票");
	gl_systemMessage.PushInformationMessage(str);

	return iCount;
}

bool CContainerChinaStock::DeleteDayLineBasicInfo(long lDate) {
	char buffer[20]{0x000};
	CSetDayLineBasicInfo setDayLineBasicInfo;

	_ltoa_s(lDate, buffer, 10);
	const CString strDate = buffer;
	setDayLineBasicInfo.m_strFilter = _T("[Date] =");
	setDayLineBasicInfo.m_strFilter += strDate;
	setDayLineBasicInfo.Open();
	setDayLineBasicInfo.m_pDatabase->BeginTrans();
	while (!setDayLineBasicInfo.IsEOF()) {
		setDayLineBasicInfo.Delete();
		setDayLineBasicInfo.MoveNext();
	}
	setDayLineBasicInfo.m_pDatabase->CommitTrans();
	setDayLineBasicInfo.Close();

	return true;
}

bool CContainerChinaStock::DeleteDayLineExtendInfo(long lDate) {
	char buffer[20]{0x000};
	CSetDayLineExtendInfo setDayLineExtendInfo;

	_ltoa_s(lDate, buffer, 10);
	const CString strDate = buffer;
	setDayLineExtendInfo.m_strFilter = _T("[Date] =");
	setDayLineExtendInfo.m_strFilter += strDate;
	setDayLineExtendInfo.Open();
	setDayLineExtendInfo.m_pDatabase->BeginTrans();
	while (!setDayLineExtendInfo.IsEOF()) {
		setDayLineExtendInfo.Delete();
		setDayLineExtendInfo.MoveNext();
	}
	setDayLineExtendInfo.m_pDatabase->CommitTrans();
	setDayLineExtendInfo.Close();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
//
// 将当日处理好的数据储存于数据库中，以备万一系统崩溃时重新装入。
//
// 调试模式下，使用原始的逐项删除法，耗时3.5秒左右,使用ExecuteSQL(_T("TRUNCATE `chinamarket`.`today`;"))耗时3.1秒。没有显著的改善。
// 决定只使用原始的逐项删除模式。
//
//////////////////////////////////////////////////////////////////////////////////
bool CContainerChinaStock::SaveTempRTData() {
	CSetDayLineTodaySaved setDayLineTemp;
	CHighPerformanceCounter counter;
	counter.start();

	DeleteTempRTData();

	setDayLineTemp.Open();
	setDayLineTemp.m_pDatabase->BeginTrans();
	// 存储今日生成的数据于DayLineToday表中。
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CChinaStockPtr pStock = GetStock(l);
		if (!pStock->IsTodayDataActive()) {
			// 此股票今天停牌,所有的数据皆为零,不需要存储.
			continue;
		}
		if (pStock->IsNeedProcessRTData() && (!pStock->IsVolumeConsistence())) {
			CString str = pStock->GetSymbol();
			str += _T(" 股数不正确");
			gl_systemMessage.PushInnerSystemInformationMessage(str);
		}
		setDayLineTemp.AddNew();
		pStock->SaveTempInfo(&setDayLineTemp);
		setDayLineTemp.Update();
	}
	setDayLineTemp.m_pDatabase->CommitTrans();
	setDayLineTemp.Close();
	counter.stop();
	char buffer[30];
	sprintf_s(buffer, _T("%lld"), counter.GetElapsedMilliSecond());
	CString str = "存储实时数据用时：";
	str += buffer;
	gl_systemMessage.PushInnerSystemInformationMessage(str);

	return true;
}

void CContainerChinaStock::DeleteTempRTData() {
	CSetDayLineTodaySaved setDayLineTemp;

	setDayLineTemp.Open();
	setDayLineTemp.m_pDatabase->BeginTrans();
	while (!setDayLineTemp.IsEOF()) {
		setDayLineTemp.Delete();
		setDayLineTemp.MoveNext();
	}
	setDayLineTemp.m_pDatabase->CommitTrans();
	setDayLineTemp.Close();
}

///////////////////////////////////////////////////////////////////////////////////
//
// 计算lDate的日线相对强度, lDate的格式为：YYYYMMDD,如 19990605.
// 将日线按涨跌排列后,其相对强弱即其在队列中的位置.
// m_dRSIndex则是计算相对指数的涨跌强度。
//
//////////////////////////////////////////////////////////////////////////////////
bool CContainerChinaStock::BuildDayLineRS(long lDate) {
	vector<CChinaStockPtr> vStock;
	vector<int> vIndex;
	vector<double> vRS;
	int iTotalAShare = 0;
	CString strSQL;
	char pch[30];
	CSetDayLineBasicInfo setDayLineBasicInfo;
	double dShanghaiIndexUpDownRate = 0;
	double dShenzhenIndexUpDownRate = 0;
	double dRSIndex;

	sprintf_s(pch, _T("%08d"), lDate);
	const CString strDate = pch;
	setDayLineBasicInfo.m_strSort = _T("[UpDownRate]");
	setDayLineBasicInfo.m_strFilter = _T("[Date] =");
	setDayLineBasicInfo.m_strFilter += strDate;
	setDayLineBasicInfo.Open();
	if (setDayLineBasicInfo.IsEOF()) {
		// 数据集为空，表明此日没有交易
		setDayLineBasicInfo.Close();
		CString str = strDate;
		str += _T("日数据集为空，没有计算相对强度");
		gl_systemMessage.PushInformationMessage(str); // 采用同步机制报告信息
		return false;
	}
	setDayLineBasicInfo.m_pDatabase->BeginTrans();
	int iStockNumber = 0;
	while (!setDayLineBasicInfo.IsEOF()) {
		if (strcmp(setDayLineBasicInfo.m_Symbol, _T("sh000001")) == 0) {
			// 上海综指
			dShanghaiIndexUpDownRate = GetUpDownRate(setDayLineBasicInfo.m_Close, setDayLineBasicInfo.m_LastClose);
		}
		else if (strcmp(setDayLineBasicInfo.m_Symbol, _T("sz399001")) == 0) {
			// 深圳成指
			dShenzhenIndexUpDownRate = GetUpDownRate(setDayLineBasicInfo.m_Close, setDayLineBasicInfo.m_LastClose);
		}
		if (IsAStock(setDayLineBasicInfo.m_Symbol)) {
			const auto lIndex = m_mapSymbol.at(setDayLineBasicInfo.m_Symbol);
			vStock.push_back(GetStock(lIndex));
			vIndex.push_back(iStockNumber); // 将A股的索引记录在容器中。
			iTotalAShare++;
		}
		iStockNumber++;
		setDayLineBasicInfo.MoveNext();
	}
	const double dIndexUpDownRate = (dShanghaiIndexUpDownRate + dShenzhenIndexUpDownRate) / 2;

	setDayLineBasicInfo.MoveFirst();
	int iCount = 0;
	int iBefore = 0;
	while (iCount < vIndex.size()) {
		// 只计算活跃股票的相对强度
		for (int i = 0; i < vIndex.at(iCount) - iBefore; i++) {
			// 根据索引去更改数据库,跨过不是A股的股票
			setDayLineBasicInfo.MoveNext();
		}
		setDayLineBasicInfo.Edit();
		const double dLastClose = atof(setDayLineBasicInfo.m_LastClose);
		const double dLow = atof(setDayLineBasicInfo.m_Low);
		const double dHigh = atof(setDayLineBasicInfo.m_High);
		const double dClose = atof(setDayLineBasicInfo.m_Close);
		// 计算指数相对强度
		if (dLastClose < 0.001) {
			// 新股上市等，昨日收盘价格为零
			dRSIndex = 50;
		}
		else {
			const double dUpDownRate = (dClose - dLastClose) / dLastClose;
			if ((dUpDownRate > 0.11) || (dUpDownRate < -0.11)) {
				// 除权等导致价格突变
				dRSIndex = 50;
			}
			else {
				dRSIndex = (dUpDownRate - dIndexUpDownRate) * 500 + 50; // 以大盘涨跌为基准（50）。
			}
		}
		setDayLineBasicInfo.m_RSIndex = ConvertValueToString(dRSIndex);

		// 计算涨跌排名相对强度
		if (dLastClose < 0.001) {
			setDayLineBasicInfo.m_RS = ConvertValueToString(50); // 新股上市或者除权除息，不计算此股
		}
		else if (((dLow / dLastClose) < 0.88) || ((dHigh / dLastClose) > 1.12)) {
			// 除权、新股上市等
			setDayLineBasicInfo.m_RS = ConvertValueToString(50); // 新股上市或者除权除息，不计算此股
		}
		else if ((fabs(dHigh - dClose) < 0.0001) && (((dClose / dLastClose)) > 1.095)) {
			// 涨停板
			setDayLineBasicInfo.m_RS = ConvertValueToString(100);
		}
		else if ((fabs(dClose - dLow) < 0.0001) && ((dClose / dLastClose) < 0.905)) {
			// 跌停板
			setDayLineBasicInfo.m_RS = ConvertValueToString(0);
		}
		else { setDayLineBasicInfo.m_RS = ConvertValueToString((static_cast<double>(iCount) * 100) / iTotalAShare); }
		setDayLineBasicInfo.Update();
		iBefore = vIndex.at(iCount++);
		setDayLineBasicInfo.MoveNext(); // 移到下一个数据。
		iBefore++; // 计数器也同时加一。
	}
	setDayLineBasicInfo.m_pDatabase->CommitTrans();
	setDayLineBasicInfo.Close();

	vStock.clear();
	vIndex.clear();
	vRS.clear();

	const CString strDate2 = ConvertDateToChineseTimeStampString(lDate);
	const CString strTemp = strDate2 + _T("的股票日线相对强度计算完成");
	gl_systemMessage.PushDayLineInfoMessage(strTemp); // 采用同步机制报告信息

	return (true);
}

///////////////////////////////////////////////////////////////////////////////////
//
// 计算lDate的周线相对强度, lDate的格式为：YYYYMMDD,如 19990605.
// 将周线按涨跌排列后,其相对强弱即其在队列中的位置.
// m_dRSIndex则是计算相对指数的涨跌强度。
//
//////////////////////////////////////////////////////////////////////////////////
bool CContainerChinaStock::BuildWeekLineRS(long lDate) {
	vector<CChinaStockPtr> vStock;
	vector<int> vIndex;
	vector<double> vRS;
	int iTotalAShare = 0;
	CString strSQL;
	char pch[30];
	//CTime ctTime;
	CSetWeekLineBasicInfo setWeekLineBasicInfo;
	double dShanghaiIndexUpDownRate = 0;
	double dShenzhenIndexUpDownRate = 0;
	double dRSIndex;

	ASSERT(GetCurrentMonday(lDate) == lDate); // 确保此日期为星期一

	sprintf_s(pch, _T("%08d"), lDate);
	const CString strDate = pch;
	setWeekLineBasicInfo.m_strSort = _T("[UpDownRate]");
	setWeekLineBasicInfo.m_strFilter = _T("[Date] =");
	setWeekLineBasicInfo.m_strFilter += strDate;
	setWeekLineBasicInfo.Open();
	if (setWeekLineBasicInfo.IsEOF()) {
		// 数据集为空，表明此日没有交易
		setWeekLineBasicInfo.Close();
		CString str = strDate;
		str += _T("日数据集为空， 没有计算相对强度");
		gl_systemMessage.PushDayLineInfoMessage(str); // 采用同步机制报告信息
		return false;
	}
	setWeekLineBasicInfo.m_pDatabase->BeginTrans();
	int iStockNumber = 0;
	while (!setWeekLineBasicInfo.IsEOF()) {
		if (strcmp(setWeekLineBasicInfo.m_Symbol, _T("sh000001")) == 0) {
			// 上海综指
			dShanghaiIndexUpDownRate = GetUpDownRate(setWeekLineBasicInfo.m_Close, setWeekLineBasicInfo.m_LastClose);
		}
		else if (strcmp(setWeekLineBasicInfo.m_Symbol, _T("sz399001")) == 0) {
			// 深圳成指
			dShenzhenIndexUpDownRate = GetUpDownRate(setWeekLineBasicInfo.m_Close, setWeekLineBasicInfo.m_LastClose);
		}
		if (IsAStock(setWeekLineBasicInfo.m_Symbol)) {
			const auto lIndex = m_mapSymbol.at(setWeekLineBasicInfo.m_Symbol);
			vStock.push_back(GetStock(lIndex));
			vIndex.push_back(iStockNumber); // 将A股的索引记录在容器中。
			iTotalAShare++;
		}
		iStockNumber++;
		setWeekLineBasicInfo.MoveNext();
	}
	const double dIndexUpDownRate = (dShanghaiIndexUpDownRate + dShenzhenIndexUpDownRate) / 2;

	setWeekLineBasicInfo.MoveFirst();
	int iCount = 0;
	int iBefore = 0;
	while (iCount < vIndex.size()) {
		// 只计算活跃股票的相对强度
		for (int i = 0; i < vIndex.at(iCount) - iBefore; i++) {
			// 根据索引去更改数据库,跨过不是A股的股票
			setWeekLineBasicInfo.MoveNext();
		}
		setWeekLineBasicInfo.Edit();
		const double dLastClose = atof(setWeekLineBasicInfo.m_LastClose);
		const double dClose = atof(setWeekLineBasicInfo.m_Close);
		// 计算指数相对强度
		if (dLastClose < 0.001) {
			// 新股上市等，昨日收盘价格为零
			dRSIndex = 50;
		}
		else {
			const double dUpDownRate = (dClose - dLastClose) / dLastClose;
			dRSIndex = (dUpDownRate - dIndexUpDownRate) * 500 + 50; // 以大盘涨跌为基准（50）。
		}
		setWeekLineBasicInfo.m_RSIndex = ConvertValueToString(dRSIndex);

		// 计算涨跌排名相对强度
		if (dLastClose < 0.001) {
			setWeekLineBasicInfo.m_RS = ConvertValueToString(50); // 新股上市或者除权除息，不计算此股
		}
		else { setWeekLineBasicInfo.m_RS = ConvertValueToString((static_cast<double>(iCount) * 100) / iTotalAShare); }
		setWeekLineBasicInfo.Update();
		iBefore = vIndex.at(iCount++);
		setWeekLineBasicInfo.MoveNext(); // 移到下一个数据。
		iBefore++; // 计数器也同时加一。
	}
	setWeekLineBasicInfo.m_pDatabase->CommitTrans();
	setWeekLineBasicInfo.Close();

	vStock.clear();
	vIndex.clear();
	vRS.clear();

	const CString strDate2 = ConvertDateToChineseTimeStampString(lDate);
	const CString strTemp = strDate2 + _T("的股票周线相对强度计算完成");
	gl_systemMessage.PushInformationMessage(strTemp); // 采用同步机制报告信息

	return (true);
}

double CContainerChinaStock::GetUpDownRate(const CString& strClose, const CString& strLastClose) noexcept {
	char* p;
	const double lastClose = strtod(strLastClose, &p);
	if (lastClose < 0.001) return 0;
	double result = (strtod(strClose, &p) - lastClose) / lastClose;
	if ((result > 0.11) || (result < -0.11)) result = 0;
	return result;
}
