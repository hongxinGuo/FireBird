#pragma once

#include "VirtualWebInquiry.h"

#include<memory>

class CQuandlWebInquiry : public CVirtualWebInquiry {
public:
	CQuandlWebInquiry();
	~CQuandlWebInquiry() override = default;

	bool PrepareNextInquiringString(void) override;
	void CreateTotalInquiringString(CString strMiddle) override;
	CString GetNextInquiringMiddleString(long, bool) override { return _T(""); }
	bool ReportStatus(long lNumberOfData) const override;
	void ConfigureSession(void) final;
	bool ParseData(CWebDataPtr pWebData) override; // ����ΪJSon��ʽ, ��Ҫ����
};

using CQuandlWebInquiryPtr = shared_ptr<CQuandlWebInquiry>;
extern CQuandlWebInquiryPtr gl_pQuandlWebInquiry;
