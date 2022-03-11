#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

class CSinaRTWebInquiry : public CVirtualWebInquiry {
public:
	CSinaRTWebInquiry();
	virtual ~CSinaRTWebInquiry();

	virtual bool ParseData(CWebDataPtr pWebData) override final { return true; } // 数据为非JSon格式，不解析

	virtual bool PrepareNextInquiringStr(void) override;
	virtual CString GetNextInquiringMiddleStr(long lTotalNumer, bool fUsingTotalStockSet = false) override;
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final; // 无需将网络上读取到的数据存入本地
};

typedef shared_ptr<CSinaRTWebInquiry> CSinaRTWebInquiryPtr;
