#pragma once

#include "WebData.h"

class CNetEaseRTWebDataForBoost final : public CWebData {
public:
  CNetEaseRTWebDataForBoost();
  ~CNetEaseRTWebDataForBoost();

  virtual bool IsNeedProcessingCurrentWebData(void) override;
  virtual bool SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount) override;
  virtual void ProcessWebDataStored(void) override;
  virtual void ReportDataError(void) override;
  virtual void ReportCommunicationError(void) override;
  virtual void InquireNextWebData(void) override;
  virtual int GetInquiringStr(CString& strInquire) override;
  virtual void StartReadingThread(void) override;

private:
  CString m_strDownLoadingStockCode;
  bool m_fNeedProcessingCurrentWebData;
};
