#pragma once

#include"VirtualWebInquiry.h"

class CNeteaseRTWebInquiry : public CVirtualWebInquiry {
public:
  CNeteaseRTWebInquiry();
  virtual ~CNeteaseRTWebInquiry();

  virtual bool ReportStatus(long lNumberOfData) override;
  virtual bool PrepareNextInquiringStr(void) override;
  virtual CString GetNextInquiringMiddleStr(long lTotalNumber = 600, bool fCheckActiveStock = false) override;
  virtual void StartReadingThread(void) override;
};

typedef shared_ptr<CNeteaseRTWebInquiry> CNeteaseRTWebInquiryPtr;
