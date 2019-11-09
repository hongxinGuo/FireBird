#pragma once
#include "WebData.h"

class CNeteaseRTWebData final : public CWebData {
public:
  CNeteaseRTWebData();
  ~CNeteaseRTWebData();

  virtual bool ReadPrefix(char*& pCurrentPos, long& iCount) override; // ����ǰ׺
  virtual bool IsReadingFinished(const char* const pCurrentPos, const long iCount) override; // ���������е�����
  virtual bool ReportStatus(long lNumberOfData) override;

  virtual bool SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount) override;
  virtual void ProcessWebDataStored(void) override;
  virtual void ReportDataError(void) override;
  virtual void ReportCommunicationError(void) override;
  virtual void InquireNextWebData(void) override;
  virtual int GetInquiringStr(CString& strInquire) override;
  virtual void StartReadingThread(void) override;

  bool IsCreateOnce(void) noexcept { return sm_fCreatedOnce; }

private:
  static bool sm_fCreatedOnce;  // ������ֻ��������һ��ʵ��
};
