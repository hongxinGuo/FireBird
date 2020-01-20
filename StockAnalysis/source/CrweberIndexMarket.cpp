#include "CrweberIndexMarket.h"

CCrweberIndexMarket::CCrweberIndexMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("CrweberIndexMarket市场变量只允许存在一个实例\n");
    ASSERT(0);
  }
}

CCrweberIndexMarket::~CCrweberIndexMarket() {
}

bool CCrweberIndexMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask(); // 调用基类调度函数，完成共同任务

  return false;
}