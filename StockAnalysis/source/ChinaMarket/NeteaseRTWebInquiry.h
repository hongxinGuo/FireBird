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
	virtual void ClearUpIfReadingWebDataFailed(void) override final { TRACE("读取网易实时数据出错\n"); }
	virtual bool ParseData(CWebDataPtr pWebData) override final;
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final; // 网易实时数据已处理，无需将其放入队列中。
};

typedef shared_ptr<CNeteaseRTWebInquiry> CNeteaseRTWebInquiryPtr;
