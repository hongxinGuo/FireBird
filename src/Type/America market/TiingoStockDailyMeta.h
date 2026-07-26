#pragma once

using std::vector;
using std::shared_ptr;
using std::chrono::local_days;
using std::literals::chrono_literals::operator ""y;

class CTiingoStockDailyMeta final {
public:
	CTiingoStockDailyMeta() {}
	~CTiingoStockDailyMeta() = default;

public:
	string m_strCode{ "" };
	string m_strName{ "" };
	string m_strDescription{ "" };
	string m_strExchange{ "" };
	local_days m_lHistoryDayLineStartDate{ 1900y / 01 / 01 };
	local_days m_lHistoryDayLineEndDate{ 1900y / 01 / 01 };
};

using CTiingoStockDailyMetaPtr = shared_ptr<CTiingoStockDailyMeta>;
