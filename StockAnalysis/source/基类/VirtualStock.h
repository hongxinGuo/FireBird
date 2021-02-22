#pragma once

#include"stdafx.h"
#include"globeSymbolDef.h"

using namespace std;
#include<memory>
#include<atomic>
#include<vector>

class CVirtualStock : public CObject {
public:
  CVirtualStock();
  ~CVirtualStock();
  virtual void Reset(void);

public:
  bool IsTodayNewStock(void) noexcept { return m_fTodayNewStock; }
  void SetTodayNewStock(bool fFlag) noexcept { m_fTodayNewStock = fFlag; }
  bool IsUpdateStockProfileDB(void) noexcept { return m_fUpdateStockProfileDB; }
  void SetUpdateStockProfileDB(bool fFlag) noexcept { m_fUpdateStockProfileDB = fFlag; }
  bool IsActive(void) noexcept { return m_fActive; }
  void SetActive(bool fFlag) noexcept { m_fActive = fFlag; }
  
  long GetDayLineStartDate(void) noexcept { return m_lDayLineStartDate; }
  void SetDayLineStartDate(long lDate) noexcept { m_lDayLineStartDate = lDate; }
  long GetDayLineEndDate(void) noexcept { return m_lDayLineEndDate; }
  void SetDayLineEndDate(long lDate) noexcept { m_lDayLineEndDate = lDate; }

  long GetIPOStatus(void) noexcept { return m_lIPOStatus; }
  void SetIPOStatus(long lValue) noexcept { m_lIPOStatus = lValue; }
  bool IsDelisted(void) noexcept { return (m_lIPOStatus == __STAKE_DELISTED__); }
  bool IsNullStock(void) noexcept { return (m_lIPOStatus == __STAKE_NULL__); }
  bool IsIPOed(void) noexcept { return(m_lIPOStatus == __STAKE_IPOED__); }
  bool IsNotChecked(void) noexcept { return(m_lIPOStatus == __STAKE_NOT_CHECKED__); }

protected:
  bool m_fTodayNewStock; // 本日新发现的股票
  bool m_fUpdateStockProfileDB; 
  bool m_fActive;	// 是否本日内有数据读入。由新浪实时行情处理函数和网易日线历史数据处理函数来设置。
  long m_lDayLineStartDate;	// 日线数据起始日。这个是处理日线历史数据时得到的起始交易日，
  long m_lDayLineEndDate;	// 日线数据更新日。这个是处理日线历史数据时得到的最新日，
  long m_lIPOStatus; // 通过网易历史日线查询，如果只有前缀信息而没有实际内容，可以确认没有实际交易。在这种情况下，新浪实时行情有数据，只是为零而已。默认情况下为已上市
               // 未上市（无效股票代码）为__STAKE_NULL__；正常为__STAKE_IPOED__；已通过IPO但尚未上市或退市为__STAKE_DELISTED；其他情况尚未出现，留待以后处理。

};

typedef shared_ptr<CVirtualStock> CVirtualStockPtr;
