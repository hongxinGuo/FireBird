#pragma once

#include"stdafx.h"

class CVirtualMarket : public CObject {
public:
  CVirtualMarket(void) : CObject() {}
  ~CVirtualMarket(void) {}
  virtual bool SchedulingTask(void) { ASSERT(0); return true; } // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��
};
