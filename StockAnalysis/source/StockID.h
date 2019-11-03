#pragma once

#include"stdafx.h"

using namespace std;
#include<memory>

class CStockID final {
public:
  CStockID();
  ~CStockID();
  void Reset(void);

protected:
};

typedef shared_ptr<CStockID> StockIDPtr;
