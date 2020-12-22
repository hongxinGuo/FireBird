#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

class CTiingoWebInquiry : public CVirtualWebInquiry {
public:
  CTiingoWebInquiry();
  virtual ~CTiingoWebInquiry();

  virtual bool PrepareNextInquiringStr(void) override;
  virtual CString GetNextInquiringMiddleStr(long lTotalNumer = 1, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;
  virtual bool ReportStatus(long lNumberOfData) override;
};

typedef shared_ptr<CTiingoWebInquiry> CTiingoWebInquiryPtr;
