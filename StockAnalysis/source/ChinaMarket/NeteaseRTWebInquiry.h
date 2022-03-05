#pragma once

#include"VirtualWebInquiry.h"
#include"WebRTData.h"

class CNeteaseRTWebInquiry : public CVirtualWebInquiry {
public:
	CNeteaseRTWebInquiry();
	virtual ~CNeteaseRTWebInquiry();

	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual bool PrepareNextInquiringStr(void) override;
	virtual CString GetNextInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet = false) override;
	virtual void ClearUpIfReadingWebDataFailed(void) override final { TRACE("��ȡ����ʵʱ���ݳ���\n"); }
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final; // �������϶�ȡ�������ݴ��뱾��
};

typedef shared_ptr<CNeteaseRTWebInquiry> CNeteaseRTWebInquiryPtr;
