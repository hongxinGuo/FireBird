#include"pch.h"
module;
#include"nlohmannjsonHeader.h"
#include<memory>
#include<vector>
export module FireBird.Stock.FinnhubCrypto;

import FireBird.Stock.Virtual;
import FireBird.Container.CryptoDayLine;
import FireBird.HistoryCandle.DayLine;

export {
	class CFinnhubCrypto : public CVirtualStock {
	public:
		CFinnhubCrypto();
		// 不允许赋值。
		CFinnhubCrypto(const CFinnhubCrypto&) = delete;
		CFinnhubCrypto& operator=(const CFinnhubCrypto&) = delete;
		CFinnhubCrypto(const CFinnhubCrypto&&) noexcept = delete;
		CFinnhubCrypto& operator=(const CFinnhubCrypto&&) noexcept = delete;

		int GetRatio() const override { return 1000; } // 比例为1000

		virtual bool UpdateDayLineDB() { return m_dataDayLine.SaveDB(m_strSymbol); }

		void SetCheckingDayLineStatus();

		CString GetFinnhubDayLineInquiryParam(time_t tCurrentTime) const;

		void UpdateDayLine(const vector<CDayLinePtr>& vDayLine) { m_dataDayLine.UpdateData(vDayLine); }
		void UnloadDayLine() { m_dataDayLine.Unload(); }
		size_t GetDayLineSize() const noexcept { return m_dataDayLine.Size(); }
		void UpdateDayLineStartEndDate();
		bool HaveNewDayLineData();

	public:
		CContainerCryptoDayLine m_dataDayLine;
	};

	using CFinnhubCryptoPtr = std::shared_ptr<CFinnhubCrypto>;
	using CFinnhubCryptosPtr = std::shared_ptr<vector<CFinnhubCryptoPtr>>;
}
