#pragma once

#include"WebData.h"

class CCrweberIndexWebData final : public CWebData
{
public:
  CCrweberIndexWebData();
  ~CCrweberIndexWebData();

  virtual void InquireNextWebData(void) override;
  virtual void StartReadingThread(void) override;
};
