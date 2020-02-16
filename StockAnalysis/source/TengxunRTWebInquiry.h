#pragma once

#include "VirtualWebInquiry.h"

class CTengxunRTWebInquiry final : public CVirtualWebInquiry {
public:
  CTengxunRTWebInquiry();
  ~CTengxunRTWebInquiry();

  virtual bool PrepareNextInquiringStr(void) override;
  virtual CString GetNextInquiringMiddleStr(long lTotalNubmer = 900, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;
  virtual bool ReportStatus(long lNumberOfData) override;
};
