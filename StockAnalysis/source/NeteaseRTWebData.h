#pragma once
#include "WebData.h"

#include"RTData.h"

class CNeteaseRTWebData final : public CWebData {
  friend class CRTData;

public:
  CNeteaseRTWebData();
  ~CNeteaseRTWebData();

  virtual bool ReadPrefix(void) override; // ����ǰ׺
  virtual bool IsReadingFinished(void) override; // ���������е�����
  virtual bool ReportStatus(long lNumberOfData) override;

  virtual bool SucceedReadingAndStoringOneWebData(void) override;
  virtual void ProcessWebDataStored(void) override;
  virtual void ReportDataError(void) override;
  virtual void ReportCommunicationError(void) override;
  virtual void InquireNextWebData(void) override;
  virtual int GetInquiringStr(CString& strInquire, long lTotalNumber = 900, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;

private:
};
