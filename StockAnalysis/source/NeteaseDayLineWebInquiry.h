#pragma once

#include "VirtualWebInquiry.h"

class CNeteaseDLWebInquiry : public CVirtualWebInquiry {
public:
  CNeteaseDLWebInquiry();
  virtual ~CNeteaseDLWebInquiry();

  virtual bool PrepareNextInquiringStr(void) override;
  virtual void StartReadingThread(void) override;

  void SetDownLoadingStockCode(CString strStockCode);
  CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }
  void ResetDownLoadingStockCode(void) noexcept { m_strDownLoadingStockCode = _T(""); }

private:
  CString m_strDownLoadingStockCode;
};

typedef shared_ptr<CNeteaseDLWebInquiry> CNeteaseDLWebInquiryPtr;
