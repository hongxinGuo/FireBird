#pragma once

#include"VirtualWebInquiry.h"

class CPotenDailyBriefingWebInquiry : public CVirtualWebInquiry
{
public:
  CPotenDailyBriefingWebInquiry();
  virtual ~CPotenDailyBriefingWebInquiry();

  virtual bool PrepareNextInquiringStr(void) override;
  virtual void StartReadingThread(void) override;

  long GetInquiringDate(void) noexcept { return m_lInquiringDay; }
  void SetInquiringDate(long lDate) noexcept { m_lInquiringDay = lDate; }

protected:
  long m_lInquiringDay; // ���ڴ洢��ǰ����ȡ��poten���ݵ����ڣ���������Ϻ���ڷ��ص����ݽṹ�С�
};

typedef shared_ptr<CPotenDailyBriefingWebInquiry> CPotenDailyBriefingWebInquiryPtr;