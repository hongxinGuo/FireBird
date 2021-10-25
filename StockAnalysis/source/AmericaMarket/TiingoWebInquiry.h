#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

class CTiingoWebInquiry : public CVirtualWebInquiry {
public:
	CTiingoWebInquiry();
	virtual ~CTiingoWebInquiry();

	virtual bool PrepareNextInquiringStr(void) override;
	virtual CString GetNextInquiringMiddleStr(long lTotalNumer = 1, bool fSkipUnactiveStock = true) override;
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void PrepareBeforeReadingWebData(void) override final;
	virtual void ProcessFailedReading(void) override final; // 处理失败的接收过程
	virtual void UpdateStatusAfterReceivingData(void) override final;
	virtual void StoreWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CTiingoWebInquiry> CTiingoWebInquiryPtr;
