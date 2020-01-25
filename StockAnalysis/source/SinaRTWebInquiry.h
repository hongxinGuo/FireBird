#pragma once

#include "VirtualWebInquiry.h"
#include"RTData.h"

class CSinaRTWebInquiry final : public CVirtualWebInquiry {
public:
  CSinaRTWebInquiry();
  ~CSinaRTWebInquiry();

  virtual void InquireNextWebData(void) override;
  virtual CString GetNextInquiringStr(long lTotalNumer = 900, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;
  virtual bool ReportStatus(long lNumberOfData) override;
};
