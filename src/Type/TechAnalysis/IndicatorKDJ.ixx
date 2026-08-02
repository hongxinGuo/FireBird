module;

#include <afxwin.h>
export module FireBirdLib.IndicatorKDJ;

import FireBirdLib.Container.HistoryCandle;

import std;
using std::vector;
using std::shared_ptr;

export {
	struct Kdj {
	public:
		double m_RSV{ 0.0 };
		double m_K{ 0.0 };
		double m_D{ 0.0 };
		double m_J{ 0.0 };
	};

	class CIndicatorKDJ {
	public:
		CIndicatorKDJ() = default;
		CIndicatorKDJ(int period) { m_Period = period; }
		CIndicatorKDJ(const CIndicatorKDJ&) = delete;
		CIndicatorKDJ& operator=(const CIndicatorKDJ&) = delete;
		CIndicatorKDJ(CIndicatorKDJ&&) noexcept = delete;
		CIndicatorKDJ& operator=(CIndicatorKDJ&&) noexcept = delete;
		~CIndicatorKDJ() = default;

		void SetCandle(shared_ptr<CVirtualDataHistoryCandle> pCandle) { m_pvCandle = pCandle; }
		void Calculate(); //计算KDJ指标

		void ToShow(CDC* pDC, CRect rectDrawArea, int iStepWidth);

	protected:
		int m_Period{ 9 }; // 默认九天为一个周期

		shared_ptr<CVirtualDataHistoryCandle> m_pvCandle{ nullptr };
		vector<Kdj> m_vKDJ;
	};
}