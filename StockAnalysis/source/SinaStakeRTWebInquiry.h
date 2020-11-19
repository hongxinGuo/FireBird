#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

class CSinaStakeRTWebInquiry : public CVirtualWebInquiry {
public:
  CSinaStakeRTWebInquiry();
  virtual ~CSinaStakeRTWebInquiry();

  virtual bool PrepareNextInquiringStr(void) override;
  virtual CString GetNextInquiringMiddleStr(long lTotalNumer = 900, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;
  virtual bool ReportStatus(long lNumberOfData) override;
};

typedef shared_ptr<CSinaStakeRTWebInquiry> CSinaStakeRTWebInquiryPtr;
