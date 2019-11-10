#pragma once
#include "WebData.h"

class CNeteaseRTWebData final : public CWebData {
public:
  CNeteaseRTWebData();
  ~CNeteaseRTWebData();

  virtual bool ReadPrefix(void) override; // 读入前缀
  virtual bool IsReadingFinished(void) override; // 读完了所有的数据
  virtual bool ReportStatus(long lNumberOfData) override;

  virtual bool SucceedReadingAndStoringOneWebData(void) override;
  virtual void ProcessWebDataStored(void) override;
  virtual void ReportDataError(void) override;
  virtual void ReportCommunicationError(void) override;
  virtual void InquireNextWebData(void) override;
  virtual int GetInquiringStr(CString& strInquire, long lTotalNumber = 900, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;

  bool IsCreateOnce(void) noexcept { return sm_fCreatedOnce; }

private:
  static bool sm_fCreatedOnce;  // 此种类只允许生成一个实例
};
