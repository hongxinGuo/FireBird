#ifndef __NETEASE_WEB_DAYLINE_DATA_H__
#define __NETEASE_WEB_DAYLINE_DATA_H__

#include "WebData.h"

class CNeteaseDayLineWebData final : public CWebData {
public:
  CNeteaseDayLineWebData();
  ~CNeteaseDayLineWebData();

  virtual void InquireNextWebData(void) override;
  virtual void StartReadingThread(void) override;

  void SetDownLoadingStockCode(CString strStockCode);
  CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }

private:
  CString m_strDownLoadingStockCode;
};

#endif
