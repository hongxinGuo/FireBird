#pragma once
#include "WebDataBase.h"

class CSinaRTWebData final : public CWebDataBase {
public:
  CSinaRTWebData();
  ~CSinaRTWebData();

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
};
