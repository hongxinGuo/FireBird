#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

class CSinaRTWebInquiry : public CVirtualWebInquiry {
public:
	CSinaRTWebInquiry();
	virtual ~CSinaRTWebInquiry();

	virtual bool ParseData(CWebDataPtr pWebData) override final { return true; } // 数据为非JSon格式，不解析

	virtual bool PrepareNextInquiringString(void) override final;
	virtual CString GetNextInquiringMiddleString(long lTotalNumer, bool fUsingTotalStockSet = false) override final;
	virtual void ConfigerateSession(void) override final; // 在读取网络数据前的准备工作，默认为设置Sina RT的m_pSession状态。
	virtual bool ReportStatus(long lNumberOfData) const override final;
	virtual void ClearUpIfReadingWebDataFailed(void) override final; // 处理失败的接收过程
	virtual void UpdateStatusAfterReadingWebData(void) override final;
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final; // 无需将网络上读取到的数据存入本地
};

typedef shared_ptr<CSinaRTWebInquiry> CSinaRTWebInquiryPtr;
