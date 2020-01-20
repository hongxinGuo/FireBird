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
  long m_lInquiringDay; // ���ڴ洢��ǰ����ȡ��poten���ݵ����ڣ���������Ϻ���ڷ��ص����ݽṹ�С�
};
