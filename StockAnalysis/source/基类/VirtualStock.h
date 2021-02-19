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
  void Reset(void);

public:
  bool IsTodayNewStock(void) noexcept { return m_fTodayNewStock; }
  void SetTodayNewStock(bool fFlag) noexcept { m_fTodayNewStock = fFlag; }
  bool IsUpdateStockProfileDB(void) noexcept { return m_fUpdateStockProfileDB; }
  void SetUpdateStockProfileDB(bool fFlag) noexcept { m_fUpdateStockProfileDB = fFlag; }
  bool IsActive(void) noexcept { return m_fActive; }
  void SetActive(bool fFlag) noexcept { m_fActive = fFlag; }

protected:
  bool m_fTodayNewStock; // 本日新发现的股票
  bool m_fUpdateStockProfileDB; 
  bool m_fActive;	// 是否本日内有数据读入。由新浪实时行情处理函数和网易日线历史数据处理函数来设置。

};

typedef shared_ptr<CVirtualStock> CVirtualStockPtr;
