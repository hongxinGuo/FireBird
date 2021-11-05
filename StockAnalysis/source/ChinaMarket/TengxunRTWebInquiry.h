#pragma once

#include "VirtualWebInquiry.h"

class CTengxunRTWebInquiry : public CVirtualWebInquiry {
public:
	CTengxunRTWebInquiry();
	virtual ~CTengxunRTWebInquiry();

	virtual bool PrepareNextInquiringStr(void) override;
	virtual CString GetNextInquiringMiddleStr(long lTotalNubmer = 900, bool fCheckActiveStock = false) override;
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final; // 将网络上读取到的数据存入本地
};

typedef shared_ptr<CTengxunRTWebInquiry> CTengxunRTWebInquiryPtr;
