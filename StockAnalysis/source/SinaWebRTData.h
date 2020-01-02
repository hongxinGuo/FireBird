#ifndef __SINA_WEB_RT_DATA_H__
#define __SINA_WEB_RT_DATA_H__
#include "WebData.h"
#include"RTData.h"

class CSinaWebRTData final : public CWebData {
public:
  CSinaWebRTData();
  ~CSinaWebRTData();

  virtual void InquireNextWebData(void) override;
  virtual CString GetNextInquiringStr(long lTotalNumer = 900, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;
  virtual bool ReportStatus(long lNumberOfData) override;
};

#endif
