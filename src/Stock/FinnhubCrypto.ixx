module;

export module FireBirdLib.Stock.FinnhubCrypto;

import FireBirdLib.Stock;

import FireBirdLib.Container.HistoryCandle.CryptoDayLine;
import HistoryCandle.DayLine;

import std;
using std::unique_ptr;
using std::shared_ptr;
using std::vector;
using std::string;
using std::time_t;

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

		void SetCheckingDayLineStatus();

		string GetFinnhubDayLineInquiryParam(time_t tCurrentTime) const;

		void UpdateDayLine(const shared_ptr<vector<CDayLine>>& vDayLine) const;
		void UnloadDayLine() const;
		size_t GetDayLineSize() const noexcept;
		void UpdateDayLineStartEndDate();
		bool HaveNewDayLineData() const; //Todo: 移至VirtualStock中，合并其他股票类型的同名函数

		void UpdateDayLineDB();
		virtual void SaveDayLineDB();
		bool IsDayLineDuplicated() noexcept final;
		void DeleteDuplicatedDayLine() noexcept final;

	public:
		unique_ptr<CContainerCryptoDayLine> m_pDayLines;
	};

	typedef shared_ptr<CFinnhubCrypto> CFinnhubCryptoPtr;
	typedef shared_ptr<vector<CFinnhubCryptoPtr>> CFinnhubCryptosPtr;
}