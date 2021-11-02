#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

class CFinnhubWebInquiry : public CVirtualWebInquiry {
public:
	CFinnhubWebInquiry();
	virtual ~CFinnhubWebInquiry();

	virtual bool PrepareNextInquiringStr(void) override;
	virtual CString GetNextInquiringMiddleStr(long lTotalNumer = 1, bool fSkipUnactiveStock = true) override;
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void PrepareReadingWebData(void) override final;
	virtual void ClearUpIfReadingWebDataFailed(void) override final; // ����ʧ�ܵĽ��չ���
	virtual void UpdateAfterReadingWebData(void) override final;
	virtual void StoreWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CFinnhubWebInquiry> CFinnhubWebInquiryPtr;
