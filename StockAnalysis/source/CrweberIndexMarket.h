#pragma once

#include"stdafx.h"

#include"VirtualMarket.h"

class CCrweberIndexMarket : public CVirtualMarket {
public:
  virtual bool SchedulingTask(void) override; // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��
  virtual void ResetMarket(void) override { Reset(); }
  void Reset(void) {}
};
