#pragma once

#include "WebDataBase.h"

class CNeteaseDayLineWebData final : public CWebDataBase {
public:
  CNeteaseDayLineWebData();
  ~CNeteaseDayLineWebData();

  virtual bool IsNeedProcessingCurrentWebData(void) override;
  virtual bool SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount) override;
  virtual void ProcessWebDataStored(void) override;
  virtual void ReportDataError(void) override;
  virtual void ReportCommunicationError(void) override;
  virtual void InquireNextWebData(void) override;
  virtual int GetInquiringStockStr(CString& strInquire) override;
  virtual void StartReadingThread(void) override;

  bool IsCreateOnce(void) { return sm_fCreatedOnce; }

private:
  static bool sm_fCreatedOnce;  // ������ֻ��������һ��ʵ��

  static bool sm_fNeedProcessingCurrentWebData;
};