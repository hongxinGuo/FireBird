#pragma once

#include"VirtualWebInquiry.h"

class CPotenDailyBriefingWebInquiry final : public CVirtualWebInquiry
{
public:
  CPotenDailyBriefingWebInquiry();
  ~CPotenDailyBriefingWebInquiry();

  virtual bool PrepareNextInquiringStr(void) override;
  virtual void StartReadingThread(void) override;

  long GetInquiringDay(void) noexcept { return m_lInquiringDay; }
  void SetInquiringDay(long lDay) noexcept { m_lInquiringDay = lDay; }

protected:
  long m_lInquiringDay; // ���ڴ洢��ǰ����ȡ��poten���ݵ����ڣ���������Ϻ���ڷ��ص����ݽṹ�С�
};
