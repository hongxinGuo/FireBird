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
	long m_lHistoryDayLineStartDate{ 19000101 };
	long m_lHistoryDayLineEndDate{ 19000101 };
};

using CTiingoStockDailyMetaPtr = shared_ptr<CTiingoStockDailyMeta>;
