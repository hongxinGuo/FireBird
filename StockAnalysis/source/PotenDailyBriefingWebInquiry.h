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

protected:
  long m_lInquiringDay; // ���ڴ洢��ǰ����ȡ��poten���ݵ����ڣ���������Ϻ���ڷ��ص����ݽṹ�С�
};
