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

protected:
  bool m_fTodayNewStock; // 本日新发现的股票
  bool m_fUpdateStockProfileDB; 

};

typedef shared_ptr<CVirtualStock> CVirtualStockPtr;
