#pragma once

#include"WebData.h"

class CPotenDailyBriefingWebData final : public CWebData
{
public:
  CPotenDailyBriefingWebData();
  ~CPotenDailyBriefingWebData();

  virtual void InquireNextWebData(void) override;
  virtual void StartReadingThread(void) override;

  long GetInquiringDay(void) noexcept { return m_lInquiringDay; }

protected:
  long m_lInquiringDay; // 用于存储当前所提取的poten数据的日期，当下载完毕后存于返回的数据结构中。
};
