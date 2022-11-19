#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

class CTengxunRTWebInquiry : public CVirtualWebInquiry {
public:
	CTengxunRTWebInquiry();
	virtual ~CTengxunRTWebInquiry();

	virtual bool ParseData(CWebDataPtr pWebData) override final { return true; } // 数据为非JSon格式，不解析

	virtual bool PrepareNextInquiringString(void) override;
	virtual CString GetNextInquiringMiddleString(long lTotalNubmer, bool fUsingTotalStockSet = false) override final;
	virtual void ConfigerateSession(void) override final; // 在读取网络数据前的准备工作，默认为设置Tengxun RT的m_pSession状态。
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final; // 无需将网络上读取到的数据存入本地
	virtual void ClearUpIfReadingWebDataFailed(void) override final; // 处理失败的接收过程
	virtual void UpdateStatusAfterReadingWebData(void) override final;

	bool IsTengxunRTDataInvalid(CWebData& WebDataReceived);
};

typedef shared_ptr<CTengxunRTWebInquiry> CTengxunRTWebInquiryPtr;
extern CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry; // 腾讯实时数据采集
