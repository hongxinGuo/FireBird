#pragma once

#include"stdafx.h"
#include"pch.h"

#include"ChinaStock.h"

namespace Testing {
  class CMockChinaStock : public CChinaStock {
  public:
  };

  typedef shared_ptr<CMockChinaStock> CMockChinaStockPtr;
}
