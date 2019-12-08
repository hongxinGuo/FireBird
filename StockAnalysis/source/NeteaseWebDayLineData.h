#pragma once

#include "WebData.h"

class CNeteaseWebDayLineData final : public CWebData {
public:
  CNeteaseWebDayLineData();
  ~CNeteaseWebDayLineData();

  virtual bool GetWebData(void) override;

  virtual void InquireNextWebData(void) override;
  virtual int GetInquiringStr(CString& strInquire, long lTotalNumber = 900, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;

  void SetDownLoadingStockCode(CString strStockCode);
  CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }

private:
  CString m_strDownLoadingStockCode;
  bool m_fNeedProcessingCurrentWebData;
};
