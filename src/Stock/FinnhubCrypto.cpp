#include"pch.h"

#include"WorldMarket.h"
#include "FinnhubCrypto.h"
#include"ContainerCryptoDayLine.h"

#include<sqlpp23/sqlpp23.h>
#include"StockMarketSQLTable.h"

#include"dataBaseConnector.h"

using namespace std;

CFinnhubCrypto::CFinnhubCrypto() {
	m_pDayLines = make_unique<CContainerCryptoDayLine>();
}

void CFinnhubCrypto::SetCheckingDayLineStatus() {
	ASSERT(IsUpdateDayLine()); // 默认状态为日线数据需要更新
	// 不再更新日线数据比上上个交易日要新的股票。其他所有的股票都查询一遍，以防止出现新股票或者老的股票重新活跃起来。
	if (gl_pWorldMarket->GetLastTradeDate() <= GetDayLineEndDate()) {
		// 最新日线数据为今日或者上一个交易日的数据。
		SetUpdateDayLine(false); // 日线数据不需要更新
	}
}

string CFinnhubCrypto::GetFinnhubDayLineInquiryParam(time_t tCurrentTime) const {
	const time_t tStartTime = (tCurrentTime - static_cast<time_t>(365) * 24 * 3600);

	string sParam = std::format("{}&resolution=D&from={:Ld}&to={:Ld}", m_strSymbol, tStartTime, tCurrentTime);

	return sParam;
}

void CFinnhubCrypto::UpdateDayLine(const CDayLinesPtr& vDayLine) const {
	m_pDayLines->UpdateData(vDayLine);
}

void CFinnhubCrypto::UnloadDayLine() const {
	m_pDayLines->Unload();
}

size_t CFinnhubCrypto::GetDayLineSize() const noexcept {
	return m_pDayLines->Size();
}

void CFinnhubCrypto::UpdateDayLineStartEndDate() {
	chrono::local_days lStartDate = chrono::local_days{ chrono::days(0) }, lEndDate = chrono::local_days{ chrono::days(0) };
	const bool fSucceed = m_pDayLines->GetStartEndDate(lStartDate, lEndDate);
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

bool CFinnhubCrypto::HaveNewDayLineData() const {
	if (m_pDayLines->Empty()) return false;
	if (m_pDayLines->GetData(m_pDayLines->Size() - 1)->GetDate() > GetDayLineEndDate()) return true;
	return false;
}

void CFinnhubCrypto::UpdateDayLineDB() {
	if (IsDayLineDuplicated()) {
		DeleteDuplicatedDayLine();
	}
	SaveDayLineDB();
	UpdateDayLineStartEndDate();
	UnloadDayLine();
}

void CFinnhubCrypto::SaveDayLineDB() {
	m_pDayLines->SaveDB(GetSymbol());
}

bool CFinnhubCrypto::IsDayLineDuplicated() noexcept {
	if (m_pDayLines->Empty()) return false;
	if (m_pDayLines->GetData(0)->GetDate() > GetDayLineEndDate()) return false;
	return true;
}

void CFinnhubCrypto::DeleteDuplicatedDayLine() noexcept {
	ASSERT(!m_pDayLines->Empty());
	using namespace StockMarket;
	const auto& t = FinnhubCryptoDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	db(sqlpp::delete_from(t).where(t.Symbol == GetSymbol() && t.Date >= toFormattedDate(m_pDayLines->GetData(0)->GetDate())));
	tx.commit();
}
