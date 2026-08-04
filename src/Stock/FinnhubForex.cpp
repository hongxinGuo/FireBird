#include"pch.h"

#include"WorldMarket.h"
#include "FinnhubForex.h"

#include<sqlpp23/sqlpp23.h>
#include"StockMarketSQLTable.h"
#include"TimeConvert.h"

#include"dataBaseConnector.h"

using namespace std;

CFinnhubForex::CFinnhubForex() {
}

void CFinnhubForex::SetCheckingDayLineStatus() {
	ABSL_DCHECK(IsUpdateDayLine()); // 默认状态为日线数据需要更新
	// 不再更新日线数据比上个交易日要新的股票。其他所有的股票都查询一遍，以防止出现新股票或者老的股票重新活跃起来。
	if (gl_pWorldMarket->GetLastTradeDate() <= GetDayLineEndDate()) {
		// 最新日线数据为今日或者上一个交易日的数据。
		SetUpdateDayLine(false); // 日线数据不需要更新
	}
}

string CFinnhubForex::GetFinnhubDayLineInquiryParam(time_t tCurrentTime) {
	time_t tStartTime = gl_pWorldMarket->ConvertToUTCTime(toFormattedDate(GetDayLineEndDate()), 150000).time_since_epoch().count();
	tStartTime = max(tStartTime, tCurrentTime - static_cast<time_t>(365) * 24 * 3600);// 免费账户只能读取一年以内的日线数据。

	string sParam = std::format("{}&resolution=D&from={:Ld}&to={:Ld}", m_strSymbol, tStartTime, tCurrentTime);

	return sParam;
}

void CFinnhubForex::UpdateDayLine(const CDayLinesPtr& vDayLine) {
	m_dataDayLines.UpdateData(vDayLine);
}

void CFinnhubForex::UnloadDayLine() {
	m_dataDayLines.Unload();
}

size_t CFinnhubForex::GetDayLineSize() const noexcept {
	return m_dataDayLines.Size();
}

void CFinnhubForex::UpdateDayLineDB() {
	if (IsDayLineDuplicated()) {
		DeleteDuplicatedDayLine();
	}
	SaveDayLineDB();
	UpdateDayLineStartEndDate();
	UnloadDayLine();
}

void CFinnhubForex::SaveDayLineDB() {
	m_dataDayLines.SaveDB(GetSymbol());
}

bool CFinnhubForex::IsDayLineDuplicated() noexcept {
	if (m_dataDayLines.Empty()) return false;
	if (m_dataDayLines.GetData(0)->GetDate() > GetDayLineEndDate()) return false;
	return true;
}

void CFinnhubForex::DeleteDuplicatedDayLine() noexcept {
	ABSL_DCHECK(!m_dataDayLines.Empty());
	using namespace StockMarket;
	const auto& t = FinnhubForexDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	db(sqlpp::delete_from(t).where(t.Symbol == GetSymbol() && t.Date >= toFormattedDate(m_dataDayLines.GetData(0)->GetDate())));
	tx.commit();
}

void CFinnhubForex::UpdateDayLineStartEndDate() {
	chrono::local_days lStartDate = chrono::local_days{ chrono::days(0) }, lEndDate = chrono::local_days{ chrono::days(0) };
	const bool fSucceed = m_dataDayLines.GetStartEndDate(lStartDate, lEndDate);
	if (!fSucceed) {
		SetDayLineStartDate(toLocalDays(29900101));
		SetDayLineEndDate(toLocalDays(19800101));
	}
	else {
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

bool CFinnhubForex::HaveNewDayLineData() {
	if (m_dataDayLines.Empty()) return false;
	if (m_dataDayLines.GetData(m_dataDayLines.Size() - 1)->GetDate() > GetDayLineEndDate()) return true;
	return false;
}
