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
  long m_lInquiringDate; // ���ڴ洢��ǰ����ȡ��poten���ݵ����ڣ���������Ϻ���ڷ��ص����ݽṹ�С�
};

typedef shared_ptr<CPotenDailyBriefingWebInquiry> CPotenDailyBriefingWebInquiryPtr;