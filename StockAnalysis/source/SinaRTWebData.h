#pragma once
#include "WebData.h"
#include"RTData.h"

class CSinaRTWebData final : public CWebData {
  friend class CRTData;
public:
  CSinaRTWebData();
  ~CSinaRTWebData();

  virtual bool IsNeedProcessingCurrentWebData(void) { return IsWebDataReceived(); }

  virtual void ProcessCurrentWebData(void) override; // Ĭ�ϴ���ǰ�������ݺ���
  virtual bool SucceedReadingAndStoringOneWebData(void) override;
  virtual void ProcessWebDataStored(void) override;
  virtual void ReportDataError(void) override;
  virtual void ReportCommunicationError(void) override;
  virtual void InquireNextWebData(void) override;
  virtual int GetInquiringStr(CString& strInquire, long lTotalNumer = 900, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;
  virtual bool ReportStatus(long lNumberOfData) override;

private:
};
