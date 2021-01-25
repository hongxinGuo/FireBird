#pragma once

#include"VirtualWebInquiry.h"

class CCrweberIndexWebInquiry : public CVirtualWebInquiry
{
public:
  CCrweberIndexWebInquiry();
  virtual ~CCrweberIndexWebInquiry();

  virtual bool PrepareNextInquiringStr(void) override;
  virtual void StartReadingThread(void) override;
};

typedef shared_ptr<CCrweberIndexWebInquiry> CCrweberIndexWebInquiryPtr;
