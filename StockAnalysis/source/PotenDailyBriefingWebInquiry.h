#pragma once

#include"VirtualWebInquiry.h"

class CPotenDailyBriefingWebInquiry : public CVirtualWebInquiry
{
public:
  CPotenDailyBriefingWebInquiry();
  virtual ~CPotenDailyBriefingWebInquiry();

  virtual bool PrepareNextInquiringStr(void) override;
  virtual void StartReadingThread(void) override;

  long GetInquiringDay(void) noexcept { return m_lInquiringDay; }
  void SetInquiringDay(long lDay) noexcept { m_lInquiringDay = lDay; }

protected:
  long m_lInquiringDay; // 用于存储当前所提取的poten数据的日期，当下载完毕后存于返回的数据结构中。
};

typedef shared_ptr<CPotenDailyBriefingWebInquiry> CPotenDailyBriefingWebInquiryPtr;