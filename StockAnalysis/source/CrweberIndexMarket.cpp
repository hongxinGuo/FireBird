#include "CrweberIndexMarket.h"

CCrweberIndexMarket::CCrweberIndexMarket() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("CrweberIndexMarket�г�����ֻ�������һ��ʵ��\n");
    ASSERT(0);
  }
}

CCrweberIndexMarket::~CCrweberIndexMarket() {
}

bool CCrweberIndexMarket::SchedulingTask(void) {
  CVirtualMarket::SchedulingTask(); // ���û�����Ⱥ�������ɹ�ͬ����

  return false;
}