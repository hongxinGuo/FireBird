#pragma once

#include"VirtualWebInquiry.h"

class CPotenDailyBriefingWebInquiry : public CVirtualWebInquiry
{
public:
  CPotenDailyBriefingWebInquiry();
  virtual ~CPotenDailyBriefingWebInquiry();

  virtual bool PrepareNextInquiringStr(void) override;
  virtual void StartReadingThread(void) override;

  long GetInquiringDate(void) noexcept { return m_lInquiringDate; }
  void SetInquiringDate(long lDate) noexcept { m_lInquiringDate = lDate; }

protected:
  long m_lInquiringDate; // 用于存储当前所提取的poten数据的日期，当下载完毕后存于返回的数据结构中。
};

typedef shared_ptr<CPotenDailyBriefingWebInquiry> CPotenDailyBriefingWebInquiryPtr;