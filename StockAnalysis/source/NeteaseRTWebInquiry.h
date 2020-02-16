#pragma once

#include"VirtualWebInquiry.h"

class CNeteaseRTWebInquiry final : public CVirtualWebInquiry {
public:
  CNeteaseRTWebInquiry();
  ~CNeteaseRTWebInquiry();

  virtual bool ReportStatus(long lNumberOfData) override;
  virtual bool PrepareNextInquiringStr(void) override;
  virtual CString GetNextInquiringMiddleStr(long lTotalNumber = 600, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;
};
