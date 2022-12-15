#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

#include<memory>

class CTiingoWebInquiry : public CVirtualWebInquiry {
public:
	CTiingoWebInquiry();
	~CTiingoWebInquiry() override = default;

	virtual bool PrepareNextInquiringString(void) override;
	virtual CString GetNextInquiringMiddleString(long, bool) override final { return _T(""); }
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void ConfigureSession(void) override final; // 设置m_pSession状态。
	virtual bool ParseData(CWebDataPtr pWebData) override; // 数据为JSon格式, 需要解析
};

typedef shared_ptr<CTiingoWebInquiry> CTiingoWebInquiryPtr;
extern CTiingoWebInquiryPtr gl_pTiingoWebInquiry;
