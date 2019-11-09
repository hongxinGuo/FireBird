#pragma once

#include "WebData.h"

class CNeteaseDayLineWebData final : public CWebData {
public:
  CNeteaseDayLineWebData();
  ~CNeteaseDayLineWebData();

  virtual bool IsNeedProcessingCurrentWebData(void) override;
  virtual bool SucceedReadingAndStoringOneWebData(void) override;
  virtual void ProcessWebDataStored(void) override;
  virtual void ReportDataError(void) override;
  virtual void ReportCommunicationError(void) override;
  virtual void InquireNextWebData(void) override;
  virtual int GetInquiringStr(CString& strInquire) override;
  virtual void StartReadingThread(void) override;

  void SetDownLoadingStockCode(CString strStockCode);
  CString GetDownLoadingStockCode(void) noexcept { return m_strDownLoadingStockCode; }

private:
  CString m_strDownLoadingStockCode;
  bool m_fNeedProcessingCurrentWebData;
};
