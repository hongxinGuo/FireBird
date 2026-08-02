module;
#include <afxwin.h>

export module FireBirdLib.IndicatorRSI;

import FireBirdLib.Container.HistoryCandle;
import FireBirdLib.HistoryCandle;

import std;
using std::vector;
using std::shared_ptr;

export {
	struct CRSI {
		double m_RSI6{ 0.0 };   // RSI value (0-100). 0 for indices before first full window.
		double m_RSI12{ 0.0 };  // RSI value (0-100). 0 for indices before first full window.
		double m_RSI24{ 0.0 };	// RSI value (0-100). 0 for indices before first full window.
	};

	namespace FireBird::Indicators {
		class CRSICalculator {
		public:
			// Compute RSI from a vector of candles.
			// - Uses the candle's GetClose() as the price (preserves same units as stored in the candles).
			// - period: lookback length (commonly 14).
			// Returns one CRSIResult per input candle; entries before the first full window have RSI==0.0.
			static std::vector<CRSI> Compute(const std::vector<CVirtualHistoryCandle>& candles, int period = 14);

			// Convenience overload for shared pointer to vector (existing codebase uses shared vectors)
			static std::vector<CRSI> Compute(const std::shared_ptr<std::vector<CVirtualHistoryCandle>>& candlesPtr, int period = 14);
		};
	} // namespace FireBird::Indicators

	class CIndicatorRSI {
	public:
		CIndicatorRSI() = default;
		CIndicatorRSI(int periodFast, int periodMiddle, int periodSlow) {
			m_PeriodFast = periodFast;
			m_PeriodMiddle = periodMiddle;
			m_PeriodSlow = periodSlow;
		}
		CIndicatorRSI(const CIndicatorRSI&) = delete;
		CIndicatorRSI& operator=(const CIndicatorRSI&) = delete;
		CIndicatorRSI(CIndicatorRSI&&) noexcept = delete;
		CIndicatorRSI& operator=(CIndicatorRSI&&) noexcept = delete;
		~CIndicatorRSI() = default;

		void SetCandle(shared_ptr<CVirtualDataHistoryCandle> pCandle) { m_pvCandle = pCandle; }
		void Calculate();
		void ToShow(CDC* pDC, CRect rectDrawArea, int iStepWidth);

	protected:
		static std::vector<double> Compute(const std::vector<CVirtualHistoryCandle>& candles, int period = 14);

		// Convenience overload for shared pointer to vector (existing codebase uses shared vectors)
		static std::vector<double> Compute(const std::shared_ptr<std::vector<CVirtualHistoryCandle>>& candlesPtr, int period = 14);

	protected:
		int m_PeriodFast{ 6 };
		int m_PeriodMiddle{ 12 };
		int m_PeriodSlow{ 24 };

		shared_ptr<CVirtualDataHistoryCandle> m_pvCandle{ nullptr };
		vector<CRSI> m_vRSI;
	};
}