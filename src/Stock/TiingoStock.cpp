#include"pch.h"

#include"TiingoStock.h"

#include "ConvertToString.h"
#include "SetTiingoStock52WeekHigh.h"
#include "SetTiingoStock52WeekLow.h"
#include "TiingoStockDailyMeta.h"
#include "TimeConvert.h"
#include "WorldMarket.h"

CTiingoStock::CTiingoStock() {
	m_v52WeekHigh.clear();
	m_v52WeekLow.clear();
	SetExchangeCode(_T("US"));
	CTiingoStock::ResetAllUpdateDate();
}

void CTiingoStock::ResetAllUpdateDate() {
	SetStatementLastUpdatedDate(0);
	SetCompanyFinancialStatementUpdateDate(19800101);
	SetDailyUpdateDate(19800101);
	SetDayLineStartDate(29900101);
	SetDayLineEndDate(19800101);
	SetDayLineProcessDate(19800101);
	SetHistoryDayLineStartDate(19000101);
	SetHistoryDayLineEndDate(19000101);
	SetUpdateStockDailyMetaDate(19800101);
}

void CTiingoStock::Load(CSetTiingoStock& setTiingoStock) {
	m_strTiingoPermaTicker = setTiingoStock.m_TiingoPermaTicker;
	m_strSymbol = setTiingoStock.m_Ticker;
	m_strName = setTiingoStock.m_Name;
	SetActive(setTiingoStock.m_IsActive);
	m_fIsADR = setTiingoStock.m_IsADR;
	m_iSicCode = setTiingoStock.m_SicCode;
	m_strSicIndustry = setTiingoStock.m_SicIndustry;
	m_strSicSector = setTiingoStock.m_SicSector;
	m_strTiingoIndustry = setTiingoStock.m_TiingoIndustry;
	m_strTiingoSector = setTiingoStock.m_TiingoSector;
	m_strReportingCurrency = setTiingoStock.m_ReportingCurrency;
	m_strLocation = setTiingoStock.m_Location;
	m_strCompanyWebSite = setTiingoStock.m_CompanyWebSite;
	m_strSECFilingWebSite = setTiingoStock.m_SECFilingWebSite;
	m_lIPOStatus = setTiingoStock.m_IPOStatus;

	LoadUpdateDate(setTiingoStock.m_UpdateDate);
}

void CTiingoStock::Append(CSetTiingoStock& setTiingoStock) {
	setTiingoStock.AddNew();
	Save(setTiingoStock);
	ASSERT(setTiingoStock.Update());
}

void CTiingoStock::Save(CSetTiingoStock& setTiingoStock) {
	if ((m_strCompanyWebSite.GetLength() > 100)
		|| (m_strName.GetLength() > 200)
		|| (m_strSECFilingWebSite.GetLength() > 150)
		|| (m_strSicIndustry.GetLength() > 100)
		|| (m_strSicSector.GetLength() > 100)
		|| (m_strTiingoIndustry.GetLength() > 100)
		|| (m_strTiingoSector.GetLength() > 100)) {
		gl_systemMessage.PushErrorMessage(_T("Tiingo stock ") + m_strName + _T(" 字符串太长"));
	}
	m_strName = m_strName.Left(200);
	m_strSicIndustry = m_strSicIndustry.Left(100);
	m_strSicSector = m_strSicSector.Left(100);
	m_strTiingoIndustry = m_strTiingoIndustry.Left(100);
	m_strTiingoSector = m_strTiingoSector.Left(100);
	m_strCompanyWebSite = m_strCompanyWebSite.Left(100);
	m_strSECFilingWebSite = m_strSECFilingWebSite.Left(150);

	setTiingoStock.m_TiingoPermaTicker = m_strTiingoPermaTicker;
	setTiingoStock.m_Ticker = m_strSymbol;
	setTiingoStock.m_Name = m_strName;
	setTiingoStock.m_IsActive = IsActive();
	setTiingoStock.m_IsADR = m_fIsADR;
	setTiingoStock.m_SicCode = m_iSicCode;
	setTiingoStock.m_SicIndustry = m_strSicIndustry;
	setTiingoStock.m_SicSector = m_strSicSector;
	setTiingoStock.m_TiingoIndustry = m_strTiingoIndustry;
	setTiingoStock.m_TiingoSector = m_strTiingoSector;
	setTiingoStock.m_ReportingCurrency = m_strReportingCurrency;
	setTiingoStock.m_Location = m_strLocation;
	setTiingoStock.m_CompanyWebSite = m_strCompanyWebSite;
	setTiingoStock.m_SECFilingWebSite = m_strSECFilingWebSite;
	setTiingoStock.m_IPOStatus = m_lIPOStatus;

	const string sUpdateDate = m_jsonUpdateDate.dump();
	setTiingoStock.m_UpdateDate = sUpdateDate.c_str();
	ASSERT(sUpdateDate.size() < 10000);
}

void CTiingoStock::Update(CSetTiingoStock& setTiingoStock) {
	setTiingoStock.Edit();
	Save(setTiingoStock);
	setTiingoStock.Update();
}

void CTiingoStock::UpdateRTData(const CTiingoIEXTopOfBookPtr& pIEXTopOfBook) {
	m_TransactionTime = pIEXTopOfBook->m_llTimestamp;
	m_lOpen = pIEXTopOfBook->m_lOpen;
	m_lHigh = pIEXTopOfBook->m_lHigh;
	m_lLow = pIEXTopOfBook->m_lLow;
	m_lLastClose = pIEXTopOfBook->m_lLastClose;
	m_lNew = pIEXTopOfBook->m_lNew;
	m_llVolume = pIEXTopOfBook->m_llVolume;
}

void CTiingoStock::UpdateFinancialStateDB() {
	ASSERT(m_pvFinancialState != nullptr);
	CSetTiingoCompanyFinancialState setFinancialState;
	vector<CTiingoCompanyFinancialStatePtr> vOldFinancialState;
	CTiingoCompanyFinancialStatePtr pTiingoFinancialState = nullptr;
	long lSizeOfOldDayLine = 0;
	const size_t lSize = m_pvFinancialState->size();
	long lLastDate = 0;

	setFinancialState.m_strFilter = _T("[Symbol] = '");
	setFinancialState.m_strFilter += m_strSymbol;
	setFinancialState.m_strFilter += _T("'");
	setFinancialState.m_strSort = _T("[yearQuarter]");
	setFinancialState.Open();
	setFinancialState.m_pDatabase->BeginTrans();

	while (!setFinancialState.IsEOF()) {
		if (setFinancialState.m_yearQuarter > lLastDate) {
			lLastDate = setFinancialState.m_yearQuarter;
			pTiingoFinancialState = make_shared<CTiingoCompanyFinancialState>();
			pTiingoFinancialState->Load(setFinancialState);

			vOldFinancialState.push_back(pTiingoFinancialState);
			lSizeOfOldDayLine++;
		}
		else {
			setFinancialState.Delete(); //删除日期重复的数据
		}
		setFinancialState.MoveNext();
	}
	setFinancialState.m_pDatabase->CommitTrans();
	setFinancialState.Close();

	setFinancialState.m_strFilter = _T("[ID] = 1");
	setFinancialState.Open();
	setFinancialState.m_pDatabase->BeginTrans();
	if (lSizeOfOldDayLine > 0) {// 有旧数据
		long lCurrentPos = 0;
		for (int i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
			pTiingoFinancialState = m_pvFinancialState->at(i);
			if (pTiingoFinancialState->m_yearQuarter < vOldFinancialState.at(0)->m_yearQuarter) {	// 有更早的新数据？
				pTiingoFinancialState->Append(setFinancialState);
			}
			else {
				while ((lCurrentPos < lSizeOfOldDayLine) && vOldFinancialState.at(lCurrentPos)->m_yearQuarter < pTiingoFinancialState->m_yearQuarter) lCurrentPos++;
				if (lCurrentPos < lSizeOfOldDayLine) {
					if (vOldFinancialState.at(lCurrentPos)->m_yearQuarter > pTiingoFinancialState->m_yearQuarter) { // 前数据集中有遗漏的日期
						pTiingoFinancialState->Append(setFinancialState);
					}
				}
				else {
					pTiingoFinancialState->Append(setFinancialState);
				}
			}
		}
	}
	else {// 没有旧数据
		for (int i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
			pTiingoFinancialState = m_pvFinancialState->at(i);
			pTiingoFinancialState->Append(setFinancialState);
		}
	}
	setFinancialState.m_pDatabase->CommitTrans();
	setFinancialState.Close();

	SetCompanyFinancialStatementUpdateDate(gl_pWorldMarket->GetMarketDate());
	SetUpdateProfileDB(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 由于tiingo更新完日线数据后需要再次处理日线，故而此处默认为不卸载日线。这样能够加速后面的处理过程。
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoStock::UpdateDayLineDB() {
	if (IsUpdateDayLineDBAndClearFlag()) {// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
		if (HaveNewDayLineData()) {
			SaveDayLineDB();
			UpdateDayLineStartEndDate();
			SetUpdateProfileDB(true);
			const CString str = GetSymbol() + _T("日线资料存储完成");
			gl_systemMessage.PushDayLineInfoMessage(str);
			UnloadDayLine();
			return true;
		}
		UnloadDayLine();
	}
	return false;
}

void CTiingoStock::SaveCurrentDataToDayLineDB(CSetTiingoStockDayLine& setDayLine, long lTradeDay) const {
	setDayLine.AddNew();
	setDayLine.m_Date = lTradeDay;
	setDayLine.m_Symbol = m_strSymbol;
	setDayLine.m_Exchange = m_strExchangeCode;
	setDayLine.m_DisplaySymbol = _T("");
	setDayLine.m_Open = ConvertValueToString(m_lOpen, GetRatio());
	setDayLine.m_High = ConvertValueToString(m_lHigh, GetRatio());
	setDayLine.m_Low = ConvertValueToString(m_lLow, GetRatio());
	setDayLine.m_Close = ConvertValueToString(m_lNew, GetRatio());
	setDayLine.m_LastClose = ConvertValueToString(m_lLastClose, GetRatio());
	setDayLine.m_Volume = ConvertValueToString(m_llVolume);
	setDayLine.Update();
}

void CTiingoStock::UpdateProfile(const CTiingoStockPtr& pStock) {
	ASSERT(gl_systemConfiguration.IsPaidTypeTiingoAccount()); // 调用此函数时，必须保证是付费账户。
	if (pStock->m_strTiingoSector.GetLength() > 1) m_strTiingoSector = pStock->m_strTiingoSector;
	if (pStock->m_strTiingoIndustry.GetLength() > 1)m_strTiingoIndustry = pStock->m_strTiingoIndustry;
	if (pStock->m_iSicCode > 0)m_iSicCode = pStock->m_iSicCode;
	if (pStock->m_strSicSector.GetLength() > 1)m_strSicSector = pStock->m_strSicSector;
	if (pStock->m_strSicIndustry.GetLength() > 1)m_strSicIndustry = pStock->m_strSicIndustry;
	if (pStock->m_strLocation.GetLength() > 1)m_strLocation = pStock->m_strLocation;
	if (pStock->m_strCompanyWebSite.GetLength() > 1)m_strCompanyWebSite = pStock->m_strCompanyWebSite;
	if (pStock->m_strSECFilingWebSite.GetLength() > 1)m_strSECFilingWebSite = pStock->m_strSECFilingWebSite;
	SetUpdateProfileDB(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 使用从daily meta处得到的数据更新tiingo股票profile。
// 目前只使用日线开始和结束两个数据，其他数据暂时忽略。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTiingoStock::UpdateDailyMeta(const CTiingoStockDailyMetaPtr& pMeta) {
	if (pMeta->m_lHistoryDayLineStartDate == 19000101) {
		SetHistoryDayLineStartDate(19500101);
		SetUpdateProfileDB(true);
	}
	else {
		if (GetHistoryDayLineStartDate() != pMeta->m_lHistoryDayLineStartDate) {
			SetHistoryDayLineStartDate(pMeta->m_lHistoryDayLineStartDate);
			SetUpdateProfileDB(true);
		}
	}
	if (pMeta->m_lHistoryDayLineEndDate == 19000101) {
		SetHistoryDayLineEndDate(19500101);
		SetUpdateProfileDB(true);
	}
	else {
		if (GetHistoryDayLineEndDate() != pMeta->m_lHistoryDayLineEndDate) {
			SetHistoryDayLineEndDate(pMeta->m_lHistoryDayLineEndDate);
			SetUpdateProfileDB(true);
		}
	}
}

void CTiingoStock::UpdateDayLineStartEndDate() {
	long lStartDate = 0, lEndDate = 0;
	if (m_dataDayLine.GetStartEndDate(lStartDate, lEndDate)) {
		if (lStartDate < GetDayLineStartDate()) {
			SetDayLineStartDate(lStartDate);
			m_fUpdateProfileDB = true;
		}
		if (lEndDate > GetDayLineEndDate()) {
			SetDayLineEndDate(lEndDate);
			m_fUpdateProfileDB = true;
		}
	}
}

bool CTiingoStock::HaveNewDayLineData() {
	if (m_dataDayLine.Empty()) return false;
	if ((m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetMarketDate() > GetDayLineEndDate())
		|| (m_dataDayLine.GetData(0)->GetMarketDate() < GetDayLineStartDate()))
		return true;
	return false;
}

void CTiingoStock::CheckUpdateStatus(long lTodayDate) {
	CheckFinancialStateUpdateStatus(lTodayDate);
	CheckDayLineUpdateStatus(lTodayDate);
	CheckStockDailyMetaStatus(lTodayDate);
}

void CTiingoStock::CheckFinancialStateUpdateStatus(long lTodayDate) {
	if (IsEarlyThen(GetCompanyFinancialStatementUpdateDate(), lTodayDate, gl_systemConfiguration.GetTiingoCompanyFinancialStateUpdateRate())) {
		m_fUpdateFinancialState = true;
	}
	else {
		m_fUpdateFinancialState = false;
	}
}

void CTiingoStock::CheckDayLineUpdateStatus(long llTodayDate) {
	if (GetDayLineEndDate() == gl_pWorldMarket->GetCurrentTradeDate()) { // 已
		m_fUpdateDayLine = false;
		return;
	}
	ASSERT(GetDayLineEndDate() < gl_pWorldMarket->GetCurrentTradeDate());
	m_fUpdateDayLine = true;
}

void CTiingoStock::CheckStockDailyMetaStatus(long lCurrentDate) {
	if (GetUpdateStockDailyMetaDate() >= gl_pWorldMarket->GetCurrentTradeDate()) {
		SetUpdateStockDailyMeta(false);
	}
	else {
		SetUpdateStockDailyMeta(true);
	}
}

long CTiingoStock::GetDayLineProcessDate() {
	long l;
	try {
		l = m_jsonUpdateDate[_T("DayLineProcessDate")];
	} catch (json::exception&) {
		m_jsonUpdateDate[_T("DayLineProcessDate")] = 19800101;
		l = 19800101;
	}
	return l;
}

void CTiingoStock::Delete52WeekLow(long lDate) {
	auto it = std::ranges::find(m_v52WeekLow, lDate);
	if (it != m_v52WeekLow.end()) {
		m_v52WeekLow.erase(it);
	}
}

void CTiingoStock::Delete52WeekHigh(long lDate) {
	auto it = std::ranges::find(m_v52WeekHigh, lDate);
	if (it != m_v52WeekHigh.end()) {
		m_v52WeekHigh.erase(it);
	}
}

void CTiingoStock::Update52WeekHighLowDB() {
	SetUpdate52WeekHighLowDB(false);
	Update52WeekHighDB();
	Update52WeekLowDB();

	m_v52WeekHigh.clear();
	m_v52WeekLow.clear();
}

void CTiingoStock::Update52WeekHighDB() const {
	CSetTiingoStock52WeekHigh set52WeekHigh;
	auto lSize = m_v52WeekHigh.size();
	Delete52WeekHighDB();

	set52WeekHigh.m_strFilter = _T("[ID] = 1");
	set52WeekHigh.Open();
	set52WeekHigh.m_pDatabase->BeginTrans();
	for (size_t index = 0; index < lSize; index++) {
		set52WeekHigh.AddNew();
		set52WeekHigh.m_Symbol = GetSymbol();
		set52WeekHigh.m_Exchange = GetExchangeCode();
		set52WeekHigh.m_Date = m_v52WeekHigh.at(index);
		set52WeekHigh.Update();
	}
	set52WeekHigh.m_pDatabase->CommitTrans();
	set52WeekHigh.Close();
}

void CTiingoStock::Update52WeekLowDB() const {
	CSetTiingoStock52WeekLow set52WeekLow;
	auto lSize = m_v52WeekLow.size();

	Delete52WeekLowDB();

	set52WeekLow.m_strFilter = _T("[ID] = 1");
	set52WeekLow.Open();
	set52WeekLow.m_pDatabase->BeginTrans();
	for (size_t index = 0; index < lSize; index++) {
		set52WeekLow.AddNew();
		set52WeekLow.m_Symbol = GetSymbol();
		set52WeekLow.m_Exchange = GetExchangeCode();
		set52WeekLow.m_Date = m_v52WeekLow.at(index);
		set52WeekLow.Update();
	}
	set52WeekLow.m_pDatabase->CommitTrans();
	set52WeekLow.Close();
}

void CTiingoStock::Delete52WeekHighDB() const {
	CSetTiingoStock52WeekHigh set52WeekHigh;

	set52WeekHigh.m_strFilter = _T("[Symbol] ='");
	set52WeekHigh.m_strFilter += GetSymbol() + _T("'");
	set52WeekHigh.m_strSort = _T("[Date]");
	set52WeekHigh.Open();
	set52WeekHigh.m_pDatabase->BeginTrans();
	while (!set52WeekHigh.IsEOF()) {
		set52WeekHigh.Delete();
		set52WeekHigh.MoveNext();
	}
	set52WeekHigh.m_pDatabase->CommitTrans();
	set52WeekHigh.Close();
}

void CTiingoStock::Delete52WeekLowDB() const {
	CSetTiingoStock52WeekLow set52WeekLow;

	set52WeekLow.m_strFilter = _T("[Symbol] ='");
	set52WeekLow.m_strFilter += GetSymbol() + _T("'");
	set52WeekLow.m_strSort = _T("[Date]");
	set52WeekLow.Open();
	set52WeekLow.m_pDatabase->BeginTrans();
	while (!set52WeekLow.IsEOF()) {
		set52WeekLow.Delete();
		set52WeekLow.MoveNext();
	}
	set52WeekLow.m_pDatabase->CommitTrans();
	set52WeekLow.Close();
}

constexpr double __SMALL_DOUBLE_ = 0.000005;
/////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理日线
// 目前只查找52周新低和52周新高。
// Note 目前采用的方法极其简单，导致计算的时间较长，每个股票的计算时间大致为1-3秒
// 故而采用后台持续执行的模式，单一工作线程调用。
// 
//
//////////////////////////////////////////////////////////////////////////////////////////////
void CTiingoStock::ProcessDayLine() {
	if (gl_systemConfiguration.IsExitingSystem()) return;
	if (!IsDayLineLoaded()) {
		m_dataDayLine.LoadDB(GetSymbol());
	}

	auto endPos = m_dataDayLine.Size();
	if (endPos < 300) return; // 最少300个日线数据
	m_v52WeekHigh.clear();
	m_v52WeekLow.clear();
	for (size_t index = 0; index < endPos; index++) {
		double d = m_dataDayLine.GetData(index)->m_lClose;
		m_vClose.push_back(d);
	}

	double dSplitFactor = CalculateSplitFactor(0, endPos);
	NormalizeStockCloseValue(dSplitFactor, 0, endPos);

	//FindHighLow3(endPos); // 这种是最简单的。耗时虽然长，但与数据库操作相比还是短的，故而采用更快速的算法没有那么必须了。
	FindHighLow2(endPos); // 这种速度较快。

	m_dataDayLine.Unload();
	SetUpdate52WeekHighLowDB(true);
}

void CTiingoStock::ProcessDayLine2() {
	if (!IsDayLineLoaded()) {
		m_dataDayLine.LoadDB(GetSymbol());
	}

	auto endPos = m_dataDayLine.Size();
	if (endPos < 300) return; // 最少300个日线数据
	m_v52WeekHigh.clear();
	m_v52WeekLow.clear();
	for (size_t index = 0; index < endPos; index++) {
		double d = m_dataDayLine.GetData(index)->m_lClose;
		m_vClose.push_back(d);
	}

	double dSplitFactor = CalculateSplitFactor(0, endPos);
	NormalizeStockCloseValue(dSplitFactor, 0, endPos);
	FindHighLow2(endPos);

	UnloadDayLine();
	SetUpdate52WeekHighLowDB(true);
}

void CTiingoStock::ProcessDayLine3() {
	if (!IsDayLineLoaded()) {
		m_dataDayLine.LoadDB(GetSymbol());
	}
	auto endPos = m_dataDayLine.Size();

	if (endPos < 300) return; // 最少300个日线数据
	m_v52WeekHigh.clear();
	m_v52WeekLow.clear();
	for (size_t index = 0; index < endPos; index++) {
		double d = m_dataDayLine.GetData(index)->m_lClose;
		m_vClose.push_back(d);
	}

	double dSplitFactor = CalculateSplitFactor(0, endPos);
	NormalizeStockCloseValue(dSplitFactor, 0, endPos);
	FindHighLow3(endPos);

	m_dataDayLine.Unload();
	SetUpdate52WeekHighLowDB(true);
}

void CTiingoStock::FindHighLow3(size_t endPos) {
	for (size_t currentPos = 250; currentPos < endPos; currentPos++) { // 从第250个位置开始寻找。
		auto dClose = m_vClose[currentPos];
		switch (IsLowOrHigh(currentPos, dClose)) {
		case -1: // new low
			Add52WeekLow(m_dataDayLine.GetData(currentPos)->GetMarketDate());
			break;
		case 1: // new high
			Add52WeekHigh(m_dataDayLine.GetData(currentPos)->GetMarketDate());
			break;
		default:
			break;
		}
	}
}

int CTiingoStock::IsLowOrHigh(size_t index, double dClose) const {
	ASSERT(index >= 250);
	bool fIsNewLow = true;
	bool fIsNewHigh = true;
	double belowClose = dClose + __SMALL_DOUBLE_; // 增加一点以利于判断相同的数值。
	for (size_t i = index - 250; i < index; i++) {
		if (m_vClose[i] < belowClose) {
			fIsNewLow = false;
			break;
		}
	}
	if (fIsNewLow) return -1; // 52周新低价
	double aboveClose = dClose - __SMALL_DOUBLE_;// 减少一点以利于判断相同的数值。
	for (size_t i = index - 250; i < index; i++) {
		if (m_vClose[i] > aboveClose) {
			fIsNewHigh = false;
			break;
		}
	}
	if (fIsNewHigh) return 1; // 52周新高价
	return 0; // 既非52周最高价亦非52周最低价
}

void CTiingoStock::FindHighLow2(size_t endPos) {
	FindAll52WeekLow(0, endPos);
	FindAll52WeekHigh(0, endPos);
}

void CTiingoStock::FindAll52WeekLow(size_t beginPos, size_t endPos) {
	bool fFound = false;
	size_t currentBeginPos = beginPos;
	size_t currentPos = 0;
	size_t current52WeekLowPos = beginPos;
	auto dCurrent52WeekLowValue = m_vClose[beginPos];

	while (currentPos < endPos) {
		if (fFound) {
			bool fCurrentFound = false;
			double belowCurrent52WeekLow = dCurrent52WeekLowValue - __SMALL_DOUBLE_;
			for (auto index = currentBeginPos; index <= current52WeekLowPos; index++) {
				if (currentPos == endPos) {
					fCurrentFound = false;
					break;
				}
				if (m_vClose[currentPos] < belowCurrent52WeekLow) { // 找到了
					Add52WeekLow(m_dataDayLine.GetData(currentPos)->m_lDate);
					dCurrent52WeekLowValue = m_vClose[currentPos];
					current52WeekLowPos = currentPos;
					currentPos++;
					currentBeginPos = currentPos - 250;
					fCurrentFound = true;
					break;
				}
				currentPos++;
			}
			if (!fCurrentFound) {
				currentBeginPos = current52WeekLowPos + 1;
				fFound = false;
			}
		}
		else { // 找寻下一个新低位置
			currentPos = currentBeginPos + 250 > endPos ? endPos : currentBeginPos + 250;
			current52WeekLowPos = FindCurrent52WeekLow(currentBeginPos, currentPos, dCurrent52WeekLowValue);
			dCurrent52WeekLowValue = m_vClose[current52WeekLowPos];
			fFound = true;
		}
	}
}

size_t CTiingoStock::FindCurrent52WeekLow(size_t beginPos, size_t endPos, double& value) const {
	ASSERT(beginPos < endPos);
	auto pos = beginPos;
	value = m_vClose[beginPos];
	double belowCurrentValue = value - __SMALL_DOUBLE_;

	for (auto index = beginPos; index < endPos; index++) {
		if (m_vClose[index] < belowCurrentValue) {
			pos = index;
			value = m_vClose[index];
			belowCurrentValue = value - __SMALL_DOUBLE_;
		}
	}
	return pos;
}

void CTiingoStock::FindAll52WeekHigh(size_t beginPos, size_t endPos) {
	bool fFound = false;
	size_t currentBeginPos = beginPos;
	size_t currentEndPos = currentBeginPos + 250 > endPos ? endPos : currentBeginPos + 250;
	size_t current52WeekHighPos = beginPos;
	auto dCurrent52WeekHighValue = m_vClose[beginPos];

	while (currentBeginPos < endPos - 1) {
		if (fFound) { // 有最高价
			auto aboveCurrent52WeekHigh = dCurrent52WeekHighValue + __SMALL_DOUBLE_;
			bool fCurrentFound = false;
			for (auto index = currentBeginPos; index <= current52WeekHighPos; index++) { // 查询到最新的新低价
				if (currentEndPos == endPos) {
					currentBeginPos = endPos - 1;
					fCurrentFound = false;
					break;
				}
				if (m_vClose[currentEndPos] > aboveCurrent52WeekHigh) { // 找到了
					Add52WeekHigh(m_dataDayLine.GetData(currentEndPos)->m_lDate);
					dCurrent52WeekHighValue = m_vClose[currentEndPos];
					current52WeekHighPos = currentEndPos;
					fCurrentFound = true;
					currentBeginPos = current52WeekHighPos - 250;
					break;
				}
				currentEndPos++;
			}
			if (!fCurrentFound) {
				if (currentEndPos < endPos - 1) {
					currentBeginPos = current52WeekHighPos + 1;
					currentEndPos = endPos < currentBeginPos + 250 ? endPos : currentBeginPos + 250;
				}
				else {
					break; // 全部查询完毕
				}
				fFound = false;
			}
		}
		else { // 找寻下一个新低位置
			if (currentBeginPos + 250 < endPos) {
				currentEndPos = currentBeginPos + 250;
			}
			else {
				currentEndPos = endPos;
			}
			current52WeekHighPos = FindCurrent52WeekHigh(currentBeginPos, currentEndPos, dCurrent52WeekHighValue);
			dCurrent52WeekHighValue = m_vClose[current52WeekHighPos];
			fFound = true;
		}
	}
}

size_t CTiingoStock::FindCurrent52WeekHigh(size_t beginPos, size_t endPos, double& value) const {
	ASSERT(beginPos < endPos);
	auto pos = beginPos;
	value = m_vClose[beginPos];
	auto aboveCurrentValue = value + __SMALL_DOUBLE_;

	for (auto index = beginPos; index < endPos; index++) {
		if (m_vClose[index] > aboveCurrentValue) {
			pos = index;
			value = m_vClose[index];
			aboveCurrentValue = value + __SMALL_DOUBLE_;
		}
	}
	return pos;
}

double CTiingoStock::CalculateSplitFactor(size_t beginPos, size_t endPos) const {
	double dRatio = 1;
	for (auto index = beginPos; index < endPos; index++) {
		auto splitFactor = m_dataDayLine.GetData(index)->m_dSplitFactor;
		if (splitFactor > __SMALL_DOUBLE_) {
			dRatio *= splitFactor;
		}
	}
	return dRatio;
}

void CTiingoStock::NormalizeStockCloseValue(double dSplitFactor, size_t calculatePos, size_t dayLineSize) {
	double dCurrentSplitFactor = 1;
	if (dSplitFactor < 1) { // 总体是缩股的。
		for (auto index = dayLineSize - 1; index > calculatePos; index--) { // 向前复权（保持目前的股价不变）
			m_vClose[index] *= dCurrentSplitFactor; // 使用除法向前复权。要先计算，然后才算splitFactor
			auto currentSplitFactor = m_dataDayLine.GetData(index)->m_dSplitFactor;
			if (currentSplitFactor < 1 + __SMALL_DOUBLE_ && currentSplitFactor > 1 - __SMALL_DOUBLE_) {
				// do nothing
			}
			else {
				if (currentSplitFactor > __SMALL_DOUBLE_) {
					dCurrentSplitFactor /= currentSplitFactor;
				}
			}
		}
	}
	else { // 总体是扩股的。
		for (auto index = calculatePos; index < dayLineSize; index++) { // 向后复权（目前的股价会变大）
			auto currentSplitFactor = m_dataDayLine.GetData(index)->m_dSplitFactor;
			if ((currentSplitFactor < 1 + __SMALL_DOUBLE_) && currentSplitFactor > 1 - __SMALL_DOUBLE_) {
				// do nothing
			}
			else {
				dCurrentSplitFactor *= currentSplitFactor;
			}
			m_vClose[index] *= dCurrentSplitFactor;
		}
	}
}
