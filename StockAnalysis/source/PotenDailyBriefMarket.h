#pragma once

#pragma once

#include"stdafx.h"

#include"VirtualMarket.h"

class CPotenDailyBriefingMarket : public CVirtualMarket {
public:
  CPotenDailyBriefingMarket(void) : CVirtualMarket() {}
  ~CPotenDailyBriefingMarket(void) {}
  virtual bool SchedulingTask(void) override; // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��
};