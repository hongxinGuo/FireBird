#pragma once

#include"Accessory.h"
#include"RTData.h"
#include"SetDayLineToday.h"

using namespace std;
#include<array>

class CStockBasicInfo
{
public:
  CStockBasicInfo();
  ~CStockBasicInfo();
  void Reset(void);

  // 数据库操作
  void StoreBasicInfo(CSetDayLine& psetDayLine);
  void StoreTempInfo(CSetDayLineToday& setDayLineToday);

  void UpdateStatus(CRTDataPtr pRTData);

  WORD GetMarket(void) noexcept { return m_wMarket; }
  void SetMarket(WORD wValue) noexcept { m_wMarket = wValue; }
  CString GetStockCode(void) { return m_strStockCode; }
  void SetStockCode(CString str) noexcept { m_strStockCode = str; }
  CString GetStockName(void) noexcept { return m_strStockName; }
  void SetStockName(CString str) noexcept { m_strStockName = str; }

  long GetOffset(void) noexcept { return m_nOffsetInContainer; }
  void SetOffset(long lValue) noexcept { m_nOffsetInContainer = lValue; }
  long GetDayLineStartDay(void) noexcept { return m_lDayLineStartDay; }
  void SetDayLineStartDay(long lDay) noexcept { m_lDayLineStartDay = lDay; }
  long GetDayLineEndDay(void) noexcept { return m_lDayLineEndDay; }
  void SetDayLineEndDay(long lDay) noexcept { m_lDayLineEndDay = lDay; }
  long GetIPOStatus(void) noexcept { return m_lIPOed; }
  void SetIPOStatus(long lValue) noexcept { m_lIPOed = lValue; }

  // 基本实时数据，需要更新
  time_t GetTransactionTime(void) noexcept { return m_TransactionTime; }
  void SetTransactionTime(time_t time) noexcept { m_TransactionTime = time; }
  long GetLastClose(void) noexcept { return m_lLastClose; }
  void SetLastClose(long lValue) noexcept { m_lLastClose = lValue; }
  long GetOpen(void) noexcept { return m_lOpen; }
  void SetOpen(long lValue) noexcept { m_lOpen = lValue; }
  long GetHigh(void) noexcept { return m_lHigh; }
  void SetHigh(long lValue) noexcept { m_lHigh = lValue; }
  long GetLow(void) noexcept { return m_lLow; }
  void SetLow(long lValue) noexcept { m_lLow = lValue; }
  long GetNew(void) noexcept { return m_lNew; }
  void SetNew(long lValue) noexcept { m_lNew = lValue; }
  INT64 GetAmount(void) noexcept { return m_llAmount; }
  void SetAmount(INT64 llValue) noexcept { m_llAmount = llValue; }
  INT64 GetVolume(void) noexcept { return m_llVolume; }
  void SetVolume(INT64 llValue) noexcept { m_llVolume = llValue; }
  long GetUpDown(void) noexcept { return m_lUpDown; }
  void SetUpDown(long lValue) noexcept { m_lUpDown = lValue; }
  double GetUpDownRate(void) noexcept { return m_dUpDownRate; }
  void SetUpDownRate(double dValue) { m_dUpDownRate = dValue; }
  double GetChangeHandRate(void) noexcept { return m_dChangeHandRate; }
  void SetChangeHandRate(double dValue) noexcept { m_dChangeHandRate = dValue; }
  void SetTotalValue(INT64 llValue) noexcept { m_llTotalValue = llValue; }
  INT64 GetTotalValue(void) noexcept { return m_llTotalValue; }
  void SetCurrentValue(INT64 llValue) noexcept { m_llCurrentValue = llValue; }
  INT64 GetCurrentValue(void) noexcept { return m_llCurrentValue; }
  long GetPBuy(int iIndex) { return m_lPBuy.at(iIndex); }
  long GetVBuy(int iIndex) { return m_lVBuy.at(iIndex); }
  long GetPSell(int iIndex) { return m_lPSell.at(iIndex); }
  long GetVSell(int iIndex) { return m_lVSell.at(iIndex); }
  void SetPBuy(int iIndex, long value) { m_lPBuy.at(iIndex) = value; }
  void SetVBuy(int iIndex, long value) { m_lVBuy.at(iIndex) = value; }
  void SetPSell(int iIndex, long value) { m_lPSell.at(iIndex) = value; }
  void SetVSell(int iIndex, long value) { m_lVSell.at(iIndex) = value; }
  double GetRelativeStrong(void) { return m_dRelativeStrong; }
  void SetRelativeStrong(double value) { m_dRelativeStrong = value; }

protected:
  WORD m_wMarket;	// 1：上海市场（不区分细类）；2：深圳市场（不区分细类）；3：上海指数；4：深圳指数；：上海三版；6：深圳中小板；
                  // 7：上海B股；8：深圳B股；9：上海科创版; 10：深圳创业板；
                  // 上海市场采用单数标示，深圳市场采用双数标示。目前暂时不用，先查看涉及到哪些函数需要修改。
  CString m_strStockCode; // 股票代码。八位，前两位为市场前缀，后六位为数字代码。如sh600601，sz000001
  CString m_strStockName; // 股票名称
  CStringW m_strStockNameReadIn; // 读入的股票名称（UniCode制式，目前暂未使用）
  long m_nOffsetInContainer;	// 在容器中的偏移量
  long m_lDayLineStartDay;	// 日线数据起始日。这个是处理日线历史数据时得到的起始交易日，
  long m_lDayLineEndDay;	// 日线数据更新日。这个是处理日线历史数据时得到的最新日，
  long m_lIPOed; // 通过网易历史日线查询，如果只有前缀信息而没有实际内容，可以确认没有实际交易。在这种情况下，新浪实时行情有数据，只是为零而已。默认情况下为已上市
                 // 未上市（无效股票代码）为__STOCK_NULL__；正常为__STOCK_IPOED__；已通过IPO但尚未上市或退市为__STOCK_DELISTED；其他情况尚未出现，留待以后处理。
  short	m_nHand;	// 每手股数

  // 实时数据,需要不断更新
  time_t m_TransactionTime; // 实时数据交易时间
  long m_lLastClose; // 以0.001元计的收盘价
  long m_lOpen; // 以0.001元计的开盘价
  long m_lHigh; // 以0.001元计的最高价
  long m_lLow; // 以0.001元计的最低价
  long m_lNew; // 以0.001元计的最新价
  long m_lUpDown; // 涨跌值
  double m_dUpDownRate; // 涨跌率
  INT64 m_llVolume;	// 以1股计的成交量
  INT64 m_llAmount; // 以元计的成交金额
  double m_dChangeHandRate; // 换手率
  INT64 m_llTotalValue;	// 总市值。单位：万元
  INT64 m_llCurrentValue;	// 流通市值。单位：万元
  array<long, 5> m_lPBuy;	// 买盘价。单位：0.001元
  array<long, 5> m_lVBuy;	// 买盘量。单位：股
  array<long, 5> m_lPSell; // 卖盘价。单位：0.001元
  array<long, 5> m_lVSell; // 卖盘量。单位：股
  double m_dRelativeStrong; // 单位：1%
};
