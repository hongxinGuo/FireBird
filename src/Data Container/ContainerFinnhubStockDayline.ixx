#include"pch.h"
module;
export module FireBird.Container.FinnhubStockDayLine;

import FireBird.Container.VirtualHistoryCandleExtend;

export {
	class CContainerFinnhubStockDayLine final : public CVirtualContainerHistoryCandleExtend {
	public:
		CContainerFinnhubStockDayLine();
		CContainerFinnhubStockDayLine(const CContainerFinnhubStockDayLine& other) = delete;
		CContainerFinnhubStockDayLine(CContainerFinnhubStockDayLine&& other) noexcept = delete;
		CContainerFinnhubStockDayLine& operator=(const CContainerFinnhubStockDayLine& other) = delete;
		CContainerFinnhubStockDayLine& operator=(CContainerFinnhubStockDayLine&& other) noexcept = delete;
		~CContainerFinnhubStockDayLine() override = default;

		bool SaveDB(const CString& strStockSymbol) override;
		bool LoadDB(const CString& strStockSymbol) override;

		// ÌØÓÐº¯Êý
	};

	using CContainerFinnhubStockDayLinePtr = shared_ptr<CContainerFinnhubStockDayLine>;
}
