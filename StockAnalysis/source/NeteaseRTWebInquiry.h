#pragma once

#include"VirtualWebInquiry.h"

class CNeteaseRTWebInquiry final : public CVirtualWebInquiry {
public:
  CNeteaseRTWebInquiry();
  ~CNeteaseRTWebInquiry();

  virtual bool ReportStatus(long lNumberOfData) override;
  virtual void InquireNextWebData(void) override;
  virtual CString GetNextInquiringStr(long lTotalNumber = 600, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;
};
