#pragma once

#include"stdafx.h"

class CVirtualMarket : public CObject {
public:
  CVirtualMarket(void) : CObject() {}
  ~CVirtualMarket(void) {}
  virtual bool SchedulingTask(void) { ASSERT(0); return true; } // 由程序的定时器调度，大约每100毫秒一次
};
