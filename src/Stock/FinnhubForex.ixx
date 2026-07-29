module;

export module FinnhubForex;

import VirtualStock;
import ContainerForexDayLine;
import DayLine;

import std;
using std::unique_ptr;
using std::shared_ptr;
using std::vector;


export {
	class CFinnhubForex : public CVirtualStock {
	public:
		CFinnhubForex();
		// 不允许赋值。
		CFinnhubForex(const CFinnhubForex&) = delete;
		CFinnhubForex& operator=(const CFinnhubForex&) = delete;
		CFinnhubForex(const CFinnhubForex&&) noexcept = delete;
		CFinnhubForex& operator=(const CFinnhubForex&&) noexcept = delete;
		~CFinnhubForex() override = default;

		int GetRatio() const final { return 1000; }

		void UpdateDayLineDB();
		virtual void SaveDayLineDB();
		bool IsDayLineDuplicated() noexcept final;
		void DeleteDuplicatedDayLine() noexcept final;

		void SetCheckingDayLineStatus();

		string GetFinnhubDayLineInquiryParam(time_t tCurrentTime);

		void UpdateDayLine(const shared_ptr<vector<CDayLine>>& vDayLine);
		void UnloadDayLine();
		size_t GetDayLineSize() const noexcept;
		void UpdateDayLineStartEndDate();
		bool HaveNewDayLineData(); //Todo: 移至VirtualStock中，合并其他股票类型的同名函数

		CContainerForexDayLine m_dataDayLines;
	};

	using CForexSymbolPtr = shared_ptr<CFinnhubForex>;
	using CForexSymbolsPtr = shared_ptr<vector<CForexSymbolPtr>>;
}