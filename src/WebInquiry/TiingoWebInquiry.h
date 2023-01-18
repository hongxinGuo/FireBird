#pragma once

#include "VirtualWebInquiry.h"

#include<memory>

class CTiingoWebInquiry : public CVirtualWebInquiry {
public:
	CTiingoWebInquiry();
	~CTiingoWebInquiry() override = default;

	bool PrepareNextInquiringString(void) override;
	void CreateTotalInquiringString(CString strMiddle) override;
	CString GetNextInquiringMiddleString(long, bool) final { return _T(""); }
	bool ReportStatus(long lNumberOfData) const override;
	void ConfigureSession(void) final; // 设置m_pSession状态。
	bool ParseData(CWebDataPtr pWebData) override; // 数据为JSon格式, 需要解析
};

using CTiingoWebInquiryPtr = shared_ptr<CTiingoWebInquiry>;
extern CTiingoWebInquiryPtr gl_pTiingoWebInquiry;
