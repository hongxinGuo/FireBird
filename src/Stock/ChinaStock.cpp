#include"pch.h"

#include"TimeConvert.h"
#include"ChinaStockCodeConverter.h"

#include "ChinaStock.h"
#include"ChinaMarket.h"
#include"MonthLine.h"

////////////////////////////////////////////////////////////////////////////////////////////
//
// 判断strStockCode是否为沪深A股主板的股票代码。
// 沪市A股代码以600或601开头，深市A股代码以000或001开头。
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool IsShareA(const string& strStockCode) {
	const string strSymbol = GetStockSymbol(strStockCode);
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
	return false;
}

bool IsChinaStock(const CVirtualStockPtr& pStock) {
	if (pStock == nullptr) return false;
	return typeid(*pStock) == typeid(CChinaStock);
}

CChinaStock::CChinaStock() {
	for (int i = 0; i < 5; i++) {
		m_lPBuy.at(i) = m_lPSell.at(i) = 0;
		m_lVBuy.at(i) = m_lVSell.at(i) = 0;
	}

	m_nHand = 100;

	m_lHighLimitFromTengxun = m_lLowLimitFromTengxun = 0;
	m_lHighLimit = m_lLowLimit = 0;
	for (int i = 0; i < 5; i++) {
		m_lPBuy.at(i) = m_lPSell.at(i) = 0;
		m_lVBuy.at(i) = m_lVSell.at(i) = 0;
	}
	m_dRealtimeRS = 0;
	m_dRealtimeRSIndex = 0;

	m_llLastSavedVolume = 0;

	m_fChosen = false;
	m_fUpdateChosenStockDB = false;

	m_fDayLineDBUpdated = false;

	m_fHaveFirstRTData = false; // 实时数据开始计算标识。第一个实时数据只能用来初始化系统，不能用于计算。从第二个数据开始计算才有效。
	m_fNeedProcessRTData = true;
	m_fRTDataCalculated = false;
	m_pLastRTData = nullptr;

	ClearRTDataDeque();
}

void CChinaStock::ClearRTDataDeque() {
	const auto lTotalNumber = GetRTDataQueueSize();
	for (size_t i = 0; i < lTotalNumber; i++) {
		CWebRTDataPtr pRTData = PopRTData();
	}
}

bool CChinaStock::HaveNewDayLineData() {
	if (m_dataDayLine.Size() <= 0) return false;
	if (m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetDate() > GetDayLineEndDate()) return true;
	return false;
}

void CChinaStock::UpdateStatusByDownloadedDayLine() {
	if (m_dataDayLine.Empty()) return;
	if (IsEarlyThen(m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetDate(), gl_pChinaMarket->GetMarketDate(), 30)) {
		// 提取到的股票日线数据其最新日早于上个月的这个交易日（退市了或相似情况，给一个月的时间观察）。
		SetIPOStatus(_STOCK_DELISTED_); // 已退市或暂停交易。
	}
	else {
		SetIPOStatus(_STOCK_IPOED_); // 正常交易股票
	}
}

void CChinaStock::ReportDayLineDownLoaded() {
	//string strTemp = GetSymbol();
	//strTemp += "日线下载完成.");
	//gl_systemMessage.PushDayLineInfoMessage(strTemp);
}

void CChinaStock::AppendTodayBasicInfo(CSetChinaMarketDayLineInfo* pSetDayLineBasicInfo) const {
	const auto pDayLine = make_shared<CDayLine>();

	ASSERT(pSetDayLineBasicInfo->IsOpen());

	UpdateCurrentHistoryCandle(pDayLine);
	pDayLine->AppendBasicData(pSetDayLineBasicInfo);
}

void CChinaStock::UpdateCurrentHistoryCandle(const CVirtualHistoryCandlePtr& pBeUpdated) const {
	pBeUpdated->SetDate(gl_pChinaMarket->ConvertToDate(GetTransactionTime()));
	pBeUpdated->SetExchange(m_strExchangeCode);
	pBeUpdated->SetStockSymbol(m_strSymbol);
	pBeUpdated->SetLastClose(m_lLastClose);
	pBeUpdated->SetOpen(m_lOpen);
	pBeUpdated->SetHigh(m_lHigh);
	pBeUpdated->SetLow(m_lLow);
	pBeUpdated->SetClose(m_lNew);
	pBeUpdated->SetVolume(m_llVolume);
	pBeUpdated->SetAmount(m_llAmount);
	pBeUpdated->SetUpDown(static_cast<double>(m_lUpDown) / GetRatio());
	pBeUpdated->SetUpDownRate(m_dUpDownRate);
	if (m_llTotalValue != 0) {
		pBeUpdated->SetChangeHandRate(static_cast<double>(100) * m_llAmount / m_llTotalValue);
	}
	else {
		pBeUpdated->SetChangeHandRate(0.0);
	}
	pBeUpdated->SetTotalValue(m_llTotalValue);
	pBeUpdated->SetCurrentValue(m_llCurrentValue);
}

void CChinaStock::UpdateRTData(const CWebRTDataPtr& pRTData) {
	SetTimePoint(pRTData->GetTimePoint());
	SetTransactionTime(pRTData->GetTransactionTime());
	SetLastClose(pRTData->GetLastClose());
	SetNew(pRTData->GetNew());
	SetHigh(pRTData->GetHigh());
	SetLow(pRTData->GetLow());
	SetOpen(pRTData->GetOpen());
	SetVolume(pRTData->GetVolume());
	SetAmount(pRTData->GetAmount());
	SetUpDown(m_lNew - m_lLastClose);
	if (m_lLastClose > 0) SetUpDownRate(static_cast<double>(m_lUpDown) * 100 / m_lLastClose);
	else SetUpDownRate(0);
	// 新浪实时数据是没有总市值和流通市值这两项的，故而需要判断一下是否为零
	if (pRTData->GetTotalValue() > 0) SetTotalValue(pRTData->GetTotalValue());
	if (pRTData->GetCurrentValue() > 0) SetCurrentValue(pRTData->GetCurrentValue());
	if (m_llTotalValue != 0) m_dChangeHandRate = static_cast<double>(m_llAmount) * 100 / m_llTotalValue;
	else m_dChangeHandRate = 0;
	for (int i = 0; i < 5; i++) {
		SetPBuy(i, pRTData->GetPBuy(i));
		SetVBuy(i, pRTData->GetVBuy(i));
		SetPSell(i, pRTData->GetPSell(i));
		SetVSell(i, pRTData->GetVSell(i));
	}
}

void CChinaStock::UpdateStatus(const CWebRTDataPtr& pRTData) {
	SetActive(true);
	SetSymbol(pRTData->GetSymbol()); // 更新全局股票池信息（有时RTData不全，无法更新退市的股票信息）
	if (!pRTData->GetStockName().empty()) SetDisplaySymbol(pRTData->GetStockName()); // 更新全局股票池信息（有时RTData不全，无法更新退市的股票信息）
	SetIPOStatus(_STOCK_IPOED_);
}

void CChinaStock::UpdateDayLineStartEndDate() {
	bool fUpdated = false;

	if (m_dataDayLine.Size() > 0) {
		if ((GetDayLineStartDate() == CHINA_MARKET_BEGIN_DATE_) || (m_dataDayLine.GetData(0)->GetDate() < GetDayLineStartDate())) {
			SetDayLineStartDate(m_dataDayLine.GetData(0)->GetDate());
			SetDayLineDBUpdated(true);
			fUpdated = true;
		}
		if (m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetDate() > GetDayLineEndDate()) {
			SetDayLineEndDate(m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetDate());
			auto day = GetDayLineEndDate();
			SetDayLineDBUpdated(true);
			fUpdated = true;
		}
	}
	if (fUpdated) {
		SetUpdateProfileDB(true);
	}
}

bool CChinaStock::Calculate10RSStrong1StockSet() {
	CSetChinaMarketDayLineInfo setDayLineBasicInfo;
	vector<double> m_v10DaysRS;
	int iCountFirst = 0, iCountSecond = 0, iCountThird = 0;

	ASSERT(m_dataDayLine.IsDataLoaded());
	const size_t iDayLineSize = GetDayLineSize();

	if (iDayLineSize < 350) return false;
	m_v10DaysRS.resize(iDayLineSize);
	CalculateDayLineRSIndex();
	Get10DaysRS(m_v10DaysRS);
	size_t i, j;

	for (i = iDayLineSize - 1; i > iDayLineSize - 40; i--) {
		if (m_v10DaysRS.at(i) > 55) iCountFirst++;
		if (iCountFirst >= 3) break;
	}
	if (iCountFirst < 3) return false;
	while (m_v10DaysRS.at(i) > 53) i--;
	for (j = i; j > i - 80; j--) {
		if (m_v10DaysRS.at(j) < 55) iCountSecond++;
		if (iCountSecond >= 78) break;
	}
	if (iCountSecond < 78) return false;
	while (m_v10DaysRS.at(j) > 53) j--;
	for (auto k = j; k > j - 130; k--) {
		if (m_v10DaysRS.at(k) > 47) iCountThird++;
		if (iCountThird > 128) break;
	}
	if (iCountThird < 128) return false;

	return true;
}

bool CChinaStock::Calculate10RSStrongStockSet(const CRSReference* pRef) {
	CSetChinaMarketDayLineInfo setDayLineBasicInfo;
	vector<double> m_v10DaysRS;
	int iCountFirst = 0, iCountSecond = 0, iCountThird = 0, iCountFourth = 0;
	bool fFind1 = false, fFind2 = false, fFind3 = false, fFind4 = false;
	double dStrong1, dStrong2, dStrong3, dStrong4;
	bool fFindHigh1, fFindHigh2, fFindHigh3, fFindHigh4;

	if (pRef->m_dRSStrong[0] < 0) {
		dStrong1 = -pRef->m_dRSStrong[0];
		fFindHigh1 = false;
	}
	else {
		dStrong1 = pRef->m_dRSStrong[0];
		fFindHigh1 = true;
	}
	if (pRef->m_dRSStrong[1] < 0) {
		dStrong2 = -pRef->m_dRSStrong[1];
		fFindHigh2 = false;
	}
	else {
		dStrong2 = pRef->m_dRSStrong[1];
		fFindHigh2 = true;
	}
	if (pRef->m_dRSStrong[2] < 0) {
		dStrong3 = -pRef->m_dRSStrong[2];
		fFindHigh3 = false;
	}
	else {
		dStrong3 = pRef->m_dRSStrong[2];
		fFindHigh3 = true;
	}
	if (pRef->m_dRSStrong[3] < 0) {
		dStrong4 = -pRef->m_dRSStrong[3];
		fFindHigh4 = false;
	}
	else {
		dStrong4 = pRef->m_dRSStrong[3];
		fFindHigh4 = true;
	}

	ASSERT(m_dataDayLine.IsDataLoaded());
	const size_t iDayLineSize = GetDayLineSize();
	if ((iDayLineSize < (pRef->m_lDayLength[0] + pRef->m_lDayLength[1] + pRef->m_lDayLength[2] + 10))
		|| (iDayLineSize < pRef->m_lDayLength[3] + 10))
		return false;

	m_v10DaysRS.resize(iDayLineSize);
	CalculateDayLineRSIndex();
	Get10DaysRS(m_v10DaysRS);
	size_t i = 0;
	size_t j = 0;

	if (pRef->m_lDayLength[0] == 0) fFind1 = true;
	else if (fFindHigh1) {
		for (i = iDayLineSize - 1; i > (iDayLineSize - pRef->m_lDayLength[0]); i--) {
			if (m_v10DaysRS.at(i) > dStrong1) iCountFirst++;
			if (iCountFirst >= pRef->m_lStrongDayLength[0]) {
				fFind1 = true;
				break;
			}
		}
	}
	else {
		for (i = iDayLineSize - 1; i > (iDayLineSize - pRef->m_lDayLength[0]); i--) {
			if (m_v10DaysRS.at(i) < dStrong1) iCountFirst++;
			if (iCountFirst >= pRef->m_lStrongDayLength[0]) {
				fFind1 = true;
				break;
			}
		}
	}
	if (!fFind1) return false;
	while (m_v10DaysRS.at(i) > (dStrong1 - 2)) i--;

	if (pRef->m_lDayLength[1] == 0) fFind2 = true;
	else if (fFindHigh2) {
		for (j = i; j > (i - pRef->m_lDayLength[1]); j--) {
			if (m_v10DaysRS.at(j) > dStrong2) iCountSecond++;
			if (iCountSecond >= pRef->m_lStrongDayLength[1]) {
				fFind2 = true;
				break;
			}
		}
	}
	else {
		for (j = i; j > (i - pRef->m_lDayLength[1]); j--) {
			if (m_v10DaysRS.at(j) < dStrong2) iCountSecond++;
			if (iCountSecond >= pRef->m_lStrongDayLength[1]) {
				fFind2 = true;
				break;
			}
		}
	}
	if (!fFind2) return false;
	while (m_v10DaysRS.at(j) > (dStrong2 - 2)) j--;

	if (pRef->m_lDayLength[2] == 0) fFind3 = true;
	else if (fFindHigh3) {
		for (auto k = j; k > (j - pRef->m_lDayLength[2]); k--) {
			if (m_v10DaysRS.at(k) > dStrong3) iCountThird++;
			if (iCountThird >= pRef->m_lStrongDayLength[2]) {
				fFind3 = true;
				break;
			}
		}
	}
	else {
		for (auto k = j; k > (j - pRef->m_lDayLength[2]); k--) {
			if (m_v10DaysRS.at(k) < dStrong3) iCountThird++;
			if (iCountThird >= pRef->m_lStrongDayLength[2]) {
				fFind3 = true;
				break;
			}
		}
	}
	if (!fFind3) return false;

	if (pRef->m_lDayLength[3] == 0) return true; // DayLength4为零的话，不做通盘选择
	if (fFindHigh4) {
		for (i = iDayLineSize - 1; i > (iDayLineSize - pRef->m_lDayLength[3]); i--) {
			if (m_v10DaysRS.at(i) > dStrong4) iCountFourth++;
			if (iCountFourth >= pRef->m_lStrongDayLength[3]) {
				fFind4 = true;
				break;
			}
		}
	}
	else {
		for (i = iDayLineSize - 1; i > (iDayLineSize - pRef->m_lDayLength[3]); i--) {
			if (m_v10DaysRS.at(i) < dStrong4) iCountFourth++;
			if (iCountFourth >= pRef->m_lStrongDayLength[3]) {
				fFind4 = true;
				break;
			}
		}
	}
	if (!fFind4) return false;

	return true;
}

bool CChinaStock::LoadStockCodeDB(CSetChinaStockSymbol& setChinaStockSymbol) {
	LoadSymbol(setChinaStockSymbol);

	return true;
}

void CChinaStock::CheckNeedProcessRTData() {
	SetNeedProcessRTData(true);
	if (IsShanghaiExchange(GetSymbol())) {
		if (GetSymbol().substr(0, 6) <= "000999") {//沪市指数？
			SetNeedProcessRTData(false);
		}
	}
	else if ((GetSymbol().substr(0, 6) >= "399000")) {// 深市指数
		SetNeedProcessRTData(false);
	}
}

void CChinaStock::CheckIPOStatus() {
	if (!IsDelisted()) {
		if (IsEarlyThen(GetDayLineEndDate(), gl_pChinaMarket->GetMarketDate(), 30)) {
			SetIPOStatus(_STOCK_DELISTED_);
			SetUpdateProfileDB(true);
		}
	}
}

bool CChinaStock::CheckDayLineStatus() {
	// 不再更新日线数据比上个交易日要新的股票。其他所有的股票都查询一遍，以防止出现新股票或者老的股票重新活跃起来。
	if (gl_pChinaMarket->GetLastTradeDate() <= GetDayLineEndDate()) {// 最新日线数据为今日或者上一个交易日的数据。
		SetUpdateDayLine(false); // 日线数据不需要更新
	}
	else if (IsDelisted()) {// 退市股票如果已下载过日线数据，则每星期一复查日线数据
		if ((gl_pChinaMarket->GetDayOfWeek() != 1) && (GetDayLineEndDate() != CHINA_MARKET_BEGIN_DATE_)) {
			SetUpdateDayLine(false);
		}
	}
	return m_fUpdateDayLine;
}

bool CChinaStock::BuildWeekLine(long lStartDate) {
	if (IsNullStock()) return true;
	if (!IsDayLineLoaded()) { LoadDayLineDB(); }
	if (GetDayLineSize() <= 0) return true;

	if (CreateWeekLine(lStartDate)) {
		SaveWeekLine();
	}

	if (!IsSelected()) {
		UnloadDayLine();
		UnloadWeekLine();
	}

	return true;
}

bool CChinaStock::CreateMonthLine() {
	ASSERT(m_dataDayLine.IsDataLoaded());
	size_t index = 0;
	CMonthLine monthLine;
	size_t monthLineSize = m_dataDayLine.Size();
	while (index < monthLineSize) {
		monthLine.Reset();
		auto pDayLine = m_dataDayLine.GetData(index++);
		long lCurrentEndDate = GetNextMonth(pDayLine->GetDate());
		monthLine.SetDate(pDayLine->GetDate());
		monthLine.SetOpen(pDayLine->GetOpen());
		monthLine.SetLow(pDayLine->GetLow());
		do {
			if (pDayLine->GetHigh() > monthLine.GetHigh()) monthLine.SetHigh(pDayLine->GetHigh());
			if (pDayLine->GetLow() < monthLine.GetLow()) monthLine.SetLow(pDayLine->GetLow());
			monthLine.SetVolume(monthLine.GetVolume() + pDayLine->GetVolume());
			monthLine.SetAmount(monthLine.GetAmount() + pDayLine->GetAmount());
			monthLine.SetClose(pDayLine->GetClose());
			if (index < monthLineSize) pDayLine = m_dataDayLine.GetData(index);
			else break;
			if (pDayLine->GetDate() < lCurrentEndDate) index++;
			else break;
		} while (true);

		if (monthLine.GetClose() > 0) m_dataMonthLine.Add(monthLine); // 有数据才存储
	}

	m_dataMonthLine.SetDataLoaded(true);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//
// 判断股票数据是否有效。采用最高价、最低价、成交量和成交额来判断，如果都为零，则认为此股今日没有有效数据
// 不采用开盘价，因开盘价有可能不为零时，其他数据皆为零（停牌时即此状态）。
//
////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::IsTodayDataActive() const {
	if (!m_fActive) return false;
	return IsTodayDataChanged();
}

////////////////////////////////////////////////////////////////////////////////
//
// 判断股票数据是否有效。采用最高价、最低价、成交量和成交额来判断，如果都为零，则认为此股今日没有有效数据
// 不采用开盘价，因开盘价有可能不为零时，其他数据皆为零（停牌时即此状态）。
//
////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::IsTodayDataChanged() const {
	if ((GetHigh() != 0) || (GetLow() != 0) || (GetVolume() != 0) || (GetAmount() != 0)) {
		return true;
	}
	return false;
}

bool CChinaStock::CreateWeekLine2(long lStartDate) {
	ASSERT(IsDayLineLoaded());
	ASSERT(m_dataDayLine.Size() > 0);
	long i = 0;

	m_dataWeekLine.Unload();
	while ((i < m_dataDayLine.Size()) && (m_dataDayLine.GetData(i)->GetDate() < lStartDate)) {
		i++;
	}
	if (i < m_dataDayLine.Size()) {
		CWeekLine weekLine;
		do {
			weekLine.Reset();
			weekLine = m_dataDayLine.CreateNewWeekLine(i);
			m_dataWeekLine.Add(weekLine);
		} while (i < m_dataDayLine.Size());
		m_dataWeekLine.SetDataLoaded(true);
		return true;
	}
	return false;
}

bool CChinaStock::CreateWeekLine(long lStartDate) {
	ASSERT(m_dataDayLine.IsDataLoaded());
	size_t index = 0;
	CTiingoCandleLine weekLine;
	size_t dayLineSize = m_dataDayLine.Size();
	while (index < dayLineSize) {
		weekLine.Reset();
		auto pDayLine = m_dataDayLine.GetData(index++);
		long lCurrentEndDate = GetNextMonday(pDayLine->GetDate());
		weekLine.SetDate(pDayLine->GetDate());
		weekLine.SetOpen(pDayLine->GetOpen());
		weekLine.SetLow(pDayLine->GetLow());
		do {
			if (pDayLine->GetHigh() > weekLine.GetHigh()) weekLine.SetHigh(pDayLine->GetHigh());
			if (pDayLine->GetLow() < weekLine.GetLow()) weekLine.SetLow(pDayLine->GetLow());
			weekLine.SetVolume(weekLine.GetVolume() + pDayLine->GetVolume());
			weekLine.SetAmount(weekLine.GetAmount() + pDayLine->GetAmount());
			weekLine.SetClose(pDayLine->GetClose());
			if (index < dayLineSize) pDayLine = m_dataDayLine.GetData(index);
			else break;
			if (pDayLine->GetDate() < lCurrentEndDate) index++;
			else break;
		} while (true);

		if (weekLine.GetClose() > 0) m_dataWeekLine.Add(weekLine); // 有数据才存储
	}

	m_dataWeekLine.SetDataLoaded(true);
	return true;
}
