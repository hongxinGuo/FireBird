#pragma once

#include "VirtualWebInquiry.h"

class CNeteaseDayLineWebInquiry : public CVirtualWebInquiry {
public:
  CNeteaseDayLineWebInquiry();
  virtual ~CNeteaseDayLineWebInquiry();

  virtual bool PrepareNextInquiringStr(void) override;
  virtual void StartReadingThread(void) override;

  void SetDownLoadingStockCode(CString strStockCode);
  CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }
  void ResetDownLoadingStockCode(void) noexcept { m_strDownLoadingStockCode = _T(""); }

private:
  CString m_strDownLoadingStockCode;
};

typedef shared_ptr<CNeteaseDayLineWebInquiry> CNeteaseDayLineWebInquiryPtr;
