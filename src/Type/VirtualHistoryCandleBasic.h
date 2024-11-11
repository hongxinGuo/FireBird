////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 历史数据的基类,其对应的数据库类为CVirtualHistoryCandleBasic
// 纯虚类，不允许生成实例。
// 为了保证测试成功，故而没有声明为纯虚类。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualSetHistoryCandleBasic.h"

#include<gsl/gsl>
using gsl::not_null;

#include<memory>

class CVirtualHistoryCandleBasic {
public:
	CVirtualHistoryCandleBasic();
	virtual ~CVirtualHistoryCandleBasic() = default;;

	void SaveBasicData(not_null<CVirtualSetHistoryCandleBasic*> pVirtualSetHistoryCandleBasic) const; // 存储基本数据
	void AppendBasicData(not_null<CVirtualSetHistoryCandleBasic*> pVirtualSetHistoryCandleBasic) const;
	void LoadBasicData(not_null<CVirtualSetHistoryCandleBasic*> pVirtualSetHistoryCandleBasic);

	virtual int GetRatio() const = 0;

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

	long GetMarketDate() const noexcept { return m_lDate; }
	void SetDate(const long lDate) noexcept { m_lDate = lDate; }
	time_t GetMarketTime() const noexcept { return m_time; }
	void SetTime(const time_t t) noexcept { m_time = t; }
	CString GetExchange() const { return m_strExchange; }
	void SetExchange(const CString& strExchange) { m_strExchange = strExchange; }
	CString GetStockSymbol() const { return m_strStockSymbol; }
	void SetStockSymbol(const CString& str) { m_strStockSymbol = str; }
	CString GetDisplaySymbol() const { return m_strDisplaySymbol; }
	void SetDisplaySymbol(const CString& str) { m_strDisplaySymbol = str; }

	long GetLastClose() const noexcept { return m_lLastClose; }
	void SetLastClose(const long lValue) noexcept { m_lLastClose = lValue; }
	long GetOpen() const noexcept { return m_lOpen; }
	void SetOpen(const long lValue) noexcept { m_lOpen = lValue; }
	long GetHigh() const noexcept { return m_lHigh; }
	void SetHigh(const long lValue) noexcept { m_lHigh = lValue; }
	long GetLow() const noexcept { return m_lLow; }
	void SetLow(const char* buffer) noexcept { m_lLow = static_cast<long>(atof(buffer)); }
	void SetLow(const long lValue) noexcept { m_lLow = lValue; }
	long GetClose() const noexcept { return m_lClose; }
	void SetClose(const char* buffer) noexcept { m_lClose = static_cast<long>(atof(buffer)); }
	void SetClose(const long lValue) noexcept { m_lClose = lValue; }
	double GetDividend() const noexcept { return m_dDividend; }
	void SetDividend(const char* buffer) noexcept { m_dDividend = atof(buffer); }
	void SetDividend(const double dValue) noexcept { m_dDividend = dValue; }
	double GetUpDown() const noexcept { return m_dUpDown; }
	void SetUpDown(const char* buffer) noexcept { m_dUpDown = atof(buffer); }
	void SetUpDown(const double dValue) noexcept { m_dUpDown = dValue; }
	double GetSplitFactor() const noexcept { return m_dSplitFactor; }
	void SetSplitFactor(const char* buffer) noexcept { m_dSplitFactor = atof(buffer); }
	void SetSplitFactor(const double dValue) noexcept { m_dSplitFactor = dValue; }
	double GetUpDownRate() const noexcept { return m_dUpDownRate; }
	void SetUpDownRate(const char* buffer) noexcept { m_dUpDownRate = atof(buffer); }
	void SetUpDownRate(const double dValue) noexcept { m_dUpDownRate = dValue; }
	double GetChangeHandRate() const noexcept { return m_dChangeHandRate; }
	void SetChangeHandRate(const char* buffer) noexcept { m_dChangeHandRate = atof(buffer); }
	void SetChangeHandRate(const double dValue) noexcept { m_dChangeHandRate = dValue; }
	INT64 GetVolume() const noexcept { return m_llVolume; }
	void SetVolume(const char* buffer) noexcept { m_llVolume = static_cast<INT64>(atof(buffer)); }
	void SetVolume(const INT64 llValue) noexcept { m_llVolume = llValue; }
	INT64 GetAmount() const noexcept { return m_llAmount; }
	void SetAmount(const char* buffer) noexcept { m_llAmount = static_cast<INT64>(atof(buffer)); }
	void SetAmount(const INT64 llValue) noexcept { m_llAmount = llValue; }
	INT64 GetTotalValue() const noexcept { return m_llTotalValue; }
	void SetTotalValue(const char* buffer) noexcept { m_llTotalValue = static_cast<INT64>(atof(buffer)); }
	void SetTotalValue(const INT64 llValue) noexcept { m_llTotalValue = llValue; }
	INT64 GetCurrentValue() const noexcept { return m_llCurrentValue; }
	void SetCurrentValue(const char* buffer) noexcept { m_llCurrentValue = static_cast<INT64>(atof(buffer)); }
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

public:
	// need to save
	long m_lDate{ 0 }; // 类型(YYYYMMDD)
	time_t m_time{ 0 };
	CString m_strExchange{ _T("") };
	CString m_strStockSymbol{ _T("") };
	CString m_strDisplaySymbol{ _T("") };

	// 在不同的市场中，以下数据会放大若干倍（百倍、千倍、万倍等）。取决于价格使用几个小数点
	long m_lLastClose{ 0 }; // 前收盘。
	long m_lOpen{ 0 }; // 开盘价
	long m_lHigh{ 0 }; // 最高价
	long m_lLow{ 0 }; // 最低价
	long m_lClose{ 0 }; // 收盘价

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

public:
	// don't need to save
	double m_d3RS{ 0 };
	double m_d5RS{ 0 };
	double m_d10RS{ 0 };
	double m_d30RS{ 0 };
	double m_d60RS{ 0 };
	double m_d120RS{ 0 };
};

using CVirtualHistoryCandleBasicPtr = shared_ptr<CVirtualHistoryCandleBasic>;
