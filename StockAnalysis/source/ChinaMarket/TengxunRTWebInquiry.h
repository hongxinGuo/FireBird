#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

class CTengxunRTWebInquiry : public CVirtualWebInquiry {
public:
	CTengxunRTWebInquiry();
	virtual ~CTengxunRTWebInquiry();

	virtual bool ParseData(CWebDataPtr pWebData) override final { return true; } // 数据为非JSon格式，不解析

	virtual bool PrepareNextInquiringStr(void) override;
	virtual CString GetNextInquiringMiddleStr(long lTotalNubmer, bool fUsingTotalStockSet = false) override;
	virtual void ConfigerateSession(void) override final; // 在读取网络数据前的准备工作，默认为设置Tengxun RT的m_pSession状态。
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final; // 无需将网络上读取到的数据存入本地

	bool IsTengxunRTDataInvalid(CWebData& WebDataReceived);
};

typedef shared_ptr<CTengxunRTWebInquiry> CTengxunRTWebInquiryPtr;
