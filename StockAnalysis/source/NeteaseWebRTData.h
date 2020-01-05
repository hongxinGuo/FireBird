#ifndef __NETEASE_WEB_RT_DATA_H__
#define __NETEASE_WEB_RT_DATA_H__
#include "WebData.h"

class CNeteaseRTWebData final : public CWebData {
public:
  CNeteaseRTWebData();
  ~CNeteaseRTWebData();

  virtual bool ReportStatus(long lNumberOfData) override;
  virtual void InquireNextWebData(void) override;
  virtual CString GetNextInquiringStr(long lTotalNumber = 600, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;
};

#endif
