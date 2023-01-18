#pragma once

#include"VirtualWebInquiry.h"

class CNeteaseRTWebInquiry : public CVirtualWebInquiry {
public:
	CNeteaseRTWebInquiry();
	~CNeteaseRTWebInquiry() override = default;

	bool ReportStatus(long lNumberOfData) const override;
	bool PrepareNextInquiringString(void) override;
	CString GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet) final;
	void ConfigureSession(void) final; // 在读取网络数据前的准备工作，默认为设置NeteaseRT的m_pSession状态。

	bool ParseData(CWebDataPtr pWebData) final;
};

using CNeteaseRTWebInquiryPtr = shared_ptr<CNeteaseRTWebInquiry>;
extern CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry; // 网易实时数据采集
