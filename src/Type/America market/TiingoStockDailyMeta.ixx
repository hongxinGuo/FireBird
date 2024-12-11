#include"pch.h"
export module FireBird.Type.TiingoStockDailyMeta;

using std::vector;
using std::shared_ptr;

export {
	class CTiingoStockDailyMeta final {
	public:
		CTiingoStockDailyMeta() {}
		~CTiingoStockDailyMeta() = default;

	public:
		CString m_strCode{ "" };
		CString m_strName{ "" };
		CString m_strDescription{ "" };
		CString m_strExchange{ "" };
		long m_lHistoryDayLineStartDate{ 19000101 };
		long m_lHistoryDayLineEndDate{ 19000101 };
	};

	using CTiingoStockDailyMetaPtr = shared_ptr<CTiingoStockDailyMeta>;
}