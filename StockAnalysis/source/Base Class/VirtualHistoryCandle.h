////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 历史数据的基类,其对应的数据库类为CVirtualHistoryCandleBasic
// 纯虚类，不允许生成实例。
// 为了保证测试成功，故而没有声明为纯虚类。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"Accessory.h"
#include"VirtualSetHistoryCandleBasic.h"

#include<gsl/gsl>
using namespace gsl;

using namespace std;
#include<memory>

class CVirtualHistoryCandle : public CObject {
public:
	CVirtualHistoryCandle();
	~CVirtualHistoryCandle();
	void Reset(void); // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。

	virtual bool SaveHistoryCandle(not_null<CVirtualSetHistoryCandleBasic*> pSetHistoryCandle);
	virtual bool AppendHistoryCandle(not_null<CVirtualSetHistoryCandleBasic*> pSetHistoryCandle);
	virtual bool LoadHistoryCandle(not_null<CVirtualSetHistoryCandleBasic*> pSetHistoryCandle);

	virtual int GetRatio(void) const = 0; // 此函数应该声明为纯虚函数，但由于需要测试此基类，故而有执行体。感觉还是声明为纯虚函数为佳。

	void CalculateRSLogarithm1(double dRS);

public:
	double GetRS(void) const noexcept { return m_dRS; }
	void SetRS(double dValue) noexcept { m_dRS = dValue; }
	double GetRSIndex(void) const noexcept { return m_dRSIndex; }
	void SetRSIndex(double dValue) noexcept { m_dRSIndex = dValue; }
	double GetRSBackup(void) const noexcept { return m_dRSBackup; }
	void SetRSBackup(double dValue) noexcept { m_dRSBackup = dValue; }
	double GetRSLogarithm(void) const noexcept { return m_dRSLogarithm; }
	void SetRSLogarithm(double dValue) noexcept { m_dRSLogarithm = dValue; }

	long GetFormatedMarketDate(void) const noexcept { return m_lDate; }
	void SetDate(long lDate) noexcept { m_lDate = lDate; }
	time_t GetFormatedMarketTime(void) const noexcept { return m_time; }
	void SetTime(time_t t) noexcept { m_time = t; }
	CString GetExchange(void) const { return m_strExchange; }
	void SetExchange(CString strExchange) { m_strExchange = strExchange; }
	CString GetStockSymbol(void) const { return m_strStockSymbol; }
	void SetStockSymbol(CString str) { m_strStockSymbol = str; }
	CString GetDisplaySymbol(void) const { return m_strDisplaySymbol; }
	void SetDisplaySymbol(CString str) { m_strDisplaySymbol = str; }

	long GetLastClose(void) const noexcept { return m_lLastClose; }
	void SetLastClose(long lValue) noexcept { m_lLastClose = lValue; }
	long GetOpen(void) const noexcept { return m_lOpen; }
	void SetOpen(long lValue) noexcept { m_lOpen = lValue; }
	long GetHigh(void) const noexcept { return m_lHigh; }
	void SetHigh(long lValue) noexcept { m_lHigh = lValue; }
	long GetLow(void) const noexcept { return m_lLow; }
	void SetLow(const char* buffer) noexcept { m_lLow = static_cast<long>(atof(buffer)); }
	void SetLow(long lValue) noexcept { m_lLow = lValue; }
	long GetClose(void) const noexcept { return m_lClose; }
	void SetClose(const char* buffer) noexcept { m_lClose = static_cast<long>(atof(buffer)); }
	void SetClose(long lValue) noexcept { m_lClose = lValue; }
	double GetUpDown(void) const noexcept { return m_dUpDown; }
	void SetUpDown(const char* buffer) noexcept { m_dUpDown = atof(buffer); }
	void SetUpDown(double dValue) noexcept { m_dUpDown = dValue; }
	double GetUpDownRate(void) const noexcept { return m_dUpDownRate; }
	void SetUpDownRate(const char* buffer) noexcept { m_dUpDownRate = atof(buffer); }
	void SetUpDownRate(double dValue) noexcept { m_dUpDownRate = dValue; }
	double GetChangeHandRate(void) const noexcept { return m_dChangeHandRate; }
	void SetChangeHandRate(const char* buffer) noexcept { m_dChangeHandRate = atof(buffer); }
	void SetChangeHandRate(double dValue) noexcept { m_dChangeHandRate = dValue; }
	INT64 GetVolume(void) const noexcept { return m_llVolume; }
	void SetVolume(const char* buffer) noexcept { m_llVolume = static_cast<INT64>(atof(buffer)); }
	void SetVolume(INT64 llValue) noexcept { m_llVolume = llValue; }
	INT64 GetAmount(void) const noexcept { return m_llAmount; }
	void SetAmount(const char* buffer) noexcept { m_llAmount = static_cast<INT64>(atof(buffer)); }
	void SetAmount(INT64 llValue) noexcept { m_llAmount = llValue; }
	INT64 GetTotalValue(void) const noexcept { return m_llTotalValue; }
	void SetTotalValue(const char* buffer) noexcept { m_llTotalValue = static_cast<INT64>(atof(buffer)); }
	void SetTotalValue(INT64 llValue) noexcept { m_llTotalValue = llValue; }
	INT64 GetCurrentValue(void) const noexcept { return m_llCurrentValue; }
	void SetCurrentValue(const char* buffer) noexcept { m_llCurrentValue = static_cast<INT64>(atof(buffer)); }
	void SetCurrentValue(INT64 llValue) noexcept { m_llCurrentValue = llValue; }

	void Set3RS(double dValue) noexcept { m_d3RS = dValue; }
	double Get3RS(void) const noexcept { return m_d3RS; }
	void Set5RS(double dValue) noexcept { m_d5RS = dValue; }
	double Get5RS(void) const noexcept { return m_d5RS; }
	void Set10RS(double dValue) noexcept { m_d10RS = dValue; }
	double Get10RS(void) const noexcept { return m_d10RS; }
	void Set30RS(double dValue) noexcept { m_d30RS = dValue; }
	double Get30RS(void) const noexcept { return m_d30RS; }
	void Set60RS(double dValue) noexcept { m_d60RS = dValue; }
	double Get60RS(void) const noexcept { return m_d60RS; }
	void Set120RS(double dValue) noexcept { m_d120RS = dValue; }
	double Get120RS(void) const noexcept { return m_d120RS; }

protected:
	// need to save
	long m_lDate; // 类型(YYYYMMDD)
	time_t m_time;
	CString m_strExchange;
	CString m_strStockSymbol;
	CString m_strDisplaySymbol;

	// 在不同的市场中，以下数据会放大若干倍（百倍、千倍等）。取决于价格使用几个小数点
	long m_lLastClose; // 前收盘。单位：0.001元
	long m_lOpen; // 开盘价
	long m_lHigh; // 最高价
	long m_lLow; // 最低价
	long m_lClose; // 收盘价

	// 以下数值是实际值
	double m_dUpDown;	// 涨跌额
	double m_dUpDownRate;
	double m_dChangeHandRate;	// 换手率
	INT64 m_llVolume;	// 成交量,单位:股
	INT64	m_llAmount;	// 成交金额,单位:元/万元（大盘）
	INT64	m_llTotalValue;	// 总市值。单位：万元
	INT64 m_llCurrentValue; // 流通市值。单位：万元
	double m_dRS; // 相对强弱（最小为0， 最大为100）
	double m_dRSIndex; // 相对强弱（最小为-50， 最大为150）
	double m_dRSBackup; // 相对强弱（最小为0， 最大为100）
	double m_dRSLogarithm; // 相对强度的对数值（最小为0， 最大为100，m_dRSLogarithm = (log(m_dRS) - log(50)) * 50 / (log(100)-log(50)) )
													// 如果小于50， 则 m_dRSLogarithm = 100 - (log(100 - m_dRS) - log(50)) * 50 / (log(100)-log(50))

public:
	// don't need to save
	double m_d3RS;
	double m_d5RS;
	double m_d10RS;
	double m_d30RS;
	double m_d60RS;
	double m_d120RS;
};

typedef shared_ptr<CVirtualHistoryCandle> CVirtualHistoryCandlePtr;
