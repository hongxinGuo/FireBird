#pragma once

#include"VirtualWebInquiry.h"

class CCrweberIndexWebInquiry final : public CVirtualWebInquiry
{
public:
  CCrweberIndexWebInquiry();
  ~CCrweberIndexWebInquiry();

  virtual void InquireNextWebData(void) override;
  virtual void StartReadingThread(void) override;
};
