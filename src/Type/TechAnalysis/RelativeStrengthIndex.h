#pragma once

struct RSIResult {
public:
		void Reset() { rsi = std::numeric_limits<double>::quiet_NaN(); }
		double Max() const { return rsi; }
		double Min() const { return rsi; }
		double rsi{ std::numeric_limits<double>::quiet_NaN() };
};

class CIndicatorRSI {
public:
		CIndicatorRSI(int period = 14) : period_(period) {}

		void reset();
		void SetCandle(CVirtualDataHistoryCandle* pCandle) { m_pvCandle = pCandle; }

		void Calculate(int period = 14);
		void Calculate2(int period = 14);

		void ToShow(CDC* pDC, CRect rectDrawArea, int iStepWidth);

private:
		void CalculateCurrentHighLow(CRect rectDrawArea, int iStepWidth);

private:
		int period_;
		CVirtualDataHistoryCandle* m_pvCandle{ nullptr };
		vector<double> m_vClose;
		vector<RSIResult> m_vRSI;

		double currentHigh_;
		double currentLow_;
		double currentDif_;
};
