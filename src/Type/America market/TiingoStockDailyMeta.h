#pragma once

using std::vector;
using std::shared_ptr;

class CTiingoStockDailyMeta final {
public:
	CTiingoStockDailyMeta() {}
	~CTiingoStockDailyMeta() = default;

public:
	string m_strCode{ "" };
	string m_strName{ "" };
	string m_strDescription{ "" };
	string m_strExchange{ "" };
	chrono::local_days m_lHistoryDayLineStartDate{ toLocalDays(19000101) };
	chrono::local_days m_lHistoryDayLineEndDate{ toLocalDays(19000101) };
};

using CTiingoStockDailyMetaPtr = shared_ptr<CTiingoStockDailyMeta>;
