////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 历史数据的基类。
// 纯虚类，不允许生成实例。
// 为了保证测试成功，故而没有声明为纯虚类。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"Accessory.h"

using namespace std;
#include<memory>

class CVirtualHistoryData : public CObject {
public:
  CVirtualHistoryData();
  ~CVirtualHistoryData();
  void Reset(void); // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。

public:
  virtual int GetRatio(void) = 0; // 此函数应该声明为纯虚函数，但由于需要测试此基类，故而有执行体。感觉还是声明为纯虚函数为佳。
  virtual void SetRatio(int iRatio = 1) = 0; // 此函数需要继承类各自实现

  long GetFormatedMarketDate(void) noexcept { return m_lDate; }
  void SetDate(long lDate) noexcept { m_lDate = lDate; }
  time_t GetFormatedMarketTime(void) noexcept { return m_time; }
  void SetTime(time_t t) noexcept { m_time = t; }
  CString GetExchange(void) { return m_strExchange; }
  void SetExchange(CString strExchange) { m_strExchange = strExchange; }
  CString GetStockSymbol(void) { return m_strStockSymbol; }
  void SetStockSymbol(CString str) { m_strStockSymbol = str; }
  CString GetStockName(void) { return m_strStockName; }
  void SetStockName(CString str) { m_strStockName = str; }

  long GetLastClose(void) noexcept { return m_lLastClose; }
  void SetLastClose(long lValue) noexcept { m_lLastClose = lValue; }
  long GetOpen(void) noexcept { return m_lOpen; }
  void SetOpen(long lValue) noexcept { m_lOpen = lValue; }
  long GetHigh(void) noexcept { return m_lHigh; }
  void SetHigh(long lValue) noexcept { m_lHigh = lValue; }
  long GetLow(void) noexcept { return m_lLow; }
  void SetLow(const char* buffer) noexcept { m_lLow = static_cast<long>(atof(buffer)); }
  void SetLow(long lValue) noexcept { m_lLow = lValue; }
  long GetClose(void) noexcept { return m_lClose; }
  void SetClose(const char* buffer) noexcept { m_lClose = static_cast<long>(atof(buffer)); }
  void SetClose(long lValue) noexcept { m_lClose = lValue; }
  double GetUpDown(void) noexcept { return m_dUpDown; }
  void SetUpDown(const char* buffer) noexcept { m_dUpDown = atof(buffer); }
  void SetUpDown(double dValue) noexcept { m_dUpDown = dValue; }
  double GetUpDownRate(void) noexcept { return m_dUpDownRate; }
  void SetUpDownRate(const char* buffer) noexcept { m_dUpDownRate = atof(buffer); }
  void SetUpDownRate(double dValue) noexcept { m_dUpDownRate = dValue; }
  double GetChangeHandRate(void) noexcept { return m_dChangeHandRate; }
  void SetChangeHandRate(const char* buffer) noexcept { m_dChangeHandRate = atof(buffer); }
  void SetChangeHandRate(double dValue) noexcept { m_dChangeHandRate = dValue; }
  INT64 GetVolume(void) noexcept { return m_llVolume; }
  void SetVolume(const char* buffer) noexcept { m_llVolume = static_cast<INT64>(atof(buffer)); }
  void SetVolume(INT64 llValue) noexcept { m_llVolume = llValue; }
  INT64 GetAmount(void) noexcept { return m_llAmount; }
  void SetAmount(const char* buffer) noexcept { m_llAmount = static_cast<INT64>(atof(buffer)); }
  void SetAmount(INT64 llValue) noexcept { m_llAmount = llValue; }
  INT64 GetTotalValue(void) noexcept { return m_llTotalValue; }
  void SetTotalValue(const char* buffer) noexcept { m_llTotalValue = static_cast<INT64>(atof(buffer)); }
  void SetTotalValue(INT64 llValue) noexcept { m_llTotalValue = llValue; }
  INT64 GetCurrentValue(void) noexcept { return m_llCurrentValue; }
  void SetCurrentValue(const char* buffer) noexcept { m_llCurrentValue = static_cast<INT64>(atof(buffer)); }
  void SetCurrentValue(INT64 llValue) noexcept { m_llCurrentValue = llValue; }

protected:
  // need to save
  long m_lDate; // 类型(YYYYMMDD)
  time_t m_time;
  CString m_strExchange;
  CString m_strStockSymbol;
  CString m_strStockName;

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

private:
  // 此类的各继承类都需要有各自的Ratio，故而需要将此静态变量声明为private，不允许其继承类使用。
};

typedef shared_ptr<CVirtualHistoryData> CVittualHistoryDataPtr;
