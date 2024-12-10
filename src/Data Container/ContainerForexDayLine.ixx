#include"pch.h"
module;
export module FireBird.Container.ForexDayLine;

import FireBird.Container.VirtualHistoryCandleExtend;

export {
	class CContainerForexDayLine final : public CVirtualContainerHistoryCandleExtend {
	public:
		CContainerForexDayLine();
		CContainerForexDayLine(const CContainerForexDayLine& other) = delete;
		CContainerForexDayLine(CContainerForexDayLine&& other) noexcept = delete;
		CContainerForexDayLine& operator=(const CContainerForexDayLine& other) = delete;
		CContainerForexDayLine& operator=(CContainerForexDayLine&& other) noexcept = delete;
		~CContainerForexDayLine() override = default;

		bool SaveDB(const CString& strCryptoSymbol) override;
		bool LoadDB(const CString& strCryptoSymbol) override;

		// ÌØÓÐº¯Êý
	};

	using CContainerForexDayLinePtr = shared_ptr<CContainerForexDayLine>;
}
