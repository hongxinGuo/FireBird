#pragma once

struct MACDResult {
public:
	void Reset() {
		macd = std::numeric_limits<double>::quiet_NaN();
		signal = std::numeric_limits<double>::quiet_NaN();
		hist = std::numeric_limits<double>::quiet_NaN();
	}

	double Max();
	double Min();
	double macd;    // MACD line (fastEMA - slowEMA) 
	double signal;  // signal line (EMA of MACD) 
	double hist;    // histogram = macd - signal };
};

struct EMA {
	EMA() : period(0), alpha(0.0), initialized(false), value(std::numeric_limits<double>::quiet_NaN()) {}
	EMA(int p) { init(p); }

	void init(int p) {
		period = p;
		alpha = 2.0 / (period + 1.0);
		initialized = false;
		value = std::numeric_limits<double>::quiet_NaN();
	}
	void reset() {
		initialized = false;
		value = std::numeric_limits<double>::quiet_NaN();
	}

	double update(double x) {
		if (!initialized) {
			value = x;
			initialized = true;
		}
		else { value = alpha * x + (1.0 - alpha) * value; }
		return value;
	}
	int period;
	double alpha;
	bool initialized;
	double value;
};

class CIndicatorMACD {
public:
	CIndicatorMACD(int fastPeriod = 12, int slowPeriod = 26, int signalPeriod = 9)
		: fastPeriod_(fastPeriod), slowPeriod_(slowPeriod), signalPeriod_(signalPeriod),
		  fastEMA_(fastPeriod), slowEMA_(slowPeriod), signalEMA_(signalPeriod) {
	}

	void reset();
	void SetCandle(CVirtualDataHistoryCandle* pCandle) { m_pvCandle = pCandle; }

	// Batch compute: returns vector of Result aligned with input prices; entries before availability are NaN.
	void Calculate(int fastPeriod = 12, int slowPeriod = 26, int signalPeriod = 9);

	void ToShow(CDC* pDC, CRect rectDrawArea, int iStepWidth);

private:
	void CalculateCurrentHighLow(CRect rectDrawArea, int iStepWidth);

private:
	int fastPeriod_;
	int slowPeriod_;
	int signalPeriod_;

	EMA fastEMA_;
	EMA slowEMA_;
	EMA signalEMA_;

	double currentHigh_;
	double currentLow_;
	double currentDif_;

	CVirtualDataHistoryCandle* m_pvCandle{ nullptr };
	vector<MACDResult> m_vMACD;
};
