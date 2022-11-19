#pragma once

#include"VirtualWebInquiry.h"
#include"WebRTData.h"

class CNeteaseRTWebInquiry : public CVirtualWebInquiry {
public:
	CNeteaseRTWebInquiry();
	virtual ~CNeteaseRTWebInquiry();

	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual bool PrepareNextInquiringString(void) override;
	virtual CString GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet = false) override final;
	virtual void ConfigerateSession(void) override final; // 在读取网络数据前的准备工作，默认为设置NeteaseRT的m_pSession状态。
	virtual void ClearUpIfReadingWebDataFailed(void) override final;
	virtual void UpdateStatusAfterReadingWebData(void) override final;
	virtual bool ParseData(CWebDataPtr pWebData) override final;
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final;
};

typedef shared_ptr<CNeteaseRTWebInquiry> CNeteaseRTWebInquiryPtr;
extern CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry; // 网易实时数据采集
