#pragma once
#include "WebData.h"

#include"RTData.h"

class CNeteaseRTWebData final : public CWebData {
  friend class CRTData;

public:
  CNeteaseRTWebData();
  ~CNeteaseRTWebData();

  virtual bool GetWebData(void) override;

  virtual bool ReportStatus(long lNumberOfData) override;

  virtual void InquireNextWebData(void) override;
  virtual int GetInquiringStr(CString& strInquire, long lTotalNumber = 900, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;

private:
};
