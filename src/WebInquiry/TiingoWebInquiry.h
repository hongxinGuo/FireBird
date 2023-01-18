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
	void ConfigureSession(void) final; // ����m_pSession״̬��
	bool ParseData(CWebDataPtr pWebData) override; // ����ΪJSon��ʽ, ��Ҫ����
};

using CTiingoWebInquiryPtr = shared_ptr<CTiingoWebInquiry>;
extern CTiingoWebInquiryPtr gl_pTiingoWebInquiry;
