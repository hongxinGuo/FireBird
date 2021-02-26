#pragma once

#include "VirtualWebInquiry.h"

class CTengxunRTWebInquiry : public CVirtualWebInquiry {
public:
  CTengxunRTWebInquiry();
  virtual ~CTengxunRTWebInquiry();

  virtual bool PrepareNextInquiringStr(void) override;
  virtual CString GetNextInquiringMiddleStr(long lTotalNubmer = 900, bool fCheckActiveStock = false) override;
  virtual void StartReadingThread(void) override;
  virtual bool ReportStatus(long lNumberOfData) override;
};

typedef shared_ptr<CTengxunRTWebInquiry> CTengxunRTWebInquiryPtr;
