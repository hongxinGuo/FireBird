#pragma once
#include "WebData.h"
#include"RTData.h"

class CTengxunRTWebData final : public CWebData {
  friend class CRTData;

public:
  CTengxunRTWebData();
  ~CTengxunRTWebData();

  virtual bool ReadPrefix(void) override; // 此处用于判断是否为无效股票代码产生的数据
  virtual bool SucceedReadingAndStoringOneWebData(void) override;
  virtual void ProcessWebDataStored(void) override;
  virtual void ReportDataError(void) override;
  virtual void ReportCommunicationError(void) override;
  virtual void InquireNextWebData(void) override;
  virtual int GetInquiringStr(CString& strInquire, long lTotalNubmer = 900, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;
  virtual bool ReportStatus(long lNumberOfData) override;

private:
};
