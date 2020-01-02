#ifndef __TENGXUN_WEB_RT_DATA_H__
#define __TENGXUN_WEB_RT_DATA_H__
#include "WebData.h"

class CTengxunWebRTData final : public CWebData {
public:
  CTengxunWebRTData();
  ~CTengxunWebRTData();

  virtual void InquireNextWebData(void) override;
  virtual CString GetNextInquiringStr(long lTotalNubmer = 900, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;
  virtual bool ReportStatus(long lNumberOfData) override;
};

#endif
