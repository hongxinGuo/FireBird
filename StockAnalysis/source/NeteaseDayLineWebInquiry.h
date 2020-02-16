#pragma once

#include "VirtualWebInquiry.h"

class CNeteaseDayLineWebInquiry final : public CVirtualWebInquiry {
public:
  CNeteaseDayLineWebInquiry();
  ~CNeteaseDayLineWebInquiry();

  virtual bool PrepareNextInquiringStr(void) override;
  virtual void StartReadingThread(void) override;

  void SetDownLoadingStockCode(CString strStockCode);
  CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }

private:
  CString m_strDownLoadingStockCode;
};
