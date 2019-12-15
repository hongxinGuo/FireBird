#ifndef __NETEASE_WEB_RT_DATA_H__
#define __NETEASE_WEB_RT_DATA_H__
#include "WebData.h"

class CNeteaseWebRTData final : public CWebData {
public:
  CNeteaseWebRTData();
  ~CNeteaseWebRTData();

  virtual bool GetWebData(void) override;
  virtual bool ReportStatus(long lNumberOfData) override;
  virtual void InquireNextWebData(void) override;
  virtual int GetInquiringStr(CString& strInquire, long lTotalNumber = 600, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;
};

#endif
