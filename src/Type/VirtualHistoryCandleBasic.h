////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 历史数据的基类,其对应的数据库类为CVirtualHistoryCandleBasic
// 纯虚类，不允许生成实例。
// 为了保证测试成功，故而没有声明为纯虚类。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualSetHistoryCandleBasic.h"

#include<memory>

class CVirtualHistoryCandleBasic {
public:
	CVirtualHistoryCandleBasic() = default;
	virtual ~CVirtualHistoryCandleBasic() = default;

	void SaveBasicData(CVirtualSetHistoryCandleBasic* pVirtualSetHistoryCandleBasic) const; // 存储基本数据
	void AppendBasicData(CVirtualSetHistoryCandleBasic* pVirtualSetHistoryCandleBasic) const;
	void LoadBasicData(const CVirtualSetHistoryCandleBasic* pVirtualSetHistoryCandleBasic);
	virtual int GetRatio() const { return 1000; };

	void AdjustByFactor(double dFactor); // 按照复权因子调整数据

	void CalculateRSLogarithm1(double dRS);

	bool IsActive() const;

public:
	double GetRS() const noexcept { return m_dRS; }
	void SetRS(const double dValue) noexcept { m_dRS = dValue; }
	double GetRSIndex() const noexcept { return m_dRSIndex; }
	void SetRSIndex(const double dValue) noexcept { m_dRSIndex = dValue; }
	double GetRSBackup() const noexcept { return m_dRSBackup; }
	void SetRSBackup(const double dValue) noexcept { m_dRSBackup = dValue; }
	double GetRSLogarithm() const noexcept { return m_dRSLogarithm; }
	void SetRSLogarithm(const double dValue) noexcept { m_dRSLogarithm = dValue; }

	long GetDate() const noexcept { return m_lDate; }
	void SetDate(const long lDate) noexcept { m_lDate = lDate; }
	auto GetMarketTimePoint() const noexcept { return m_tpTime; }
	time_t GetMarketTime() const noexcept { return m_tpTime.time_since_epoch().count(); }
	void SetTime(std::chrono::sys_seconds time) noexcept { m_tpTime = time; }
	void SetTime(const time_t t) noexcept { m_tpTime = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(t)); }
	const string& GetExchange() const { return m_strExchange; }
	void SetExchange(const string& strExchange) { m_strExchange = strExchange; }
	const string& GetStockSymbol() const { return m_strStockSymbol; }
	void SetStockSymbol(const string& str) { m_strStockSymbol = str; }

	auto GetLastClose() const noexcept { return m_lLastClose; }
	void SetLastClose(const long long lValue) noexcept { m_lLastClose = lValue; }
	auto GetOpen() const noexcept { return m_lOpen; }
	void SetOpen(const long long lValue) noexcept { m_lOpen = lValue; }
	auto GetHigh() const noexcept { return m_lHigh; }
	void SetHigh(const long long lValue) noexcept { m_lHigh = lValue; }
	auto GetLow() const noexcept { return m_lLow; }
	void SetLow(const string& str) noexcept { m_lLow = static_cast<long>(strtod(str.c_str(), nullptr)); }
	void SetLow(const long long lValue) noexcept { m_lLow = lValue; }
	auto GetClose() const noexcept { return m_lClose; }
	void SetClose(const string& str) noexcept { m_lClose = static_cast<long>(strtod(str.c_str(), nullptr)); }
	void SetClose(const long long lValue) noexcept { m_lClose = lValue; }
	double GetDividend() const noexcept { return m_dDividend; }
	void SetDividend(const string& str) noexcept { m_dDividend = strtod(str.c_str(), nullptr); }
	void SetDividend(const double dValue) noexcept { m_dDividend = dValue; }
	double GetUpDown() const noexcept { return m_dUpDown; }
	void SetUpDown(const string& str) noexcept { m_dUpDown = strtod(str.c_str(), nullptr); }
	void SetUpDown(const double dValue) noexcept { m_dUpDown = dValue; }
	double GetSplitFactor() const noexcept { return m_dSplitFactor; }
	void SetSplitFactor(const string& str) noexcept { m_dSplitFactor = strtod(str.c_str(), nullptr); }

	void SetSplitFactor(const double dValue) noexcept { m_dSplitFactor = dValue; }
	double GetUpDownRate() const noexcept { return m_dUpDownRate; }
	void SetUpDownRate(const string& str) noexcept { m_dUpDownRate = strtod(str.c_str(), nullptr); }
	void SetUpDownRate(const double dValue) noexcept { m_dUpDownRate = dValue; }
	double GetChangeHandRate() const noexcept { return m_dChangeHandRate; }
	void SetChangeHandRate(const string& str) noexcept { m_dChangeHandRate = strtod(str.c_str(), nullptr); }
	void SetChangeHandRate(const double dValue) noexcept { m_dChangeHandRate = dValue; }
	INT64 GetVolume() const noexcept { return m_llVolume; }
	void SetVolume(const string& str) noexcept { m_llVolume = static_cast<INT64>(strtod(str.c_str(), nullptr)); }
	void SetVolume(const INT64 llValue) noexcept { m_llVolume = llValue; }
	INT64 GetAmount() const noexcept { return m_llAmount; }
	void SetAmount(const string& str) noexcept { m_llAmount = static_cast<INT64>(strtod(str.c_str(), nullptr)); }
	void SetAmount(const INT64 llValue) noexcept { m_llAmount = llValue; }
	INT64 GetTotalValue() const noexcept { return m_llTotalValue; }
	void SetTotalValue(const string& str) noexcept { m_llTotalValue = static_cast<INT64>(strtod(str.c_str(), nullptr)); }
	void SetTotalValue(const INT64 llValue) noexcept { m_llTotalValue = llValue; }
	INT64 GetCurrentValue() const noexcept { return m_llCurrentValue; }
	void SetCurrentValue(const string& str) noexcept { m_llCurrentValue = static_cast<INT64>(strtod(str.c_str(), nullptr)); }
	void SetCurrentValue(const INT64 llValue) noexcept { m_llCurrentValue = llValue; }

	void Set3RS(const double dValue) noexcept { m_d3RS = dValue; }
	double Get3RS() const noexcept { return m_d3RS; }
	void Set5RS(const double dValue) noexcept { m_d5RS = dValue; }
	double Get5RS() const noexcept { return m_d5RS; }
	void Set10RS(const double dValue) noexcept { m_d10RS = dValue; }
	double Get10RS() const noexcept { return m_d10RS; }
	void Set30RS(const double dValue) noexcept { m_d30RS = dValue; }
	double Get30RS() const noexcept { return m_d30RS; }
	void Set60RS(const double dValue) noexcept { m_d60RS = dValue; }
	double Get60RS() const noexcept { return m_d60RS; }
	void Set120RS(const double dValue) noexcept { m_d120RS = dValue; }
	double Get120RS() const noexcept { return m_d120RS; }

	void SetAverage(size_t length, long lValue) noexcept;
	long GetAverage(int length) const noexcept;

protected:
	// need to save
	long m_lDate{ 0 }; // 类型(YYYYMMDD)
	std::chrono::sys_seconds m_tpTime{ std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(0)) };
	string m_strExchange{};
	string m_strStockSymbol{};

	// 在不同的市场中，以下数据会放大若干倍（百倍、千倍、万倍等）。取决于价格使用几个小数点
	long long m_lLastClose{ 0 }; // 前收盘。
	long long m_lOpen{ 0 }; // 开盘价
	long long m_lHigh{ 0 }; // 最高价
	long long m_lLow{ 0 }; // 最低价
	long long m_lClose{ 0 }; // 收盘价

	// 以下数值是实际值
	double m_dDividend{ 0 }; // 股息
	double m_dSplitFactor{ 1.0 }; // 拆分比率
	double m_dUpDown{ 0 }; // 涨跌额
	double m_dUpDownRate{ 0 };
	double m_dChangeHandRate{ 0 }; // 换手率
	INT64 m_llVolume{ 0 }; // 成交量,单位:股
	INT64 m_llAmount{ 0 }; // 成交金额,单位:元/万元（大盘）
	INT64 m_llTotalValue{ 0 }; // 总市值。单位：万元
	INT64 m_llCurrentValue{ 0 }; // 流通市值。单位：万元
	double m_dRS{ 0 }; // 相对强弱（最小为0， 最大为100）
	double m_dRSIndex{ 0 }; // 相对强弱（最小为-50， 最大为150）
	double m_dRSBackup{ 0 }; // 相对强弱（最小为0， 最大为100）
	double m_dRSLogarithm{ 0 }; // 相对强度的对数值（最小为0， 最大为100，m_dRSLogarithm = (log(m_dRS) - log(50)) * 50 / (log(100)-log(50)) )
	// 如果小于50， 则 m_dRSLogarithm = 100 - (log(100 - m_dRS) - log(50)) * 50 / (log(100)-log(50))

protected:
	// don't need to save
	double m_d3RS{ 0 };
	double m_d5RS{ 0 };
	double m_d10RS{ 0 };
	double m_d30RS{ 0 };
	double m_d60RS{ 0 };
	double m_d120RS{ 0 };

	// 各类平均线
	long m_lAverage{ 0 };
	long m_l5Average{ 0 };
	long m_l10Average{ 0 };
	long m_l20Average{ 0 };
	long m_l30Average{ 0 };
	long m_l60Average{ 0 };
	long m_l120Average{ 0 };
	long m_l200Average{ 0 };
	long m_l240Average{ 0 };
};

using CVirtualHistoryCandleBasicPtr = shared_ptr<CVirtualHistoryCandleBasic>;
using CVirtualHistoryCandleBasicsPtr = shared_ptr<vector<CVirtualHistoryCandleBasicPtr>>;
