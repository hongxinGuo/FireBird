#pragma once

#include"stdafx.h"

#include"VirtualMarket.h"

class CCrweberIndexMarket : public CVirtualMarket {
public:
  virtual bool SchedulingTask(void) override; // 由程序的定时器调度，大约每100毫秒一次
};
