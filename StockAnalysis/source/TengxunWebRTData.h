#pragma once

#include "WebData.h"

class CTengxunRTWebData final : public CWebData {
public:
  CTengxunRTWebData();
  ~CTengxunRTWebData();

  virtual void InquireNextWebData(void) override;
  virtual CString GetNextInquiringStr(long lTotalNubmer = 900, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;
  virtual bool ReportStatus(long lNumberOfData) override;
};
