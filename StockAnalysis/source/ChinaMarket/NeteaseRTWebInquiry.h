#pragma once

#include"VirtualWebInquiry.h"

class CNeteaseRTWebInquiry : public CVirtualWebInquiry {
public:
	CNeteaseRTWebInquiry();
	virtual ~CNeteaseRTWebInquiry();

	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual bool PrepareNextInquiringStr(void) override;
	virtual CString GetNextInquiringMiddleStr(long lTotalNumber = 600, bool fCheckActiveStock = false) override;
	virtual void ProcessFailedReading(void) override final { TRACE("��ȡ����ʵʱ���ݳ���\n"); }
	virtual void StoreWebData(CWebDataPtr pWebData) override final; // �������϶�ȡ�������ݴ��뱾��
};

typedef shared_ptr<CNeteaseRTWebInquiry> CNeteaseRTWebInquiryPtr;
