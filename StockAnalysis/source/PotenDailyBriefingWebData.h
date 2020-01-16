#pragma once

#include"WebData.h"

class CPotenDailyBriefingWebData final : public CWebData
{
public:
  CPotenDailyBriefingWebData();
  ~CPotenDailyBriefingWebData();

  virtual void InquireNextWebData(void) override;
  virtual void StartReadingThread(void) override;

  void SetInquiringDay(long lDay) noexcept { m_lInquiringDay = lDay; }
  long GetInquiringDay(void) noexcept { return m_lInquiringDay; }

protected:
  long m_lInquiringDay;
};
